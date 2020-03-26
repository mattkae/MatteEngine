#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"
#include "Shader.h"
#include "Vector3f.h"
#include <GL/glew.h>
#include <vector>

struct LoadMaterial;
struct List;

struct Material {
    std::string name;
	Vector3f emissive = { 0.f, 0.f, 0.f };
    Vector3f ambient = { 1.f, 1.f, 1.f };
    Vector3f diffuse = { 1.f, 1.f, 1.f };
    Vector3f specular = { 1.f, 1.f, 1.f };
    Vector3f diffuseProperty = { 1.f, 1.f, 1.f };
    Vector3f specularProperty = { 1.f, 1.f, 1.f };
    float specularComponent = 20.f;
    float transparency = 1.0;

	GLuint diffuseTexture = 0;
	GLuint specularTexture = 0;
	GLuint ambientTexture = 0;

    void initialize(LoadMaterial& material, List* list);
    void render(const Shader& shader) const;
};

#endif
