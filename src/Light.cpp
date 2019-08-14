#include "Light.h"
#include "Camera.h"
#include "Constants.h"
#include "GlmUtility.h"
#include "ImageUtil.h"
#include "Scene.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

glm::mat4 get_light_projection(const Light &light) {
    switch (light.type) {
    case Directional:
        return glm::ortho<float>(-10, 10, -10.f, 10, Constants.near,
                                 Constants.far);
    case Spot:
        return glm::perspective<float>(glm::radians(45.f),
                                       Constants.aspectRatio, Constants.near,
                                       Constants.far);
    case Point:
    default:
        cerr << "Attempting to get a view for unknown light: " << light.type
             << endl;
        return glm::mat4(0.0);
    }
}

glm::mat4 get_light_view(const Light &light) {
    switch (light.type) {
    case Directional: {
        auto lightPosition = (-Constants.far / 2.0f) * light.direction;
        return glm::lookAt(lightPosition, lightPosition + light.direction,
                           light.up);
    }
    case Spot:
        return glm::lookAt(light.position, light.position + light.direction,
                           light.up);
    case Point:
    default:
        cerr << "Attempting to get a view for unknown light: " << light.type
             << endl;
        return glm::mat4(0.0);
    }
}

void to_json(json &j, const Light &light) {
    j = json{{"type", light.type},
             {"color", glm::vec3ToArray(light.color)},
             {"direction", glm::vec3ToArray(light.direction)},
             {"up", glm::vec3ToArray(light.up)},
             {"position", glm::vec3ToArray(light.position)},
             {"constant", light.constant},
             {"linear", light.linear},
             {"quadratic", light.quadratic},
             {"cosineCutOff", light.cosineCutOff},
             {"dropOff", light.dropOff},
             {"usesShadows", light.usesShadows},
             {"isOn", light.isOn}};
}

void from_json(const json &j, Light &light) {
    std::vector<float> color;
    std::vector<float> direction;
    std::vector<float> up;
    std::vector<float> position;

    j.at("type").get_to(light.type);
    j.at("color").get_to<std::vector<float>>(color);
    j.at("usesShadows").get_to(light.usesShadows);
    if (light.usesShadows) {
        light.generate(Constants.width, Constants.height);
    }
    j.at("isOn").get_to(light.isOn);
    light.color = glm::arrayToVec3(color);

    if (light.type != LightType::Point) {
        j.at("direction").get_to<std::vector<float>>(direction);
        light.direction = glm::arrayToVec3(direction);
        j.at("up").get_to<std::vector<float>>(up);
        light.up = glm::arrayToVec3(up);
    }

    if (light.type != LightType::Directional) {
        j.at("constant").get_to(light.constant);
        j.at("linear").get_to(light.linear);
        j.at("quadratic").get_to(light.quadratic);
        j.at("cosineCutOff").get_to(light.cosineCutOff);
        j.at("dropOff").get_to(light.dropOff);
        j.at("position").get_to<std::vector<float>>(position);
        light.position = glm::arrayToVec3(position);
    }

    light.projection = get_light_projection(light);
    light.view = get_light_view(light);
}

void Light::create_shadow_texture_for_omnidirectional_light(int width,
                                                            int height) {
    glGenTextures(1, &shadowTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE,
                    GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    for (int fidx = 0; fidx < 6; fidx++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, 0,
                     GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT,
                     GL_FLOAT, 0);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Generate the framebuffer for the shadow
    glGenFramebuffers(1, &depthFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shadowWidth = width;
    shadowHeight = height;
}

void Light::create_shadow_texture_for_directional_light(int width, int height) {
    glGenTextures(1, &shadowTexture);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                    GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &depthFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           shadowTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << "Shadow framebuffer is not okay for light." << endl;
    } else {
        shadowWidth = width;
        shadowHeight = height;
    }

    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Light::generate(int width, int height) {
    switch (type) {
    case Spot:
    case Directional:
        create_shadow_texture_for_directional_light(width, height);
        break;
    case Point:
        create_shadow_texture_for_omnidirectional_light(width, height);
        break;
    default:
        break;
    }

    hasGenerated = true;
    return true;
}

void Light::update(double dt) {
    
}

void Light::render_omindirectional_shadows(const Shader &shader,
                                           const Scene &scene) const {
    glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);
    glViewport(0, 0, shadowWidth, shadowHeight);
    glClearDepth(1.0);

    for (int fidx = 0; fidx < 6; fidx++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx,
                               shadowTexture, 0);
        glClear(GL_DEPTH_BUFFER_BIT);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            cerr << "Shadow FBO is broken with code " << status << endl;
        }

        glm::vec3 currentDirection;
        glm::vec3 up;
        switch (fidx) {
        case 0:
            currentDirection = glm::vec3(1, 0, 0);
            up = glm::vec3(0, 1, 0);
            break;
        case 1:
            currentDirection = glm::vec3(-1, 0, 0);
            up = glm::vec3(0, -1, 0);
            break;
        case 2:
            currentDirection = glm::vec3(0, 1, 0);
            up = glm::vec3(0.0, 0.0, 1.0);
            break;
        case 3:
            currentDirection = glm::vec3(0, -1, 0);
            up = glm::vec3(0.0, 0.0, -1.0);
            break;
        case 4:
            currentDirection = glm::vec3(0, 0, 1);
            up = glm::vec3(0, 1, 0);
            break;
        case 5:
            currentDirection = glm::vec3(0, 0, -1);
            up = glm::vec3(0, -1, 0);
            break;
        default:
            cerr << "Invalid direction for cube map." << endl;
            break;
        }

        glm::mat4 view = glm::lookAt(position, position + currentDirection, up);
        glm::mat4 proj =
            glm::perspective(glm::radians(45.f), Constants.aspectRatio,
                             Constants.near, Constants.far);
        shader.set_uniform_matrix_4fv("uViewProj", 1, GL_FALSE,
                                      glm::value_ptr(proj * view));
        scene.renderModels(shader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glViewport(0, 0, Constants.width, Constants.height);
}

void Light::render_directional_shadows(const Shader &shader,
                                       const Scene &scene) const {
    glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);
    glViewport(0, 0, shadowWidth, shadowHeight);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    shader.set_uniform_matrix_4fv("uView", 1, GL_FALSE, glm::value_ptr(view));
    shader.set_uniform_matrix_4fv("uProjection", 1, GL_FALSE,
                                  glm::value_ptr(projection));
    scene.renderModels(shader, false);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glViewport(0, 0, Constants.width, Constants.height);
}

void Light::render_shadows(const Shader &shader, const Scene &scene) const {
    if (!isOn || !usesShadows)
        return;

    switch (this->type) {
    case Point:
        // render_omindirectional_shadows(light, shader, scene);
        break;
    case Spot:
    case Directional:
        render_directional_shadows(shader, scene);
        break;
    default:
        cerr << "Rendering shadows for invalid light type " << type << "."
             << endl;
        break;
    }
}

static string get_array_uniform(const int lightIndex, const char *attribute,
                                const char *property = nullptr) {
    ostringstream ss;
    ss << attribute << "[" << lightIndex << "]";

    if (property) {
        ss << "." << property;
    }

    return ss.str();
}

void Light::render_point_light(const Shader &shader, const int index) const {
    if (usesShadows) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowTexture);
    }

    shader.set_uniform_3f(
        get_array_uniform(index, "uLights", "direction").c_str(), 0, 0, 0);
    shader.set_uniform_3f(
        get_array_uniform(index, "uLights", "position").c_str(), position.x,
        position.y, position.z);
    shader.set_uniform_1f(
        get_array_uniform(index, "uLights", "constant").c_str(), constant);
    shader.set_uniform_1f(get_array_uniform(index, "uLights", "linear").c_str(),
                          linear);
    shader.set_uniform_1f(
        get_array_uniform(index, "uLights", "quadratic").c_str(), quadratic);
    shader.set_uniform_1f(
        get_array_uniform(index, "uLights", "cosineCutOff").c_str(), -1);
    shader.set_uniform_1f(
        get_array_uniform(index, "uLights", "dropOff").c_str(), 1);

    float near = Constants.near;
    float far = Constants.far;
    float diff = far - near;
    glm::vec2 uFarNear;
    uFarNear.x = (far + near) / diff * 0.5f + 0.5f;
    uFarNear.y = -(far * near) / diff;
    shader.set_uniform_2f("uFarNear", uFarNear.x, uFarNear.y);
    shader.set_uniform_1i(get_array_uniform(index, "uLights", "usesShadows").c_str(), usesShadows);
}

void Light::render_directional_light(const Shader &shader,
                                     const int index) const {
    if (usesShadows) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowTexture);
        shader.set_uniform_1i("uDirShadow", 0);
        shader.set_uniform_matrix_4fv("uShadowMatrix", 1, GL_FALSE,
                                      glm::value_ptr(projection * view));
    }

    glm::vec3 position = glm::vec3(direction);
    position *= -Constants.far;
    shader.set_uniform_3f( get_array_uniform(index, "uLights", "direction").c_str(), direction.x, direction.y, direction.z);
    shader.set_uniform_3f(get_array_uniform(index, "uLights", "position").c_str(), position.x,position.y, position.z);
    shader.set_uniform_1f(get_array_uniform(index, "uLights", "constant").c_str(), 1);
    shader.set_uniform_1f(get_array_uniform(index, "uLights", "linear").c_str(), 0);
    shader.set_uniform_1f(get_array_uniform(index, "uLights", "quadratic").c_str(), 0);
    shader.set_uniform_1f(get_array_uniform(index, "uLights", "cosineCutOff").c_str(), -1);
    shader.set_uniform_1f(get_array_uniform(index, "uLights", "dropOff").c_str(), 1);
    shader.set_uniform_1i(get_array_uniform(index, "uLights", "usesShadows").c_str(), usesShadows);
}

void Light::render_spot_light(const Shader &shader, const int index) const {
    if (usesShadows) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowTexture);
        shader.set_uniform_1i("uDirShadow", 0);
        shader.set_uniform_matrix_4fv("uShadowMatrix", 1, GL_FALSE,
                                      glm::value_ptr(projection * view));
    }

    shader.set_uniform_3f(
        get_array_uniform(index, "uLights", "direction").c_str(), direction.x,
        direction.y, direction.z);
    shader.set_uniform_3f(
        get_array_uniform(index, "uLights", "position").c_str(), position.x,
        position.y, position.z);
    shader.set_uniform_1f(
        get_array_uniform(index, "uLights", "constant").c_str(), constant);
    shader.set_uniform_1f(get_array_uniform(index, "uLights", "linear").c_str(),
                          linear);
    shader.set_uniform_1f(
        get_array_uniform(index, "uLights", "quadratic").c_str(), quadratic);
    shader.set_uniform_1f(
        get_array_uniform(index, "uLights", "cosineCutOff").c_str(),
        cosineCutOff);
    shader.set_uniform_1f(
        get_array_uniform(index, "uLights", "dropOff").c_str(), dropOff);
    shader.set_uniform_1i(get_array_uniform(index, "uLights", "usesShadows").c_str(), usesShadows);
}

void Light::render(const Shader &shader, const int index) const {
    if (!isOn) {
        return;
    }

    shader.set_uniform_3f(get_array_uniform(index, "uLights", "color").c_str(),
                          color.x, color.y, color.z);

    switch (type) {
    case Directional:
        render_directional_light(shader, index);
        break;
    case Point:
        render_point_light(shader, index);
        break;
    case Spot:
        render_spot_light(shader, index);
        break;
    default:
        cerr << "Unknown light type: " << type << endl;
        break;
    }
}

void Light::free() {
    if (hasGenerated) {
        glDeleteTextures(1, &shadowTexture);
        glDeleteFramebuffers(1, &depthFbo);
    }
}
