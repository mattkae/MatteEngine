#version 410

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec3 worldPosition;
layout (location = 2) in vec4 color;

uniform mat4 uVp;
uniform mat4 uModel;
uniform vec3 uCameraRight;
uniform vec3 uCameraUp;

out vec4 vColor;

void main()
{
   vec3 adjustedPosition = worldPosition + uCameraRight * vertexPosition.x  + uCameraUp * vertexPosition.y;
   gl_Position = uVp * uModel * vec4(adjustedPosition, 1.0);
   vColor = color;
}
