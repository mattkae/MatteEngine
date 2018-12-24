#pragma once
#include "Shader.h"
#include "gl_includes.h"
#include <vector>

struct DepthVisualizer {
	Shader shader;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint depthTexture;
	bool isInited = false;
};

DepthVisualizer create_depth_visualizer(GLuint depthTexture);
void render_depth_visualization(DepthVisualizer& visualizer);