#version 330 core

out vec4 fragColor;
in vec2 vTexCoord;

uniform vec2 uResolution;
uniform vec2 uPosition;
uniform vec2 uScale;
uniform float uMaxHealth;
uniform float uCurrentHealth;


vec3 color = vec3(0.9059f, 0.2980f, 0.2353f);

float checkRect(vec2 pos, vec2 translate, vec2 scale) {
    vec2 result = step(translate - scale / 2, pos) - step(translate + scale / 2, pos);
    return result.x * result.y;
}

void main() {
    vec2 uv = vTexCoord;
    uv = uv * 2.0 - 1.0;
    uv.x *= uResolution.x / uResolution.y;


    float width = (uCurrentHealth / uMaxHealth);
    vec2 size = vec2(uScale.x * width,uScale.y);
    float inRect = checkRect(uv, uPosition, size);

    fragColor = vec4(color, inRect);
}
