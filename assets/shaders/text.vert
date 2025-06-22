#version 330 core
layout(location = 0) in vec4 aVertex;

out vec2 vTexCoord;

uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * vec4(aVertex.xy, 0.0, 1.0);
    vTexCoord = aVertex.zw;
}
