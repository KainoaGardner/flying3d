#include "../include/display.h"

void displayCubeMap(Shader *shader, Geometry geometry, unsigned int texture,
                    glm::mat4 view, glm::mat4 projection) {
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);

  glBindVertexArray(geometry.vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

  shader->use();
  shader->setMatrix4fv("uProjection", projection);
  glm::mat4 noTranslateView = glm::mat4(glm::mat3(view));

  shader->setMatrix4fv("uView", noTranslateView);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
}

void displayBullets(Shader *shader, Geometry cubeGeometry,
                    Geometry beamGeometry, Player *player, float timePassed) {
  shader->use();
  glBindVertexArray(cubeGeometry.vao);

  for (unsigned int i = 0; i < projectiles.size(); i++) {
    Projectile &projectile = projectiles[i];
    if (projectile.bullet) {
      projectile.bullet->draw(shader);
    }
  }

  glBindVertexArray(beamGeometry.vao);
  if (!(player->ship == player::tankShip && player->abilityTimer > 0.0f)) {
    player->laser->draw(shader, timePassed);

    player->blade->draw(shader, timePassed);
  }
}

void displayParticles(Shader *shader, Geometry geometry) {
  for (unsigned int i = 0; i < particles.size(); i++) {
    Particle &particle = particles[i];
    if (particle.explosion) {
      particle.explosion->draw(shader);
    }
  }
}

void displayScreen(Shader *shader, Geometry geometry,
                   unsigned int colorTexture) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  glBindVertexArray(geometry.vao);
  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, colorTexture);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
