#include "SceneLoader.h"
#include "Sphere.h"
#include "StringUtil.h"
#include "FileHelper.h"

const char* START_OBJECT_TOKEN = "::";
const char* END_OBJECT_TOKEN = ";;";
const char* IGNORE_OBJECT_TOKEN = "//";

void loadSkybox(FILE* file, Skybox& skybox, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]);
void loadLights(FILE* file, Light* lights, size_t& numLights, char buffer[StringUtil::DEFAULT_BUFFER_SIZE], const Shader& shader);
void loadModels(FILE* file, Scene& scene, size_t& numModels, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]);
void loadSpheres(FILE* file, Model* models, Box* boxes, size_t& numModels, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]);
void loadParticleEmitters(FILE* file, ParticleEmitter* emitters, size_t& numEmitters, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]);
void loadTerrain(FILE* file, Terrain& terrain, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]);
void ignoreObject(FILE* file, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void SceneLoader::loadScene(const char* filepath, Scene& scene) {
    FILE* file = FileHelper::openFile(filepath);

    char buffer[StringUtil::DEFAULT_BUFFER_SIZE];
    if (file == NULL) {
        // @TODO: Error out
        return;
    }

    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        const char* token = nullptr;
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            ptr += strlen(START_OBJECT_TOKEN);
            token = START_OBJECT_TOKEN;
            if (StringUtil::startsWith(ptr, "skybox")) {
                loadSkybox(file, scene.mSkybox, buffer);
            } else if (StringUtil::startsWith(ptr, "lights")) {
                loadLights(file, scene.lights, scene.numLightsUsed, buffer, scene.mSceneShader);
            } else if (StringUtil::startsWith(ptr, "models")) {
                loadModels(file, scene, scene.numModels, buffer);
            } else if (StringUtil::startsWith(ptr, "spheres")) {
                loadSpheres(file, scene.models, scene.modelBoundingBoxes, scene.numModels, buffer);
            } else if (StringUtil::startsWith(ptr, "particleEmitters")) {
                loadParticleEmitters(file, scene.emitters, scene.numEmitters, buffer);
            } else if (StringUtil::startsWith(ptr, "terrain")) {
                loadTerrain(file, scene.mTerrain, buffer);
            }
        } else if (StringUtil::ifEqualWalkToValue(ptr, "useDeferredRendering")) {
            StringUtil::strToBool(ptr, scene.useDefferredRendering);
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
            }
        } else if (StringUtil::startsWith(ptr, IGNORE_OBJECT_TOKEN)) {
            ignoreObject(file, buffer);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "textures")) {
            scene.modelLoader.loadTextureList(ptr);
        }
    }

    scene.ui.init();
    scene.ui.showModelSelector(scene.models, scene.numModels);
    scene.isDying = false;
    scene.mShadowShader = loadShader("src/shaders/shadows.vert", "src/shaders/shadows.frag");
    fclose(file);
}

void loadSkybox(FILE* file, Skybox& skybox, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char facePaths[6][128];
    char* ptr = &buffer[0];
    while (StringUtil::processLine(file, buffer, ptr)) {
        size_t faceStrLen = 0;
        size_t faceIndex = 0;

        if (StringUtil::startsWith(ptr, "front")) {
            faceStrLen = strlen("front");
            faceIndex = 0;
        } else if (StringUtil::startsWith(ptr, "back")) {
            faceStrLen = strlen("back");
            faceIndex = 1;
        } else if (StringUtil::startsWith(ptr, "up")) {
            faceStrLen = strlen("up");
            faceIndex = 2;
        } else if (StringUtil::startsWith(ptr, "down")) {
            faceStrLen = strlen("down");
            faceIndex = 3;
        } else if (StringUtil::startsWith(ptr, "right")) {
            faceStrLen = strlen("right");
            faceIndex = 4;
        } else if (StringUtil::startsWith(ptr, "left")) {
            faceStrLen = strlen("left");
            faceIndex = 5;
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }

        ptr += faceStrLen;
        StringUtil::trimLeft(ptr);
        StringUtil::substring(facePaths[faceIndex], ptr, StringUtil::getLengthToEndOfLine(ptr));
    }

    initSkybox(skybox, facePaths);
}

inline void loadLight(FILE* file, Light& light, int numLights, char buffer[StringUtil::DEFAULT_BUFFER_SIZE], const Shader& shader) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "type")) {
            int value;
            StringUtil::strToInt(ptr, value);
            light.type = static_cast<LightType>(value);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "color")) {
            StringUtil::strToVec3(ptr, light.color);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "direction")) {
            StringUtil::strToVec3(ptr, light.direction);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "position")) {
            StringUtil::strToVec3(ptr, light.position);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "up")) {
            StringUtil::strToVec3(ptr, light.up);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "usesShadows")) {
            StringUtil::strToBool(ptr, light.usesShadows);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "isOn")) {
            StringUtil::strToBool(ptr, light.isOn);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "constant")) {
            StringUtil::strToFloat(ptr, light.constant);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "linear")) {
            StringUtil::strToFloat(ptr, light.linear);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "quadratic")) {
            StringUtil::strToFloat(ptr, light.quadratic);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "cosineCutOff")) {
            StringUtil::strToFloat(ptr, light.cosineCutOff);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "dropOff")) {
            StringUtil::strToFloat(ptr, light.dropOff);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }

    initLight(light, shader, numLights);
}

void loadLights(FILE* file, Light* lights, size_t& numLights, char buffer[StringUtil::DEFAULT_BUFFER_SIZE], const Shader& shader) {
    numLights = 0;
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            loadLight(file, lights[numLights], numLights, buffer, shader);
            numLights++;
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

inline void loadModel(FILE* file, Scene& scene, size_t& numModels, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "path")) {
            ModelLoader::ModelLoadResult retval = scene.modelLoader.loadSerializedModel(ptr);
            scene.models[numModels] = retval.model;
            scene.modelBoundingBoxes[numModels] = retval.box;
        } else if (StringUtil::ifEqualWalkToValue(ptr, "transform")) {
            StringUtil::strToMyMat4(ptr, scene.models[numModels].model);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void loadModels(FILE* file, Scene& scene, size_t& numModels, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            loadModel(file, scene, numModels, buffer);
            numModels++;
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

inline void loadSphere(FILE* file, Model& model, Box& box, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    Sphere sphere;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "radius")) {
            StringUtil::strToInt(ptr, sphere.radius);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "radius")) {
            StringUtil::strToFloat(ptr, sphere.angleIncrements);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "transform")) {
            StringUtil::strToMyMat4(ptr, sphere.model);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }

    initializeSphere(sphere, model, box);
}

void loadSpheres(FILE* file, Model* models, Box* boxes, size_t& numModels, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            loadSphere(file, models[numModels], boxes[numModels], buffer);
            numModels++;
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void loadRandomizableFloat(FILE* file, RandomizableFloat& v, char buffer[StringUtil::DEFAULT_BUFFER_SIZE])
{
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "baseValue")) {
            StringUtil::strToFloat(ptr, v.baseValue);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "randomOffset")) {
            StringUtil::strToFloat(ptr, v.randomOffset);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void loadRandomizableVec3(FILE* file, RandomizableVec3& v, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "baseValue")) {
            StringUtil::strToVec3(ptr, v.baseValue);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "randomOffset")) {
            StringUtil::strToVec3(ptr, v.randomOffset);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void loadFloatFunc(FILE* file, FunctionFloat& v, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "initial")) {
            StringUtil::strToFloat(ptr, v.initial);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "linear")) {
            StringUtil::strToFloat(ptr, v.linear);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "quadratic")) {
            StringUtil::strToFloat(ptr, v.quadratic);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "sinValue")) {
            StringUtil::strToFloat(ptr, v.sinValue);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "cosValue")) {
            StringUtil::strToFloat(ptr, v.cosValue);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void loadVec4Func(FILE* file, FunctionVec4& v, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "initial")) {
            StringUtil::strToVec4(ptr, v.initial);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "linear")) {
            StringUtil::strToVec4(ptr, v.linear);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "quadratic")) {
            StringUtil::strToVec4(ptr, v.quadratic);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "sinValue")) {
            StringUtil::strToVec4(ptr, v.sinValue);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "cosValue")) {
            StringUtil::strToVec4(ptr, v.cosValue);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void loadVec3Func(FILE* file, FunctionVec3& v, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "initial")) {
            StringUtil::strToVec3(ptr, v.initial);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "linear")) {
            StringUtil::strToVec3(ptr, v.linear);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "quadratic")) {
            StringUtil::strToVec3(ptr, v.quadratic);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "sinValue")) {
            StringUtil::strToVec3(ptr, v.sinValue);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "cosValue")) {
            StringUtil::strToVec3(ptr, v.cosValue);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

inline void loadParticleEmitter(FILE* file, ParticleEmitter& emitter, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    int initialParticleCount = 0;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            ptr += strlen(START_OBJECT_TOKEN);
            if (StringUtil::ifEqualWalkToValue(ptr, "spawnFrequencySeconds")) {
                loadRandomizableFloat(file, emitter.spawnFrequencySeconds, buffer);
            } else if (StringUtil::ifEqualWalkToValue(ptr, "colorFunction")) {
                loadVec4Func(file, emitter.colorFunction, buffer);
            } else if (StringUtil::ifEqualWalkToValue(ptr, "movementFunction")) {
                loadVec3Func(file, emitter.movementFunction, buffer);
            } else if (StringUtil::ifEqualWalkToValue(ptr, "particlePosition")) {
                loadRandomizableVec3(file, emitter.particlePosition, buffer);
            } else if (StringUtil::ifEqualWalkToValue(ptr, "particleTimeToLiveSeconds")) {
                loadRandomizableFloat(file, emitter.particleTimeToLiveSeconds, buffer);
            }
        } else if (StringUtil::ifEqualWalkToValue(ptr, "model")) {
            StringUtil::strToMat4(ptr, emitter.model);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "initialNumParticles")) {
            StringUtil::strToInt(ptr, initialParticleCount);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "numVertices")) {
            StringUtil::strToInt(ptr, emitter.numVertices);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "particleDimension")) {
            StringUtil::strToVec2(ptr, emitter.particleDimension);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }

    initializeParticleEmitter(emitter, initialParticleCount);
}

void loadParticleEmitters(FILE* file, ParticleEmitter* emitters, size_t& numEmitters, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    numEmitters = 0;
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            loadParticleEmitter(file, emitters[numEmitters], buffer);
            numEmitters++;
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void loadTerrain(FILE* file, Terrain& terrain, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    GenerationParameters gp;
    char* ptr;
    while (StringUtil::processLine(file, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "size")) {
            StringUtil::strToInt(ptr, gp.size);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "granularity")) {
            StringUtil::strToInt(ptr, gp.granularity);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "permSize")) {
            StringUtil::strToInt(ptr, gp.permSize);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "minMaxHeight")) {
            StringUtil::strToFloat(ptr, gp.minMaxHeight);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "scaleFactor")) {
            StringUtil::strToFloat(ptr, gp.scaleFactor);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "ampFactor")) {
            StringUtil::strToFloat(ptr, gp.ampFactor);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "frequencyFactor")) {
            StringUtil::strToFloat(ptr, gp.frequencyFactor);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "numOctaves")) {
            StringUtil::strToInt(ptr, gp.numOctaves);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }

    terrain.initialize(gp);
}
