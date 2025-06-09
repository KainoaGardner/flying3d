#version 330 core

out vec4 fragColor;
in vec2 vTexCoord;

uniform sampler2D uTexture0;

void main() {
    vec3 color = texture(uTexture0, vTexCoord).rgb;
    fragColor = vec4(color, 1.0);
}
