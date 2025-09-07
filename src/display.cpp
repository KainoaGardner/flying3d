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
  if (!(player->ship == player::tankShip && player->abilityTimer > 0.0f) &&
      player->alive) {
    player->laser->draw(timePassed);

    player->blade->draw(timePassed);
  }
}

void displayParticles() {
  glBindVertexArray(geometry::geometry.cube.vao);
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

void renderText(glm::mat4 projection, std::string text, float x, float y,
                float scale, glm::vec3 color) {

  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(geometry::geometry.text.vao);

  shader::shader.text->use();
  shader::shader.text->setMatrix4fv("uProjection", projection);
  shader::shader.text->setInt("uText", 0);
  shader::shader.text->setVec3f("uTextColor", color);

  std::string::const_iterator c;

  float totalX = 0.0f;
  for (c = text.begin(); c != text.end(); c++) {
    text::Character ch = text::characters[*c];
    totalX += (ch.advance >> 6) * scale;
  }

  x -= totalX / 2.0f;

  for (c = text.begin(); c != text.end(); c++) {
    text::Character ch = text::characters[*c];
    float xPos = x + ch.bearing.x * scale;
    float yPos = y + (ch.size.y - ch.bearing.y) * scale;
    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    float vertices[6][4] = {
        {xPos, yPos + h, 0.0f, 0.0f},    {xPos, yPos, 0.0f, 1.0f},
        {xPos + w, yPos, 1.0f, 1.0f},

        {xPos, yPos + h, 0.0f, 0.0f},    {xPos + w, yPos, 1.0f, 1.0f},
        {xPos + w, yPos + h, 1.0f, 0.0f}};

    glBindTexture(GL_TEXTURE_2D, ch.textureId);

    glBindBuffer(GL_ARRAY_BUFFER, geometry::geometry.text.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    x += (ch.advance >> 6) * scale;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
