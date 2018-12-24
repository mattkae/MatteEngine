#include "ImageUtil.h"
#include "gl_includes.h"
#include <iostream>

const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

void writeTextureToFile(const char* filePath) {
	int width = 0;
	int height = 0;
	glfwGetWindowSize(glfwGetCurrentContext() , &width, &height);

	const int imageSize = width * height * 3;
	const int fileSize = fileHeaderSize + infoHeaderSize + imageSize;

	unsigned char* imageData = new unsigned char[imageSize];
	glReadBuffer(GL_BACK_LEFT);
	glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, imageData);

	FILE* file;

	file = fopen(filePath, "wb");

	unsigned char bmpfileheader[fileHeaderSize] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[infoHeaderSize] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[4] = { 0,0,0,0 };
	bmpfileheader[2] = (unsigned char)(fileSize);
	bmpfileheader[3] = (unsigned char)(fileSize >> 8);
	bmpfileheader[4] = (unsigned char)(fileSize >> 16);
	bmpfileheader[5] = (unsigned char)(fileSize >> 24);

	bmpinfoheader[4] = (unsigned char)(width);
	bmpinfoheader[5] = (unsigned char)(width >> 8);
	bmpinfoheader[6] = (unsigned char)(width >> 16);
	bmpinfoheader[7] = (unsigned char)(width >> 24);
	bmpinfoheader[8] = (unsigned char)(height);
	bmpinfoheader[9] = (unsigned char)(height >> 8);
	bmpinfoheader[10] = (unsigned char)(height >> 16);
	bmpinfoheader[11] = (unsigned char)(height >> 24);

	fwrite(bmpfileheader, 1, fileHeaderSize, file);
	fwrite(bmpinfoheader, 1, infoHeaderSize, file);
	fwrite(imageData, 1, imageSize, file);

	fclose(file);
	delete imageData;
}
