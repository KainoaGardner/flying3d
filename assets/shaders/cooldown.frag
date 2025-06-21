#version 330 core

out vec4 fragColor;
in vec2 vTexCoord;

uniform vec2 uTranslate;
uniform vec2 uResolution;
uniform float uTimer;
uniform float uCooldown;

#define M_PI 3.1415926535897932384626433832795

vec3 color = vec3(1.0);
vec3 darkColor = vec3(0.0);

float size = 0.25;
float borderSize = 0.01;
float opacity = 0.9;

float checkRect(vec2 pos, vec2 translate, vec2 scale) {
    float resultY = step(translate.y, pos.y) - step(translate.y + scale.y, pos.y);
    float resultX = step(translate.x - scale.x * 0.5, pos.x) - step(translate.x + scale.x * 0.5, pos.x);
    return resultX * resultY;
}

float checkRectCenter(vec2 pos, vec2 translate, vec2 scale) {
    vec2 result = step(translate - scale * 0.5, pos) - step(translate + scale * 0.5, pos);
    return result.x * result.y;
}

void main() {
    vec2 uv = vTexCoord;
    uv = uv * 2.0 - 1.0;
    uv.x *= uResolution.x / uResolution.y;

    float progress = 1.0 - min(uTimer / uCooldown, 1.0);
    float finished = clamp(progress, 0.0, 1.0);

    vec2 translate = uTranslate;
    vec2 scale = vec2(1.0, progress) * size;
    float result = checkRect(uv, translate, scale);

    vec4 cooldownColor = vec4(darkColor, result * opacity);

    translate += vec2(0.0, 1.0) * scale.x * 0.5;
    float outsideBorder = checkRectCenter(uv, translate, vec2(1.0) * size);
    float insideBorder = checkRectCenter(uv, translate, vec2(1.0) * size - borderSize);

    result = outsideBorder - insideBorder;
    vec4 borderColor = vec4(color.xyz, 1.0);

    vec4 finalColor = mix(cooldownColor, borderColor, result);

    fragColor = vec4(finalColor);
}

vec2 translate = vec2(-0.75, -0.95);
