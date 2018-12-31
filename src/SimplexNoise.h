#ifndef SIMPLEX_NOISE_H
#define SIMPLEX_NOISE_H
#include <glm/glm.hpp>

int* get_simplex_array(int permSize);
float calculate_simplex_value(glm::vec2 coordinate, float maxHeight,
                                     float scaleFactor, float ampFactor,
                                     float frequencyFactor, int numOctaves,
                                     int *perm, int permIndexCap);

#endif
