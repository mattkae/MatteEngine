#pragma once
#include "Scene.h"
#include "ModelLoader.h"

struct SceneLoader {
	ModelLoader mModelLoader;
	FILE* mFile;
	Scene* mScene;
	char buffer[512];

	void loadScene(const char* filepath);

	Light loadLight();

	void loadEntity();
	void loadEntityList();

	void loadSphere();
	void loadSpheres();

	void loadParticleEmitter();
	void loadParticleEmitters();
	void loadTerrain();
	void loadCamera();

	// Helper methods
	void loadRandomizableFloat(RandomizableFloat& v);
	void loadRandomizableVec3(RandomizableVec3& v);
	void loadFloatFunc(FunctionFloat& v);
	void loadVec4Func(FunctionVec4& v);
	void loadVec3Func(FunctionVec3& v);
};