#include "Material.h"
#include "Shader.h"
#include <iostream>
#include <sstream>
#include <string>

const int MATERIAL_TEX_INDEX = 8;

void render_material(const Shader &shader, const Material &material) {
    shader.set_uniform_4f("uMaterial.diffuse", material.diffuse.r,
                          material.diffuse.g, material.diffuse.b, 1.0);
    shader.set_uniform_4f("uMaterial.specular", material.specular.r,
                          material.specular.g, material.specular.b, 1.0);
    shader.set_uniform_4f("uMaterial.emissive", material.emissive.r,
                          material.emissive.g, material.emissive.b, 1.0);
    shader.set_uniform_3f(
        "uMaterial.diffuseProperty", material.diffuseProperty.r,
        material.diffuseProperty.g, material.diffuseProperty.b);
    shader.set_uniform_3f(
        "uMaterial.specularProperty", material.specularProperty.r,
        material.specularProperty.g, material.specularProperty.b);
    shader.set_uniform_1f("uMaterial.shininess", material.specularComponent);
    shader.set_uniform_1f("uMaterial.opacity", material.transparency);

    // @TODO: Render textures once we have them hooked up to the loader
}
