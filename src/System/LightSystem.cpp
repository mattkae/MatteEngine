#include "LightSystem.h"
#include "../ShaderUniformMapping.h"

void LightSystem::initialize() {

}

void LightSystem::update(float dtMs) {

}

void LightSystem::render(const LightUniformMapping* mapping) const {
	setShaderInt(mapping->LIGHT_NUM_LIGHTS, mEntites.mNumElements);
	FOREACH_FIXED_CONST(mEntites) {
		value->mLight.render(mapping);
	}
}

void LightSystem::renderShadows(Scene* scene) {
	useShader(ShaderUniformMapping::GlobalShadowShaderMapping.shader);

	FOREACH_FIXED(mEntites) {
		value->mLight.renderShadows(*scene);
	}
}

void LightSystem::free() {
	FOREACH_FIXED(mEntites) {
		value->mLight.free();
	}
	mEntites.clear();
}
