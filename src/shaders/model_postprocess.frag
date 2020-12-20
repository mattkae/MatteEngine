#version 410

in vec2 VERT_texCoord;

uniform sampler2D uSceneTexture;
uniform float uExposure;

out vec4 FRAG_color;

void main() {
	 const float gamma = 2.2;

	 vec3 hdrColor = texture(uSceneTexture, VERT_texCoord).xyz;

	 vec3 retval = vec3(1.0) - exp(-hdrColor * uExposure);
	 retval = pow(retval, vec3(1.0 / gamma));

	 FRAG_color = vec4(retval, 1.0);
}