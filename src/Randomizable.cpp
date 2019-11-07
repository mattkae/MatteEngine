#include "Randomizable.h"

// @TODO: Investigate how to easily use std::random, or just write
// it myself if it mucks up my use case too much...
float getRandomFloat(float randomOffset) {
	return (2 * randomOffset) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - randomOffset;
}

float getRandom(const RandomizableFloat& randomizable) {
	return randomizable.baseValue + getRandomFloat(randomizable.randomOffset);
}

glm::vec3 getRandom(const RandomizableVec3& randomizable) {
	return glm::vec3(
		randomizable.baseValue.x + getRandomFloat(randomizable.randomOffset.x),
		randomizable.baseValue.y + getRandomFloat(randomizable.randomOffset.y),
		randomizable.baseValue.z + getRandomFloat(randomizable.randomOffset.z)
	);
}

glm::vec4 getRandom(const RandomizableVec4& randomizable) {
	return glm::vec4(
		randomizable.baseValue.x + getRandomFloat(randomizable.randomOffset.x),
		randomizable.baseValue.y + getRandomFloat(randomizable.randomOffset.y),
		randomizable.baseValue.z + getRandomFloat(randomizable.randomOffset.z),
		randomizable.baseValue.w + getRandomFloat(randomizable.randomOffset.w)
	);
}