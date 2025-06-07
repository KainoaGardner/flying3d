#version 330 core

out vec4 fragColor;

in vec2 vTexCoord;

uniform float uTime;
uniform sampler2D uTexture0;

void main() {
    // vec2 vTexCoordUpdate = (vTexCoord) / vec2(4.0, 3.0);
    // vec3 color = texture(uTexture0, vTexCoordUpdate).rgb;
    // color = mix(color, vec3(0.0), 0.5);
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    fragColor = color;
}
