#include "Shader.h"
#include "DirectoryUtil.h"
#include "Logger.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#ifdef __WIN32
#include <tchar.h>
#include <windows.h>
#endif
#include <chrono>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "List.h"
#include "MyString.h"
#include "Logger.h"

const char* INCLUDE_STRING = "#include ";

enum ShaderDependencyType {
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    DEPENDENCY
};

struct ShaderDependency {
    Shader shader;

    String vertexPath;
    String fragPath;
    String geomPath;
};

List<ShaderDependency> GlobalShaderRegistry;

void freeGlobalShaderRegistry() {
    GlobalShaderRegistry.deallocate();
}

std::string readGlslFile(const GLchar* path) {
    std::string shaderCode = "";
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::badbit);
    try {
        shaderFile.open(path);

        std::string line;
        while (std::getline(shaderFile, line)) {
            std::stringstream shaderStream(line);

            if (line.length() > strlen(INCLUDE_STRING) && line.substr(0, strlen(INCLUDE_STRING)) == INCLUDE_STRING) {
                line = readGlslFile((getEnclosingFolder(path) + "/" + line.substr(strlen(INCLUDE_STRING))).c_str());
            }

            shaderCode += line + "\n";
        }
        shaderFile.close();
    } catch (std::exception e) {
        logger_error("Error: Shader not successfuly read from file: %s", path);
        return "";
    }

    return shaderCode;
}

GLuint loadIndividualShader(GLenum shaderType, const GLchar* path) {
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
        logger_error("Vertex shader failed to comipile, path: %s, reason: %s", path, infoLog);
        return 0;
    }

    return shader;
}

void attachShaders(Shader& retVal, const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath) {
    ShaderDependency dependency;

    GLuint vertex = 0, fragment = 0, geometry = 0;
    if (vertexPath) {
        dependency.vertexPath = vertexPath;
        vertex = loadIndividualShader(GL_VERTEX_SHADER, vertexPath);
        glAttachShader(retVal, vertex);
    }

    if (fragmentPath) {
        dependency.fragPath = fragmentPath;
        fragment = loadIndividualShader(GL_FRAGMENT_SHADER, fragmentPath);
        glAttachShader(retVal, fragment);
    }

    if (geomPath) {
        dependency.geomPath = geomPath;
        geometry = loadIndividualShader(GL_GEOMETRY_SHADER, geomPath);
        glAttachShader(retVal, geometry);
    }

    glLinkProgram(retVal);
    GLint isLinked = 0;
    glGetProgramiv(retVal, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(retVal, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLchar* infoLog = new GLchar[maxLength];
        glGetProgramInfoLog(retVal, maxLength, &maxLength, infoLog);
        glDeleteProgram(retVal);
        printf("Error. Could not initialize shader with vertex=%s, error=%s\n", vertexPath, infoLog);
        delete []infoLog;
    }

    if (vertexPath)
        glDeleteShader(vertex);
    if (fragmentPath)
        glDeleteShader(fragment);
    if (geomPath)
        glDeleteShader(geometry);

    dependency.shader = retVal;
    GlobalShaderRegistry.add(dependency);
}

Shader loadShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath) {
    Shader retVal;
    retVal = glCreateProgram();

    attachShaders(retVal, vertexPath, fragmentPath, geomPath);
    useShader(retVal);

    return retVal;
}

Shader reloadShader(Shader shader) {
    GLsizei count;
    GLuint shaders[3];
    glGetAttachedShaders(shader, 3, &count, shaders);
    GLenum error = glGetError();
    switch (error) {
    case GL_INVALID_VALUE:
        printf("not a valid program!");
        return shader;
    case GL_INVALID_OPERATION:
        printf("not a program object");
        return shader;
    }

    for (int shaderIdx = 0; shaderIdx < count; shaderIdx++) {
        glDetachShader(shader, shaders[shaderIdx]);
    }

    ShaderDependency* dependency = nullptr;
    FOREACH(GlobalShaderRegistry) {
        if (value->shader == shader) {
            dependency = value;
            break;
        }
    }

    if (dependency) {
        attachShaders(shader, dependency->vertexPath.getValue(), dependency->fragPath.getValue(), dependency->geomPath.getValue());
    } else {
        logger_warning("Unable to reload shader, because it does not exist in the registry");
    }

    return shader;
}
