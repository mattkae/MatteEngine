#version 410
in vec2 oTexCoords;
in vec4 oColor;
out vec4 Color;

uniform sampler2D uGlyphTexture;

void main() {
    vec4 sampled = vec4(1, 1, 1, texture(uGlyphTexture, oTexCoords).r);
    Color = oColor * sampled;
}