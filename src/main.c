// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
// clang-format on

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

// We do not have to transform vertex data to NDC in this case since our data
// (vertices) already is NDC. Usually this is something that we need to do
// however

const char *VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n" // Value of vec4
                                                          // gl_Position will be
                                                          // output of vertex
                                                          // shader
    "}\0";

const char *FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

int main(void) {
  // clang-format off
  // x,y,z in normalized device coordinates
  const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };
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

  int compile_status;
  char info_log[512];

  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER); // Create shader object
  glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertex_shader); // Compiles shader OBJECT
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);

  if (compile_status == GL_FALSE) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    printf("Failed to compile vertex shader:\n%s", info_log);
  }

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);

  if (compile_status == GL_FALSE) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    printf("Failed to compile fragment shader:\n%s", info_log);
  }

  // Link both shader OBJECTS into a shader PROGRAM
  unsigned int shader_program;
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_status);

  if (compile_status == GL_FALSE) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    printf("Failed to link shaders:\n%s", info_log);
  }

  // Shader objects no longer needed after successful linking
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
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

  // Specify what input data goes to which vertex attribute in the vertex shader
  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
      (void *)0); // location of the vertex attribute we want to configure is at
                  // 0 (this is why layout = 0 was specified in the vertex
                  // shader), each position is 3 32-bit floating point values
  glEnableVertexAttribArray(0);

  // NOTE: Each vertex attribute takes data from memory managed by VBO. The VBO
  // is the one currently bound to the buffer (GL_ARRAY_BUFFER)
  // Therefore our VBO is now associated with vertex attribute 0

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

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
