#include "Skybox.h"
#include <SOIL.h>
#include <iostream>

using namespace std;

const GLfloat vertices[] = {1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f};

const GLuint indices[] = {0, 1, 3, 1, 2, 3};

void initSkybox(Skybox& skybox, const char paths[6][128]) {
    skybox.mSkyboxShader = loadShader("src/shaders/skybox.vert", "src/shaders/skybox.frag");

    glGenTextures(1, &skybox.mTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.mTexture);
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
            cerr << "Unable to load image for skybox from path " << paths[fidx] << endl;
			return;
        }

        if (w != h) {
            cerr << "Width and height are unequal in skybox: " << w << " != " << h << endl;
            return;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Generate buffers
    glGenVertexArrays(1, &skybox.vao);
    glGenBuffers(1, &skybox.vbo);
    glGenBuffers(1, &skybox.ebo);

    glBindVertexArray(skybox.vao);

    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // Index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void renderSkybox(const Skybox& skybox, const BetterCamera &camera) {
    if (skybox.vao == 0) {
        return;
	}

	useShader(skybox.mSkyboxShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.mTexture);
    glDisable(GL_DEPTH_TEST);
	
	setShaderMat3(skybox.mSkyboxShader, "uView", fromMat4(getCameraViewMatrix(camera)));
	setShaderMat4(skybox.mSkyboxShader, "uProjection", getCameraProjection(camera));

    glBindVertexArray(skybox.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void freeSkybox(Skybox& skybox) {
    if (skybox.vao != 0) {
        glDeleteVertexArrays(1, &skybox.vao);
        glDeleteBuffers(1, &skybox.vbo);
        glDeleteBuffers(1, &skybox.ebo);
    }
}
