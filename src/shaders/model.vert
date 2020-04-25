#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec4 boneWeights;
layout (location = 4) in ivec4 boneIndices;

out vec4 oFragPos;
out vec3 oNormal;
out vec2 oTexCoords;
out vec3 oEye;

uniform int uNumLights;
uniform mat4 uVp;
uniform mat4 uModel;
uniform mat4 uBones[64];
uniform vec3 uEye;

void main() {
  mat4 boneTransform = uBones[boneIndices.x] * boneWeights.x;
    boneTransform += uBones[boneIndices.y] * boneWeights.y;
    boneTransform += uBones[boneIndices.z] * boneWeights.w;
    boneTransform += uBones[boneIndices.w] * boneWeights.z;

  vec4 bonePosition = boneTransform * vec4(position, 1.0f);
  vec4 fragPos = uModel * bonePosition;
  gl_Position = uVp * fragPos;
  oFragPos = fragPos;
  oTexCoords = texCoords;
  oNormal = (boneTransform * vec4(normal, 0.0f)).xyz;
  oEye = uEye;
}
