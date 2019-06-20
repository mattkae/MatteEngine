#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 oFragPos;
out vec3 oNormal;

uniform mat4 uModel;
uniform mat4 uVp;

void main()
{
    vec4 fragPos = uModel * vec4(position, 1.0);
    oFragPos = fragPos.xyz; 
    oNormal = normal;

    gl_Position = uVp * fragPos;
}