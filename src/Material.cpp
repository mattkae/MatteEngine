#include "Material.h"
#include "Shader.h"

void render_material(Material* material, Shader* shader) {
  shader->SetUniform4f("u_material.diffuse", material->diffuse.r, material->diffuse.g, material->diffuse.b, material->diffuse.a);
  shader->SetUniform4f("u_material.specular", material->specular.r, material->specular.g, material->specular.b, material->specular.a);
  shader->SetUniform4f("u_material.emissive", material->emissive.r, material->emissive.g, material->emissive.b, material->emissive.a);
  shader->SetUniform3f("u_material.diffuseProperty", material->diffuseProperty.r, material->diffuseProperty.g, material->diffuseProperty.b);
  shader->SetUniform3f("u_material.specularProperty", material->specularProperty.r, material->specularProperty.g, material->specularProperty.b);
  shader->SetUniform1f("u_material.shininess", material->shininess);
}
