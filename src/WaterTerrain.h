#ifndef WATER_TERRAIN_H
#define WATER_TERRAIN_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;

struct WaterTerrain {
public:
    bool generate(const std::vector<glm::vec2> positions);
    void render(const Shader& shader, const glm::vec3 yOffsets) const;
    void free();    
private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    bool hasGenerated = false;
};

#endif
