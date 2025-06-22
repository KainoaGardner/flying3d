#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aIndex;

out vec2 vTexCoord;

uniform mat4 uProjection;
uniform int uTileCols;
uniform int uTileRows;

void main() {
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);

    int col = int(mod(aIndex, uTileCols));
    int row = int(aIndex / uTileCols);

    vec2 tileSize = vec2(1.0 / uTileCols, 1.0 / uTileRows);
    vec2 tileOffset = vec2(tileSize.x * col, tileSize.y * row);

    vTexCoord = aTexCoord * tileSize + tileOffset;
}
