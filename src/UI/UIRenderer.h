#pragma once

#include "../List.h"
#include <GL/glew.h>
#include "../Vector2f.h"
#include "../Vector4f.h"

// .ttf files. In the future, maybe we can investigate loading
// them ourselves, for practice's sake. For now, though, this
// seems like the best option to get a functional UI up and
// running quickly.
#include <ft2build.h>
#include <freetype/freetype.h>

struct String;

namespace UI {
	struct BatchedBox {
		Vector2f position;
		Vector2f size;
		Vector4f color;
	};

	struct BatchedText {
		Vector2f position;
		Vector4f clip;
		Vector4f color;
		String* content;
	};
	
	struct RenderCommand {
	    BatchedBox box;
		BatchedText text;
	};

	const int MAX_BATCH_VERTICES = 6 * 128; // 128 Triangles

	struct UIBatchVertex {
		// @TODO: Maybe add a layer to this position as well
		Vector2f position;
		Vector4f color;
	};

	struct UIRenderer {
		GLuint mVao;
		GLuint mVbo;

		int numVerticesUsed = 0;
		UIBatchVertex vertices[MAX_BATCH_VERTICES];

		void initialize();
	    void add(BatchedBox* box);
		void flush();
		void render() const;
		void free();
	};

	struct TextBatchVertex {
		Vector2f texCoord;
		Vector2f position;
		Vector4f color;
		Vector4f clip;
	};

	struct CharacterRenderInfo {
		Vector2f mAdvance;
		Vector2f mSize;
		Vector2f mBearing;
		float mTextureOffset;
	};

	const int MAX_SUPPORTED_CHARS = 128;
	const int MAX_TEXT_RENDERER_VERTICES = 6 * 256; // Able to render 256 characters at a time

	struct TextRenderer {
		void initialize(GLint size, GLchar* path);
		void add(BatchedText* text, float scale);
		void flush();
		void render() const;
		void free();
		float getStringWidth(String* str, GLfloat scale, int length = -1) const;
		float getCharWidth(char c, GLfloat scale) const;
		float getStringHeight(String* str, GLfloat scale) const;
		float getCharHeight(char c, GLfloat scale) const;

		// @TODO: Might want to consider freeing these immediately after load, seems unnessary
		FT_Library mLib;
		FT_Face mFace;
		GLint mSize;
		GLuint mTextureId;
		GLuint mVao;
		GLuint mVbo;
		float mAtlasWidth = 0;
		float mAtlasHeight = 0;
		CharacterRenderInfo mCharRenderInfo[MAX_SUPPORTED_CHARS];

		int numVerticesUsed = 0;
		TextBatchVertex vertices[MAX_TEXT_RENDERER_VERTICES];
	};

}
