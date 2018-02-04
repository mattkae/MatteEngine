#version 410 core

#define MAX_LIGHTS 4

// Data types
struct Material {
  vec4 diffuse;
  vec4 specular;
  vec4 emissive;
  vec3 diffuseProperty;
  vec3 specularProperty;
  float shininess;

  int texCount;
  sampler2D diffuseTex;
  sampler2D specularTex;
};

struct Light {
  vec3 direction;
  vec3 position;
  vec3 color;
  float constant;
  float linear;
  float quadratic;
  float cosineCutOff;
  float dropOff;

  mat4 projection;
};

// Output color
out vec4 Color;

// Input from vertex shader
in vec4 o_FragPos;
in vec3 o_Normal;
in vec2 o_TexCoords;
in vec3 o_Eye;
in vec4 o_ShadowCoords[MAX_LIGHTS];

// Uniform variables
//uniform sampler2DShadow u_depthTextures[MAX_LIGHTS];
uniform samplerCubeShadow u_pointDepthTexture;
uniform Material u_material;
uniform int u_numLights;
uniform Light u_lights[MAX_LIGHTS];
uniform vec3 u_ambient;

// Helper functions
vec3 get_light(Light directionalLight, vec3 normal, vec3 viewDir, vec4 diffuse, vec4 specular);
vec3 get_diffuse(vec3 normal, vec3 lightDir, vec3 color, vec4 diffuse);
vec3 get_specular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color, vec4 specular);


float VectorToDepth (vec3 Vec)
{
    vec3 AbsVec = abs(Vec);
    float LocalZcomp = max(AbsVec.x, max(AbsVec.y, AbsVec.z));

    // Replace f and n with the far and near plane values you used when
    //   you drew your cube map.
    const float f = 1000.0;
    const float n = 1.0;

    float NormZComp = (f+n) / (f-n) - (2*f*n)/(f-n)/LocalZcomp;
    return (NormZComp + 1.0) * 0.5;
}

void main() {
  vec3 viewDir = normalize(o_Eye - o_FragPos.xyz);
  vec3 normal = normalize(o_Normal);

  vec4 diffuse = u_material.diffuse;
  vec4 specular = u_material.specular;
  if (u_material.texCount > 0) {
    diffuse = texture(u_material.diffuseTex, o_TexCoords);
    specular = texture(u_material.specularTex, o_TexCoords);
  }
  
  vec3 finalColor = u_ambient + u_material.emissive.rgb;
  for (int lightIndex = 0; lightIndex < u_numLights; lightIndex++) {
    float visibility = 1.0;
    if (u_lights[lightIndex].direction == vec3(0.0)) {
      // Select correct face
      vec4 abs_coord = abs(o_ShadowCoords[lightIndex]);
      float fs_z=-max(abs_coord.x, max(abs_coord.y, abs_coord.z));

      vec4 clip = u_lights[lightIndex].projection * vec4(0.0, 0.0, -fs_z, 1.0);
      float depth = (clip.z / clip.w) * 0.5 + 0.5;

      visibility = texture(u_pointDepthTexture, vec4(o_ShadowCoords[lightIndex].xyz, depth));
    } else {
      //vec3 lP = vec3(o_ShadowCoords[lightIndex] / o_ShadowCoords[lightIndex].w) * 0.5 + 0.5;
      //visibility = texture(u_depthTextures[lightIndex], lP);
    }
    
    finalColor += visibility * get_light(u_lights[lightIndex], normal, viewDir, diffuse, specular);
  }
  
  Color = vec4(clamp(finalColor, vec3(0.0), vec3(1.0)), 1.0);
}


vec3 get_light(Light light, vec3 normal, vec3 viewDir, vec4 diffuse, vec4 specular) {
  // Get vector from the light, to the fragment
  vec3 posToFrag = light.position - o_FragPos.xyz;
  float delta = length(posToFrag);
  posToFrag = normalize(posToFrag);

  // Point lights will have no light direction
  vec3 direction = (light.direction == vec3(0.0)) ? posToFrag : -light.direction;
  direction = normalize(direction);
  float normalDotDir = max(0.f, dot(normal, direction));
  if (normalDotDir == 0) {
    return vec3(0.f);
  }

  // Check if frag is within spot
  // Point and Direction have cutoff = 0.0 (so never less)
  float angleBetween = min(dot(posToFrag, direction), 1.0);
  if (angleBetween < light.cosineCutOff) {
    return vec3(0.f);
  }

  // Calculate intensity of the light (Directional and Point have no drop off)
  float attenuation = (light.constant + delta * light.linear + delta * delta * light.quadratic);
  vec3 intensity = (light.color * pow(angleBetween, light.dropOff)) / attenuation;

  // Calculate diffuse and specular
  vec3 diffuseOut = get_diffuse(normal, direction, intensity, diffuse);
  vec3 specularOut = get_specular(normal, direction, viewDir, intensity, specular) ;

  return diffuseOut + specularOut;
}


vec3 get_diffuse(vec3 normal, vec3 lightDir, vec3 color, vec4 diffuse) {
  vec3 diffuseFactor = diffuse.rgb
    * u_material.diffuseProperty
    * max(0.f, dot(normal, lightDir));
   return color * diffuseFactor;
}


vec3 get_specular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color, vec4 specular) {
  vec3 reflection = reflect(-lightDir, normal);
  vec3 specularFactor = specular.rgb
    * u_material.specularProperty
    * pow(max(0.f, dot(viewDir, reflection)), u_material.shininess);
  return specularFactor;
}
