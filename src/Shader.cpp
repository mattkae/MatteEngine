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
#include "StringUtil.h"
#include "Logger.h"

const char* INCLUDE_STRING = "#include ";

enum GlslPreprocessorToken {
	GlslPreprocessorToken_NONE,
	GlslPreprocessorToken_VERSION,
	GlslPreprocessorToken_DEFINE,
	GlslPreprocessorToken_INCLUDE,
	GlslPreprocessorToken_IFDEF,
	GlslPreprocessorToken_ENDIF
};

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

inline GlslPreprocessorToken getPreprocessorToken(char* ptr) {
	if (StringUtil::ifEqualWalkToValue(ptr, "#include")) {
		return GlslPreprocessorToken_INCLUDE;
	} else if (StringUtil::ifEqualWalkToValue(ptr, "#define")) {
		return GlslPreprocessorToken_DEFINE;
	} else if (StringUtil::ifEqualWalkToValue(ptr, "#ifdef")) {
		return GlslPreprocessorToken_IFDEF;
	} else if (StringUtil::ifEqualWalkToValue(ptr, "#endif")) {
		return GlslPreprocessorToken_ENDIF;
	} else if (StringUtil::ifEqualWalkToValue(ptr, "#version")) {
		return GlslPreprocessorToken_VERSION;
	} else {
		return GlslPreprocessorToken_NONE;
	}
}

bool tryReadGlslFile(const GLchar* path, String* result) {
	StringBuilder sb;
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		result = NULL;
		logger_error("Failed to open file: %s", path);
		return false;
	}

	logger_info("Compiling Glsl file at path: %s", path);

	int lineNumber = 0;
	const int bufferLength = 256;
	char buffer[bufferLength];
	while(fgets(buffer, bufferLength, file)) {
		lineNumber++;
		char* ptr = &buffer[0];
		int length = strlen(buffer);
		if (buffer[length - 1] == '\n') {
			buffer[length - 1] = '\0';
		}
		
		StringUtil::trimLeft(ptr);

		if (ptr[0] == '\n' || ptr[0] == '\0') {
			continue;
		}

		if (ptr[0] == '#') { // Came across a preprocessor statement.
			auto preprocessorToken = getPreprocessorToken(ptr);
			switch (preprocessorToken) {
			case GlslPreprocessorToken_INCLUDE: {
				char includeFilePath[256] = "src/shaders/";
				char includeFileName[128];
				int tokenSize = strlen("#include ");
				StringUtil::substring(includeFileName, buffer, length - tokenSize, tokenSize);
				strcat(includeFilePath, includeFileName);
				
				logger_info("Including file: %s", includeFilePath);
				String includeStr;
				tryReadGlslFile(includeFilePath, &includeStr);
				sb.addStr(&includeStr);
				includeStr.free();
				break;
			}
			case GlslPreprocessorToken_DEFINE: {
				sb.addStr(buffer);
				break;
			}
			case GlslPreprocessorToken_IFDEF: {
				break;
			}
			case GlslPreprocessorToken_ENDIF: {
				break;
			}
			case GlslPreprocessorToken_VERSION: {
				sb.addStr(buffer);
				break;
			}
			case GlslPreprocessorToken_NONE: {
				logger_error("[%s: %d]: Unknown preprocessor statement", path, lineNumber);
				break;
			}
			}
		} else {
			sb.addStr(buffer);
		}
		sb.addChar('\n');
	}

	*result = sb.toString();
	sb.free();
    return true;
}

GLuint loadIndividualShader(GLenum shaderType, const GLchar* path) {
    String glShaderCode;
	if (!tryReadGlslFile(path, &glShaderCode)) {
		return 0;
	}
    GLuint shader;
	const char* cCode = glShaderCode.getValue();

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &cCode, 0);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, 0, infoLog);
        logger_error("Vertex shader failed to compile, path: %s, reason: %s", path, infoLog);
        return 0;
    }

	glShaderCode.free();
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
	logger_info("Loading shader: VERT=%s, FRAG=%s", vertexPath, fragmentPath);
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
