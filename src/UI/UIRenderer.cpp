#include "UIRenderer.h"
#include "../ShaderUniformMapping.h"
#include "../Shader.h"
#include "../Matrix4x4f.h"
#include "../App.h"
#include "../MyString.h"
#include "../MathHelper.h"

namespace UI {

	void UIRenderer::initialize() {
		glGenVertexArrays(1, &mVao);
		glGenBuffers(1, &mVbo);

		glBindVertexArray(mVao);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBufferData(GL_ARRAY_BUFFER, MAX_BATCH_VERTICES * sizeof(UIBatchVertex), vertices, GL_DYNAMIC_DRAW);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIBatchVertex), (GLvoid *)0);

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(UIBatchVertex), (GLvoid *)offsetof(UIBatchVertex, color));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void UIRenderer::add(BatchedBox* box) {
		vertices[numVerticesUsed++] = { box->position, box->color }; // Bottom Left
		vertices[numVerticesUsed++] = { box->position + Vector2f { 0, box->size.y }, box->color }; // Top Left
		vertices[numVerticesUsed++] = { box->position + box->size, box->color }; // Top Right
		vertices[numVerticesUsed++] = { box->position + box->size, box->color }; // Top Right
		vertices[numVerticesUsed++] = { box->position + Vector2f { box->size.x, 0 }, box->color }; // Bottom Right
		vertices[numVerticesUsed++] = { box->position, box->color }; // Bottom Left
	}

	void UIRenderer::flush() {
		numVerticesUsed = 0;
	}

	void UIRenderer::render() const {
		const Matrix4x4f projection = getOrthographicProjection(0, GlobalApp.floatWidth, 0, GlobalApp.floatHeight);
		useShader(ShaderUniformMapping::GlobalOrthographicShaderMapping.shader);
		setShaderMat4(ShaderUniformMapping::GlobalOrthographicShaderMapping.PROJECTION, projection);

	    glBindVertexArray(mVao);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numVerticesUsed * sizeof(UIBatchVertex), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, numVerticesUsed);
		glBindVertexArray(0);
		useShader(0);
	}

	void TextRenderer::initialize(GLint size, GLchar* path) {
		int libInitResult = FT_Init_FreeType(&mLib);
		if (libInitResult > 0) {
			logger_error("Failed to initialize FreeType Library. Error code: %d", libInitResult);
			return;
		}

		mSize = size;

		int faceInitResult = FT_New_Face(mLib, path, 0, &mFace);
		if (faceInitResult > 0) {
			logger_error("Failed to initialize Face for FreeType Library. Error code: %d", faceInitResult);
			return;
		}

		int setSizeResult = FT_Set_Pixel_Sizes(mFace, 0, mSize);
		if (setSizeResult > 0) {
			logger_error("Failed to set size for FreeType Library. Error code: %d", setSizeResult);
			return;
		}

		/* Calculate the dimensions we'll need for the bitmap texture */
		int w = 0, h = 0;
	    for (GLubyte c = 32; c < MAX_SUPPORTED_CHARS; c++) {
			if(FT_Load_Char(mFace, c, FT_LOAD_RENDER)) {
				logger_error( "Loading character %c failed", c);
				continue;
			}

			FT_GlyphSlot glyph = mFace->glyph;
			w += glyph->bitmap.width;
			h = MAX(h, glyph->bitmap.rows);
		}

		mAtlasWidth = static_cast<float>(w);
		mAtlasHeight = static_cast<float>(h);

		/* Get all of the render info */
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &mTextureId);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// @TODO: Load only the characters specified by the program
		float xOffset = 0;
		for (GLubyte c = 32; c < MAX_SUPPORTED_CHARS; c++) {
			int charLoadResult = FT_Load_Char(mFace, c, FT_LOAD_RENDER);
			if (charLoadResult) {
				logger_error("Failed to load character from FreeType library. Error code: %d", charLoadResult);
				continue;
			}

			FT_GlyphSlot glyph = mFace->glyph;
			FT_Bitmap bitmap = glyph->bitmap;

			glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, 0, bitmap.width, bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

			CharacterRenderInfo renderInfo = {
				Vector2f { static_cast<float>(glyph->advance.x >> 6), static_cast<float>(glyph->advance.y >> 6) },
				Vector2f { static_cast<float>(bitmap.width), static_cast<float>(bitmap.rows) }, 
				Vector2f { static_cast<float>(glyph->bitmap_left), static_cast<float>(glyph->bitmap_top) },
				xOffset / mAtlasWidth
			};

		    mCharRenderInfo[c] = renderInfo;
			xOffset += static_cast<float>(bitmap.width);
		}

		/* Buffer generation time */

		glGenVertexArrays(1, &mVao);
		glGenBuffers(1, &mVbo);
		glBindVertexArray(mVao);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TextBatchVertex) * MAX_BATCH_VERTICES, nullptr, GL_DYNAMIC_DRAW);

		// Texture Coordinate
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextBatchVertex), 0);

		// Position
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextBatchVertex), (GLvoid*)offsetof(TextBatchVertex, position));

		// Color
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(TextBatchVertex), (GLvoid*)offsetof(TextBatchVertex, color));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void TextRenderer::flush() {
		numVerticesUsed = 0;
	}

	const int ADVANCE_BITSHIFT_AMT = 6;

	void TextRenderer::add(BatchedText* text, float scale) {
		Vector2f position = text->position;
		for (int strIdx = 0; strIdx < text->content->length; strIdx++) {
			GLchar c = text->content->getValue()[strIdx];
			CharacterRenderInfo renderInfo = mCharRenderInfo[c];

			GLfloat xStart = position.x + renderInfo.mBearing.x * scale;
			GLfloat yStart = -position.y - renderInfo.mBearing.y * scale;
			GLfloat w = renderInfo.mSize.x * scale;
			GLfloat h = renderInfo.mSize.y * scale;

			position.x += renderInfo.mAdvance.x * scale;
			position.y += renderInfo.mAdvance.y * scale;

			vertices[numVerticesUsed++] = {
				Vector2f { renderInfo.mTextureOffset, 0 },
				Vector2f { xStart, -yStart },
				text->color
			};

			vertices[numVerticesUsed++] = {
				Vector2f { renderInfo.mTextureOffset + renderInfo.mSize.x / mAtlasWidth, 0 },
				Vector2f { xStart + w, -yStart },
				text->color
			};

			vertices[numVerticesUsed++] = {
				Vector2f { renderInfo.mTextureOffset, renderInfo.mSize.y / mAtlasHeight },
				Vector2f { xStart , -yStart - h },
				text->color
			};

			vertices[numVerticesUsed++] = {
				Vector2f { renderInfo.mTextureOffset + renderInfo.mSize.x / mAtlasWidth, 0 },
				Vector2f { xStart + w, -yStart },
				text->color
			};

			vertices[numVerticesUsed++] = {
				Vector2f { renderInfo.mTextureOffset, renderInfo.mSize.y / mAtlasHeight },
				Vector2f { xStart, -yStart - h },
				text->color
			};

			vertices[numVerticesUsed++] = {
				Vector2f { renderInfo.mTextureOffset + renderInfo.mSize.x / mAtlasWidth, renderInfo.mSize.y / mAtlasHeight },
				Vector2f { xStart + w, -yStart - h },
				text->color
			};
		}
	}

	void TextRenderer::render() const {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		const Matrix4x4f projection = getOrthographicProjection(0, GlobalApp.floatWidth, 0, GlobalApp.floatHeight);

		useShader(ShaderUniformMapping::GlobalTextShaderMapping.shader);
		setShaderMat4(ShaderUniformMapping::GlobalTextShaderMapping.PROJECTION, projection);

		glActiveTexture(GL_TEXTURE0);
		setShaderInt(ShaderUniformMapping::GlobalTextShaderMapping.GLYPH_TEXTURE, 0);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		 
		glBindVertexArray(mVao);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextBatchVertex) * numVerticesUsed, vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, numVerticesUsed);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);

		useShader(0);
	}

	void TextRenderer::free() {
		FT_Done_Face(mFace);
		FT_Done_FreeType(mLib);

		glDeleteVertexArrays(1, &mVao);
		glDeleteBuffers(1, &mVbo);
	}
}
