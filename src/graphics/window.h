#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

int window_init(int width, int height, GLFWwindow* window[static 1]);

#endif /* end of include guard: WINDOW_H */
