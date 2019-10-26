#include "Light.h"
#include "Camera.h"
#include "GlobalApplicationState.h"
#include "GlmUtility.h"
#include "ImageUtil.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

glm::mat4 get_light_projection(const Light &light) {
    switch (light.type) {
    case Directional:
        return glm::ortho<float>(-10, 10, -10.f, 10, GlobalAppState.near, GlobalAppState.far);
    case Spot:
        return glm::perspective<float>(glm::radians(45.f), GlobalAppState.aspectRatio, GlobalAppState.near, GlobalAppState.far);
    case PointLight:
    default:
        std::cerr << "Attempting to get a view for unknown light: " << light.type << std::endl;
        return glm::mat4(0.0);
    }
}

glm::mat4 get_light_view(const Light &light) {
    switch (light.type) {
    case Directional: {
        auto lightPosition = (-GlobalAppState.far / 2.0f) * light.direction;
        return glm::lookAt(lightPosition, lightPosition + light.direction, light.up);
    }
    case Spot:
        return glm::lookAt(light.position, light.position + light.direction, light.up);
    case PointLight:
    default:
        std::cerr << "Attempting to get a view for unknown light: " << light.type << std::endl;
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
		light.shadowWidth = GlobalAppState.width;
		light.shadowHeight = GlobalAppState.height;
		initLight(light);
    }
    j.at("isOn").get_to(light.isOn);
    light.color = glm::arrayToVec3(color);

    if (light.type != LightType::PointLight) {
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

void createShadowTextureForPointLight(Light& light) {
    glGenTextures(1, &light.shadowTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, light.shadowTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    for (int fidx = 0; fidx < 6; fidx++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, 0,
                     GL_DEPTH_COMPONENT24, light.shadowWidth, light.shadowHeight, 0, GL_DEPTH_COMPONENT,
                     GL_FLOAT, 0);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Generate the framebuffer for the shadow
    glGenFramebuffers(1, &light.depthFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void createShadowTextureForDirectionalLight(Light& light) {
	if (light.usesShadows == false) {
		return;
	}

    glGenTextures(1, &light.shadowTexture);
    glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, light.shadowWidth, light.shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &light.depthFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light.shadowTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Shadow framebuffer is not okay for light." << std::endl;
    }

    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool initLight(Light& light) {
    switch (light.type) {
    case Spot:
    case Directional:
        createShadowTextureForDirectionalLight(light);
        break;
    case PointLight:
        createShadowTextureForPointLight(light);
        break;
    default:
        break;
    }

    return true;
}

void renderPointShadows(const Light& light, const Shader &shader, const BetterScene &scene) {
    glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);
    glViewport(0, 0, light.shadowWidth, light.shadowHeight);
    glClearDepth(1.0);

    for (int fidx = 0; fidx < 6; fidx++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, light.shadowTexture, 0);
        glClear(GL_DEPTH_BUFFER_BIT);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Shadow FBO is broken with code " << status << std::endl;
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
            std::cerr << "Invalid direction for cube map." << std::endl;
            break;
        }

        glm::mat4 view = glm::lookAt(light.position, light.position + currentDirection, up);
        glm::mat4 proj = glm::perspective(glm::radians(45.f), GlobalAppState.aspectRatio, GlobalAppState.near, GlobalAppState.far);
		setShaderMat4(shader, "uViewProj", proj * view);
		renderModels(scene, shader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glViewport(0, 0, GlobalAppState.width, GlobalAppState.height);
}

void renderDirectionalShadows(const Light& light, const Shader shader, const BetterScene &scene) {
    glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);
    glViewport(0, 0, light.shadowWidth, light.shadowHeight);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

	setShaderMat4(shader, "uView", light.view);
	setShaderMat4(shader, "uProjection", light.projection);
    renderModels(scene, shader, false);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glViewport(0, 0, GlobalAppState.width, GlobalAppState.height);
}

void renderLightShadows(const Light& light, const Shader shader, const BetterScene& scene) {
    if (!light.isOn || !light.usesShadows) {
        return;
	}

    switch (light.type) {
    case PointLight:
        renderPointShadows(light, shader, scene);
        break;
    case Spot:
    case Directional:
        renderDirectionalShadows(light, shader, scene);
        break;
    default:
        std::cerr << "Rendering shadows for invalid light type " << light.type << "." << std::endl;
        break;
    }
}

static std::string getArrayUniform(const int lightIndex, const char *attribute, const char *property = nullptr) {
    std::ostringstream ss;
    ss << attribute << "[" << lightIndex << "]";

    if (property) {
        ss << "." << property;
    }

    return ss.str();
}

void renderLight(const Light& light, const Shader shader, const int index) {
    if (!light.isOn) {
        return;
    }

	setShaderVec3(shader, getArrayUniform(index, "uLights", "color").c_str(), light.color);

	// Render shadow map
	if (light.usesShadows) {
		switch (light.type) {
		case PointLight:
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
			break;
		case Directional:
		case Spot:
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
			setShaderInt(shader, "uDirShader", 0);
			setShaderMat4(shader, "uShadowMatrix", light.projection * light.view);
			break;
		default:
			std::cerr << "Unknown light type: " << light.type << std::endl;
			break;
		}
	}

	// Render the light
	bool isDirectional = light.type == LightType::Directional;
	bool isPoint = light.type == LightType::PointLight;

	glm::vec3 position = isDirectional ? glm::vec3(light.direction) * -GlobalAppState.far : light.position;
	glm::vec3 direction = isPoint ? glm::vec3(0) : light.direction;
	setShaderVec3(shader, getArrayUniform(index, "uLights", "direction").c_str(), direction);
	setShaderVec3(shader, getArrayUniform(index, "uLights", "position").c_str(), position);
	setShaderFloat(shader, getArrayUniform(index, "uLights", "constant").c_str(), isDirectional ? 1.f : light.constant);
	setShaderFloat(shader, getArrayUniform(index, "uLights", "linear").c_str(), isDirectional ? 0.f : light.linear);
	setShaderFloat(shader, getArrayUniform(index, "uLights", "quadratic").c_str(), isDirectional ? 0.f : light.quadratic);
	setShaderFloat(shader, getArrayUniform(index, "uLights", "cosineCutOff").c_str(), isPoint || isDirectional ? -1.f : light.cosineCutOff);
	setShaderFloat(shader, getArrayUniform(index, "uLights", "dropOff").c_str(), isPoint || isDirectional ? 1.f : light.dropOff);
	setShaderInt(shader, getArrayUniform(index, "uLights", "usesShadows").c_str(), light.usesShadows);

	if (isPoint) {
		float near = GlobalAppState.near;
		float far = GlobalAppState.far;
		float diff = far - near;
		glm::vec2 uFarNear;
		uFarNear.x = (far + near) / diff * 0.5f + 0.5f;
		uFarNear.y = -(far * near) / diff;
		setShaderVec2(shader, "uFarNear", uFarNear);
	}
}

void freeLight(Light& light) {
	if (light.shadowTexture != 0) {
        glDeleteTextures(1, &light.shadowTexture);
		light.shadowTexture = 0;
	}
	if (light.depthFbo != 0) {
        glDeleteFramebuffers(1, &light.depthFbo);
		light.depthFbo = 0;
	}
}
