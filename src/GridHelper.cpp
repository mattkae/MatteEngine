#include "GridHelper.h"
#include <cmath>
#include <iostream>

GridResult GridHelper::allocateGridIndices(GridParameters* params) {
	GridResult retval;

	retval.verticesWidth = static_cast<int>(ceil(params->width * params->verticesPerUnit));
	retval.verticesHeight = static_cast<int>(ceil(params->height * params->verticesPerUnit));
	retval.numVertices = (retval.verticesWidth + 1) * (retval.verticesHeight + 1);
	retval.numIndices = 6 * retval.numVertices;
	retval.indices = new GLuint[retval.numIndices];
	int indicesIdx = 0;

	for (int row = 0; row < retval.verticesHeight; row++) {
		int row1 = row * (retval.verticesHeight + 1);
		int row2 = (row + 1) * (retval.verticesHeight + 1);

		for (int col = 0; col < retval.verticesWidth; col++) {
            retval.indices[indicesIdx++] = row1 + col;
            retval.indices[indicesIdx++] = row1 + col + 1;
            retval.indices[indicesIdx++] = row2 + col + 1;
            retval.indices[indicesIdx++] = row1 + col;
            retval.indices[indicesIdx++] = row2 + col + 1;
            retval.indices[indicesIdx++] = row2 + col;
		}
	}

	return retval;
}