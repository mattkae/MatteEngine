#version 410

layout (location = 5) in float layer;

#define HAS_MAIN

flat out int vertexLayer;

#include primary.vert

void runMainExtra() {
	 vertexLayer = int(layer);
}