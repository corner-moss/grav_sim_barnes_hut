#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "window.h"

int window_init(int width, int height, GLFWwindow* window[static 1]) {
  /* Initialize GLFW */
  if (!glfwInit()) {
    fprintf(stderr, "%s", "GLFW failed to initialize\n");
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  /* Create window */
  *window = glfwCreateWindow(width, height, "grav sim", NULL, NULL);
  if (!*window) {
    fprintf(stderr, "%s", "window could not be created\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(*window);

  /* Initialize glad */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "%s", "error initializing GLAD\n");
    glfwTerminate();
    return -1;
  }

  return 0;
}
