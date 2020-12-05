#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoords;
layout (location = 5) in vec3 texWeights;
layout (location = 6) in vec4 boneWeights;
layout (location = 7) in ivec4 boneIndices;

uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;
uniform mat4 uBones[64];
uniform bool uDisableBones;

void main() {
	vec4 bonePosition = vec4(position, 1.0f);
	if (uDisableBones == false) {
	   mat4 boneTransform = uBones[boneIndices[0]] * boneWeights[0];
       boneTransform += uBones[boneIndices[1]] * boneWeights[1];
	   boneTransform += uBones[boneIndices[2]] * boneWeights[2];
       boneTransform += uBones[boneIndices[3]] * boneWeights[3];
	   bonePosition = boneTransform * bonePosition;
	}

  	vec4 fragPos = uModel * bonePosition;

  	gl_Position = uProjection * uView * fragPos;
}
