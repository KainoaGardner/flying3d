#include "../include/display.h"

void displayCubeMap(glm::mat4 view, glm::mat4 projection) {
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);

  glBindVertexArray(geometry::geometry.cube.vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textures::texture.space0);

  shader::shader.cubemap->use();
  shader::shader.cubemap->setMatrix4fv("uProjection", projection);
  glm::mat4 noTranslateView = glm::mat4(glm::mat3(view));

  shader::shader.cubemap->setMatrix4fv("uView", noTranslateView);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
}

void displayBullets(Player *player, float timePassed) {
  shader::shader.bullet->use();
  glBindVertexArray(geometry::geometry.cube.vao);

  for (unsigned int i = 0; i < projectiles.size(); i++) {
    Projectile &projectile = projectiles[i];
    if (projectile.bullet) {
      projectile.bullet->draw();
    }
  }

  glBindVertexArray(geometry::geometry.beam.vao);
  if (!(player->ship == player::tankShip && player->abilityTimer > 0.0f)) {
    player->laser->draw(timePassed);

    player->blade->draw(timePassed);
  }
}

void displayParticles() {
  for (unsigned int i = 0; i < particles.size(); i++) {
    Particle &particle = particles[i];
    if (particle.explosion) {
      particle.explosion->draw();
    }
  }
}

void displayScreen(unsigned int colorTexture) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader::shader.screen->use();
  glBindVertexArray(geometry::geometry.screen.vao);
  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, colorTexture);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void renderChar(char c, float x, float y, glm::vec2 size) {

  float tileIndex = float(c - 32);

  float w = size.x;
  float h = size.y;

  float vertices[6][5] = {
      {x, y + h, 0.0f, 0.0f, tileIndex},     {x, y, 0.0f, 1.0f, tileIndex},
      {x + w, y, 1.0f, 1.0f, tileIndex},

      {x, y + h, 0.0f, 0.0f, tileIndex},     {x + w, y, 1.0f, 1.0f, tileIndex},
      {x + w, y + h, 1.0f, 0.0f, tileIndex},
  };

  glBindBuffer(GL_ARRAY_BUFFER, geometry::geometry.text.vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glBindVertexArray(geometry::geometry.text.vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderText(glm::mat4 projection, std::string text, float x, float y,
                float size, glm::vec3 color) {

  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures::texture.text);

  glm::vec2 scale = glm::vec2(32.0f / 60.0f, 1.0f) * size;
  shader::shader.text->use();
  shader::shader.text->setMatrix4fv("uProjection", projection);
  shader::shader.text->setInt("uTileCols", 24);
  shader::shader.text->setInt("uTileRows", 4);

  shader::shader.text->setVec3f("uTextColor", color);

  for (char c : text) {
    renderChar(c, x, y, scale);
    x += scale.x;
  }
}
