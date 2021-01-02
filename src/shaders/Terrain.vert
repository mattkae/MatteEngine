#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform vec4 uClipPlane;
uniform vec3 uEye;

out vec3 vertexNormal;
out vec4 vertexFragPos;
flat out int vertexLayer;
out vec2 vertexTexCoords;
out vec3 vertexViewDir;
out mat3 vertexTBN;
out vec4 vertFragPos;

void main() {
	 vec4 fragPos = uModel * vec4(position, 1.0);
	 gl_Position = uProjection * uView * fragPos;
	 gl_ClipDistance[0] = dot(fragPos, uClipPlane);

	 vertexNormal = normal;
	 vertFragPos = fragPos;
	 vertexLayer = int(texCoords.z);
	 vertexTexCoords = vec2(texCoords.x, texCoords.y);
	 vertexViewDir = normalize(uEye - fragPos.xyz);
	 vec3 vertTangent_CameraSpace = normalize((uModel * vec4(tangent, 0.0)).xyz);
	 vec3 vertBitangent_CameraSpace = normalize((uModel * vec4(bitangent, 0.0)).xyz);
	 vec3 vertexNormal_CameraSpace = normalize((uModel * vec4(normal, 0.0)).xyz);
	 vertexTBN = mat3(vertTangent_CameraSpace,
     		   vertBitangent_CameraSpace,
     		   vertexNormal_CameraSpace);
}