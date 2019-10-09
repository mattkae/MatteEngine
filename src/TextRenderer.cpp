#include <glm/gtc/matrix_transform.hpp>
#include "TextRenderer.h"
#include "Logger.h"

bool TextRenderer::initialize(GLint size, GLchar* path)
{
    int libInitResult = FT_Init_FreeType(&mLib);
    if (libInitResult > 0) {
        Logger::logError("Failed to initialize FreeType Library. Error code: " + libInitResult);
        return false;
	}

    mSize = size;

	int faceInitResult = FT_New_Face(mLib, path, 0, &mFace);
    if (faceInitResult > 0) {
        Logger::logError("Failed to initialize Face for FreeType Library. Error code: " + faceInitResult);
        return false;
	}

	int setSizeResult = FT_Set_Pixel_Sizes(mFace, 0, mSize);
    if (setSizeResult > 0) {
        Logger::logError("Failed to set size for FreeType Library. Error code: " + setSizeResult);
        return false;
	}

	// @TODO: Load only the characters specified by the program
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++) {
        this->loadChar(c);   
	}

	mShader = loadShader("src/shaders/Text.vert", "src/shaders/Text.frag");

	// Initialize render VAO for a a single character
    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);
    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Initialize a reusable vertices array, and add in the texture coordinates
    mVertices = new GLfloat[24];
    mVertices[2] = 0;
    mVertices[3] = 0;
    mVertices[6] = 0;
    mVertices[7] = 1;
    mVertices[10] = 1;
    mVertices[11] = 1;
    mVertices[14] = 0;
    mVertices[15] = 0;
    mVertices[18] = 1;
    mVertices[19] = 1;
    mVertices[22] = 1;
    mVertices[23] = 0;

	return true;
}

bool TextRenderer::loadChar(GLchar c)
{
    int charLoadResult = FT_Load_Char(mFace, c, FT_LOAD_RENDER);
    if (charLoadResult) {
        Logger::logError("Failed to load character from FreeType library. Error code: " + charLoadResult);
        return false;
	}

	FT_GlyphSlot glyph = mFace->glyph;
    FT_Bitmap bitmap = glyph->bitmap;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CharacterRenderInfo renderInfo = {
        texture,
        glm::ivec2(bitmap.width, bitmap.rows),
        glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
        static_cast<GLuint>(glyph->advance.x)
    };

    mCharToRenderInfoMap.insert(std::pair<GLchar, CharacterRenderInfo>(c, renderInfo));

    return true;
}

void TextRenderer::renderText(Shader originalShader, std::string str, glm::vec2 position, GLfloat scale, glm::vec3 color, GLfloat scrollX) const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 projection = glm::ortho(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	
	useShader(mShader);
	setShaderFloat(mShader, "scrollX", scrollX);
	setShaderMat4(mShader, "uProjection", projection);
	setShaderVec3(mShader, "uColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(mVao);

    for (auto it = str.cbegin(); it != str.cend(); ++it) {
        CharacterRenderInfo renderInfo = mCharToRenderInfoMap.at(*it);

        GLfloat xStart = position.x + renderInfo.bearing.x * scale;
        GLfloat yStart = position.y - (renderInfo.size.y - renderInfo.bearing.y) * scale;
        GLfloat xEnd = xStart + renderInfo.size.x * scale;
        GLfloat yEnd = yStart + renderInfo.size.y * scale;

		mVertices[0] = xStart;
		mVertices[1] = yEnd;
		mVertices[4] = xStart;
		mVertices[5] = yStart;
		mVertices[8] = xEnd;
		mVertices[9] = yStart;
		mVertices[12] = xStart;
		mVertices[13] = yEnd;
		mVertices[16] = xEnd;
		mVertices[17] = yStart;
		mVertices[20] = xEnd;
		mVertices[21] = yEnd;

		glBindTexture(GL_TEXTURE_2D, renderInfo.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 24, mVertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

		position.x += (renderInfo.advance >> 6) * scale;
    }

	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);

	useShader(originalShader);
}

void TextRenderer::free() {
    FT_Done_Face(mFace);
    FT_Done_FreeType(mLib);

	// @TODO Free all da data
}

GLfloat TextRenderer::getStringWidth(std::string str, GLfloat scale) const {
	GLfloat width = 0;

	for (auto it = str.cbegin(); it != str.cend(); ++it) {
        CharacterRenderInfo renderInfo = mCharToRenderInfoMap.at(*it);
		width += renderInfo.size.x;
    }

	return width;
}