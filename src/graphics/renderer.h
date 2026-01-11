#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../physics.h"
#include "../octree.h"

typedef struct renderer renderer;

typedef void render_function(renderer* self, struct octree* tree,
                             unsigned int n_m, mass m_arr[static 1]);

typedef struct renderer {
  unsigned int dot_shader_program;
  unsigned int dot_vao;
  unsigned int dot_vbo;
  unsigned int tree_shader_program;
  unsigned int tree_vao;
  unsigned int tree_vbo;
  render_function* render;
  GLFWwindow* window;

  vec3* tree_buffer;
  unsigned int tree_buffer_size;
  int tree_buffer_element_size;
} renderer;

void renderer_init(renderer* self, GLFWwindow* window);
void renderer_tree_init(renderer* self, unsigned int number_nodes);
void renderer_draw_tree(struct renderer* self, struct octree* tree);
void renderer_render(renderer* self, struct octree* tree, unsigned int n_m,
                     mass m_arr[static n_m]);
void renderer_cleanup(renderer* self);
#endif /* Ends include guard for RENDER_H */
