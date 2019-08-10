#version 410
in vec2 oTexCoords;
out vec4 Color;

uniform sampler2D glyphTexture;
uniform vec3 uColor;

void main() {
    vec4 sampled = vec4(1, 1, 1, texture(glyphTexture, oTexCoords).r);
    Color = vec4(uColor, 1.0) * sampled;
}