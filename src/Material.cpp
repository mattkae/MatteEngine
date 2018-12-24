#include "Material.h"
#include "Shader.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const int MATERIAL_TEX_INDEX = 8;

void render_material(const Material& material, Shader& shader) {
  shader.set_uniform_4f("uMaterial.diffuse", material.diffuse.r, material.diffuse.g, material.diffuse.b, material.diffuse.a);
  shader.set_uniform_4f("uMaterial.specular", material.specular.r, material.specular.g, material.specular.b, material.specular.a);
  shader.set_uniform_4f("uMaterial.emissive", material.emissive.r, material.emissive.g, material.emissive.b, material.emissive.a);
  shader.set_uniform_3f("uMaterial.diffuseProperty", material.diffuseProperty.r, material.diffuseProperty.g, material.diffuseProperty.b);
  shader.set_uniform_3f("uMaterial.specularProperty", material.specularProperty.r, material.specularProperty.g, material.specularProperty.b);
  shader.set_uniform_1f("uMaterial.shininess", material.shininess);
  shader.set_uniform_1i("uMaterial.texCount", material.textures.size());

  if (material.textures.size() == 0) {
    shader.set_uniform_1i("uMaterial.diffuseTex", MATERIAL_TEX_INDEX);
    shader.set_uniform_1i("uMaterial.specularTex", MATERIAL_TEX_INDEX + 1);
  }
    
  for (unsigned int tidx = 0; tidx < material.textures.size(); tidx++) {
    glActiveTexture(GL_TEXTURE0 + MATERIAL_TEX_INDEX + tidx);
    glBindTexture(GL_TEXTURE_2D, material.textures[tidx].id);

    if (material.textures[tidx].type == TEX_DIFFUSE) {
      shader.set_uniform_1i("uMaterial.diffuseTex", MATERIAL_TEX_INDEX + tidx);
    } else {
      shader.set_uniform_1i("uMaterial.specularTex", MATERIAL_TEX_INDEX + tidx);
    }
  }
}
