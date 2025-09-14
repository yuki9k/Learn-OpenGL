#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>

void create_shader_prog(const char *frag_src_path, const char *vert_src_path);
int load_shader_src(const char *shader_src_path, char shader_src[1024]);
int compile_shader(GLuint shader, const char *shader_src);

#endif
