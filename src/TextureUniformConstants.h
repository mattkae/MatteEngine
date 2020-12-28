#pragma once

namespace TextureUniformConstants {
	const int LIGHT_SHADOW_TEXTURE_POSITION_START = 0;
	const int DIFFUSE_TEXTURE_POSITION = 6;
	const int SPECULAR_TEXTURE_POSITION = 7;
	const int AMBIENT_TEXTURE_POSITION = 8;
	const int NORMAL_TEXTURE_POSITION = 9;

	const int WATER_REFLECTION_TEXTURE_POSITION = DIFFUSE_TEXTURE_POSITION;
	const int WATER_REFRACTION_TEXTURE_POSITION = DIFFUSE_TEXTURE_POSITION + 1;
	const int WATER_DU_DV_TEXTURE_POSITION = DIFFUSE_TEXTURE_POSITION + 2;
	const int WATER_NORMAL_TEXTURE_POSITION = DIFFUSE_TEXTURE_POSITION + 3;
};
