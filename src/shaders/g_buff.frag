#version 410

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gDiffuse;
layout (location = 3) out vec3 gSpecular;
layout (location = 4) out vec3 gEmissive;
layout (location = 5) out vec3 gMaterialInfo;

in vec4 oFragPos;
in vec3 oNormal;
in vec3 oDiffuse;
in vec3 oSpecular;
in vec3 oEmissive;
in vec3 oMaterialInfo;

void main()
{    
    gPosition = oFragPos;
    gNormal = normalize(oNormal);
    gDiffuse = oDiffuse;
    gSpecular = oSpecular;
    gEmissive = oEmissive;
    gMaterialInfo = oMaterialInfo;
} 