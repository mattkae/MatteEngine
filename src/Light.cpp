#include "Light.h"
#include "Camera.h"
#include "GlobalApplicationState.h"
#include "ImageUtil.h"
#include "Scene.h"
#include "TextureUniformConstants.h"
#include <iostream>
#include <sstream>
#include <string>

inline Matrix4x4f getLightProjection(const Light &light) {
    switch (light.type) {
    case Directional:
		// @TODO: Almost certian these values shouldn't be this random
        return getOrthographicProjection(GlobalAppState.near, GlobalAppState.far, -10, 10, -10.f, 10);
    case Spot:
		// @TODO: Don't default the FOV to 45 degrees
		return getPerspectiveProjection(GlobalAppState.near, GlobalAppState.far, 0.7853982f, GlobalAppState.aspectRatio);
    case PointLight:
    default:
        Logger::logError("Attempting to get a view for unknown light: " + light.type);
        return Matrix4x4f();
    }
}

inline Matrix4x4f getLightView(const Light &light) {
    switch (light.type) {
    case Directional: {
        auto lightPosition = light.direction * (-GlobalAppState.far / 2.0f);
		return getLookAt(lightPosition, lightPosition + light.direction, light.up);
    }
    case Spot:
		return getLookAt(light.position, light.position + light.direction, light.up);
    case PointLight:
    default:
        Logger::logError("Attempting to get a view for unknown light: " + light.type);
        return Matrix4x4f();
    }
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

static GLint getArrayUniform(const Shader& shader, const int lightIndex, const char *attribute, const char *property = nullptr) {
    std::ostringstream ss;
    ss << attribute << "[" << lightIndex << "]";

    if (property) {
        ss << "." << property;
    }

    return getShaderUniform(shader, ss.str().c_str());
}

bool initLight(Light& light, const Shader& shader, int lightIndex) {
	light.index = lightIndex;
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

	useShader(shader);
	light.colorUniform = getArrayUniform(shader, light.index, "uLights", "color");
	light.directionUniform = getArrayUniform(shader, light.index, "uLights", "direction");
	light.positionUniform = getArrayUniform(shader, light.index, "uLights", "position");
	light.constantUniform = getArrayUniform(shader, light.index, "uLights", "constant");
	light.linearUniform = getArrayUniform(shader, light.index, "uLights", "linear");
	light.quadraticUniform = getArrayUniform(shader, light.index, "uLights", "quadratic");
	light.cosineCutoffUniform = getArrayUniform(shader, light.index, "uLights", "cosineCutOff");
	light.dropOffUniform = getArrayUniform(shader, light.index, "uLights", "dropOff");
	light.usesShadowsUniform = getArrayUniform(shader, light.index, "uLights", "usesShadows");
	if (light.usesShadows) {
		light.dirShadowUniform = getArrayUniform(shader, light.index, "uDirShadow");
		light.shadowMatrixUniform = getArrayUniform(shader, light.index, "uLights", "shadowMatrix");
		light.view = getLightView(light);
		light.projection = getLightProjection(light);
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

        Vector3f currentDirection;
        Vector3f up;
        switch (fidx) {
        case 0:
            currentDirection = getVec3(1, 0, 0);
            up = getVec3(0, 1, 0);
            break;
        case 1:
            currentDirection = getVec3(-1, 0, 0);
            up = getVec3(0, -1, 0);
            break;
        case 2:
            currentDirection = getVec3(0, 1, 0);
            up = getVec3(0.0, 0.0, 1.0);
            break;
        case 3:
            currentDirection = getVec3(0, -1, 0);
            up = getVec3(0.0, 0.0, -1.0);
            break;
        case 4:
            currentDirection = getVec3(0, 0, 1);
            up = getVec3(0, 1, 0);
            break;
        case 5:
            currentDirection = getVec3(0, 0, -1);
            up = getVec3(0, -1, 0);
            break;
        default:
            std::cerr << "Invalid direction for cube map." << std::endl;
            break;
        }

        Matrix4x4f view = getLookAt(light.position, light.position + currentDirection, up);
        Matrix4x4f proj = getPerspectiveProjection(GlobalAppState.near, GlobalAppState.far, 0.7853982f, GlobalAppState.aspectRatio);
		setShaderMat4(shader, "uViewProj", proj * view);
		renderModels(scene, shader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glViewport(0, 0, GlobalAppState.width, GlobalAppState.height);
}

void renderDirectionalShadows(const Light& light, const Shader shader, const BetterScene &scene) {
    glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);
    glViewport(0, 0, light.shadowWidth, light.shadowHeight);
    glClearDepth(1.f);
    glClear(GL_DEPTH_BUFFER_BIT);

	setShaderMat4(shader, "uView", light.view);
	setShaderMat4(shader, "uProjection", light.projection);
    renderModels(scene, shader, false);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_DEPTH_TEST);
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

void renderLight(const Light& light, const Shader shader, const int index) {
    if (!light.isOn) {
        return;
    }

	setShaderVec3WithUniform(shader, light.colorUniform, light.color);

	// Render the light
	bool isDirectional = light.type == LightType::Directional;
	bool isPoint = light.type == LightType::PointLight;

	Vector3f position = isDirectional ? light.direction * -GlobalAppState.far : light.position;
	Vector3f direction = isPoint ? getVec3(0) : light.direction;
	setShaderVec3WithUniform(shader, light.directionUniform, direction);
	setShaderVec3WithUniform(shader, light.positionUniform, position);
	setShaderFloatWithUniform(shader, light.constantUniform, isDirectional ? 1.f : light.constant);
	setShaderFloatWithUniform(shader, light.linearUniform, isDirectional ? 0.f : light.linear);
	setShaderFloatWithUniform(shader, light.quadraticUniform, isDirectional ? 0.f : light.quadratic);
	setShaderFloatWithUniform(shader, light.cosineCutoffUniform, isPoint || isDirectional ? -1.f : light.cosineCutOff);
	setShaderFloatWithUniform(shader, light.dropOffUniform, isPoint || isDirectional ? 1.f : light.dropOff);
	setShaderIntWithUniform(shader, light.usesShadowsUniform, light.usesShadows);

	// Render shadow map
	if (light.usesShadows) {
		switch (light.type) {
		case PointLight:
			// @TODO: Point light shadows
			break;
		case Directional:
		case Spot: {
			int shadowMapIndex = TextureUniformConstants::LIGHT_SHADOW_TEXTURE_POSITION_START + light.index;
			glActiveTexture(shadowMapIndex);
			glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
			setShaderIntWithUniform(shader, light.dirShadowUniform, shadowMapIndex);
			setShaderMat4WithUniform(shader, light.shadowMatrixUniform, light.projection * light.view);
			break;
		}
		default:
			Logger::logError("Unknown light type: " + light.type);
			break;
		}
	}

	if (isPoint) {
		float near = GlobalAppState.near;
		float far = GlobalAppState.far;
		float diff = far - near;
		Vector2f uFarNear = {
			(far + near) / diff * 0.5f + 0.5f,
			-(far * near) / diff
		};
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
