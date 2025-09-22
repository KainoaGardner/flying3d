#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../include/glm/glm.hpp"
#include <iostream>
#include <map>

namespace text {
struct Character {
  unsigned int textureId;
  glm::ivec2 size;
  glm::ivec2 bearing;
  long advance;
};

void getFont(std::string fontPath);

extern std::map<char, Character> characters;
} // namespace text

#endif
