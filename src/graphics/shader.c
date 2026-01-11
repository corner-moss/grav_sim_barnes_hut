#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

static char* shader_read(const char filename[]) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Could not open shader %s\n", filename);
  }

  /* get file size */
  unsigned int file_length = 0;
  while (fgetc(file) != EOF)
    ++file_length;
  rewind(file);

  /* read file */
  char* shader = calloc(file_length + 1, sizeof(char));
  fread(shader, sizeof(char), file_length + 1, file);
  fclose(file);

  return shader;
}

unsigned int shader_create(const char* vertex, const char* fragment) {
  int success;
  char info_log[512];

  /* Vertex shader */
  char* vertex_shader_source = shader_read(vertex);
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, (const GLchar* const*)&vertex_shader_source,
                 NULL);
  glCompileShader(vertex_shader);
  // check for shader compile errors
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    fprintf(stderr, "%s", "Vertex shader failed to compile\n");
  }
  free(vertex_shader_source);

  // fragment shader
  char* fragment_shader_source = shader_read(fragment);
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1,
                 (const GLchar* const*)&fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  // check for shader compile errors
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    fprintf(stderr, "%s", "Fragment shader failed to compile\n");
  }
  free(fragment_shader_source);

  // link shaders
  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  // check for linking errors
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    fprintf(stderr, "%s", "Shader linking failed \n");
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}
