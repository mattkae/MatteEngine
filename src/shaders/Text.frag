#version 410
in vec2 oTexCoords;
in vec4 oColor;
out vec4 Color;

uniform sampler2D uGlyphTexture;

void main() {
	Color = vec4(oColor.x, oColor.y, oColor.z, texture(uGlyphTexture, oTexCoords).r);
}