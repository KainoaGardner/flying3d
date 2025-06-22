#version 330 core

out vec4 fragColor;
in vec2 vTexCoord;

uniform sampler2D uText;
uniform vec3 uTextColor;

void main() {
    float alpha = texture(uText, vTexCoord).r;
    alpha = step(0.9, alpha);
    fragColor = vec4(uTextColor, alpha);
}
