#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in float phase;
layout (location = 2) in vec2 texCoords;

uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;
uniform vec3 uEye;
uniform float uTimePassedMs;
uniform float uPeriod;
uniform float uAmplitude;

out vec3 vNormal;
out vec4 vFragPos;
out vec3 vViewDir;
out vec4 vClipSpaceCoordinates;
out vec2 vTexCoords;
out float vPhase;

void main() {
    float angle = (uPeriod * (uTimePassedMs + phase));
    vec4 wavePosition = vec4(position.x, 
        uAmplitude * sin(angle),
        position.z, 
        1.0);
    vFragPos = uModel * wavePosition;

	vClipSpaceCoordinates = uProjection * uView * vFragPos;
    gl_Position = vClipSpaceCoordinates;
	

    vNormal = normalize(vec3(-uAmplitude * uPeriod * cos(angle), 1.0, 0.0));
    vViewDir = normalize(uEye - vFragPos.xyz);
	vTexCoords = texCoords;
	vPhase = phase;
}