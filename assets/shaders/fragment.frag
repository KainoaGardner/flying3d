#version 330 core

out vec4 fragColor;

in vec2 vTexCoord;

uniform float uTime;
uniform sampler2D uTexture0;

void main() {
    vec4 color = texture(uTexture0, vTexCoord);
    fragColor = color;
}
