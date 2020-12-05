// TODO: Maybe think of a better way to add our .cpp files here
#include "MyString.cpp"
#include "Logger.cpp"
#include "Vector2f.cpp"
#include "Vector3f.cpp"
#include "Matrix4x4f.cpp"
#include "BinarySerializer.cpp"
#include "TextureInfo.cpp"
#include "LoadModel.cpp"
#include "AnimationController.cpp"
#include "Bone.cpp"
#include "Quaternion.cpp"

#include "ToolModelLoader.h"
#include "MyString.h"
#include "DirectoryReader.h"


int main() {
    ToolModelLoader tml;
    List<String> modelFiles = readModelDirectory();

    FOREACH(modelFiles) {
        tml.loadModel(value->getValue());
    }

    modelFiles.deallocate();
    tml.writeModels();
    tml.writeTextures();

    FOREACH(modelFiles) {
        value->free();
    }

    return EXIT_SUCCESS;
}