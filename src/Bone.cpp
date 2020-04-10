#include "Bone.h"

void Bone::free() {
	if (childrenBoneIndices) {
		delete childrenBoneIndices;
	}
}