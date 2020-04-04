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
#include "AnimationController.cpp"

void processNode(std::string fullPath, 
    const aiNode* node, 
    const aiScene* scene, 
    LoadModel& model);
void processAnimations(aiAnimation** const animations, 
    unsigned int numAnimations,
    LoadModel& model);
Vector3f assimpColor4ToVec3(aiColor4D inColor);
Vector3f assimpVec3ToVec3(aiVector3D v);
Matrix4x4f assimpMatrixToMatrix(const aiMatrix4x4& matrix);

unsigned int nextNodeNameUniqueId;
GLuint nextTextureUniqueId = 1;

std::vector<TextureInfo> outTextures;
std::vector<LoadModel> outModels;
std::map<std::string, unsigned int> nodeNameToUniqueIdMapping;

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

        nodeNameToUniqueIdMapping.clear();
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

    std::string nodeName = node->mName.C_Str();
    if (nodeNameToUniqueIdMapping.find(nodeName) == nodeNameToUniqueIdMapping.end()) {
        nodeNameToUniqueIdMapping.insert(std::pair<std::string, unsigned int>(nodeName, nextNodeNameUniqueId++));
    }

    const unsigned int nodeUniqueId = nodeNameToUniqueIdMapping.at(nodeName);

    for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++) {
        LoadMesh mesh;
        const aiMesh* assimpMesh = scene->mMeshes[node->mMeshes[meshIndex]];
        
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

        for (unsigned int faceIndex = 0; faceIndex < assimpMesh->mNumFaces; faceIndex++) {
            aiFace face = assimpMesh->mFaces[faceIndex];
            for (unsigned int indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++) {
                mesh.indices.push_back(face.mIndices[indexIndex]);
            }
        }

        for (unsigned int boneIndex = 0; boneIndex < assimpMesh->mNumBones; boneIndex++) {
            const aiBone* assimpBone = assimpMesh->mBones[boneIndex];

            LoadBone bone;
            std::string boneName = assimpBone->mName.C_Str();
            if (nodeNameToUniqueIdMapping.find(boneName) == nodeNameToUniqueIdMapping.end()) {
                nodeNameToUniqueIdMapping.insert(std::pair<std::string, unsigned int>(boneName, nextNodeNameUniqueId++));
            }

            bone.nodeUniqueId = nodeNameToUniqueIdMapping[boneName];
            bone.offsetMatrix = assimpMatrixToMatrix(assimpBone->mOffsetMatrix);
            model.bones.push_back(bone);

            for (unsigned int weightIndex = 0; weightIndex < assimpBone->mNumWeights; weightIndex++) {
                aiVertexWeight assimpWeight = assimpBone->mWeights[weightIndex];
                LoadVertexBoneData boneData;
                boneData.boneIndex = model.bones.size() - 1;
                boneData.weight = assimpWeight.mWeight;
                //printf("Bone %s, %f\n", assimpBone->mName.C_Str(), boneData.weight);
                mesh.vertices[assimpWeight.mVertexId].boneInfoList.push_back(boneData);
            }
        }

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

    for (unsigned int childNodeIndex = 0; childNodeIndex < node->mNumChildren; childNodeIndex++) {
        processNode(fullPath, node->mChildren[childNodeIndex], scene, model);
    }
}

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
            std::string nodeName = assimpNodeAnim->mNodeName.C_Str();
            unsigned int uniqueId = nodeNameToUniqueIdMapping.at(nodeName);

            AnimationNode& node = animation.nodes[channelIndex];
            node.nodeUniqueId = uniqueId;
            node.numPositions = assimpNodeAnim->mNumPositionKeys;
            node.positions = new Vector3f[node.numPositions];
            for (unsigned int positionIndex = 0; positionIndex < node.numPositions; positionIndex++) {
                node.positions[positionIndex] = assimpVec3ToVec3(assimpNodeAnim->mPositionKeys[positionIndex].mValue);
            }

            node.numScalings = assimpNodeAnim->mNumScalingKeys;
            node.scalings = new Vector3f[node.numScalings];
            for (unsigned int scalingIndex = 0; scalingIndex < node.numScalings; scalingIndex++) {
                node.scalings[scalingIndex] = assimpVec3ToVec3(assimpNodeAnim->mScalingKeys[scalingIndex].mValue);
            }

            node.numRotations = assimpNodeAnim->mNumRotationKeys;
            node.rotations = new Quaternion[node.numRotations];
            for (unsigned int rotationIdx = 0; rotationIdx < node.numRotations; rotationIdx++) {
                node.rotations[rotationIdx].w = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.w;
                node.rotations[rotationIdx].x = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.x;
                node.rotations[rotationIdx].y = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.y;
                node.rotations[rotationIdx].z = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.z;
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

    for (int index = 0; index < 16; index++) {
        retval.values[index] = matrix[floor(index / 4)][index % 4];
    }

    return retval;
}