#ifndef DISPLAY_H
#define DISPLAY_H

#include "../include/bullet.h"
#include "../include/geomety.h"
#include "../include/player.h"
#include "../include/shader.h"

void displayCubeMap(Shader *shader, Geometry geometry, unsigned int texture,
                    glm::mat4 view, glm::mat4 projection);

void displayBullets(Shader *shader, Geometry cubeGeometry,
                    Geometry beamGeometry, Player *player, float timePassed);

void displayParticles(Shader *shader, Geometry geometry);

void displayScreen(Shader *shader, Geometry geometry,
                   unsigned int colorTexture);

#endif
