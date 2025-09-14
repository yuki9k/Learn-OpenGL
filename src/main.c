#include "shader.h"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <math.h>
#include <stdio.h>
// clang-format on

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

// We do not have to transform vertex data to NDC in this case since our data
// (vertices) already is NDC. Usually this is something that we need to do
// however

int main(void) {
  // clang-format off
  const GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };
  //
  // Rectangle
  // const float vertices[] = {
  //   0.5f, 0.5f, 0.0f,
  //   0.5f, -0.5f, 0.0f,
  //   -0.5f, -0.5f, 0.0f,
  //   -0.5f, 0.5f, 0.0f
  // };
  //
  // const unsigned int indices[] = {
  //   0, 1, 3,
  //   1, 2, 3
  // };
  //
  // Triforce
 // const GLfloat vertices[] = {
 //    -0.5f, -0.5f, 0.0f,
 //    0.0f, -0.5f, 0.0f,
 //    -0.25f, 0.0f, 0.0f,
 //    0.5f, -0.5f, 0.0f,
 //    0.25f, 0.0f, 0.0f,
 //    0.0f, 0.5f, 0.0f} ;
 //
 //  const GLuint indices[]={
 //    0, 1, 2,
 //    1, 3, 4,
 //    2, 4, 5
 //  };
  // clang-format on

  const int win_width = 800;
  const int win_height = 600;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(win_width, win_height, "LearnOpenGL", NULL, NULL);

  if (window == NULL) {
    printf("Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD");
    return -1;
  }

  GLuint shader_prog = 0;
  create_shader_prog(&shader_prog, "src/shader/shader.vert",
                     "src/shader/shader.frag");

  // unsigned int VAO, VBO, EBO;
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO); // Generates buffer ID
  // Buffer type of VBO is GL_ARRAY_BUFFER
  // NOTE: We can bind several buffer objects at once as long as they're
  // different types
  glBindBuffer(GL_ARRAY_BUFFER,
               VBO); // VBO is created (ur used if already exist) here
  // From this point any buffer calls we make (on the GL_ARRAY_BUFFER target)
  // will be used to configure the VBO above
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW); // Allocates and stores data in the currently
                                // bound buffer object (rn VBO)

  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  // Specify what input data goes to which vertex attribute in the vertex shader
  // Tells OpenGL how to read the VBO
  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
      (void *)0); // location of the vertex attribute we want to configure is at
                  // 0 (this is why layout = 0 was specified in the vertex
                  // shader), each position is 3 32-bit floating point values
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  // NOTE: Each vertex attribute takes data from memory managed by VBO. The VBO
  // is the one currently bound to the buffer (GL_ARRAY_BUFFER)
  // Therefore our VBO is now associated with vertex attribute 0

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    // double glfw_time = glfwGetTime();
    // float red_value = (sin(glfw_time));
    // float green_value = (cos(glfw_time));
    // float blue_value = (tan(glfw_time));
    // int vertex_color_location = glGetUniformLocation(shader_program,
    // "uColor");
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_prog);
    // glUniform4f(vertex_color_location, red_value, green_value, blue_value,
    //             1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);
  glDeleteProgram(shader_prog);
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  // GLFW_RELEASE 0
  // GLFW_PRESS  1
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}
