#ifndef SHADER_H
#define SHADER_H
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
// clang-format on

void create_shader_prog(GLuint *shader_prog, const char *vert_src_path,
                        const char *frag_src_path);
char *load_shader_src(const char *shader_src_path);
void compile_shader(GLuint shader, const char *shader_src);
long get_file_size(FILE *f);

#endif
