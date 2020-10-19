#version 410

layout (location = 0) in vec3 position;

uniform mat4 uView;
uniform mat4 uProjection;
uniform vec4 uHighColor;
uniform vec4 uLowColor;
uniform vec2 uMixYStartEnd;

smooth out vec4 vColor;

void main() {
	 gl_Position = uProjection * uView * vec4(position, 1.0);

	 if (position.y < uMixYStartEnd.x) {
	 	vColor = uLowColor;
	 } else if (position.y > uMixYStartEnd.y) {
	   vColor = uHighColor;
	 } else {
	   float mixPercent = (uMixYStartEnd.y - position.y) / (uMixYStartEnd.y - uMixYStartEnd.x);
	   vColor = mix(uHighColor, uLowColor, mixPercent);
	 }
}