#version 410

out vec4 Color;

in vec2 oTexCoords;
uniform float uNear;
uniform float uFar;
uniform sampler2D uTexture;

float LinearizeDepth(in vec2 uv)
{
    float depth = texture(uTexture, uv).x;
    return (2.0 * uNear) / (uFar + uNear - depth * (uFar - uNear));
}

void main()
{
    float c = LinearizeDepth(oTexCoords);
    Color = vec4(c, c, c, 1.0);
}
