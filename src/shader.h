#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>

extern const int SHADER_SRC_SIZE;

void create_shader_prog(GLuint *shader_prog, const char *vert_src_path,
                        const char *frag_src_path);
void load_shader_src(const char *shader_src_path,
                     char shader_src[SHADER_SRC_SIZE]);
void compile_shader(GLuint shader, const char *shader_src);

#endif
