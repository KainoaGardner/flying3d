
#include "../include/bullet.h"
#include "../include/camera.h"
#include "../include/geomety.h"
#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/shader.h"
#include "../include/stb_image.h"
#include "../include/utils.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void proccesInput(GLFWwindow *window);
void mouseInputCallback(GLFWwindow *window, double mouseX, double mouseY);
// void scrollInputCallback(GLFWwindow *window, double xOffset, double yOffset);

// const unsigned int SCR_WIDTH = 1920;
// const unsigned int SCR_HEIGHT = 1080;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// const unsigned int SCR_WIDTH = 640;
// const unsigned int SCR_HEIGHT = 360;

float dt = 0.0f;
float lastFrame = 0.0f;

float lastMouseX = SCR_WIDTH / 2.0f;
float lastMouseY = SCR_HEIGHT / 2.0f;

bool firstMouseInput = true;

const float sensitivity = 0.3f;

Camera camera(CAMERA_POSITION, CAMERA_UP, CAMERA_FRONT, CAMERA_ORIENTATION,
              SPEED, MAX_SPEED, TURN_SPEED, SENSITIVITY, FOV);

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
  // glfwSetCursorPosCallback(window, mouseInputCallback);
  // glfwSetScrollCallback(window, scrollInputCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Shader shader("./assets/shaders/vertex.vert",
                "./assets/shaders/fragment.frag");
  Shader bulletShader("./assets/shaders/vertex.vert",
                      "./assets/shaders/bullet.frag");

  Shader cubemapShader("./assets/shaders/cubemap.vert",
                       "./assets/shaders/cubemap.frag");

  Shader arrowShader("./assets/shaders/screen.vert",
                     "./assets/shaders/arrow.frag");

  Shader screenShader("./assets/shaders/screen.vert",
                      "./assets/shaders/screen.frag");

  shader.use();

  // textures
  stbi_set_flip_vertically_on_load(true);

  std::vector<std::string> skyboxTextures = {
      "./assets/imgs/cubemaps/gray/right.png",
      "./assets/imgs/cubemaps/gray/left.png",
      "./assets/imgs/cubemaps/gray/top.png",
      "./assets/imgs/cubemaps/gray/bottom.png",
      "./assets/imgs/cubemaps/gray/front.png",
      "./assets/imgs/cubemaps/gray/back.png",
  };

  stbi_set_flip_vertically_on_load(false);
  unsigned int skyboxTexture = loadCubemap(skyboxTextures);
  unsigned int cubeTexture = loadTexture("./assets/imgs/box.jpg");

  glActiveTexture(GL_TEXTURE0);

  Geometry cubeGeometry = createCube();
  Geometry skyboxGeometry = createCubemap();
  Geometry screenGeometry = createScreen();

  unsigned int matrixUniformBlockMain =
      glGetUniformBlockIndex(shader.program, "Matrices");
  unsigned int matrixUniformBlockBullet =
      glGetUniformBlockIndex(bulletShader.program, "Matrices");

  glUniformBlockBinding(shader.program, matrixUniformBlockMain, 0);
  glUniformBlockBinding(bulletShader.program, matrixUniformBlockBullet, 0);

  unsigned int uboMatrices;
  glGenBuffers(1, &uboMatrices);

  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0,
                    2 * sizeof(glm::mat4));

  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  unsigned int textureColorBuffer;
  glGenTextures(1, &textureColorBuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH,
                        SCR_HEIGHT);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         textureColorBuffer, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR Framebuffer not complete" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float bossShootCooldown = 0.1f;
  float bossShootCounter = 0.0f;
  float startTime = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    float currFrame = glfwGetTime();
    dt = currFrame - lastFrame;
    lastFrame = currFrame;

    proccesInput(window);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    camera.update(dt);
    // camera.updateCameraMovement(dt);

    glm::vec3 bossPos = glm::vec3(0.0f);
    bossShootCounter += dt;
    if (bossShootCounter >= bossShootCooldown) {
      glm::vec3 toPlayer = glm::normalize(camera.position - bossPos);
      toPlayer.x += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
      toPlayer.y += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
      toPlayer.z += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;

      Bullet bullet = Bullet(bossPos, toPlayer, toPlayer, glm::vec3(5.0f),
                             glm::vec3(1.0f, 0.0f, 0.0f), 0.5f, 1.0f);
      bullets.push_back(bullet);
      bossShootCounter = 0.0f;
    }

    float timePassed = glfwGetTime() - startTime;
    shader.setFloat("uTime", timePassed);

    glm::mat4 view = glm::mat4(1.0f);
    view = camera.getViewMatrix(bossPos);

    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(camera.fov),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                    glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(projection));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    glBindVertexArray(skyboxGeometry.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

    cubemapShader.use();
    cubemapShader.setMatrix4fv("uProjection", projection);
    glm::mat4 noTranslateView = glm::mat4(glm::mat3(view));

    cubemapShader.setMatrix4fv("uView", noTranslateView);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(cubeGeometry.vao);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
    // model = glm::rotate(model, timePassed * 3.0f,
    //                     glm::normalize(glm::vec3(2.0f, 0.0f, 1.0)));
    model = glm::scale(model, glm::vec3(50.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    shader.use();
    shader.setMatrix4fv("uModel", model);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    bulletShader.use();

    for (unsigned int i = 0; i < bullets.size(); i++) {
      Bullet &bullet = bullets[i];
      bullet.update(dt);
      bullet.killBullet(camera.position);
      if (!bullet.alive) {
        bullets.erase(bullets.begin() + i);
        continue;
      };

      model = glm::mat4(1.0f);
      model = glm::translate(model, bullet.position);
      model = glm::rotate(model, timePassed * 25.0f,
                          glm::normalize(bullet.rotation));
      model = glm::scale(model, bullet.scale);

      bulletShader.setMatrix4fv("uModel", model);
      bulletShader.setVec3f("uColor", bullet.color);

      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    screenShader.use();
    glBindVertexArray(screenGeometry.vao);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glm::vec4 bossClip = projection * view * glm::vec4(bossPos, 1.0f);
    bossClip /= bossClip.w;

    bool onScreen = bossClip.x >= -1.0f && bossClip.x <= 1.0f &&
                    bossClip.y >= -1.0f && bossClip.y <= 1.0f &&
                    bossClip.z >= -1.0f && bossClip.z <= 1.0f;

    glm::vec3 toBoss = bossPos - camera.position;
    glm::vec3 bossDirView = glm::mat3(view) * toBoss;

    glm::vec2 arrowDir = glm::normalize(glm::vec2(bossDirView));

    if (!onScreen) {
      arrowShader.use();
      arrowShader.setVec2f("uArrowPos", arrowDir);
      arrowShader.setVec2f("uResolution", glm::vec2(SCR_WIDTH, SCR_HEIGHT));

      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  camera.handleKeyboardInput(window, dt);
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

// void scrollInputCallback(GLFWwindow *window, double xOffset, double
// yOffset)
// {
//   camera.handleScrollInput(yOffset, 10.0f, 45.0f);
// }
