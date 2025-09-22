#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include "../include/player.h"

void displayCubeMap(glm::mat4 view, glm::mat4 projection);

void displayBullets(Player *player, float timePassed);

void displayParticles();

void displayScreen(unsigned int colorTexture);
void renderText(glm::mat4 projection, std::string text, float x, float y,
                float size, glm::vec3 color);

#endif
