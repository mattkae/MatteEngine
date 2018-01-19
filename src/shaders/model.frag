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
  uint type;
  mat4 shadowMatrix;
};

// Output color
out vec4 Color;

// Input from vertex shader
in vec4 o_FragPos;
in vec3 o_Normal;
in vec3 o_TexCoord;
in vec3 o_Eye;
in vec4 o_ShadowCoords[MAX_LIGHTS];

// Uniform variables
uniform Material u_material;
uniform int u_numLights;
uniform Light u_lights[MAX_LIGHTS];
uniform vec3 u_ambient;
uniform sampler2DShadow u_depthTexture[MAX_LIGHTS];

// Helper functions
vec3 get_light(Light directionalLight, vec3 normal, vec3 viewDir);
vec3 get_diffuse(vec3 normal, vec3 lightDir, vec3 color);
vec3 get_specular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color);


void main() {
  vec3 viewDir = normalize(o_Eye - o_FragPos.xyz);
  vec3 normal = normalize(o_Normal);
  vec3 finalColor = u_ambient + u_ material.emissive.rgb;

  for (int lightIndex = 0; lightIndex < u_numLights; lightIndex++) {
    vec3 lP = vec3(o_ShadowCoords[lightIndex] / o_ShadowCoords[lightIndex].w) * 0.5 + 0.5;
    Light light = u_lights[lightIndex];
    float f = texture(u_depthTextures[lightIndex], lP);
    finalColor += get_light(light, normal, viewDir);
  }

  Color = vec4(clamp(finalColor, vec3(0.0), vec3(1.0)), 1.0);
}


vec3 get_light(Light light, vec3 normal, vec3 viewDir) {
  // Get vector from the light, to the fragment
  vec3 posToFrag = light.position - o_FragPos.xyz;
  float distance = length(lightMinusFrag);
  posToFrag = normalize(posToFrag);

  // Point lights will have no light direction
  vec3 direction = (light.direction == vec3(0.0)) ? posToFrag : light.direction;
  float normalDotDir = max(0.f, dot(normal, direction));
  if (normalDotDir == 0) {
    return vec3(0.f);
  }

  // Check if frag is within spot
  float angleBetween = dot(posToFrag, direction);
  if (dotProd < light.cosineCutOff) {
    // Point and Direction have cutoff = 0.0 (so never less)
    return vec3(0.f);
  }

  // Calculate intensity of the light (Directional and Point have no drop off)
  float attenuation = (light.constant + delta * light.linear + delta * delta * light.quadratic);
  vec3 intensity = light.color * pow(angleBetween, light.dropOff) / attenuation; 

  // Calculate diffuse and specular
  vec3 diffuse = get_diffuse(normal, lightDir, light.color) ;
  vec3 specular = get_specular(normal, lightDir, viewDir, light.color) ;
		       
  return min(diffuse + specular, vec3(1.0));
}


vec3 get_diffuse(vec3 normal, vec3 lightDir, vec3 color) {
  vec3 diffuseFactor = u_material.diffuse.rgb
    * u_material.diffuseProperty
    * max(0.f, dot(normal, lightDir));
   return color * diffuseFactor;
}


vec3 get_specular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color) {
  vec3 reflection = reflect(-lightDir, normal);
  vec3 specularFactor = u_material.specular.rgb
    * u_material.specularProperty
    * pow(max(0.f, dot(viewDir, reflection)), u_material.shininess);
  return specularFactor;
}