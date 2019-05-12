#ifndef SIMPLEX_NOISE_H
#define SIMPLEX_NOISE_H
#include <glm/glm.hpp>

int* getSimplexArray(int permSize);
float calculateSimplexValue(glm::vec2 coordinate, float maxHeight,
                                     float scaleFactor, float ampFactor,
                                     float frequencyFactor, int numOctaves,
                                     int *perm, int permIndexCap);

#endif
