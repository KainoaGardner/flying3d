#version 330 core
out vec4 fragColor;

// in vec3 vColor;
in vec2 vTexCoord;

uniform float uTime;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

void main() {
    // vec4 color0 = texture(uTexture0, vTexCoord) * vec4(vColor, 1.0);
    vec4 color0 = texture(uTexture0, vTexCoord);
    vec4 color1 = texture(uTexture1, vec2(1. - vTexCoord.x, vTexCoord.y));
    vec4 color = mix(color0, color1, color1.w);
    fragColor = color;
}
