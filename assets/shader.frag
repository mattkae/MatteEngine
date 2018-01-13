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

struct DirectionalLight {
  vec3 direction;
  vec3 color;
};

struct PointLight {
  vec3 position;
  vec3 color;
  float constant;
  float linear;
  float quadratic;
};

// Output color
out vec4 Color;

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec3 TexCoords;
in vec3 Eye;

// Uniform variables
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform vec3 ambient;

// Helper functions
vec3 get_directional_light(vec3 normal, vec3 viewDir);
vec3 get_point_light(vec3 normal, vec3 viewDir);
vec3 get_diffuse(vec3 normal, vec3 lightDir, vec3 color);
vec3 get_specular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color);

void main() {
  vec3 viewDir = normalize(Eye - FragPos);
  vec3 normal = normalize(Normal);
  
  vec3 finalColor = ambient
    + material.emissive.rgb
    + get_directional_light(normal, viewDir)
    + get_point_light(normal, viewDir);
  
  Color = vec4(normalize(finalColor), 1.0);
}


vec3 get_directional_light(vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-directionalLight.direction);
  
  float normalDotDir = max(0.f, dot(normal, lightDir));
  if (normalDotDir == 0) {
    return vec3(0.f);
  }
  
  vec3 diffuse = get_diffuse(normal, lightDir, directionalLight.color);
  vec3 specular = get_specular(normal, lightDir, viewDir, directionalLight.color);
		       
  return min(diffuse + specular, vec3(1.0));
}

vec3 get_point_light(vec3 normal, vec3 viewDir) {
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
