#version 330 core

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
};

// Constants
const uint Directional = 0x00000001u;
const uint Point       = 0x00000002u;
const uint Spot        = 0x00000004u;
const int MAX_LIGHTS = 16;

// Output color
out vec4 Color;

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec3 TexCoords;
in vec3 Eye;

// Uniform variables
uniform Material material;
uniform int numLights;
uniform Light lights[MAX_LIGHTS];
uniform vec3 ambient;

// Helper functions
vec3 get_directional_light(Light directionalLight, vec3 normal, vec3 viewDir);
vec3 get_point_light(Light pointLight, vec3 normal, vec3 viewDir);
vec3 get_spot_light(Light spotLight, vec3 normal, vec3 viewDir);
vec3 get_diffuse(vec3 normal, vec3 lightDir, vec3 color);
vec3 get_specular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color);


void main() {
  vec3 viewDir = normalize(Eye - FragPos);
  vec3 normal = normalize(Normal);
  vec3 finalColor = ambient + material.emissive.rgb;

  // Iterate over all the lights in the scene
  for (int lightIndex = 0; lightIndex < numLights; lightIndex++) {
    Light light = lights[lightIndex];

    if (light.type == Directional) {
      finalColor += get_directional_light(light, normal, viewDir);
    } else if (light.type == Point) {
      finalColor += get_point_light(light, normal, viewDir);
    } else if (light.type == Spot) {
      finalColor += get_spot_light(light, normal, viewDir);
    }
  }

  Color = vec4(clamp(finalColor, vec3(0.0), vec3(1.0)), 1.0);
}


vec3 get_directional_light(Light directionalLight, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-directionalLight.direction);
  
  float normalDotDir = max(0.f, dot(normal, lightDir));
  if (normalDotDir == 0) {
    return vec3(0.f);
  }
  
  vec3 diffuse = get_diffuse(normal, lightDir, directionalLight.color);
  vec3 specular = get_specular(normal, lightDir, viewDir, directionalLight.color);
		       
  return min(diffuse + specular, vec3(1.0));
}

vec3 get_point_light(Light pointLight, vec3 normal, vec3 viewDir) {
  vec3 lightMinusFrag = pointLight.position - FragPos;
  float delta = length(lightMinusFrag);
  vec3 lightDir = normalize(lightMinusFrag);

  float normalDotDir = max(0.f, dot(normal, lightDir));
  if (normalDotDir == 0) {
    return vec3(0.f);
  }

  vec3 intensity = pointLight.color / (pointLight.constant + delta * pointLight.linear + delta * delta * pointLight.quadratic);

  vec3 diffuse = get_diffuse(normal, lightDir, intensity);
  vec3 specular = get_specular(normal, lightDir, viewDir, intensity);
  
  return min(diffuse + specular, vec3(1.0));
}

vec3 get_spot_light(Light spotLight, vec3 normal, vec3 viewDir) {
  vec3 lightMinusFrag = spotLight.position - FragPos;
  float delta = length(lightMinusFrag);
  vec3 lightToPosDir = normalize(lightMinusFrag);
  vec3 lightDir = normalize(-spotLight.direction);
  float dotProd = dot(lightDir, lightToPosDir);
  if (dotProd < spotLight.cosineCutOff) {
    return vec3(0.f);
  }

  vec3 intensity = (spotLight.color * pow(dotProd, spotLight.dropOff)) / (spotLight.constant + delta * spotLight.linear + delta * delta * spotLight.quadratic);

  vec3 diffuse = get_diffuse(normal, lightDir, intensity);
  vec3 specular = get_specular(normal, lightDir, viewDir, intensity);
  
  return min(diffuse + specular, vec3(1.0));
}

vec3 get_diffuse(vec3 normal, vec3 lightDir, vec3 color) {
  vec3 diffuseFactor = material.diffuse.rgb
    * material.diffuseProperty
    * max(0.f, dot(normal, lightDir));
   return color * diffuseFactor;
}

vec3 get_specular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color) {
  vec3 reflection = reflect(-lightDir, normal);
  vec3 specularFactor = material.specular.rgb
    * material.specularProperty
    * pow(max(0.f, dot(viewDir, reflection)), material.shininess);
  return specularFactor;
}
