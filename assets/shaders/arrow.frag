#version 330 core

out vec4 fragColor;
in vec2 vTexCoord;

uniform vec2 uResolution;
uniform vec2 uArrowPos;
uniform sampler2D uTexture0;

float checkCircle(vec2 pos, vec2 circlePos, float r) {
    float dist = distance(pos, circlePos);
    return step(dist, r);
}

void main() {
    vec2 uv = vTexCoord;
    uv = uv * 2.0 - 1.0;

    uv.x *= uResolution.x / uResolution.y;

    float inCircle = checkCircle(uv, uArrowPos, 0.2);
    // fragColor = vec4(vec3(1.0), inCircle);
    fragColor = vec4(vec3(0.0, 1.0, 0.0), inCircle * 0.3);
}
