#include "Terrain.h"
#include "SimplexNoise.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

static float random_float(float min, float max) {
    return (min + (rand() / (static_cast<float>(RAND_MAX) / (max - min))));
}

glm::vec3 calculate_color(float height, float max) {
    glm::vec3 color;
    if (height > max * 0.98f) {
        color = glm::vec3(random_float(0.95f, 1.0f));
    } else if (height > max * 0.85f) {
        color = glm::vec3(0.84, 0.74, 0.84);
    } else if (height > (-max) * 0.25f) {
        color = glm::vec3(0.0, random_float(0.5f, 0.6f), 0.1);
    } else if (height > (-max) * 0.35f) {
        color = glm::vec3(0.925, 0.78, 0.68);
    } else {
        color =
            glm::vec3(0.1, random_float(0.3f, 0.5f), random_float(0.8f, 1.f));
    }

    return color;
}

void to_json(json &j, const Terrain &terrain) {
    // TODO!
}

void from_json(const json &j, Terrain &terrain) {
    GenerationParameters genParams;
    j.at("size").get_to<int>(genParams.size);
    j.at("granularity").get_to<int>(genParams.granularity);
    j.at("permSize").get_to<int>(genParams.permSize);
    j.at("scaleFactor").get_to<float>(genParams.scaleFactor);
    j.at("minMaxHeight").get_to<float>(genParams.minMaxHeight);
    j.at("ampFactor").get_to<float>(genParams.ampFactor);
    j.at("frequencyFactor").get_to<float>(genParams.frequencyFactor);
    j.at("numOctaves").get_to<int>(genParams.numOctaves);
    terrain.generate(genParams);
}

bool Terrain::generate(const GenerationParameters &params) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    float squareSize = ((float)params.size / (float)params.granularity) / 2.f;
    float vertexArrySize = 6 * params.granularity * params.granularity;
    float indexArrySize = 6 * params.granularity * params.granularity;

    // Establish array of values
    int* perm = get_simplex_array(params.permSize);

    // Generate vertices
    int permIndexCap = (params.permSize / 2) - 1;
    int index = 0;
    GLfloat *vertices = new GLfloat[vertexArrySize];
    int halfSize = params.granularity / 2;
    for (int r = -halfSize; r < halfSize; r++) {
        for (int c = -halfSize; c < halfSize; c++) {
            glm::vec2 position = squareSize * glm::vec2(c, r);
            vertices[index] = position.x;
            vertices[index + 1] = calculate_simplex_value(
                (halfSize * squareSize) + position, params.minMaxHeight,
                params.scaleFactor, params.ampFactor, params.frequencyFactor,
                params.numOctaves, perm, permIndexCap);
            vertices[index + 2] = position.y;

            glm::vec3 color =
                calculate_color(vertices[index + 1], params.minMaxHeight);
            vertices[index + 3] = color.x;
            vertices[index + 4] = color.y;
            vertices[index + 5] = color.z;

            index += 6;
        }
    }

    // Generate indices
    index = 0;
    GLuint *indices = new GLuint[indexArrySize];
    for (GLuint r = 0; r < params.granularity - 1; r++) {
        for (GLuint c = 0; c < params.granularity - 1; c++) {
            if (r % 2 == 0) {
                GLuint idx = r * params.granularity + c;
                indices[index] = idx;
                indices[index + 1] = idx + 1;
                indices[index + 2] = idx + params.granularity;
                indices[index + 3] = idx + params.granularity;
                indices[index + 4] = idx + params.granularity + 1;
                indices[index + 5] = idx + 1;
            } else {
                GLuint idx =
                    r * params.granularity + (params.granularity - c - 1);
                indices[index] = idx;
                indices[index + 1] = idx - 1;
                indices[index + 2] = idx + params.granularity;
                indices[index + 3] = idx + params.granularity;
                indices[index + 4] = idx + params.granularity - 1;
                indices[index + 5] = idx - 1;
            }

            index += 6;
        }
    }

    // Put the vertex data into OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexArrySize * sizeof(GLfloat), vertices,
                 GL_STATIC_DRAW);

    // Put the index data into OpenGL
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrySize * sizeof(GLuint),
                 indices, GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
                          (GLvoid *)0);

    // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
                          (GLvoid *)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);

    numIndices = indexArrySize;
    hasGenerated = true;
    delete[] vertices;
    delete[] indices;
    return true;
}

static glm::mat4 get_viewport() {
    glm::mat4 s = glm::scale(
        glm::mat4(1.0),
        glm::vec3(Constants.width / 2.f, Constants.height / 2.f, 1.f / 2.f));
    glm::mat4 t = glm::translate(
        glm::mat4(1.0),
        glm::vec3(Constants.width / 2.f, Constants.height / 2.f, 1.f / 2.f));
    return t * s;
}

const static glm::mat4 VIEWPORT = get_viewport();

void Terrain::render(const Shader &shader, const Camera &camera) const {
    camera.render(shader);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.set_uniform_matrix_4fv("uViewportMatrix", 1, GL_FALSE,
                                  glm::value_ptr(VIEWPORT));
    shader.set_uniform_1f("uLineWidth", 1.0);
    shader.set_uniform_1i("uShowWireframe", wireframeMode);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Terrain::free() {
    if (hasGenerated) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    hasGenerated = false;
}
