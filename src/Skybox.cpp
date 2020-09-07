#include "Skybox.h"
#include "ShaderUniformMapping.h"
#include <SOIL.h>

const GLfloat vertices[] = {1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f};

const GLuint indices[] = {0, 1, 3, 1, 2, 3};

void Skybox::initialize(const char paths[6][128]) {
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Set data from image
    for (int fidx = 0; fidx < 6; fidx++) {
        GLint w, h;
        unsigned char *image = SOIL_load_image(paths[fidx], &w, &h, 0, SOIL_LOAD_RGBA);
        if (!image) {
            Logger::logError("Unable to load image for skybox from path " + std::string(paths[fidx]));
			return;
        }

        if (w != h) {
            Logger::logError("Width and height are unequal in skybox: " + std::string(paths[fidx]));
            return;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Generate buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // Index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Skybox::render(const Camera &camera) const {
    if (vao == 0) {
        return;
	}

	useShader(ShaderUniformMapping::GlobalSkyboxShaderMapping.shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    glDisable(GL_DEPTH_TEST);
	
	setShaderMat3(ShaderUniformMapping::GlobalSkyboxShaderMapping.VIEW, fromMat4(camera.viewMatrix));
	setShaderMat4(ShaderUniformMapping::GlobalSkyboxShaderMapping.PROJECTION, camera.projectionMatrix);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void Skybox::free() {
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
}
