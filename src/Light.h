#ifndef LIGHT_H
#define LIGHT_H

#include "Model.h"
#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

class Scene;

enum LightType { Directional = 0, Point, Spot, Inactive };

struct Light {
  public:
    LightType type;
    glm::vec3 color = glm::vec3(1.0);
    glm::vec3 direction = glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 position = glm::vec3(0.0);

    float constant = 1.0f;
    float linear = 0.0f;
    float quadratic = 0.f;

    float cosineCutOff = 1.0f;
    float dropOff = 1.0f;

    bool usesShadows = false;
    int shadowWidth = 800;
    int shadowHeight = 600;

    bool isOn = true;
    glm::mat4 projection;
    glm::mat4 view;

    bool generate(int width, int height);
    void update(double dt);
    void render_shadows(const Shader &shader, const Scene &scene) const;
    void render(const Shader &shader, const int index) const;
    void free();

  private:
    void create_shadow_texture_for_directional_light(int width, int height);
    void create_shadow_texture_for_omnidirectional_light(int width, int height);
    void render_omindirectional_shadows(const Shader &shader, const Scene &scene) const;
    void render_directional_shadows(const Shader &shader, const Scene &scene) const;
    void render_point_light(const Shader &shader, const int index) const;
    void render_directional_light(const Shader &shader, const int index) const;
    void render_spot_light(const Shader &shader, const int index) const;

    GLuint shadowTexture = 0;
    GLuint depthFbo = 0;
    bool hasGenerated = false;
};

void to_json(json &j, const Light &light);
void from_json(const json &j, Light &light);
#endif
