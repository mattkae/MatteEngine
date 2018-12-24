cmake_minimum_required (VERSION 3.0)
project(Matte)

file(GLOB_RECURSE sources src/*.cpp src/*.h)
file(GLOB_RECURSE data assets/*)
add_executable(Matte ${sources})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")

find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL REQUIRED)
find_package(nlohmann_json QUIET)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(Matte PRIVATE glm assimp SOIL glfw3 GLEW::GLEW)
