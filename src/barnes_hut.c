#include "graphics/renderer.h"
#include "graphics/window.h"
#include "libvecmath/vec3.h"
#include "physics.h"
#include "sim_state.h"
#include "util/sim_time.h"
#include "octree.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <assert.h>

int main(void) {
  /* Window params */
  const int win_x = 900;
  const int win_y = 900;
  const double target_fps = 40.0;
  const double target_tps = 60.0;

  /* Símulation params */
  const double t_step = 0.000005;
  unsigned int n_masses = 4000;
  const double c_grav = 0.1;
  const double norm = 0.00001;
  const double threshhold = 0.5;
  const double tolerance = 0.000000001;

  /* Initialize window, timer and renderer */
  GLFWwindow* window;
  if (window_init(win_x, win_y, &window)) {
    printf("Renderer initialization failed\n");
    fflush(stdout);
  }

  renderer Renderer;
  renderer_init(&Renderer, window);
  renderer_tree_init(&Renderer, 2 * n_masses + 1);

  sim_time time;
  sim_time_init(&time);

  /* Initialize sim state */
  sim_state state = {0};
  sim_state_init(&state, n_masses, c_grav, norm, t_step);
  mass* m_arr = state.m_array;

  /* Mass array configuration and inital characteristics */
  init_galaxy(m_arr, n_masses, 1000000, 1, c_grav);

  /* Initialize octtree */
  struct octree tree = octree_init(n_masses * 2 + 1, threshhold, tolerance);

  /* Program loop */
  while (!glfwWindowShouldClose(window)) {
    /* Continue if tick is too early */
    if (sim_time_dt_lt(&time) < MUS_PER_S / target_tps) {
      continue;
    }
    sim_time_lt_now(&time);

    /* Barnes-Hut method */
    sim_state_key_vals(&state);
    octree_prepare(&tree, &state);

    /* Generate Oct-tree */
    for (unsigned int i = 0; i < state.n_m; ++i) {
      struct octree_node node = (struct octree_node){0};
      node.center_of_mass = state.m_array[i].pos;
      node.mass = state.m_array[i].m;
      octree_insert(&tree, node);
    }
    octree_propagate(&tree);

    /* Update velocities */
    for (unsigned int i = 0; i < state.n_m; ++i) {
      vec3 accel = octree_accel(&tree, &state, state.m_array[i].pos);
      state.m_array[i].vel =
          vec3_sum(state.m_array[i].vel, vec3_sp(t_step, accel));
    }

    /* Update positions */
    for (unsigned int i = 0; i < n_masses; ++i) {
      m_arr[i].pos = vec3_sum(m_arr[i].pos, vec3_sp(t_step, m_arr[i].vel));
    }

    /* Only render if new frame is needed */
    if (sim_time_dt_lf(&time) >= MUS_PER_S / target_fps) {
      sim_time_lf_now(&time);
      Renderer.render(&Renderer, &tree, n_masses, m_arr);
    }
    glfwPollEvents();
    printf("%f ", state.max_x1);
    printf("%f ", state.min_x1);
    printf("%f ", state.max_x2);
    printf("%f\n", state.min_x2);
  }

  glfwTerminate();
  sim_state_cleanup(&state);
  octree_cleanup(&tree);

  return 0;
}
