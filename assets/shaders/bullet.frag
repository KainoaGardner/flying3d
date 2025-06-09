#version 330 core

out vec4 fragColor;

in vec2 vTexCoord;

uniform float uTime;
uniform vec3 uColor;

float checkBorder(vec2 pos, float width) {
    vec2 result = step(vec2(width), pos) - step(1.0 - vec2(width), pos);
    return 1.0 - (result.x * result.y);
}

void main() {
    vec3 color = uColor;
    float border = checkBorder(vTexCoord, 0.1);
    color = mix(color, vec3(0.0), border);
    fragColor = vec4(color, 1.0);
}
