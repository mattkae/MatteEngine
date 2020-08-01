#include "Include.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h> 
#include <assimp/postprocess.h>

void processNode(std::string fullPath, const aiNode* node, const aiScene* scene, LoadModel& model);
void processAnimations(aiAnimation** const animations, unsigned int numAnimations,LoadModel& model);
void postProcessBones(LoadModel& model, const aiScene* scene);
Vector3f assimpColor4ToVec3(aiColor4D inColor);
Vector3f assimpVec3ToVec3(aiVector3D v);
Matrix4x4f assimpMatrixToMatrix(const aiMatrix4x4& matrix);

GLuint nextTextureUniqueId = 1;
List<TextureInfo> outTextures;
List<LoadModel> outModels;

int main() {
    List<String> modelFiles = readModelDirectory();

    Assimp::Importer importer;
    FOR(idx, modelFiles) {
        String modelFile = modelFiles[idx];
        const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene) {
            Logger::logError("Failed to read file at path: " + modelFile);
            continue;
        }


        LoadModel model;
        model.modelPath = modelFile;
        inverse(assimpMatrixToMatrix(scene->mRootNode->mTransformation), model.inverseRootNode);
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
        LoadModel newModel;
        newModel.readLoadModel(rs);

        for (int boneIdx = 0; boneIdx < model.bones.size(); boneIdx++) {
            auto firstBone = model.bones[boneIdx];
            auto secondBone = newModel.bones[boneIdx];
            for (int i = 0; i < 16; i++) {
                if (firstBone.offsetMatrix.values[i] != firstBone.offsetMatrix.values[i]) {
                    printf("WOwowow");
                }
            }
        }
    }

    // Write out all the textures
    BinarySerializer textureInfoSerializer("assets/texture-info.mattl", SerializationMode::WRITE);
    textureInfoSerializer.writeInt32(outTextures.numElements);
    for (int idx = 0; idx < outTextures.numElements; idx++) {
        TextureInfo& textureInfo = outTextures[idx];
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

            mesh.vertices.add(&vertex);
        }

        // Read faces
        for (unsigned int faceIndex = 0; faceIndex < assimpMesh->mNumFaces; faceIndex++) {
            aiFace face = assimpMesh->mFaces[faceIndex];
            for (unsigned int indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++) {
                GLint index = face.mIndices[indexIndex];
                mesh.indices.add(&index);
            }
        }

        // Fill in tangents and bitangents for each vetex
        for (int indexIndex = 0; indexIndex < mesh.indices.numElements; indexIndex+=3) {
            GLint firstIndex = mesh.indices[indexIndex];
            GLint secondIndex = mesh.indices[indexIndex + 1];
            GLint thirdIndex = mesh.indices[indexIndex + 2];

            LoadVertex& firstVertex = mesh.vertices[firstIndex];
            LoadVertex& secondVertex = mesh.vertices[secondIndex];
            LoadVertex& thirdVertex = mesh.vertices[thirdIndex];

            Vector3f deltaPosition1 = secondVertex.position - firstVertex.position;
            Vector3f deltaPosition2 = thirdVertex.position - firstVertex.position;

            Vector2f deltaUV1 = secondVertex.texCoords - firstVertex.texCoords;
            Vector2f deltaUV2 = thirdVertex.texCoords - firstVertex.texCoords;

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            Vector3f tangent = (deltaPosition1 * deltaUV2.y   - deltaPosition2 * deltaUV1.y)*r;
            Vector3f bitangent = (deltaPosition2 * deltaUV1.x   - deltaPosition1 * deltaUV2.x)*r;

            firstVertex.tangent = tangent;
            secondVertex.tangent = tangent;
            thirdVertex.tangent = tangent;

            firstVertex.bitangent = bitangent;
            secondVertex.bitangent = bitangent;
            thirdVertex.bitangent = bitangent;
        }

        // Read bones
        for (unsigned int boneIndex = 0; boneIndex < assimpMesh->mNumBones; boneIndex++) {
            const aiBone* assimpBone = assimpMesh->mBones[boneIndex];

            LoadBone bone;
            bone.identifier = assimpBone->mName.C_Str();
            bone.offsetMatrix = assimpMatrixToMatrix(assimpBone->mOffsetMatrix);
            int foundBoneIndex = model.bones.numElements;

            bool alreadyExists = false;
            for (unsigned int existingIdx = 0; existingIdx < model.bones.numElements; existingIdx++) {
                LoadBone& existing = model.bones[existingIdx];
                if (existing.identifier == bone.identifier) {
                    foundBoneIndex = existingIdx;
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists) {
                model.bones.add(&bone);
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
    retval.nodeTransform = assimpMatrixToMatrix(node->mTransformation);
    for (unsigned int boneIdx = 0; boneIdx < model.bones.size(); boneIdx++) {
       if (model.bones[boneIdx].identifier == node->mName.C_Str()) {
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
            return node;
       }
    }

    for (unsigned int childIdx = 0; childIdx < node->mNumChildren; childIdx++) {
        const aiNode* childRetval = getRoot(node->mChildren[childIdx], model);
        if (childRetval != nullptr) {
            return node;
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

    model.rootNode = processNode(scene->mRootNode, model);
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
            node->positions = new LerpableValue<Vector3f>[node->numPositions];
            for (unsigned int positionIndex = 0; positionIndex < node->numPositions; positionIndex++) {
                node->positions[positionIndex].value = assimpVec3ToVec3(assimpNodeAnim->mPositionKeys[positionIndex].mValue);
                node->positions[positionIndex].time = assimpNodeAnim->mPositionKeys[positionIndex].mTime;
            }

            node->numScalings = assimpNodeAnim->mNumScalingKeys;
            node->scalings = new LerpableValue<Vector3f>[node->numScalings];
            for (unsigned int scalingIndex = 0; scalingIndex < node->numScalings; scalingIndex++) {
                node->scalings[scalingIndex].value = assimpVec3ToVec3(assimpNodeAnim->mScalingKeys[scalingIndex].mValue);
                node->scalings[scalingIndex].time = assimpNodeAnim->mScalingKeys[scalingIndex].mTime;
            }

            node->numRotations = assimpNodeAnim->mNumRotationKeys;
            node->rotations = new LerpableValue<Quaternion>[node->numRotations];
            for (unsigned int rotationIdx = 0; rotationIdx < node->numRotations; rotationIdx++) {
                node->rotations[rotationIdx].value.w = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.w;
                node->rotations[rotationIdx].value.x = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.x;
                node->rotations[rotationIdx].value.y = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.y;
                node->rotations[rotationIdx].value.z = assimpNodeAnim->mRotationKeys[rotationIdx].mValue.z;
                node->rotations[rotationIdx].time = assimpNodeAnim->mRotationKeys[rotationIdx].mTime;
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

    retval.values[0] = matrix.a1;
    retval.values[1] = matrix.a2;
    retval.values[2] = matrix.a3;
    retval.values[3] = matrix.a4;

    retval.values[4] = matrix.b1;
    retval.values[5] = matrix.b2;
    retval.values[6] = matrix.b3;
    retval.values[7] = matrix.b4;

    retval.values[8] = matrix.c1;
    retval.values[9] = matrix.c2;
    retval.values[10] = matrix.c3;
    retval.values[11] = matrix.c4;

    retval.values[12] = matrix.d1;
    retval.values[13] = matrix.d2;
    retval.values[14] = matrix.d3;
    retval.values[15] = matrix.d4;

    return transpose(retval);
}