#ifndef SIMPLEX_NOISE_H
#define SIMPLEX_NOISE_H
#include "Vector2f.h"

int* getSimplexArray(int permSize);
float calculateSimplexValue(Vector2f coordinate, float maxHeight,
                                     float scaleFactor, float ampFactor,
                                     float frequencyFactor, int numOctaves,
                                     int *perm, int permIndexCap);

#endif
