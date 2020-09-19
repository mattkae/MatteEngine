#version 410

in vec4 oColor;
in vec2 oTexCoord;

uniform sampler2D uTexture;
uniform bool uUseTexture;

out vec4 Color;

void main() {
    if (uUseTexture) {
        Color = texture(uTexture, oTexCoord);
    } else {
        Color = oColor;
    }
}