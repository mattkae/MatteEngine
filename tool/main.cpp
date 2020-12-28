// TODO: Maybe think of a better way to add our .cpp files here
#include "../src/MyString.cpp"
#include "../src/Logger.cpp"
#include "../src/Vector2f.cpp"
#include "../src/Vector3f.cpp"
#include "../src/Matrix4x4f.cpp"
#include "../src/BinarySerializer.cpp"
#include "../src/TextureInfo.cpp"
#include "../src/LoadModel.cpp"
#include "../src/AnimationController.cpp"
#include "../src/Bone.cpp"
#include "../src/Quaternion.cpp"

#include "ToolModelLoader.h"
#include "../src/MyString.h"
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
