#include "../include/camera.h"
#include "../include/geomety.h"
#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/shader.h"
#include "../include/stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void proccesInput(GLFWwindow *window);
void mouseInputCallback(GLFWwindow *window, double mouseX, double mouseY);
void scrollInputCallback(GLFWwindow *window, double xOffset, double yOffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

float dt = 0.0f;
float lastFrame = 0.0f;

float lastMouseX = SCR_WIDTH / 2.0f;
float lastMouseY = SCR_HEIGHT / 2.0f;

bool firstMouseInput = true;

const float sensitivity = 0.3f;

Camera camera(CAMERA_POSITION, CAMERA_FRONT, CAMERA_UP, YAW, PITCH, SPEED,
              SENSITIVITY, FOV);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn Opengl", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseInputCallback);
  glfwSetScrollCallback(window, scrollInputCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("./assets/imgs/spiral.jpg", &width, &height, &nrChannels, 0);

  if (!data) {
    std::cout << "ERROR: image not loaded" << std::endl;
    return -1;
  }

  unsigned int texture0, texture1;
  // unsigned int texture0;
  glGenTextures(1, &texture0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  data = stbi_load("./assets/imgs/smile.png", &width, &height, &nrChannels, 0);

  if (!data) {
    std::cout << "ERROR: image not loaded" << std::endl;
    return -1;
  }

  glGenTextures(1, &texture1);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  Shader shader("./assets/shaders/vertex.vert",
                "./assets/shaders/fragment.frag");

  unsigned int RECTANGLE_VAO;
  glGenVertexArrays(1, &RECTANGLE_VAO);
  glBindVertexArray(RECTANGLE_VAO);

  unsigned int RECTANGLE_VBO;
  glGenBuffers(1, &RECTANGLE_VBO);

  glBindBuffer(GL_ARRAY_BUFFER, RECTANGLE_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERTICES), RECTANGLE_VERTICES,
               GL_STATIC_DRAW);

  unsigned int RECTANGLE_EBO;
  glGenBuffers(1, &RECTANGLE_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RECTANGLE_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDICIES),
               RECTANGLE_INDICIES, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int TRIANGLE_COLOR_VAO;
  glGenVertexArrays(1, &TRIANGLE_COLOR_VAO);
  glBindVertexArray(TRIANGLE_COLOR_VAO);

  unsigned int TRIANGLE_COLOR_VBO;
  glGenBuffers(1, &TRIANGLE_COLOR_VBO);

  glBindBuffer(GL_ARRAY_BUFFER, TRIANGLE_COLOR_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLE_COLOR_VERTICES),
               TRIANGLE_COLOR_VERTICES, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int CUBE_VAO;
  glGenVertexArrays(1, &CUBE_VAO);
  glBindVertexArray(CUBE_VAO);

  unsigned int CUBE_VBO;
  glGenBuffers(1, &CUBE_VBO);

  glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  shader.use();

  int texture0UniformLocation =
      glGetUniformLocation(shader.program, "uTexture0");
  glUniform1i(texture0UniformLocation, 0);
  int texture1UniformLocation =
      glGetUniformLocation(shader.program, "uTexture1");
  glUniform1i(texture1UniformLocation, 1);

  glEnable(GL_DEPTH_TEST);

  glm::vec3 cubePositions[10] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  float startTime = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    float currFrame = glfwGetTime();
    dt = currFrame - lastFrame;
    lastFrame = currFrame;

    proccesInput(window);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float timePassed = glfwGetTime() - startTime;
    shader.setFloat("uTime", timePassed);

    glm::mat4 view = glm::mat4(1.0f);
    view = camera.getViewMatrix();
    shader.setMatrix4fv("uView", view);

    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(camera.fov),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMatrix4fv("uProjection", projection);

    glBindVertexArray(CUBE_VAO);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = (i + 1) * timePassed * 25.0f;

      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

      shader.setMatrix4fv("uModel", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glBindVertexArray(TRIANGLE_COLOR_VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // glBindVertexArray(RECTANGLE_VAO);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void proccesInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  bool sprint = false;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    sprint = true;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.handleKeyboardInput(FORWARD, dt, sprint);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.handleKeyboardInput(BACKWARD, dt, sprint);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.handleKeyboardInput(LEFT, dt, sprint);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.handleKeyboardInput(RIGHT, dt, sprint);
  }
}

void mouseInputCallback(GLFWwindow *window, double mouseX, double mouseY) {

  if (firstMouseInput) {
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    firstMouseInput = false;
  }

  float xOffset = mouseX - lastMouseX;
  float yOffset = lastMouseY - mouseY;
  lastMouseX = mouseX;
  lastMouseY = mouseY;

  camera.handleMouseInput(xOffset, yOffset);
}

void scrollInputCallback(GLFWwindow *window, double xOffset, double yOffset) {
  camera.handleScrollInput(yOffset, 10.0f, 45.0f);
}
