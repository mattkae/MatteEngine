#pragma once

// @Investigate: For now, I'm going to use FreeType to load
// .ttf files. In the future, maybe we can investigate loading
// them ourselves, for practice's sake. For now, though, this
// seems like the best option to get a functional UI up and
// running quickly.
#include <ft2build.h>
#include <freetype/freetype.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include "Shader.h"
#include "Constants.h"

struct CharacterRenderInfo {
    GLuint textureId = 0;
    glm::vec2 size = glm::vec2(0, 0);
    glm::vec2 bearing = glm::vec2(0, 0);
    GLuint advance = 0;
};

struct TextRenderer {
public:
    bool initialize(GLint size, GLchar* path);
    void renderText(std::string str, glm::vec2 position, GLfloat scale, glm::vec3 color) const;
    void free();
	inline GLint getFontSize() const { return mSize; }
	GLfloat getStringWidth(std::string str, GLfloat scale) const;

private:
    glm::mat4 mProjection;
    std::map<GLchar, CharacterRenderInfo> mCharToRenderInfoMap;
    bool loadChar(GLchar c);
    FT_Library mLib;
    FT_Face mFace;
    GLint mSize;
    Shader mShader;

	GLuint mVao;
    GLuint mVbo;
    GLfloat* mVertices;
};