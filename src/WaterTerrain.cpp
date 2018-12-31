#include "WaterTerrain.h"
#include "Shader.h"

// TODO: WIP
bool WaterTerrain::generate(const std::vector<glm::vec2> positions) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //    glBufferData(GL_ARRAY_BUFFER,

    glBindVertexArray(0);

    hasGenerated = true;
    return true;
}

void WaterTerrain::render(const Shader &shader,
                          const glm::vec3 yOffsets) const {
    shader.set_uniform_3f("uYoffsets", yOffsets[0], yOffsets[1], yOffsets[2]);

    glBindVertexArray(vao);

    glBindVertexArray(0);
}

void WaterTerrain::free() {
    if (hasGenerated) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(2, &vbo); // todo: check if this actually works
    }

    hasGenerated = false;
}
