#include "Skybox.h"
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

const GLfloat vertices[] = {1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f};

const GLuint indices[] = {0, 1, 3, 1, 2, 3};

void to_json(json &j, const Skybox &skybox) {
    // TODO
}

void from_json(const json &j, Skybox &skybox) {
    std::string skyboxPaths[6];
    j.at("front").get_to(skyboxPaths[0]);
    j.at("back").get_to(skyboxPaths[1]);
    j.at("up").get_to(skyboxPaths[2]);
    j.at("down").get_to(skyboxPaths[3]);
    j.at("right").get_to(skyboxPaths[4]);
    j.at("left").get_to(skyboxPaths[5]);
    skybox.generate(skyboxPaths);
}

bool Skybox::generate(std::string *paths) {
    mSkyboxShader = loadShader("src/shaders/skybox.vert", "src/shaders/skybox.frag");

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
        unsigned char *image =
            SOIL_load_image(paths[fidx].c_str(), &w, &h, 0, SOIL_LOAD_RGBA);
        if (!image) {
            cerr << "Unable to load image for skybox from path " << paths[fidx]
                 << endl;
            return false;
        }

        if (w != h) {
            cerr << "Width and height are unequal in skybox: " << w
                 << " != " << h << endl;
            return false;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, 0, GL_RGBA, w, h, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, image);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLfloat),
                 &vertices[0], GL_STATIC_DRAW);

    // Index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    mIsInited = true;

    return true;
}

void Skybox::render(const BetterCamera &camera) const {
    if (!mIsInited) {
        return;
	}

	useShader(mSkyboxShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    glDisable(GL_DEPTH_TEST);
	
	setShaderMat3(mSkyboxShader, "uView", fromMat4(getCameraViewMatrix(camera)));
	setShaderMat4(mSkyboxShader, "uProjection", getCameraProjection(camera));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void Skybox::free() {
    if (mIsInited) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    mIsInited = false;
}
