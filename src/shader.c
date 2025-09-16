#include "shader.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>

void create_shader_prog(GLuint *shader_prog, const char *vert_src_path,
                        const char *frag_src_path) {
  GLint link_success;
  GLchar link_log[512];

  GLuint vert_shader, frag_shader;
  char *vert_shader_src = load_shader_src(vert_src_path);
  char *frag_shader_src = load_shader_src(frag_src_path);

  if (!vert_shader_src || !frag_shader_src) {
    // TODO: Handle this
    return;
  }

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
    fprintf(stderr, "Failed to link shaders\n%s\n", link_log);
  } else if (link_success == GL_TRUE) {
    printf("Succesfully linked shaders\n");
  }

  free(vert_shader_src);
  free(frag_shader_src);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
}

char *load_shader_src(const char *shader_src_path) {
  FILE *f = fopen(shader_src_path, "r");
  long f_size;

  if (!f) {
    fprintf(stderr, "Failed to open file '%s'\n", shader_src_path);
    perror("fopen() failed: ");
    return NULL;
  }

  f_size = get_file_size(f);

  if (ferror(f) || f_size == -1) {
    fprintf(stderr, "Error reading file '%s'\n", shader_src_path);
    perror("get_file_size() failed: ");
    fclose(f);
    return NULL;
  }

  char *buffer = malloc(f_size + 1);
  if (!buffer) {
    fprintf(stderr, "Error reading file '%s':\nCould not allocate memory\n",
            shader_src_path);
    fclose(f);
    return NULL;
  }

  if (fread(buffer, sizeof(char), f_size, f) != (size_t)f_size) {
    if (feof(f)) {
      fprintf(stderr, "Error reading file '%s':\nUnexpected EOF\n",
              shader_src_path);
    } else if (ferror(f)) {
      perror("Error reading file '%s':\n");
    }

    free(buffer);
    fclose(f);
    return NULL;
  }

  buffer[f_size] = '\0';
  fclose(f);

  printf("Shader file '%s' successfully read\n", shader_src_path);
  return buffer;
}

void compile_shader(GLuint shader, const char *shader_src) {
  GLint compile_success, shader_type;
  GLchar compile_log[512];

  glShaderSource(shader, 1, &shader_src, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
  glGetShaderiv(shader, GL_SHADER_TYPE, &shader_type);

  if (compile_success == GL_FALSE) {
    glGetShaderInfoLog(shader, 512, NULL, compile_log);
    printf("Failed to compile shader %d\n%s", shader_type, compile_log);
    return;
  }

  printf("Succesfully compiled shader %d\n", shader_type);
}

long get_file_size(FILE *f) {
  if (fseek(f, 0, SEEK_END) != 0 || ferror(f)) {
    return -1;
  }

  long f_size = ftell(f);
  rewind(f);
  return f_size;
}
