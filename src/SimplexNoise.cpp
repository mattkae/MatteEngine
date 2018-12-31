#include "SimplexNoise.h"
#include <cmath>
#include <chrono>
#include <random>

int *get_simplex_array(int permSize) {
    int *perm = new int[permSize];
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
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
    5, 2, 2, 5, -5, 2, -2, 5, 5, -2, 2, -5, -5, -2, -2, -5,
};

float grad(glm::vec2 in, glm::vec2 distance, const int *perm,
                  const int permIndexCap) {
    int hash =
        perm[(perm[(int)in.x & permIndexCap] + (int)in.y) & permIndexCap] &
        0x0E;
    return gradients2D[hash] * distance.x + gradients2D[hash + 1] * distance.y;
}

float simplex(glm::vec2 in, const int *perm, const int permIndexCap) {
    float sample = 0; // Resultant value

    glm::vec2 s0 =
        in + (in.x + in.y) * SKEW_FACTOR; // in point in Simplex-Space
    glm::vec2 v0 =
        glm::vec2(fast_floor(s0.x),
                  fast_floor(s0.y)); // Lower left point in Simplex-Space

    glm::vec2 p0 =
        v0 - (v0.x + v0.y) * UNSKEW_FACTOR; // Lower left point in IN-space
    glm::vec2 d0 =
        in - p0; // Distance between in point and lower left point, in IN-space

    glm::vec2 v1 =
        (d0.x > d0.y)
            ? glm::vec2(1.f, 0.f)
            : glm::vec2(0.f, 1.f); // Upper left point, or lower right point

    // https://github.com/WardBenjamin/SimplexNoise/blob/master/SimplexNoise/Noise.cs
    glm::vec2 d1 = d0 - v1 + UNSKEW_FACTOR;
    glm::vec2 d2 = d0 - 1.f + 2.f * UNSKEW_FACTOR;

    // Sample from bottom left point
    float sample0 = 0;
    float t0 = 0.5f - (d0.x * d0.x) - (d0.y * d0.y);
    if (t0 > 0) {
        t0 *= t0;
        sample0 = t0 * t0 * grad(v0, d0, perm, permIndexCap);
    }

    // Sample from upper left/bottom right point
    float sample1 = 0;
    float t1 = 0.5f - (d1.x * d1.x) - (d1.y * d1.y);
    if (t1 > 0) {
        t1 *= t1;
        sample1 = t1 * t1 * grad(v0 + v1, d1, perm, permIndexCap);
    }

    // Sample from upper right point
    float sample2 = 0;
    float t2 = 0.5f - (d2.x * d2.x) - (d2.y * d2.y);
    if (t2 > 0) {
        t2 *= t2;
        sample2 = t2 * t2 * grad(v0 + glm::vec2(1, 1), d2, perm, permIndexCap);
    }

    // Adjust sample appropriately
    return 40.f * (sample0 + sample1 + sample2);
}

float calculate_simplex_value(glm::vec2 coordinate, float maxHeight,
                                     float scaleFactor, float ampFactor,
                                     float frequencyFactor, int numOctaves,
                                     int *perm, int permIndexCap) {
    float frequency = scaleFactor;
    float amp = 1.f;
    float maxAmp = 0;
    float noise = 0;
    for (int i = 0; i < numOctaves; i++) {
        noise += (simplex(frequency * coordinate, perm, permIndexCap) * amp);
        maxAmp += amp;
        amp *= ampFactor;
        frequency *= frequencyFactor;
    }

    // Take the averafe values of the iterations
    noise /= maxAmp;

    // Normalize the result
    return noise * (maxHeight + maxHeight) / 2.f;
}
