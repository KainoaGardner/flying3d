#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

unsigned int loadTexture(char const *path);
unsigned int loadCubemap(std::vector<std::string> faces);

#endif
