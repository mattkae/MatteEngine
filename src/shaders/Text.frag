#version 410
in vec2 oTexCoords;
in vec4 oColor;
in vec4 vertexClip;
in vec2 vertexPosition;

out vec4 Color;

uniform sampler2D uGlyphTexture;

void main() {
	 if (vertexPosition.x < vertexClip.x || vertexPosition.x > vertexClip.y
	 	|| vertexPosition.y < vertexClip.z || vertexPosition.y > vertexClip.w) {
	 	discard;
	 }

	Color = vec4(oColor.x, oColor.y, oColor.z, texture(uGlyphTexture, oTexCoords).r);
}