#include "Light.h"
#include "Camera.h"
#include "App.h"
#include "ImageUtil.h"
#include "Scene.h"
#include "TextureUniformConstants.h"
#include "ShaderUniformMapping.h"
#include <iostream>
#include <sstream>
#include <string>

inline Matrix4x4f getLightProjection(const Light &light) {
    switch (light.type) {
    case Directional:
		// @TODO: Almost certian these values shouldn't be this random
        return getOrthographicProjection(GlobalApp.near, GlobalApp.far, -10, 10, -10.f, 10);
    case Spot:
		// @TODO: Don't default the FOV to 45 degrees
		return getPerspectiveProjection(GlobalApp.near, GlobalApp.far, 0.7853982f, GlobalApp.aspectRatio);
    case PointLight:
    default:
        logger_error("Attempting to get a view for unknown light: %d", light.type);
        return Matrix4x4f();
    }
}

inline Matrix4x4f getLightView(const Light &light) {
    switch (light.type) {
    case Directional: {
        auto lightPosition = light.direction * (-GlobalApp.far / 2.0f);
		return getLookAt(lightPosition, lightPosition + light.direction, light.up);
    }
    case Spot:
		return getLookAt(light.position, light.position + light.direction, light.up);
    case PointLight:
    default:
        logger_error("Attempting to get a view for unknown light: %d", light.type);
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

void Light::initialize(int lightIndex) {
	index = lightIndex;
    switch (type) {
    case Spot:
    case Directional:
        createShadowTextureForDirectionalLight(*this);
        break;
    case PointLight:
        createShadowTextureForPointLight(*this);
        break;
    default:
        break;
    }

	if (usesShadows) {
		view = getLightView(*this);
		projection = getLightProjection(*this);
	}
}

void renderPointShadows(const Light& light, const Scene &scene) {
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
        Matrix4x4f proj = getPerspectiveProjection(GlobalApp.near, GlobalApp.far, 0.7853982f, GlobalApp.aspectRatio);
		setShaderMat4(ShaderUniformMapping::GlobalShadowShaderMapping.LIGHT_PROJ_MATRIX, proj);
		setShaderMat4(ShaderUniformMapping::GlobalShadowShaderMapping.LIGHT_VIEW_MATRIX, view);
		scene.renderModels(ShaderUniformMapping::GlobalShadowShaderMapping.modelUniformMapping, false);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glViewport(0, 0, GlobalApp.width, GlobalApp.height);
}

void renderDirectionalShadows(const Light& light, const Scene &scene) {
    glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);
    glViewport(0, 0, light.shadowWidth, light.shadowHeight);
    glClearDepth(1.f);
    glClear(GL_DEPTH_BUFFER_BIT);

    setShaderMat4(ShaderUniformMapping::GlobalShadowShaderMapping.LIGHT_PROJ_MATRIX, light.projection);
	setShaderMat4(ShaderUniformMapping::GlobalShadowShaderMapping.LIGHT_VIEW_MATRIX, light.view);
    scene.renderModels(ShaderUniformMapping::GlobalShadowShaderMapping.modelUniformMapping, false);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, GlobalApp.width, GlobalApp.height);
}

void Light::renderShadows(const Scene& scene) {
    if (!isOn || !usesShadows) {
        return;
	}

    switch (type) {
    case PointLight:
        renderPointShadows(*this, scene);
        break;
    case Spot:
    case Directional:
        renderDirectionalShadows(*this, scene);
        break;
    default:
        std::cerr << "Rendering shadows for invalid light type " << type << "." << std::endl;
        break;
    }
}

void Light::render(const int index, const LightUniformMapping* uniformMapping) const {
    if (!isOn) {
        return;
    }

	setShaderVec3(uniformMapping->LIGHT_COLOR[index], color);

	// Render the light
	bool isDirectional = type == LightType::Directional;
	bool isPoint = type == LightType::PointLight;

	Vector3f shaderPosition = isDirectional ? direction * -GlobalApp.far : position;
	Vector3f shaderDirection = isPoint ? getVec3(0) : direction;
	setShaderVec3(uniformMapping->LIGHT_DIRECTION[index], shaderDirection);
	setShaderVec3(uniformMapping->LIGHT_POSITION[index], shaderPosition);
	setShaderFloat(uniformMapping->LIGHT_CONSTANT[index], isDirectional ? 1.f : constant);
	setShaderFloat(uniformMapping->LIGHT_LINEAR[index], isDirectional ? 0.f : linear);
	setShaderFloat(uniformMapping->LIGHT_QUADRATIC[index], isDirectional ? 0.f : quadratic);
	setShaderFloat(uniformMapping->LIGHT_COSINE_CUTOFF[index], isPoint || isDirectional ? -1.f : cosineCutOff);
	setShaderFloat(uniformMapping->LIGHT_DROP_OFF[index], isPoint || isDirectional ? 1.f : dropOff);
	setShaderInt(uniformMapping->LIGHT_USES_SHADOWS[index], usesShadows);

	// Render shadow map
	if (usesShadows) {
		switch (type) {
		case PointLight:
			// @TODO: Point light shadows
			break;
		case Directional:
		case Spot: {
			int shadowMapIndex = TextureUniformConstants::LIGHT_SHADOW_TEXTURE_POSITION_START + index;
			glActiveTexture(GL_TEXTURE0 + shadowMapIndex);
			glBindTexture(GL_TEXTURE_2D, shadowTexture);
			setShaderInt(uniformMapping->LIGHT_DIR_SHADOW[index], shadowMapIndex);
			setShaderMat4(uniformMapping->LIGHT_SHADOWMATRIX[index], projection * view);
			break;
		}
		default:
			logger_error("Unknown light type: %d", type);
			break;
		}
	}

	if (isPoint) {
		float near = GlobalApp.near;
		float far = GlobalApp.far;
		float diff = far - near;
		Vector2f uFarNear = {
			(far + near) / diff * 0.5f + 0.5f,
			-(far * near) / diff
		};
		setShaderVec2(uniformMapping->LIGHT_FAR_NEAR_PLANE, uFarNear);
	}
}

void Light::free() {
	if (shadowTexture != 0) {
        glDeleteTextures(1, &shadowTexture);
		shadowTexture = 0;
	}

	if (depthFbo != 0) {
        glDeleteFramebuffers(1, &depthFbo);
		depthFbo = 0;
	}
}
