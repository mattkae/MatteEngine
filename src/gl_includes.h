#ifndef GL_INCLUDES_H
#define GL_INCLUDES_H

#ifdef _WIN32
#ifdef _WIN64

#else

#endif
#elif __APPLE__
#include <GL/glew.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>
#elif __linux__

#else

#endif

#endif
