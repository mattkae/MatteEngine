#include "SceneLoader.h"
#include "Sphere.h"
#include "StringUtil.h"
#include "FileHelper.h"
#include "ShaderUniformMapping.h"
#include "Logger.h"

const char* START_OBJECT_TOKEN = "::";
const char* END_OBJECT_TOKEN = ";;";
const char* IGNORE_OBJECT_TOKEN = "//";

void ignoreObject(FILE* mFile, char buffer[StringUtil::DEFAULT_BUFFER_SIZE]) {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void SceneLoader::loadScene(const char* filepath) {
    mFile = FileHelper::openFile(filepath);
    mScene->initialize();

    if (mFile == NULL) {
        // @TODO: Error out
        logger_error("Unable to open mFile: %s", filepath);
        return;
    }

    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        const char* token = nullptr;
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            ptr += strlen(START_OBJECT_TOKEN);
            token = START_OBJECT_TOKEN;
            if (StringUtil::startsWith(ptr, "lights")) {
                loadLights();
            } else if (StringUtil::startsWith(ptr, "entities")) {
                loadEntityList();
            } else if (StringUtil::startsWith(ptr, "spheres")) {
                loadSpheres();
            } else if (StringUtil::startsWith(ptr, "particleEmitters")) {
                loadParticleEmitters();
            } else if (StringUtil::startsWith(ptr, "terrain")) {
                loadTerrain();
            } else if (StringUtil::startsWith(ptr, "camera")) {
                loadCamera();
            }
        } else if (StringUtil::ifEqualWalkToValue(ptr, "useDeferredRendering")) {
            StringUtil::strToBool(ptr, mScene->useDefferredRendering);
        } else if (StringUtil::startsWith(ptr, IGNORE_OBJECT_TOKEN)) {
            ignoreObject(mFile, buffer);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "textures")) {
            mModelLoader.loadTextureList(ptr);
        }
    }

    fclose(mFile);
}

void SceneLoader::loadLight() {
    Light light;

    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
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

    light.initialize(mScene->numLightsUsed);
    mScene->lights[mScene->numLightsUsed] = light;
    mScene->numLightsUsed++;
}

void SceneLoader::loadLights() {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            loadLight();
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void SceneLoader::loadEntity() {
    u8 eId = mScene->systemEngine.registerEntity();

    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            ptr += strlen(START_OBJECT_TOKEN);
            if (StringUtil::ifEqualWalkToValue(ptr, "renderable")) {
                RenderableEntity re;

                while (StringUtil::processLine(mFile, buffer, ptr)) {
                    if (StringUtil::ifEqualWalkToValue(ptr, "path")) {
                        ModelLoader::ModelLoadResult retval = mModelLoader.loadSerializedModel(ptr);
                        re.mModel = retval.model;
                        //mScene->models[numModels] = retval.model;
                        //mScene->modelBoundingBoxes[numModels] = retval.box;
                    } else if (StringUtil::ifEqualWalkToValue(ptr, "translation")) {
                        StringUtil::strToVec3(ptr, re.mModel.translation);
                    } else if (StringUtil::ifEqualWalkToValue(ptr, "scale")) {
                        StringUtil::strToVec3(ptr, re.mModel.scale);
                    } else if (StringUtil::ifEqualWalkToValue(ptr, "rotation")) {
                        StringUtil::strToQuaternion(ptr, re.mModel.rotation);
                    } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
                        break;
                    }
                }

                re.mEntityId = eId;
                re.mShouldRender = true;
                mScene->systemEngine.mRenderSystem.mEntities.add(re);
            }
        }
    }
}

void SceneLoader::loadEntityList() {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            loadEntity();
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void SceneLoader::loadSphere() {
    char* ptr;
    Sphere sphere;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
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

    //initializeSphere();
}

void SceneLoader::loadSpheres() {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            loadSphere();
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void SceneLoader::loadRandomizableFloat(RandomizableFloat& v) {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "baseValue")) {
            StringUtil::strToFloat(ptr, v.baseValue);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "randomOffset")) {
            StringUtil::strToFloat(ptr, v.randomOffset);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void SceneLoader::loadRandomizableVec3(RandomizableVec3& v) {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "baseValue")) {
            StringUtil::strToVec3(ptr, v.baseValue);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "randomOffset")) {
            StringUtil::strToVec3(ptr, v.randomOffset);
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void SceneLoader::loadFloatFunc(FunctionFloat& v) {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
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

void SceneLoader::loadVec4Func(FunctionVec4& v) {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
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

void SceneLoader::loadVec3Func(FunctionVec3& v) {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
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

void SceneLoader::loadParticleEmitter() {
    char* ptr;
    int initialParticleCount = 0;
    ParticleEmitter emitter;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            ptr += strlen(START_OBJECT_TOKEN);
            if (StringUtil::ifEqualWalkToValue(ptr, "spawnFrequencySeconds")) {
                loadRandomizableFloat(emitter.spawnFrequencySeconds);
            } else if (StringUtil::ifEqualWalkToValue(ptr, "colorFunction")) {
                loadVec4Func(emitter.colorFunction);
            } else if (StringUtil::ifEqualWalkToValue(ptr, "movementFunction")) {
                loadVec3Func(emitter.movementFunction);
            } else if (StringUtil::ifEqualWalkToValue(ptr, "particlePosition")) {
                loadRandomizableVec3(emitter.particlePosition);
            } else if (StringUtil::ifEqualWalkToValue(ptr, "particleTimeToLiveSeconds")) {
                loadRandomizableFloat(emitter.particleTimeToLiveSeconds);
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

void SceneLoader::loadParticleEmitters() {
    mScene->numEmitters = 0;
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::startsWith(ptr, START_OBJECT_TOKEN)) {
            loadParticleEmitter();
            mScene->numEmitters++;
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}

void SceneLoader::loadTerrain() {
    GenerationParameters gp;
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
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

    mScene->mTerrain.initialize(gp);
}


void SceneLoader::loadCamera() {
    char* ptr;
    while (StringUtil::processLine(mFile, buffer, ptr)) {
        if (StringUtil::ifEqualWalkToValue(ptr, "position")) {
            Vector3f cameraPosition;
            StringUtil::strToVec3(ptr, cameraPosition);
            mScene->mCamera.position = cameraPosition;
        } else if (StringUtil::ifEqualWalkToValue(ptr, "pitch")) {
            float pitch;
            StringUtil::strToFloat(ptr, pitch);
            mScene->mCamera.pitch = pitch;
        } else if (StringUtil::startsWith(ptr, END_OBJECT_TOKEN)) {
            break;
        }
    }
}