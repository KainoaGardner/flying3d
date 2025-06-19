#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

#include "../include/bullet.h"
#include "../include/config.h"
#include "../include/display.h"
#include "../include/geomety.h"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/key.h"
#include "../include/particle.h"
#include "../include/player.h"
#include "../include/shader.h"
#include "../include/stb_image.h"
#include "../include/utils.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Player *player);
void keyInputCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods);

glm::vec3 bossPos = glm::vec3(0.0f);
float bossShootCooldown = 10.0f;
float bossShootCounter = 0.0f;

void update(Player *player);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  unsigned int weapons[2] = {player::machineGun, player::shotGun};
  GLFWwindow *window =
      glfwCreateWindow(config::gameConfig.width, config::gameConfig.height,
                       "Learn Opengl", NULL, NULL);

  Player player(glm::vec3(0.0f, 10.0f, 0.0f), global::cameraUp,
                global::cameraFront, global::cameraOrientation,
                player::vampireShip, weapons);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, keyInputCallback);
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
  Geometry beamGeometry = createBeam();
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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config::gameConfig.width,
               config::gameConfig.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        config::gameConfig.width, config::gameConfig.height);
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

  float startTime = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    float currentTime = glfwGetTime();
    processInput(window, &player);

    if (currentTime - config::gameConfig.lastUpdateTime >=
        config::gameConfig.logicIntervalTime) {
      update(&player);
      config::gameConfig.lastUpdateTime += config::gameConfig.logicIntervalTime;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    float timePassed = glfwGetTime() - startTime;
    shader.setFloat("uTime", timePassed);

    glm::mat4 view = glm::mat4(1.0f);
    view = player.getViewMatrix(bossPos);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(player.fov),
                                  (float)config::gameConfig.width /
                                      (float)config::gameConfig.height,
                                  0.1f, 10000.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                    glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // display
    displayCubeMap(&cubemapShader, skyboxGeometry, skyboxTexture, view,
                   projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(50.0f));

    // display boss
    glBindVertexArray(cubeGeometry.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    shader.use();
    shader.setMatrix4fv("uModel", model);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    // display enemies

    displayBullets(&bulletShader, cubeGeometry, beamGeometry, &player,
                   timePassed);
    displayParticles(&bulletShader, cubeGeometry);

    displayScreen(&screenShader, screenGeometry, textureColorBuffer);

    glm::vec4 bossClip = projection * view * glm::vec4(bossPos, 1.0f);
    bossClip /= bossClip.w;

    bool onScreen = bossClip.x >= -1.0f && bossClip.x <= 1.0f &&
                    bossClip.y >= -1.0f && bossClip.y <= 1.0f &&
                    bossClip.z >= -1.0f && bossClip.z <= 1.0f;

    glm::vec3 toBoss = bossPos - player.position;
    glm::vec3 bossDirView = glm::mat3(view) * toBoss;

    glm::vec2 arrowDir = glm::normalize(glm::vec2(bossDirView));

    if (!onScreen) {
      arrowShader.use();
      arrowShader.setVec2f("uArrowPos", arrowDir);
      arrowShader.setVec2f("uResolution", glm::vec2(config::gameConfig.width,
                                                    config::gameConfig.height));

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

void processInput(GLFWwindow *window, Player *player) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void keyInputCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods) {

  auto it = keys::keyToAction.find(key);
  if (it != keys::keyToAction.end()) {
    const keys::actions actionName = it->second;

    if (action == GLFW_PRESS) {

      keys::actionPressed[actionName] = true;
    } else if (action == GLFW_RELEASE) {
      keys::actionPressed[actionName] = false;
    }
  }
}

void update(Player *player) {
  player->update();
  player->handleKeyboardInput();

  bossShootCounter += 1.0f * player->timeSlowAmount;
  if (bossShootCounter >= bossShootCooldown) {
    glm::vec3 toPlayer = glm::normalize(player->position - bossPos);
    toPlayer.x += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
    toPlayer.y += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
    toPlayer.z += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;

    Projectile projectile;
    projectile.bullet = std::make_unique<Bullet>(
        bossPos, toPlayer, toPlayer, global::cameraOrientation, glm::vec3(5.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, true);
    projectiles.push_back(std::move(projectile));
    bossShootCounter = 0.0f;
  }

  for (unsigned int i = 0; i < projectiles.size(); i++) {
    Projectile &projectile = projectiles[i];
    if (projectile.bullet) {
      if (projectile.bullet->enemyBullet) {
        if (player->ship == player::timeShip) {
          float timeSlow =
              player->getBulletTimeSlow(projectile.bullet->position) *
              player->timeSlowAmount;
          projectile.bullet->update(timeSlow);
        } else {
          projectile.bullet->update(1.0f);
        }
      } else {
        projectile.bullet->update(1.0f);
      }

      projectile.bullet->killBullet(player->position);
      if (!projectile.bullet->alive) {
        projectiles.erase(projectiles.begin() + i);
        continue;
      };
    }
  }

  for (unsigned int i = 0; i < particles.size(); i++) {
    Particle &particle = particles[i];
    if (particle.explosion) {
      particle.explosion->update();
      if (!particle.explosion->alive) {
        particles.erase(particles.begin() + i);
        continue;
      };
    }
  }
};
