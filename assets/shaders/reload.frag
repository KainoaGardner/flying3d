#version 330 core

out vec4 fragColor;
in vec2 vTexCoord;

uniform vec2 uResolution;
uniform float uShootCounter;
uniform float uShootCooldown;

#define M_PI 3.1415926535897932384626433832795

vec3 color = vec3(1.0);

float scale = 0.1;
float size = 1.0 * scale;
float borderSize = 0.1 * scale;

void main() {
    vec2 uv = vTexCoord;
    uv = uv * 2.0 - 1.0;
    uv.x *= uResolution.x / uResolution.y;

    float r = length(uv);
    float a = atan(uv.y, uv.x) + M_PI;
    a = mod(a + 1.5 * M_PI, 2.0 * M_PI);

    float reloadAngle = uShootCounter / uShootCooldown * M_PI * 2.0;

    float inCircle = step(r, size) - step(r, size - borderSize);
    float progress = step(a, reloadAngle);
    float result = inCircle * progress;

    vec2 translate = vec2(0.0, -0.9);

    fragColor = vec4(color, result * 0.5);
}
