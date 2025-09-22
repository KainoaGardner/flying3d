#include <typeinfo>

#include "../include/glad/glad.h"

#include "../include/bullet.h"
#include "../include/config.h"
#include "../include/display.h"
#include "../include/geometry.h"
#include "../include/boss.h"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/textures.h"

#include "../include/key.h"
#include "../include/particle.h"
#include "../include/player.h"
#include "../include/shader.h"
#include "../include/stb_image.h"
#include "../include/text.h"
#include "../include/utils.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Player *player);
void keyInputCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods);

glm::vec3 bossPos = glm::vec3(0.0f);

void update(Player *player, Boss *boss);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  unsigned int weapons[2] = {player::flameThrower, player::bombLauncher};
  GLFWwindow *window =
      glfwCreateWindow(config::gameConfig.width, config::gameConfig.height,
                       "Learn Opengl", NULL, NULL);

  Player player(glm::vec3(0.0f, 0.0f, 500.0f), global::cameraUp,
                global::cameraFront, global::cameraOrientation,
                player::normalShip, weapons);

  Cube boss(glm::vec3(0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3(50.0f), boss::cube.health);

  player.boss = &boss;
  boss.player = &player;

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

  Shader healthShader("./assets/shaders/screen.vert",
                      "./assets/shaders/health.frag");

  Shader reloadShader("./assets/shaders/screen.vert",
                      "./assets/shaders/reload.frag");

  Shader cooldownShader("./assets/shaders/screen.vert",
                        "./assets/shaders/cooldown.frag");

  Shader screenShader("./assets/shaders/screen.vert",
                      "./assets/shaders/screen.frag");

  Shader textShader("./assets/shaders/text.vert", "./assets/shaders/text.frag");

  shader::shader = {
      .normal = &shader,
      .bullet = &bulletShader,
      .cubemap = &cubemapShader,
      .arrow = &arrowShader,
      .health = &healthShader,
      .reload = &reloadShader,
      .cooldown = &cooldownShader,
      .screen = &screenShader,
      .text = &textShader,
  };

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

  text::getFont("./assets/fonts/arcade.ttf");

  textures::texture.space0 = skyboxTexture;
  textures::texture.cube = cubeTexture;

  glActiveTexture(GL_TEXTURE0);

  Geometry cubeGeometry = createCube();
  Geometry beamGeometry = createBeam();
  Geometry skyboxGeometry = createCubemap();
  Geometry screenGeometry = createScreen();
  Geometry textGeometry = createText();

  geometry::geometry = {
      .cube = cubeGeometry,
      .beam = beamGeometry,
      .skybox = skyboxGeometry,
      .screen = screenGeometry,
      .text = textGeometry,
  };

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

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    config::gameConfig.width = fbWidth;
    config::gameConfig.height = fbHeight;

    if (currentTime - config::gameConfig.lastUpdateTime >=
        config::gameConfig.logicIntervalTime) {
      update(&player, &boss);
      config::gameConfig.lastUpdateTime += config::gameConfig.logicIntervalTime;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    float timePassed = glfwGetTime() - startTime;
    shader::shader.normal->setFloat("uTime", timePassed);

    glm::mat4 view = glm::mat4(1.0f);
    view = player.getViewMatrix();

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(player.fov),
                                  (float)config::gameConfig.width /
                                      (float)config::gameConfig.height,
                                  0.1f, 10000.0f);

    glm::mat4 textProjection =
        glm::ortho(0.0f, float(fbWidth), 0.0f, float(fbHeight));


    player.updateDisplayContext(projection,view,textProjection);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                    glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // display


    displayCubeMap(view, projection);


    boss.display();
    // display enemies

    displayBullets(&player, timePassed);
    displayParticles();

    displayScreen(textureColorBuffer);

    player.displayScreen();
    boss.displayScreen(player.displayContext);

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

void update(Player *player, Boss *boss) {
  player->update();

  boss->update(player);
  boss::bossPosition = boss->position;

  // std::cout << projectiles.size() <<std::endl;
  for (auto it = projectiles.begin(); it != projectiles.end();) {
    Projectile &projectile = *it;
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

      projectile.bullet->outOfBoundsBullet(player->position);
      if (!projectile.bullet->alive) {
        if (auto c = dynamic_cast<BombBullet*>(projectile.bullet.get())){
          c->explode();
        }

        it = projectiles.erase(it);
      } else {
        ++it;
      }
    }
  }

  for (unsigned int i = 0; i < particles.size(); i++) {
    ParticleList &particle = particles[i];
    if (particle.particle) {
      particle.particle->update();
      if (!particle.particle->alive) {
        particles.erase(particles.begin() + i);
        continue;
      };
    }
  }

  for (unsigned int i = 0; i < damageTextParticles.size(); i++) {
    DamageText &damageText = damageTextParticles[i];
    damageText.update();
    if (!damageText.alive) {
      damageTextParticles.erase(damageTextParticles.begin() + i);
      continue;
    };
  }

};
