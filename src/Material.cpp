#include "Material.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <string>

const int MATERIAL_TEX_INDEX = 8;

void render_material(const Shader &shader, const Material &material) {
	setShaderVec3(shader, "uMaterial.diffuse", material.diffuse);
	setShaderVec3(shader, "uMaterial.specular", material.specular);
	setShaderVec3(shader, "uMaterial.emissive", material.emissive);
	setShaderVec3(shader, "uMaterial.diffuseProperty", material.diffuseProperty);
	setShaderVec3(shader, "uMaterial.specularProperty", material.specularProperty);
	setShaderFloat(shader, "uMaterial.shininess", material.specularComponent);
	setShaderFloat(shader, "uMaterial.opacity", material.transparency);
    // @TODO: Render textures once we have them hooked up to the loader
}
