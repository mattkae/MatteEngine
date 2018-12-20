#include "DepthVisualizer.h"
#include "Constants.h"
#include <iostream>

GLfloat vertices[] = {
	 // positions       // texture coords
	 -0.7f, 1.0f, 0.0f, 1.0f, 1.0f,
	 -0.7f,	0.5f, 0.0f, 1.0f, 0.0f,
	 -1.0f, 0.5f, 0.0f, 0.0f, 0.0f,
	 -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

GLuint indices[] = {
	0, 1, 3,
	1, 2, 3
};

DepthVisualizer create_depth_visualizer(GLuint depthTexture) {
	DepthVisualizer visualizer;
	visualizer.depthTexture = depthTexture;
	visualizer.shader.load("src/shaders/depthVisualization.vert", "src/shaders/depthVisualization.frag");

	glGenVertexArrays(1, &visualizer.vao);
	glGenBuffers(1, &visualizer.vbo);
	glGenBuffers(1, &visualizer.ebo);

	glBindVertexArray(visualizer.vao);

	glBindBuffer(GL_ARRAY_BUFFER, visualizer.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, visualizer.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	visualizer.isInited = true;
	return visualizer;
}

void render_depth_visualization(DepthVisualizer& visualizer) {
	if (!visualizer.isInited) {
		return;
	}
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, visualizer.depthTexture);
	visualizer.shader.use();
	visualizer.shader.set_uniform_1i("uTexture", 0);
	visualizer.shader.set_uniform_1f("uNear", Constants.near);
	visualizer.shader.set_uniform_1f("uFar", Constants.far);

	glBindVertexArray(visualizer.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}