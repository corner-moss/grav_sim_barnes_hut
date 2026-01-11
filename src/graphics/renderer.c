#include "renderer.h"
#include "shader.h"
#include "../octree.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <strings.h>
#include <libvecmath/vec3.h>

void renderer_init(renderer* self, GLFWwindow* window) {
  /* Shader init */
  self->dot_shader_program = shader_create("res/shaders/dot_2d_vert.glsl",
                                           "res/shaders/dot_2d_frag.glsl");
  self->tree_shader_program = shader_create("res/shaders/tree_2d_vert.glsl",
                                            "res/shaders/tree_2d_frag.glsl");
  self->render = renderer_render;
  self->window = window;

  /* Dots */
  glPointSize(2.0f);
  /* Generating and binding buffers */
  glGenBuffers(1, &self->dot_vbo);
  glGenVertexArrays(1, &self->dot_vao);
  glBindVertexArray(self->dot_vao);
  glBindBuffer(GL_ARRAY_BUFFER, self->dot_vbo);

  /* Configuring VAO and point size */
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 7 * sizeof(double),
                        (void*)0);
  glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 7 * sizeof(double),
                        (void*)(3 * sizeof(double)));
  glVertexAttribPointer(2, 1, GL_DOUBLE, GL_FALSE, 7 * sizeof(double),
                        (void*)(6 * sizeof(double)));
  glEnableVertexAttribArray(0);

  /* Tree */
  glLineWidth(2.0);
  /* Generating and binding buffers */
  glGenBuffers(1, &self->tree_vbo);
  glGenVertexArrays(1, &self->tree_vao);
  glBindVertexArray(self->tree_vao);
  glBindBuffer(GL_ARRAY_BUFFER, self->tree_vbo);

  /* Configuring VAO and point size */
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double),
                        (void*)0);
  glEnableVertexAttribArray(0);
}

void renderer_tree_init(renderer* self, unsigned int number_nodes) {
  self->tree_buffer_size = number_nodes;
  self->tree_buffer_element_size = 8;
  self->tree_buffer =
      calloc(number_nodes, self->tree_buffer_element_size * sizeof(vec3));
}

void renderer_draw_tree(struct renderer* self, struct octree* tree) {
  memset(self->tree_buffer, 0,
         self->tree_buffer_size * self->tree_buffer_element_size *
             sizeof(vec3));

  vec3 center;
  double sidelength;
  for (unsigned int i = 0; i < tree->number_nodes; ++i) {
    center = tree->stack[i].center;
    sidelength = tree->stack[i].edge_length;

    self->tree_buffer[8 * i] = vec3_sum(
        center, (vec3){.x1 = sidelength / 2, .x2 = sidelength / 2, .x3 = 0.0});
    self->tree_buffer[8 * i + 1] = vec3_sum(
        center, (vec3){.x1 = sidelength / 2, .x2 = -sidelength / 2, .x3 = 0.0});
    self->tree_buffer[8 * i + 2] = vec3_sum(
        center, (vec3){.x1 = sidelength / 2, .x2 = -sidelength / 2, .x3 = 0.0});
    self->tree_buffer[8 * i + 3] = vec3_sum(
        center,
        (vec3){.x1 = -sidelength / 2, .x2 = -sidelength / 2, .x3 = 0.0});
    self->tree_buffer[8 * i + 4] = vec3_sum(
        center,
        (vec3){.x1 = -sidelength / 2, .x2 = -sidelength / 2, .x3 = 0.0});
    self->tree_buffer[8 * i + 5] = vec3_sum(
        center, (vec3){.x1 = -sidelength / 2, .x2 = sidelength / 2, .x3 = 0.0});
    self->tree_buffer[8 * i + 6] = vec3_sum(
        center, (vec3){.x1 = -sidelength / 2, .x2 = sidelength / 2, .x3 = 0.0});
    self->tree_buffer[8 * i + 7] = vec3_sum(
        center, (vec3){.x1 = sidelength / 2, .x2 = sidelength / 2, .x3 = 0.0});
  }
  glBindVertexArray(self->tree_vao);
  glBindBuffer(GL_ARRAY_BUFFER, self->tree_vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 8 * tree->number_nodes,
               self->tree_buffer, GL_DYNAMIC_DRAW);

  glUseProgram(self->tree_shader_program);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_LINES, 0, 8 * tree->number_nodes);
}

void renderer_render(renderer* self, struct octree* tree, unsigned int n_m,
                     mass m_arr[static n_m]) {
  /* Setup */
  glfwMakeContextCurrent(self->window);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  /* Tree */
  renderer_draw_tree(self, tree);

  /* Dots */
  glBindVertexArray(self->dot_vao);
  glBindBuffer(GL_ARRAY_BUFFER, self->dot_vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(m_arr[0]) * n_m, m_arr, GL_DYNAMIC_DRAW);

  glUseProgram(self->dot_shader_program);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_POINTS, 0, n_m);
  glfwSwapBuffers(self->window);
}

void renderer_cleanup(renderer* self) {
  if (self->tree_buffer) {
    free(self->tree_buffer);
  }
}
