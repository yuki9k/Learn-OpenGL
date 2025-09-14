#include "shader.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

const int SHADER_SRC_SIZE = 1024 * 16; // 16kb

void create_shader_prog(GLuint *shader_prog, const char *vert_src_path,
                        const char *frag_src_path) {
  GLint link_success;
  GLchar link_log[512];

  GLuint vert_shader, frag_shader;
  char vert_shader_src[SHADER_SRC_SIZE];
  char frag_shader_src[SHADER_SRC_SIZE];

  load_shader_src(vert_src_path, vert_shader_src);
  load_shader_src(frag_src_path, frag_shader_src);

  *shader_prog = glCreateProgram();
  vert_shader = glCreateShader(GL_VERTEX_SHADER);
  frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

  compile_shader(vert_shader, vert_shader_src);
  compile_shader(frag_shader, frag_shader_src);

  glAttachShader(*shader_prog, vert_shader);
  glAttachShader(*shader_prog, frag_shader);
  glLinkProgram(*shader_prog);

  glGetProgramiv(*shader_prog, GL_LINK_STATUS, &link_success);

  if (link_success == GL_FALSE) {
    glGetProgramInfoLog(*shader_prog, 512, NULL, link_log);
    fprintf(stderr, "Failed to link shaders\n%s", link_log);
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
}

void load_shader_src(const char *shader_src_path,
                     char shader_src[SHADER_SRC_SIZE]) {
  FILE *f = fopen(shader_src_path, "r");

  if (!f) {
    fprintf(stderr, "Failed to open file '%s'", shader_src_path);
    perror("fopen() failed:");
  }

  int ch;
  int idx = 0;

  while ((ch = fgetc(f)) != EOF) {
    shader_src[idx++] = ch;
  }

  if (ferror(f)) {
    fprintf(stderr, "Error reading file '%s'", shader_src_path);
  }

  fclose(f);
  printf("Shader file '%s' successfully read\n", shader_src_path);
}

void compile_shader(GLuint shader, const char shader_src[SHADER_SRC_SIZE]) {
  GLint compile_success, shader_type;
  GLchar compile_log[512];

  glShaderSource(shader, 1, &shader_src, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
  glGetShaderiv(shader, GL_SHADER_TYPE, &shader_type);

  if (compile_success == GL_FALSE) {
    glGetShaderInfoLog(shader, 512, NULL, compile_log);
    printf("Failed to compile shader %d\n%s", shader_type, compile_log);
  }

  printf("Succesfully compiled shader %d\n", shader_type);
}
