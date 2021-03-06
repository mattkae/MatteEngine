#include "OpenSimplexNoise.h"
#include <chrono>
#include <cmath>
#include <random>

int* getSimplexArray(int permSize)
{
    int* perm = new int[permSize];
    unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> dist(1, permSize);
    for (int p = 0; p < permSize; p++) {
        perm[p] = dist(generator);
    }
    return perm;
}

const float SKEW_FACTOR = (sqrt(3.f) - 1.f) / 2.f;
const float UNSKEW_FACTOR = (3.f - sqrt(3.f)) / 6.f;

inline int32_t fast_floor(float fp) {
    int32_t i = static_cast<int32_t>(fp);
    return (fp < i) ? (i - 1) : (i);
}

int gradients2D[] = {
    5,
    2,
    2,
    5,
    -5,
    2,
    -2,
    5,
    5,
    -2,
    2,
    -5,
    -5,
    -2,
    -2,
    -5,
};

float grad(const Vector2f& in, const Vector2f& distance, const int* perm,const int permIndexCap) {
    int hash = perm[(perm[(int)in.x & permIndexCap] + (int)in.y) & permIndexCap] & 0x0E;
    return gradients2D[hash] * distance.x + gradients2D[hash + 1] * distance.y;
}

float simplex(const Vector2f& pointWorldSpace, const int* perm, const int permIndexCap){
    Vector2f inSimplex = pointWorldSpace + (pointWorldSpace.x + pointWorldSpace.y) * SKEW_FACTOR; // in point in Simplex-Space
	Vector2f lowerLeftSimplexSpace = Vector2f{
		static_cast<GLfloat>(fast_floor(inSimplex.x)),
		static_cast<GLfloat>(fast_floor(inSimplex.y))
	}; // Lower left point in Simplex-Space

    Vector2f lowerLeftWorldSpace = lowerLeftSimplexSpace - (lowerLeftSimplexSpace.x + lowerLeftSimplexSpace.y) * UNSKEW_FACTOR; // Lower left point in IN-space
    Vector2f d0 = pointWorldSpace - lowerLeftWorldSpace; // Distance between in point and lower left point, in IN-space

	Vector2f upperLeftOrLowerRightPoint = (d0.x > d0.y) ? Vector2f{1.f, 0.f} : Vector2f{0.f, 1.f}; // Upper left point, or lower right point

    // https://github.com/WardBenjamin/SimplexNoise/blob/master/SimplexNoise/Noise.cs
    Vector2f d1 = d0 - upperLeftOrLowerRightPoint + UNSKEW_FACTOR;
    Vector2f d2 = d0 - 1.f + 2.f * UNSKEW_FACTOR;

    // Sample from bottom left point
    float bottomLeftSample = 0;
    float t0 = 0.5f - (d0.x * d0.x) - (d0.y * d0.y);
    if (t0 > 0) {
        t0 *= t0;
        bottomLeftSample = t0 * t0 * grad(lowerLeftSimplexSpace, d0, perm, permIndexCap);
    }

    // Sample from upper left/bottom right point
    float upperLeftOrBottomRightSample = 0;
    float t1 = 0.5f - (d1.x * d1.x) - (d1.y * d1.y);
    if (t1 > 0) {
        t1 *= t1;
        upperLeftOrBottomRightSample = t1 * t1 * grad(lowerLeftSimplexSpace + upperLeftOrLowerRightPoint, d1, perm, permIndexCap);
    }

    // Sample from upper right point
    float upperRightSample = 0;
    float t2 = 0.5f - (d2.x * d2.x) - (d2.y * d2.y);
    if (t2 > 0) {
        t2 *= t2;
		upperRightSample = t2 * t2 * grad(lowerLeftSimplexSpace + Vector2f{1, 1}, d2, perm, permIndexCap);
    }

    // Adjust sample appropriately
    return 40.f * (bottomLeftSample + upperLeftOrBottomRightSample + upperRightSample);
}

float calculateSimplexValue(Vector2f coordinate, float maxHeight,
    float scaleFactor, float ampFactor,
    float frequencyFactor, int numOctaves,
    int* perm, int permIndexCap)
{
    float frequency = scaleFactor;
    float amp = 1.f;
    float maxAmp = 0;
    float noise = 0;
    for (int i = 0; i < numOctaves; i++) {
        noise += (simplex(coordinate * frequency, perm, permIndexCap) * amp);
        maxAmp += amp;
        amp *= ampFactor;
        frequency *= frequencyFactor;
    }

    // Take the averafe values of the iterations
    noise /= maxAmp;

    // Normalize the result
    return noise * (maxHeight + maxHeight) / 2.f;
}
