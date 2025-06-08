
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
  Shader cubemapShader("./assets/shaders/cubemap.vert",
                       "./assets/shaders/cubemap.frag");

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

  glActiveTexture(GL_TEXTURE0);

  Geometry cube = createCube();
  Geometry skybox = createCubemap();

  glEnable(GL_DEPTH_TEST);

  float bossShootCooldown = 0.05f;
  float bossShootCounter = 0.0f;
  float startTime = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    float currFrame = glfwGetTime();
    dt = currFrame - lastFrame;
    lastFrame = currFrame;

    proccesInput(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.update(dt);
    // camera.updateCameraMovement(dt);

    bossShootCounter += dt;
    if (bossShootCounter >= bossShootCooldown) {
      glm::vec3 bossPos = glm::vec3(0.0f);
      glm::vec3 toPlayer = glm::normalize(camera.position - bossPos);
      toPlayer.x += ((float(rand() % 100) / 100.0) - 0.5) / 2.0;
      toPlayer.y += ((float(rand() % 100) / 100.0) - 0.5) / 2.0;
      toPlayer.z += ((float(rand() % 100) / 100.0) - 0.5) / 2.0;

      Bullet bullet =
          Bullet(bossPos, toPlayer, toPlayer, glm::vec3(2.0f), 0.5f, 1.0f);
      bullets.push_back(bullet);
      bossShootCounter = 0.0f;
    }

    float timePassed = glfwGetTime() - startTime;
    shader.setFloat("uTime", timePassed);

    glm::mat4 view = glm::mat4(1.0f);
    view = camera.getViewMatrix();

    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(camera.fov),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    glBindVertexArray(skybox.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

    cubemapShader.use();
    cubemapShader.setMatrix4fv("uProjection", projection);
    glm::mat4 noTranslateView = glm::mat4(glm::mat3(view));

    cubemapShader.setMatrix4fv("uView", noTranslateView);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(cube.vao);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
    // model = glm::rotate(model, timePassed * 3.0f,
    //                     glm::normalize(glm::vec3(2.0f, 0.0f, 1.0)));
    model = glm::scale(model, glm::vec3(50.0f));

    shader.use();
    shader.setMatrix4fv("uView", view);
    shader.setMatrix4fv("uProjection", projection);
    shader.setMatrix4fv("uModel", model);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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

      shader.setMatrix4fv("uModel", model);

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

// void scrollInputCallback(GLFWwindow *window, double xOffset, double yOffset)
// {
//   camera.handleScrollInput(yOffset, 10.0f, 45.0f);
// }
