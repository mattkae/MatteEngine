#include "LoadModel.h"
#include "LoadModel.cpp"
#include "DirectoryReader.h"
#include "TextureInfo.h"
#include "TextureInfo.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h> 
#include <assimp/postprocess.h>
#include <map>

#include "Logger.h"
#include "Vector3f.cpp"
#include "BinarySerializer.cpp"
#include "Matrix4x4f.h"
#include "Quaternion.h"
#include "Quaternion.cpp"
#include "AnimationController.cpp"
#include "Matrix4x4f.cpp"
#include "Bone.cpp"

void processNode(std::string fullPath, const aiNode* node, const aiScene* scene, LoadModel& model);
void processAnimations(aiAnimation** const animations, unsigned int numAnimations,LoadModel& model);
void postProcessBones(LoadModel& model, const aiScene* scene);
Vector3f assimpColor4ToVec3(aiColor4D inColor);
Vector3f assimpVec3ToVec3(aiVector3D v);
Matrix4x4f assimpMatrixToMatrix(const aiMatrix4x4& matrix);

GLuint nextTextureUniqueId = 1;
std::vector<TextureInfo> outTextures;
std::vector<LoadModel> outModels;

int main() {
    std::vector<std::string> modelFiles = readModelDirectory();

    Assimp::Importer importer;
    for (std::string modelFile : modelFiles) {
        const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene) {
            Logger::logError("Failed to read file at path: " + modelFile);
            continue;
        }


        LoadModel model;
        model.modelPath = modelFile;

        processNode(modelFile, scene->mRootNode, scene, model);
        postProcessBones(model, scene);
        processAnimations(scene->mAnimations, scene->mNumAnimations, model);
        outModels.push_back(model);
    }

    // Write out all the models
    for (LoadModel& model : outModels) {
        std::string outputFile = model.modelPath.substr(0, model.modelPath.find_last_of('.')) + ".mattl";
        BinarySerializer modelSerializer(outputFile.c_str(), SerializationMode::WRITE);
        model.writeLoadModel(modelSerializer);
        modelSerializer.close();

        BinarySerializer rs(outputFile.c_str(), SerializationMode::READ);
        model.readLoadModel(rs);
    }

    // Write out all the textures
    BinarySerializer textureInfoSerializer("assets/texture-info.mattl", SerializationMode::WRITE);
    textureInfoSerializer.writeInt32(outTextures.size());
    for (TextureInfo& textureInfo : outTextures) {
        textureInfo.write(textureInfoSerializer);
    }

    return EXIT_SUCCESS;
}

const aiTextureType TEXTURES_TO_READ[] = {
    aiTextureType_AMBIENT,
    aiTextureType_DIFFUSE,
    aiTextureType_SPECULAR,
};

void processNode(std::string fullPath, 
    const aiNode* node, 
    const aiScene* scene, 
    LoadModel& model) {
    for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++) {
        LoadMesh mesh;
        const aiMesh* assimpMesh = scene->mMeshes[node->mMeshes[meshIndex]];
        
        // Read vertices
        for (unsigned int vertexIndex = 0; vertexIndex < assimpMesh->mNumVertices; vertexIndex++) {
            LoadVertex vertex;

            const aiVector3D assimpPosition = assimpMesh->mVertices[vertexIndex];
            vertex.position = getVec3(assimpPosition.x, assimpPosition.y, assimpPosition.z);

            if (assimpMesh->HasNormals()) {
                const aiVector3D assimpNormal = assimpMesh->mNormals[vertexIndex];
                vertex.normal = getVec3(assimpNormal.x, assimpNormal.y, assimpNormal.z);
            }

            if (assimpMesh->HasTextureCoords(0)) {
                vertex.texCoords = {
                    assimpMesh->mTextureCoords[0][vertexIndex].x,
                    assimpMesh->mTextureCoords[0][vertexIndex].y
                };
            }

            mesh.vertices.push_back(vertex);
        }

        // Read faces
        for (unsigned int faceIndex = 0; faceIndex < assimpMesh->mNumFaces; faceIndex++) {
            aiFace face = assimpMesh->mFaces[faceIndex];
            for (unsigned int indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++) {
                mesh.indices.push_back(face.mIndices[indexIndex]);
            }
        }

        // Read bones
        for (unsigned int boneIndex = 0; boneIndex < assimpMesh->mNumBones; boneIndex++) {
            const aiBone* assimpBone = assimpMesh->mBones[boneIndex];

            LoadBone bone;
            bone.identifier = assimpBone->mName.C_Str();
            bone.offsetMatrix = assimpMatrixToMatrix(assimpBone->mOffsetMatrix);
            int foundBoneIndex = model.bones.size();

            bool alreadyExists = false;
            for (unsigned int existingIdx = 0; existingIdx < model.bones.size(); existingIdx++) {
                LoadBone& existing = model.bones[existingIdx];
                if (existing.identifier == bone.identifier) {
                    foundBoneIndex = existingIdx;
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists) {
                model.bones.push_back(bone);
            }

            for (unsigned int weightIndex = 0; weightIndex < assimpBone->mNumWeights; weightIndex++) {
                aiVertexWeight assimpWeight = assimpBone->mWeights[weightIndex];
                LoadVertexBoneData boneData;
                boneData.boneIndex = foundBoneIndex;
                boneData.weight = assimpWeight.mWeight;
                mesh.vertices[assimpWeight.mVertexId].boneInfoList.push_back(boneData);
            }
        }

        // Read material
        if (assimpMesh->mMaterialIndex >= 0) {
            const aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];
            
            // Read in material properties
            aiColor4D color;
            aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color);
            mesh.material.diffuse = assimpColor4ToVec3(color);

            aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &color);
            mesh.material.emissive = assimpColor4ToVec3(color);

            aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color);
            mesh.material.ambient = assimpColor4ToVec3(color);

            aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color);
            mesh.material.specular = assimpColor4ToVec3(color);

            float tranparency;
            aiGetMaterialFloat(material, AI_MATKEY_OPACITY, &tranparency);
            mesh.material.transparency = tranparency;

            // Read in textures
            for (int textureTypeIndex = 0; textureTypeIndex < 3; textureTypeIndex++) {
                const aiTextureType type = TEXTURES_TO_READ[textureTypeIndex];
                for (unsigned int texIndex = 0; texIndex < material->GetTextureCount(type); texIndex++) {
                    aiString filePath;
                    material->GetTexture(aiTextureType_DIFFUSE, texIndex, &filePath);
                    
                    std::string texturePath = fullPath.substr(0, fullPath.find_last_of('/')) + "/" + std::string(filePath.C_Str());
                    TextureInfo info;
                    bool foundInfo = false;
                    for (TextureInfo textureInfo : outTextures) {
                        if (textureInfo.fullpath == texturePath) {
                            info = textureInfo;
                            foundInfo = true;
                            break;
                        }
                    }

                    if (!foundInfo) {
                        info = { texturePath, nextTextureUniqueId++ };
                        outTextures.push_back(info);
                    }

                    switch (type) {
                    case aiTextureType_AMBIENT:
                        mesh.material.ambientUniqueTextureId = info.uniqueId;
                        break;
                    case aiTextureType_DIFFUSE:
                        mesh.material.diffuseUniqueTextureId = info.uniqueId;
                        break;
                    case aiTextureType_SPECULAR:
                        mesh.material.specularUniqueTextureId = info.uniqueId;
                        break;
                    default:
                        Logger::logWarning("Encountered unknown texture type: " + type);
                        break;
                    }
                }
            }
        }


        model.meshes.push_back(mesh);
    }

    // Read children
    for (unsigned int childNodeIndex = 0; childNodeIndex < node->mNumChildren; childNodeIndex++) {
        processNode(fullPath, node->mChildren[childNodeIndex], scene, model);
    }
}

LoadBoneNode processNode(const aiNode* node, LoadModel& model, int depth = 0) {
    LoadBoneNode retval;
    for (unsigned int boneIdx = 0; boneIdx < model.bones.size(); boneIdx++) {
       if (model.bones[boneIdx].identifier == node->mName.C_Str()) {
           retval.nodeTransform = assimpMatrixToMatrix(node->mTransformation);
           retval.boneIndex = boneIdx;
           break;
       }
    }

    for (unsigned int childIdx = 0; childIdx < node->mNumChildren; childIdx++) {
        retval.children.push_back(processNode(node->mChildren[childIdx], model, depth++));
    }

    return retval;
}

const aiNode* getRoot(const aiNode* node, LoadModel& model) {
    for (unsigned int boneIdx = 0; boneIdx < model.bones.size(); boneIdx++) {
       if (model.bones[boneIdx].identifier == node->mName.C_Str()) {
            inverse(assimpMatrixToMatrix(node->mTransformation), model.inverseRootNode);
            return node;
       }
    }

    for (unsigned int childIdx = 0; childIdx < node->mNumChildren; childIdx++) {
        const aiNode* childRetval = getRoot(node->mChildren[childIdx], model);
        if (childRetval != nullptr) {
            return childRetval;
        }
    }

    return nullptr;
}

// In this step, we match the bone indices up with the bone identifiers, so that
// we don't have to send the identifiers over the wire later on.
void postProcessBones(LoadModel& model, const aiScene* scene) {
    if (model.bones.size() == 0) {
        return;
    }

    model.rootNode = processNode(getRoot(scene->mRootNode, model), model);
}

// Read animations
void processAnimations(aiAnimation**const animations, unsigned int numAnimations, LoadModel& model) {
    for (unsigned int animIndex = 0; animIndex < numAnimations; animIndex++) {
        const aiAnimation* assimpAnimation = animations[animIndex];
        Animation animation;
        animation.duration = assimpAnimation->mDuration;
        animation.ticksPerSecond = assimpAnimation->mTicksPerSecond;
        animation.numNodes = assimpAnimation->mNumChannels;
        animation.nodes = new AnimationNode[animation.numNodes];

        for (unsigned int channelIndex = 0; channelIndex < assimpAnimation->mNumChannels; channelIndex++) {
            // For each channel, look up the node connected to that channel
            const aiNodeAnim* assimpNodeAnim = assimpAnimation->mChannels[channelIndex];
            std::string identifier = assimpNodeAnim->mNodeName.C_Str();

            AnimationNode* node = &animation.nodes[channelIndex];
            for (unsigned int boneIndex = 0; boneIndex < model.bones.size(); boneIndex++) {
                if (identifier == model.bones[boneIndex].identifier) {
                    node->boneIndex = boneIndex;
                    break;
                }
            }

            node->numPositions = assimpNodeAnim->mNumPositionKeys;
            node->positions = new Vector3f[node->numPositions];
            for (unsigned int positionIndex = 0; positionIndex < node->numPositions; positionIndex++) {
                node->positions[positionIndex] = assimpVec3ToVec3(assimpNodeAnim->mPositionKeys[positionIndex].mValue);
            }

            node->numScalings = assimpNodeAnim->mNumScalingKeys;
            node->scalings = new Vector3f[node->numScalings];
            for (unsigned int scalingIndex = 0; scalingIndex < node->numScalings; scalingIndex++) {
                node->scalings[scalingIndex] = assimpVec3ToVec3(assimpNodeAnim->mScalingKeys[scalingIndex].mValue);
            }

            node->numRotations = assimpNodeAnim->mNumRotationKeys;
            node->rotations = new Quaternion[node->numRotations];
            for (unsigned int rotationIdx = 0; rotationIdx < node->numRotations; rotationIdx++) {
                node->rotations[rotationIdx].w = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.w;
                node->rotations[rotationIdx].x = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.x;
                node->rotations[rotationIdx].y = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.y;
                node->rotations[rotationIdx].z = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.z;
            }
        }

        model.animations.push_back(animation);
    }
}

Vector3f assimpColor4ToVec3(aiColor4D inColor) {
    return { inColor.r, inColor.g, inColor.b };
}

Vector3f assimpVec3ToVec3(aiVector3D v) {
    return { v.x, v.y, v.z };
}

Matrix4x4f assimpMatrixToMatrix(const aiMatrix4x4& matrix) {
    Matrix4x4f retval;

    retval.values[0] = (GLfloat)matrix.a1; 
    retval.values[1] = (GLfloat)matrix.b1;  
    retval.values[2] = (GLfloat)matrix.c1; 
    retval.values[3] = (GLfloat)matrix.d1;

    retval.values[4] = (GLfloat)matrix.a2; 
    retval.values[5] = (GLfloat)matrix.b2;  
    retval.values[6] = (GLfloat)matrix.c2; 
    retval.values[7] = (GLfloat)matrix.d2;

    retval.values[8] = (GLfloat)matrix.a3; 
    retval.values[9] = (GLfloat)matrix.b3;  
    retval.values[10] = (GLfloat)matrix.c3; 
    retval.values[11] = (GLfloat)matrix.d3;

    return retval;
}