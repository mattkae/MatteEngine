#include "ToolModelLoader.h"
#include "AssimpHelper.h"
#include "Logger.h"
#include "BinarySerializer.h"

const int BUFFER_SIZE = 256;

bool ToolModelLoader::loadModel(char* path) {
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        logger_error("Failed to read file at path: %s", path);
        return false;
    }


    LoadModel model;
    strncpy_s(model.modelPath, path, strlen(path));
    inverse(assimpMatrixToMatrix(scene->mRootNode->mTransformation), model.inverseRootNode);
    processNode(path, scene->mRootNode, scene, model);
    postProcessBones(model, scene);
    processAnimations(scene->mAnimations, scene->mNumAnimations, model);
    mModelList.add(&model);
    return true;
}

const aiTextureType TEXTURES_TO_READ[] = {
    aiTextureType_AMBIENT,
    aiTextureType_DIFFUSE,
    aiTextureType_SPECULAR,
};

int lastIndexOf(char* str, char c) {
    int retval = -1;
    int length = strlen(str);
    for (int idx = 0; idx < length; idx++) {
        if (str[idx] == c) {
            retval = idx;
        }
    }

    return retval;
}

void ToolModelLoader::processNode(char* fullPath, const aiNode* node, const aiScene* scene, LoadModel& model) {
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

            mesh.vertices.add(vertex);
        }

        // Read faces
        for (unsigned int faceIndex = 0; faceIndex < assimpMesh->mNumFaces; faceIndex++) {
            aiFace face = assimpMesh->mFaces[faceIndex];
            for (unsigned int indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++) {
                mesh.indices.add((GLint)face.mIndices[indexIndex]);
            }
        }

        // Fill in tangents and bitangents for each vetex
        for (size_t indexIndex = 0; indexIndex < mesh.indices.numElements; indexIndex+=3) {
            if (mesh.indices.numElements >= indexIndex + 2) {
                continue;
            }


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
                if (existing.identifier.equals(bone.identifier)) {
                    foundBoneIndex = existingIdx;
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists) {
                model.bones.add(bone);
            }

            for (unsigned int weightIndex = 0; weightIndex < assimpBone->mNumWeights; weightIndex++) {
                aiVertexWeight assimpWeight = assimpBone->mWeights[weightIndex];
                LoadVertexBoneData boneData;
                boneData.boneIndex = foundBoneIndex;
                boneData.weight = assimpWeight.mWeight;
                List<LoadVertexBoneData>* boneInfoList = &mesh.vertices[assimpWeight.mVertexId].boneInfoList;
                boneInfoList->add(boneData);
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

            float shininess;
            aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
            mesh.material.transparency = shininess;

            // Read in textures
            StringBuilder texturePathBuilder;
            for (int textureTypeIndex = 0; textureTypeIndex < 3; textureTypeIndex++) {
                const aiTextureType type = TEXTURES_TO_READ[textureTypeIndex];
                for (unsigned int texIndex = 0; texIndex < material->GetTextureCount(type); texIndex++) {
                    aiString filePath;
                    material->GetTexture(aiTextureType_DIFFUSE, texIndex, &filePath);
                    
                    int lastIndexOfSlash = lastIndexOf(fullPath, '/');
                    if (lastIndexOfSlash == -1) {
                        logger_error("Unable to find / in str: %s", fullPath);
                        continue;
                    }

                    char prePathBuffer[BUFFER_SIZE];
                    strncpy_s(prePathBuffer, fullPath, lastIndexOfSlash);
                    prePathBuffer[lastIndexOfSlash] = '\0';
                    texturePathBuilder.clear();
                    texturePathBuilder.format("%s/%s", prePathBuffer, filePath.C_Str());
                    String texturePath = texturePathBuilder.toString();

                    TextureInfo info;
                    bool foundInfo = false;
                    FOREACH(mTextureList) {
                        if (strcmp(value->fullpath.getValue(), texturePath.getValue()) == 0) {
                            info = *value;
                            foundInfo = true;
                            break;
                        }
                    }

                    if (!foundInfo) {
                        info = { texturePath, nextTextureUniqueId++ };
                        mTextureList.add(&info);
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
                        logger_warning("Encountered unknown texture type: %d", type);
                        break;
                    }
                }
            }
        }


        model.meshes.add(mesh);
    }

    // Read children
    for (unsigned int childNodeIndex = 0; childNodeIndex < node->mNumChildren; childNodeIndex++) {
        processNode(fullPath, node->mChildren[childNodeIndex], scene, model);
    }
}

LoadBoneNode processBoneNode(const aiNode* node, LoadModel& model, int depth = 0) {
    LoadBoneNode retval;
    retval.nodeTransform = assimpMatrixToMatrix(node->mTransformation);
    for (unsigned int boneIdx = 0; boneIdx < model.bones.numElements; boneIdx++) {
       if (model.bones[boneIdx].identifier.equalsCstr(node->mName.C_Str())) {
           retval.boneIndex = boneIdx;
           break;
       }
    }

    for (unsigned int childIdx = 0; childIdx < node->mNumChildren; childIdx++) {
        retval.children.add(processBoneNode(node->mChildren[childIdx], model, depth++));
    }

    return retval;
}

const aiNode* getRoot(const aiNode* node, LoadModel& model) {
    for (unsigned int boneIdx = 0; boneIdx < model.bones.numElements; boneIdx++) {
       if (model.bones[boneIdx].identifier.equalsCstr(node->mName.C_Str())) {
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
void ToolModelLoader::postProcessBones(LoadModel& model, const aiScene* scene) {
    if (model.bones.numElements == 0) {
        return;
    }

    model.rootNode = processBoneNode(scene->mRootNode, model);
}

// Read animations
void ToolModelLoader::processAnimations(aiAnimation**const animations, unsigned int numAnimations, LoadModel& model) {
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
            String identifier = assimpNodeAnim->mNodeName.C_Str();

            AnimationNode* node = &animation.nodes[channelIndex];
            for (unsigned int boneIndex = 0; boneIndex < model.bones.numElements; boneIndex++) {
                if (identifier.equals(model.bones[boneIndex].identifier)) {
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

        model.animations.add(animation);
    }
}


void ToolModelLoader::writeModels() {
    FOREACH (mModelList) {
        int lastIndexOfDot = lastIndexOf(value->modelPath, '.');
        if (lastIndexOfDot == -1) {
            logger_error("Unable to find . in str: %s", value->modelPath);
            continue;
        }

        char pathBuffer[BUFFER_SIZE];
        strncpy_s(pathBuffer, value->modelPath, lastIndexOfDot);
        pathBuffer[lastIndexOfDot] = '\0';
        strcat_s(pathBuffer, ".mattl");
        logger_info("Writing model to path: %s...", pathBuffer);
        BinarySerializer modelSerializer(pathBuffer, SerializationMode::WRITE);
        value->writeLoadModel(modelSerializer);
        modelSerializer.close();
        logger_info("Wrote model success");

        BinarySerializer rs(pathBuffer, SerializationMode::READ);
        LoadModel newModel;
        newModel.readLoadModel(rs);
    }
}

void ToolModelLoader::writeTextures() {
    BinarySerializer textureInfoSerializer("assets/texture-info.mattl", SerializationMode::WRITE);
    textureInfoSerializer.writeInt32(mTextureList.numElements);
    FOREACH(mTextureList) {
        value->write(textureInfoSerializer);
    }
}