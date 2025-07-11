#include "../include/text.h"

namespace text {
void getFont(std::string fontPath) {
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cout << "Error could not make freetype" << std::endl;
    FT_Done_FreeType(ft);
    return;
  }

  FT_Face face;
  if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
    std::cout << "Error could not find font" << std::endl;
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return;
  }

  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "Could not make " << c << std::endl;
      continue;
    }

    float width = face->glyph->bitmap.width;
    float height = face->glyph->bitmap.rows;
    unsigned char *buffer = face->glyph->bitmap.buffer;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
                 GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x,
    };

    characters.insert(std::pair<char, Character>(c, character));
  }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

std::map<char, Character> characters;
} // namespace text
