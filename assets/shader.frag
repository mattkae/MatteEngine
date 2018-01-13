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
uniform vec3 ambient;

vec3 get_directional_light(vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-directionalLight.direction);
  
  float normalDotDir = max(0.f, dot(normal, lightDir));
  if (normalDotDir == 0) {
    return vec3(0.f);
  }
  
  // Calculate diffuse
  vec3 diffuseFactor = material.diffuse.rgb
    * material.diffuseProperty
    * max(0.f, dot(normal, lightDir));
  vec3 diffuse = directionalLight.color * diffuseFactor;

  // Calculate specular
  vec3 reflection = reflect(-lightDir, normal);
  vec3 specularFactor = material.specular.rgb
    * material.specularProperty
    * pow(max(0.f, dot(viewDir, reflection)), material.shininess);
  vec3 specular = specularFactor;
		       
  return min(diffuse + specular, vec3(1.0));;
}

void main() {
  vec3 viewDir = normalize(Eye - FragPos);
  vec3 normal = normalize(Normal);
  
  vec3 finalColor = ambient + material.emissive.rgb + get_directional_light(normal, viewDir);
  Color = vec4(normalize(finalColor), 1.0);
}
