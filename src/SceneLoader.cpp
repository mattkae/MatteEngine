#include "SceneLoader.h"
#include "ObjModel.h"
#include "SceneUI.h"
#include "Sphere.h"

const char* START_OBJECT_TOKEN = "::";
const char* END_OBJECT_TOKEN = ";;";
const char* IGNORE_OBJECT_TOKEN = "//";

inline void loadSkybox(FILE* file, Skybox& skybox, char buffer[SCENE_FILE_BUFFER_SIZE]);
inline void loadLights(FILE* file, Light* lights, size_t& numLights, char buffer[SCENE_FILE_BUFFER_SIZE], const Shader& shader);
inline void loadModels(FILE* file, Model* models, size_t& numModels, char buffer[SCENE_FILE_BUFFER_SIZE]);
inline void loadSpheres(FILE* file, Model* models, size_t& numModels, char buffer[SCENE_FILE_BUFFER_SIZE]);
inline void loadParticleEmitters(FILE* file, ParticleEmitter* emitters, size_t& numEmitters, char buffer[SCENE_FILE_BUFFER_SIZE]);
inline void loadTerrain(FILE* file, Terrain& terrain, char buffer[SCENE_FILE_BUFFER_SIZE]);
inline void ignoreObject(FILE* file, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file, buffer, ptr)) {
		if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

void loadScene(const char* filepath, BetterScene& scene) {
	FILE* file;
	fopen_s(&file, filepath, "r");
	char buffer[SCENE_FILE_BUFFER_SIZE];
	if (file == NULL) {
		// @TODO: Error out
		return;
	}

	char* ptr;
	while (processLine(file, buffer, ptr)) {
		const char* token = nullptr;
		if (startsWith(ptr, START_OBJECT_TOKEN)) {
			ptr += strlen(START_OBJECT_TOKEN);
			token = START_OBJECT_TOKEN;
			if (startsWith(ptr, "skybox")) {
				loadSkybox(file, scene.mSkybox, buffer);
			} else if (startsWith(ptr, "lights")) {
				loadLights(file, scene.lights, scene.numLightsUsed, buffer, scene.mSceneShader);
			} else if (startsWith(ptr, "models")) {
				loadModels(file, scene.models, scene.numModels, buffer);
			} else if (startsWith(ptr, "spheres")) {
				loadSpheres(file, scene.models, scene.numModels, buffer);
			} else if (startsWith(ptr, "particleEmitters")) {
				loadParticleEmitters(file, scene.emitters, scene.numEmitters, buffer);
			} else if (startsWith(ptr, "terrain")) {
				loadTerrain(file, scene.mTerrain, buffer);
			}
		} else if (ifEqualWalkToValue(ptr, "useDeferredRendering")) {
			strToBool(ptr, scene.useDefferredRendering);
			if (scene.useDefferredRendering) {
				scene.mSceneShader = loadShader("src/shaders/DefferedModel.vert", "src/shaders/DefferedModel.frag");
				useShader(scene.mSceneShader);
				setShaderInt(scene.mSceneShader, "uPosition", 0);
				setShaderInt(scene.mSceneShader, "uNormal", 1);
				setShaderInt(scene.mSceneShader, "uDiffuse", 2);
				setShaderInt(scene.mSceneShader, "uSpecular", 3);
				setShaderInt(scene.mSceneShader, "uEmissive", 4);
				setShaderInt(scene.mSceneShader, "uMaterialInfo", 5);
			} else {
				scene.mSceneShader = loadShader("src/shaders/model.vert", "src/shaders/model.frag");
				useShader(scene.mSceneShader);
				setShaderInt(scene.mSceneShader, "uDirShadow", 0);
				setShaderInt(scene.mSceneShader, "uMaterial.diffuseTex", 8);
				setShaderInt(scene.mSceneShader, "uMaterial.specularTex", 9);
			}
		} else if (startsWith(ptr, IGNORE_OBJECT_TOKEN)) {
			ignoreObject(file, buffer);
		}
	}

	initUI(scene.ui);
	getSceneUI(scene, scene.ui);

	scene.isDying = false;
	scene.mShadowShader = loadShader("src/shaders/shadows.vert", "src/shaders/shadows.frag");
	scene.mHotreloadThreader = std::thread(watchForDirectorychanges, std::ref(scene.shadersToReload), std::ref(scene.isDying));
	fclose(file);
}

void loadSkybox(FILE* file, Skybox& skybox, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char facePaths[6][128];
	char* ptr = &buffer[0];
	while (processLine(file, buffer, ptr)) {
		size_t faceStrLen = 0;
		size_t faceIndex = 0;

		if (startsWith(ptr, "front")) {
			faceStrLen = strlen("front");
			faceIndex = 0;
		} else if (startsWith(ptr, "back")) {
			faceStrLen = strlen("back");
			faceIndex = 1;
		} else if (startsWith(ptr, "up")) {
			faceStrLen = strlen("up");
			faceIndex = 2;
		} else if (startsWith(ptr, "down")) {
			faceStrLen = strlen("down");
			faceIndex = 3;
		} else if (startsWith(ptr, "right")) {
			faceStrLen = strlen("right");
			faceIndex = 4;
		} else if (startsWith(ptr, "left")) {
			faceStrLen = strlen("left");
			faceIndex = 5;
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}

		ptr += faceStrLen;
		trimLeft(ptr);
		substring(facePaths[faceIndex], ptr, getLengthToEndOfLine(ptr));
	}

	initSkybox(skybox, facePaths);
}

inline void loadLight(FILE* file, Light& light, char buffer[SCENE_FILE_BUFFER_SIZE], const Shader& shader) {
	char* ptr;
	while (processLine(file, buffer, ptr)) {
		if (ifEqualWalkToValue(ptr, "type")) {
			int value;
			strToInt(ptr, value);
			light.type = static_cast<LightType>(value);
		} else if (ifEqualWalkToValue(ptr, "color")) {
			strToVec3(ptr, light.color);
		} else if (ifEqualWalkToValue(ptr, "direction")) {
			strToVec3(ptr, light.direction);
		} else if (ifEqualWalkToValue(ptr, "position")) {
			strToVec3(ptr, light.position);
		} else if (ifEqualWalkToValue(ptr, "up")) {
			strToVec3(ptr, light.up);
		} else if (ifEqualWalkToValue(ptr, "usesShadows")) {
			strToBool(ptr, light.usesShadows);
		} else if (ifEqualWalkToValue(ptr, "isOn")) {
			strToBool(ptr, light.isOn);
		} else if (ifEqualWalkToValue(ptr, "constant")) {
			strToFloat(ptr, light.constant);
		} else if (ifEqualWalkToValue(ptr, "linear")) {
			strToFloat(ptr, light.linear);
		} else if (ifEqualWalkToValue(ptr, "quadratic")) {
			strToFloat(ptr, light.quadratic);
		} else if (ifEqualWalkToValue(ptr, "cosineCutOff")) {
			strToFloat(ptr, light.cosineCutOff);
		} else if (ifEqualWalkToValue(ptr, "dropOff")) {
			strToFloat(ptr, light.dropOff);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}

	initLight(light, shader);
}

void loadLights(FILE* file, Light* lights, size_t& numLights, char buffer[SCENE_FILE_BUFFER_SIZE], const Shader& shader) {
	numLights = 0;
	char* ptr;
	while (processLine(file,buffer,  ptr)) {
		if (startsWith(ptr, START_OBJECT_TOKEN)) {
			loadLight(file, lights[numLights], buffer, shader);
			numLights++;
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

inline void loadModel(FILE* file, Model& model, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file,buffer,  ptr)) {
		if (ifEqualWalkToValue(ptr, "path")) {
			loadFromObj(ptr, model);
		} else if (ifEqualWalkToValue(ptr, "transform")) {
			strToMat4(ptr, model.model);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}

	initializeModel(model);
}

void loadModels(FILE* file, Model* models, size_t& numModels, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file,buffer,  ptr)) {
		if (startsWith(ptr, START_OBJECT_TOKEN)) {
			loadModel(file, models[numModels], buffer);
			numModels++;
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

inline void loadSphere(FILE* file, Model& model, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	Sphere sphere;
	while (processLine(file,buffer,  ptr)) {
		if (ifEqualWalkToValue(ptr, "radius")) {
			strToInt(ptr, sphere.radius);
		} else if (ifEqualWalkToValue(ptr, "radius")) {
			strToFloat(ptr, sphere.angleIncrements);
		} else if (ifEqualWalkToValue(ptr, "transform")) {
			strToMat4(ptr, sphere.model);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}

	initializeSphere(sphere, model);
}

void loadSpheres(FILE* file, Model* models, size_t& numModels, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file,buffer,  ptr)) {
		if (startsWith(ptr, START_OBJECT_TOKEN)) {
			loadSphere(file, models[numModels], buffer);
			numModels++;
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

void loadRandomizableFloat(FILE* file, RandomizableFloat& v, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file, buffer,  ptr)) {
		if (ifEqualWalkToValue(ptr, "baseValue")) {
			strToFloat(ptr, v.baseValue);
		} else if (ifEqualWalkToValue(ptr, "randomOffset")) {
			strToFloat(ptr, v.randomOffset);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

void loadRandomizableVec3(FILE* file, RandomizableVec3& v, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file, buffer,  ptr)) {
		if (ifEqualWalkToValue(ptr, "baseValue")) {
			strToVec3(ptr, v.baseValue);
		} else if (ifEqualWalkToValue(ptr, "randomOffset")) {
			strToVec3(ptr, v.randomOffset);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

void loadFloatFunc(FILE* file, FunctionFloat& v, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file, buffer,  ptr)) {
		if (ifEqualWalkToValue(ptr, "initial")) {
			strToFloat(ptr, v.initial);
		} else if (ifEqualWalkToValue(ptr, "linear")) {
			strToFloat(ptr, v.linear);
		} else if (ifEqualWalkToValue(ptr, "quadratic")) {
			strToFloat(ptr, v.quadratic);
		} else if (ifEqualWalkToValue(ptr, "sinValue")) {
			strToFloat(ptr, v.sinValue);
		} else if (ifEqualWalkToValue(ptr, "cosValue")) {
			strToFloat(ptr, v.cosValue);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}


void loadVec4Func(FILE* file, FunctionVec4& v, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file, buffer,  ptr)) {
		if (ifEqualWalkToValue(ptr, "initial")) {
			strToVec4(ptr, v.initial);
		} else if (ifEqualWalkToValue(ptr, "linear")) {
			strToVec4(ptr, v.linear);
		} else if (ifEqualWalkToValue(ptr, "quadratic")) {
			strToVec4(ptr, v.quadratic);
		} else if (ifEqualWalkToValue(ptr, "sinValue")) {
			strToVec4(ptr, v.sinValue);
		} else if (ifEqualWalkToValue(ptr, "cosValue")) {
			strToVec4(ptr, v.cosValue);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

void loadVec3Func(FILE* file, FunctionVec3& v, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	while (processLine(file, buffer,  ptr)) {
		if (ifEqualWalkToValue(ptr, "initial")) {
			strToVec3(ptr, v.initial);
		} else if (ifEqualWalkToValue(ptr, "linear")) {
			strToVec3(ptr, v.linear);
		} else if (ifEqualWalkToValue(ptr, "quadratic")) {
			strToVec3(ptr, v.quadratic);
		} else if (ifEqualWalkToValue(ptr, "sinValue")) {
			strToVec3(ptr, v.sinValue);
		} else if (ifEqualWalkToValue(ptr, "cosValue")) {
			strToVec3(ptr, v.cosValue);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

inline void loadParticleEmitter(FILE* file, ParticleEmitter& emitter, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	char* ptr;
	int initialParticleCount = 0;
	while (processLine(file,buffer,  ptr)) {
		if (startsWith(ptr, START_OBJECT_TOKEN)) {
			ptr += strlen(START_OBJECT_TOKEN);
			if (ifEqualWalkToValue(ptr, "spawnFrequencySeconds")) {
				loadRandomizableFloat(file, emitter.spawnFrequencySeconds, buffer);
			} else if (ifEqualWalkToValue(ptr, "colorFunction")) {
				loadVec4Func(file, emitter.colorFunction, buffer);
			} else if (ifEqualWalkToValue(ptr, "movementFunction")) {
				loadVec3Func(file, emitter.movementFunction, buffer);
			} else if (ifEqualWalkToValue(ptr, "particlePosition")) {
				loadRandomizableVec3(file, emitter.particlePosition, buffer);
			} else if (ifEqualWalkToValue(ptr, "particleTimeToLiveSeconds")) {
				loadRandomizableFloat(file, emitter.particleTimeToLiveSeconds, buffer);
			}
		} else if (ifEqualWalkToValue(ptr, "radius")) {
			strToMat4(ptr, emitter.model);
		} else if (ifEqualWalkToValue(ptr, "initialNumParticles")) {
			strToInt(ptr, initialParticleCount);
		} else if (ifEqualWalkToValue(ptr, "numVertices")) {
			strToInt(ptr, emitter.numVertices);
		} else if (ifEqualWalkToValue(ptr, "particleDimension")) {
			strToVec2(ptr, emitter.particleDimension);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}

	initializeParticleEmitter(emitter, initialParticleCount);
}

void loadParticleEmitters(FILE* file, ParticleEmitter* emitters, size_t& numEmitters, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	numEmitters = 0;
	char* ptr;
	while (processLine(file,buffer,  ptr)) {
		if (startsWith(ptr, START_OBJECT_TOKEN)) {
			loadParticleEmitter(file, emitters[numEmitters], buffer);
			numEmitters++;
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}
}

void loadTerrain(FILE* file, Terrain& terrain, char buffer[SCENE_FILE_BUFFER_SIZE]) {
	GenerationParameters gp;
	char* ptr;
	while (processLine(file, buffer, ptr)) {
		if (ifEqualWalkToValue(ptr, "size")) {
			strToInt(ptr, gp.size);
		} else if (ifEqualWalkToValue(ptr, "granularity")) {
			strToInt(ptr, gp.granularity);
		} else if (ifEqualWalkToValue(ptr, "permSize")) {
			strToInt(ptr, gp.permSize);
		} else if (ifEqualWalkToValue(ptr, "minMaxHeight")) {
			strToFloat(ptr, gp.minMaxHeight);
		} else if (ifEqualWalkToValue(ptr, "scaleFactor")) {
			strToFloat(ptr, gp.scaleFactor);
		} else if (ifEqualWalkToValue(ptr, "ampFactor")) {
			strToFloat(ptr, gp.ampFactor);
		} else if (ifEqualWalkToValue(ptr, "frequencyFactor")) {
			strToFloat(ptr, gp.frequencyFactor);
		} else if (ifEqualWalkToValue(ptr, "numOctaves")) {
			strToInt(ptr, gp.numOctaves);
		} else if (startsWith(ptr, END_OBJECT_TOKEN)) {
			break;
		}
	}

	initializeTerrain(terrain, gp);
}