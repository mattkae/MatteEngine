#include "Shader.h"
#include "DirectoryUtil.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string INCLUDE_STRING = "#include ";

std::string readGlslFile(const GLchar* path) {
    std::string shaderCode = "";
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::badbit);
    try {
        shaderFile.open(path);

		std::string line;
		while (std::getline(shaderFile, line)) {
			std::stringstream shaderStream(line);

			if (line.length() > INCLUDE_STRING.length() && line.substr(0, INCLUDE_STRING.length()) == INCLUDE_STRING) {
				line = readGlslFile((getEnclosingFolder(path) + "/" + line.substr(INCLUDE_STRING.length())).c_str());
			}

			shaderCode += line + "\n";
		}

    } catch (std::exception e) {
        std::cerr << "Error: Shader not successfuly read from file: " << path << std::endl;
        return "";
    }

	return shaderCode;
}

GLuint loadIndividualShader(GLenum shaderType, const GLchar *path) {
    const std::string glShaderCode = readGlslFile(path);
	const char* glShaderCodeCStr = glShaderCode.c_str();
    GLuint shader;

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &glShaderCodeCStr, 0);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, 0, infoLog);
        std::cerr << "Error: Vertex shader failed to compile - " << infoLog
                  << std::endl;
        return 0;
    }

    return shader;
}

Shader loadShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath) {
	Shader retVal = glCreateProgram();
	GLuint vertex = 0, fragment = 0, geometry = 0;
	if (vertexPath) {
        vertex = loadIndividualShader(GL_VERTEX_SHADER, vertexPath);
        glAttachShader(retVal, vertex);
    }

    if (fragmentPath) {
        fragment = loadIndividualShader(GL_FRAGMENT_SHADER, fragmentPath);
        glAttachShader(retVal, fragment);
    }

    if (geomPath) {
        geometry = loadIndividualShader(GL_GEOMETRY_SHADER, geomPath);
        glAttachShader(retVal, geometry);
    }

    glLinkProgram(retVal);
    GLint isLinked = 0;
    glGetProgramiv(retVal, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(retVal, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(retVal, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(retVal);
        printf("Error. Could not initialize shader with vertex=%s\n", vertexPath);
    }

    if (vertexPath) glDeleteShader(vertex);
    if (fragmentPath) glDeleteShader(fragment);
    if (geomPath) glDeleteShader(geometry);

	return retVal;
}