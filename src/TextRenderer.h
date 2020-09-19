#pragma once

// @Investigate: For now, I'm going to use FreeType to load
// .ttf files. In the future, maybe we can investigate loading
// them ourselves, for practice's sake. For now, though, this
// seems like the best option to get a functional UI up and
// running quickly.
#include <ft2build.h>
#include <freetype/freetype.h>
#include <GL/glew.h>
#include <map>
#include "Shader.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "MyString.h"

struct CharacterRenderInfo {
    GLuint textureId = 0;
    Vector2f size;
    Vector2f bearing;
    GLuint advance = 0;
};

struct TextRenderer {
public:
    bool initialize(GLint size, GLchar* path);
    void renderText(Shader originalShader, const String& str, Vector2f position, GLfloat scale, const Vector4f& color, GLfloat scrollX = 0) const;
    void free();
	inline GLint getFontSize() const { return mSize; }
	GLfloat getStringWidth(String str, GLfloat scale) const;
	GLfloat getStringWidth(StringPointer str, GLfloat scale) const;
    GLfloat getCharWidth(char c, GLfloat scale) const;

private:
    std::map<GLchar, CharacterRenderInfo> mCharToRenderInfoMap;
    bool loadChar(GLchar c);
    FT_Library mLib;
    FT_Face mFace;
    GLint mSize;

	GLuint mVao;
    GLuint mVbo;
    GLfloat* mVertices;
};