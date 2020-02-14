#include "ColladaFileLoader.h"
#include "Model.h"
#include "FileHelper.h"

void loadFromCollada(const char* path, Model& model) {
	FILE* file = FileHelper::openFile(path);

	fclose(file);
}