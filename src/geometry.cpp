#include "../include/geomety.h"

float CUBE_VERTICES[120] = {
    // front back
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // A 0
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // B 1
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // C 2
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // D 3
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // E 4
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // F 5
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // G 6
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // H 7

    // up down
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  // D 8
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // A 9
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,  // E 10
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // H 11
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // B 12
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f,   // C 13
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // G 14
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f,   // F 15

    // left right
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // A 16
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // B 17
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f,   // F 18
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,  // E 19
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f,   // C 20
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f,  // D 21
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // H 22
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // G 23
};

float CUBE_MAP_VERTICES[72] = {
    -0.5f, -0.5f, -0.5f, // arsoienarsietn
    0.5f,  -0.5f, -0.5f, // arsoienarsietn
    0.5f,  0.5f,  -0.5f, // arsoienarsietn
    -0.5f, 0.5f,  -0.5f, // arsoienarsietn
    -0.5f, -0.5f, 0.5f,  // arsoienarsietn
    0.5f,  -0.5f, 0.5f,  // arsoienarsietn
    0.5f,  0.5f,  0.5f,  // arsoienarsietn
    -0.5f, 0.5f,  0.5f,  // arsoienarsietn

    -0.5f, 0.5f,  -0.5f, // arsoienarsietn
    -0.5f, -0.5f, -0.5f, // arsoienarsietn
    -0.5f, -0.5f, 0.5f,  // arsoienarsietn
    -0.5f, 0.5f,  0.5f,  // arsoienarsietn
    0.5f,  -0.5f, -0.5f, // arsoienarsietn
    0.5f,  0.5f,  -0.5f, // arsoienarsietn
    0.5f,  0.5f,  0.5f,  // arsoienarsietn
    0.5f,  -0.5f, 0.5f,  // arsoienarsietn

    -0.5f, -0.5f, -0.5f, // arsoienarsietn
    0.5f,  -0.5f, -0.5f, // arsoienarsietn
    0.5f,  -0.5f, 0.5f,  // arsoienarsietn
    -0.5f, -0.5f, 0.5f,  // arsoienarsietn
    0.5f,  0.5f,  -0.5f, // arsoienarsietn
    -0.5f, 0.5f,  -0.5f, // arsoienarsietn
    -0.5f, 0.5f,  0.5f,  // arsoienarsietn
    0.5f,  0.5f,  0.5f,  // arsoienarsietn
};

unsigned int CUBE_INDICES[36] = {
    // front and back
    0, 3, 2, 2, 1, 0, 4, 5, 6, 6, 7, 4,
    // left and right
    11, 8, 9, 9, 10, 11, 12, 13, 14, 14, 15, 12,
    // bottom and top
    16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

float bHalf = LASER_LENGTH / 2.0f;
float b = LASER_LENGTH;
float BEAM_VERTICES[120] = {
    // front back
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // A 0
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // B 1
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // C 2
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // D 3
    -0.5f, -0.5f, -b, 0.0f, 0.0f,   // E 4
    0.5f, -0.5f, -b, 1.0f, 0.0f,    // F 5
    0.5f, 0.5f, -b, 1.0f, 1.0f,     // G 6
    -0.5f, 0.5f, -b, 0.0f, 1.0f,    // H 7

    // up down
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  // D 8
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // A 9
    -0.5f, -0.5f, -b, 1.0f, 1.0f,   // E 10
    -0.5f, 0.5f, -b, 0.0f, 1.0f,    // H 11
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // B 12
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // C 13
    0.5f, 0.5f, -b, 1.0f, 1.0f,     // G 14
    0.5f, -0.5f, -b, 0.0f, 1.0f,    // F 15

    // left right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // A 16
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // B 17
    0.5f, -0.5f, -b, 1.0f, 1.0f,    // F 18
    -0.5f, -0.5f, -b, 0.0f, 1.0f,   // E 19
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f,   // C 20
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // D 21
    -0.5f, 0.5f, -b, 1.0f, 1.0f,    // H 22
    0.5f, 0.5f, -b, 0.0f, 1.0f,     // G 23
};

// float BEAM_VERTICES[120] = {
//     // front back
//     -0.5f, -0.5f, -bHalf, 0.0f, 0.0f, // A 0
//     0.5f, -0.5f, -bHalf, 1.0f, 0.0f,  // B 1
//     0.5f, 0.5f, -bHalf, 1.0f, 1.0f,   // C 2
//     -0.5f, 0.5f, -bHalf, 0.0f, 1.0f,  // D 3
//     -0.5f, -0.5f, bHalf, 0.0f, 0.0f,  // E 4
//     0.5f, -0.5f, bHalf, 1.0f, 0.0f,   // F 5
//     0.5f, 0.5f, bHalf, 1.0f, 1.0f,    // G 6
//     -0.5f, 0.5f, bHalf, 0.0f, 1.0f,   // H 7
//
//     // up down
//     -0.5f, 0.5f, -bHalf, 0.0f, 0.0f,  // D 8
//     -0.5f, -0.5f, -bHalf, 1.0f, 0.0f, // A 9
//     -0.5f, -0.5f, bHalf, 1.0f, 1.0f,  // E 10
//     -0.5f, 0.5f, bHalf, 0.0f, 1.0f,   // H 11
//     0.5f, -0.5f, -bHalf, 0.0f, 0.0f,  // B 12
//     0.5f, 0.5f, -bHalf, 1.0f, 0.0f,   // C 13
//     0.5f, 0.5f, bHalf, 1.0f, 1.0f,    // G 14
//     0.5f, -0.5f, bHalf, 0.0f, 1.0f,   // F 15
//
//     // left right
//     -0.5f, -0.5f, -bHalf, 0.0f, 0.0f, // A 16
//     0.5f, -0.5f, -bHalf, 1.0f, 0.0f,  // B 17
//     0.5f, -0.5f, bHalf, 1.0f, 1.0f,   // F 18
//     -0.5f, -0.5f, bHalf, 0.0f, 1.0f,  // E 19
//     0.5f, 0.5f, -bHalf, 0.0f, 0.0f,   // C 20
//     -0.5f, 0.5f, -bHalf, 1.0f, 0.0f,  // D 21
//     -0.5f, 0.5f, bHalf, 1.0f, 1.0f,   // H 22
//     0.5f, 0.5f, bHalf, 0.0f, 1.0f,    // G 23
// };

float SCREEN_VERTICES[20] = {
    // positions                     // texture coords
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top right
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f  // top left
};

unsigned int SCREEN_INDICES[6] = {
    0, 1, 3, // first tri
    1, 2, 3, // second tri
};

Geometry createCubemap() {
  Geometry geometry;
  glGenVertexArrays(1, &geometry.vao);
  glBindVertexArray(geometry.vao);

  glGenBuffers(1, &geometry.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_MAP_VERTICES), CUBE_MAP_VERTICES,
               GL_STATIC_DRAW);

  glGenBuffers(1, &geometry.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CUBE_INDICES), CUBE_INDICES,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  return geometry;
}

Geometry createCube() {
  Geometry geometry;
  glGenVertexArrays(1, &geometry.vao);
  glBindVertexArray(geometry.vao);

  glGenBuffers(1, &geometry.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES,
               GL_STATIC_DRAW);

  glGenBuffers(1, &geometry.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CUBE_INDICES), CUBE_INDICES,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  return geometry;
}

Geometry createScreen() {
  Geometry geometry;
  glGenVertexArrays(1, &geometry.vao);
  glBindVertexArray(geometry.vao);

  glGenBuffers(1, &geometry.vbo);

  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_VERTICES), SCREEN_VERTICES,
               GL_STATIC_DRAW);

  glGenBuffers(1, &geometry.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SCREEN_INDICES), SCREEN_INDICES,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  return geometry;
}

Geometry createBeam() {
  Geometry geometry;
  glGenVertexArrays(1, &geometry.vao);
  glBindVertexArray(geometry.vao);

  glGenBuffers(1, &geometry.vbo);

  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(BEAM_VERTICES), BEAM_VERTICES,
               GL_STATIC_DRAW);

  glGenBuffers(1, &geometry.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CUBE_INDICES), CUBE_INDICES,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  return geometry;
}
