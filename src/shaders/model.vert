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
uniform bool uDisableBones;

void main() {
  vec4 bonePosition = vec4(position, 1.0f);
  vec3 outNormal = normal;
  if (uDisableBones == false) {
    mat4 boneTransform = uBones[boneIndices[0]] * boneWeights[0];
    boneTransform += uBones[boneIndices[1]] * boneWeights[1];
    boneTransform += uBones[boneIndices[2]] * boneWeights[2];
    boneTransform += uBones[boneIndices[3]] * boneWeights[3];
    bonePosition = boneTransform * bonePosition;
    outNormal = (boneTransform * vec4(normal, 0)).xyz;
  }

  vec4 fragPos = uModel * bonePosition;
  gl_Position = uVp * fragPos;
  oFragPos = fragPos;
  oTexCoords = texCoords;
  oNormal = outNormal;
  oEye = uEye;
}
