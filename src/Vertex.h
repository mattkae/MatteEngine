#ifndef VERTEX_H
#define VERTEX_H
#include "Vector2f.h"
#include "Vector3f.h"

struct Vertex {
    Vector3f position;
    Vector3f normal;
    Vector2f texCoords;
};

#endif
