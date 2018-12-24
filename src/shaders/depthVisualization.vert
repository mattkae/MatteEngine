#version 410

layout (location = 0) in vec3 postion;
layout (location = 1) in vec2 texCoord;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(postion, 1.0);
    TexCoord = texCoord;
}
