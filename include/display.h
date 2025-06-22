#ifndef DISPLAY_H
#define DISPLAY_H

#include "../include/bullet.h"
#include "../include/geomety.h"
#include "../include/player.h"
#include "../include/shader.h"
#include "../include/textures.h"

void displayCubeMap(glm::mat4 view, glm::mat4 projection);

void displayBullets(Player *player, float timePassed);

void displayParticles();

void displayScreen(unsigned int colorTexture);
void renderChar(char c, float x, float y, glm::vec2 size);
void renderText(glm::mat4 projection, std::string text, float x, float y,
                float size, glm::vec3 color);

#endif
