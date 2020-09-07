#pragma once
#include <GL/glew.h>

struct GridParameters {
	GLint width;
	GLint height;
	GLfloat verticesPerUnit;
};

struct GridResult {
	GLint verticesWidth = 0;
	GLint verticesHeight = 0;
	GLint numVertices = 0;
	GLint numIndices = 0;
	GLuint* indices = nullptr;
};

namespace GridHelper {
	/**
	 Allocates the data require for a Grid's vertices, as well 
	 as the number of indices that you'll need to render. It is
	 up to the end user to set up the OpenGL end of this.
	*/
	GridResult allocateGridIndices(GridParameters* params);
};