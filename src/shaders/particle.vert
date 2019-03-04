#version 410

layout(location = 0) in vec3 position;

uniform mat4 uVp;
uniform mat4 uModel;
uniform vec3 uPosition;
uniform vec4 uColor;

out vec4 vColor;

void main()
{
    gl_Position = uVp * uModel * vec4(uPosition + position, 1.0);

    vColor = uColor;
}
