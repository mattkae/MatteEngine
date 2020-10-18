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

out vec4 vFragPos;
out vec3 vViewDir;
out vec4 vClipSpaceCoordinates;
out vec2 vTexCoords;
out float vPhase;
out mat3 vTBN;

const vec4 TANGENT = vec4(1, 0, 0, 0);
const vec4 BITANGENT = vec4(0, 0, 1, 0);

void main() {
    float angle = (uPeriod * (uTimePassedMs + phase));
    vec4 wavePosition = vec4(position.x, 
       0,// uAmplitude * sin(angle),
        position.z, 
        1.0);
    vFragPos = uModel * wavePosition;

	vClipSpaceCoordinates = uProjection * uView * vFragPos;
    gl_Position = vClipSpaceCoordinates;
	

    vec3 normal = normalize(vec3(-uAmplitude * uPeriod * cos(angle), 1.0, 0.0));
    vViewDir = normalize(uEye - vFragPos.xyz);
	vTexCoords = texCoords;
	vPhase = phase;

	vec3 vertTangent_CameraSpace = normalize(uModel * TANGENT).xyz;
	vec3 vertBitangent_CameraSpace = normalize(uModel * BITANGENT).xyz;
	vec3 vertexNormal_CameraSpace = normalize(uModel * vec4(normal, 0.0)).xyz;
	vTBN = mat3(vertTangent_CameraSpace, vertBitangent_CameraSpace, vertexNormal_CameraSpace);
}