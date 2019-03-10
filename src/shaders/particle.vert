#version 410

layout(location = 0) in vec2 position;

uniform mat4 uVp;
uniform mat4 uModel;
uniform vec3 uPosition;
uniform vec4 uColor;
uniform vec3 uCameraRight;
uniform vec3 uCameraUp;

out vec4 vColor;

void main()
{
   vec3 adjustedPosition = uPosition + uCameraRight * position.x  + uCameraUp * position.y;
   gl_Position = uVp * uModel * vec4(adjustedPosition, 1.0);
   vColor = uColor;
}
