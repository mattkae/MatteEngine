#version 410

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 oFragPos;
in vec3 oNormal;
in vec4 oAlbedoSpec;

void main()
{    
    gPosition = oFragPos;
    gNormal = normalize(oNormal);
    gAlbedoSpec = oAlbedoSpec;
} 