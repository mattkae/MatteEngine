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

const std::string INCLUDE_STRING = "#include ";

enum ShaderDependencyType {
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    DEPENDENCY
};

struct ShaderDependency {
    ShaderDependencyType dependencyType;
    std::string path;
};

std::map<Shader, std::vector<ShaderDependency>> GlobalShaderRegistry;

void watchForDirectorychanges(std::vector<Shader>& shadersToReload, const bool& isDying)
{
#ifdef __WIN32
    BYTE info[1024 * 8];
    OVERLAPPED overlapped = { 0 };

    HANDLE handle = CreateFileW(L"src/shaders",
        FILE_LIST_DIRECTORY | GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL);
    overlapped.hEvent = CreateEvent(nullptr, true, false, nullptr);
    while (!isDying) {
        BOOL rdc = ReadDirectoryChangesW(handle, info, std::size(info), TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY,
            NULL, &overlapped, NULL);

        DWORD dwBytesReturned = 0;
        if (GetOverlappedResult(handle, &overlapped, &dwBytesReturned, FALSE)) {
            if (dwBytesReturned == 0) {
                continue;
            }

            FILE_NOTIFY_INFORMATION* fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&info[0]);

            if (fni->Action == 0) {
                continue;
            }

            if (fni->Action != FILE_ACTION_MODIFIED) {
                continue;
            }

            std::wstring wideFileName = std::wstring { fni->FileName, fni->FileName + fni->FileNameLength / sizeof(wchar_t) };
            std::string filename = "src/shaders/" + std::string(wideFileName.begin(), wideFileName.end());

            for (std::pair<Shader, std::vector<ShaderDependency>> pair : GlobalShaderRegistry) {
                Shader shader = pair.first;
                for (const ShaderDependency& dependency : pair.second) {
                    if (dependency.path.compare(filename) == 0) {
                        shadersToReload.push_back(shader);
                        dwBytesReturned = 0;
                        break;
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
#endif
}

std::string readGlslFile(const GLchar* path)
{
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

GLuint loadIndividualShader(GLenum shaderType, const GLchar* path)
{
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

void attachShaders(Shader& retVal, const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath)
{
    std::vector<ShaderDependency> dependencies;

    GLuint vertex = 0, fragment = 0, geometry = 0;
    if (vertexPath) {
        dependencies.push_back({ ShaderDependencyType::VERTEX, std::string(vertexPath) });
        vertex = loadIndividualShader(GL_VERTEX_SHADER, vertexPath);
        glAttachShader(retVal, vertex);
    }

    if (fragmentPath) {
        dependencies.push_back({ ShaderDependencyType::FRAGMENT, std::string(fragmentPath) });
        fragment = loadIndividualShader(GL_FRAGMENT_SHADER, fragmentPath);
        glAttachShader(retVal, fragment);
    }

    if (geomPath) {
        dependencies.push_back({ ShaderDependencyType::GEOMETRY, std::string(geomPath) });
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

    if (GlobalShaderRegistry.find(retVal) != GlobalShaderRegistry.end()) {
        GlobalShaderRegistry[retVal] = dependencies;
    } else {
        GlobalShaderRegistry.insert(std::make_pair(retVal, dependencies));
    }
}

Shader loadShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath)
{
    Shader retVal;
    retVal = glCreateProgram();

    attachShaders(retVal, vertexPath, fragmentPath, geomPath);
    useShader(retVal);

    return retVal;
}

Shader reloadShader(Shader shader)
{
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

    if (GlobalShaderRegistry.find(shader) != GlobalShaderRegistry.end()) {
        const GLchar* vertexPath = nullptr;
        const GLchar* fragmentPath = nullptr;
        const GLchar* geometryPath = nullptr;

        for (const auto& dependency : GlobalShaderRegistry[shader]) {
            switch (dependency.dependencyType) {
            case ShaderDependencyType::DEPENDENCY:
            case ShaderDependencyType::GEOMETRY:
                geometryPath = dependency.path.c_str();
                break;
            case ShaderDependencyType::VERTEX:
                vertexPath = dependency.path.c_str();
                break;
            case ShaderDependencyType::FRAGMENT:
                fragmentPath = dependency.path.c_str();
                break;
            }
        }

        attachShaders(shader, vertexPath, fragmentPath, geometryPath);
    } else {
        Logger::logWarning("Unable to reload shader, because it does not exist in the registry");
    }

    return shader;
}
