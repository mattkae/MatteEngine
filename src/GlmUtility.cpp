#include "GlmUtility.h"
#include <random>
#include <iostream>
#include <iomanip>

namespace glm {
  static std::default_random_engine generator;

  glm::vec3 get_random_within(glm::vec3 origin, glm::vec3 size) {
    std::uniform_real_distribution<float> xDistribution(origin.x - size.x, origin.x + size.x);
    std::uniform_real_distribution<float> yDistribution(origin.y - size.y, origin.y + size.y);
    std::uniform_real_distribution<float> zDistribution(origin.z - size.z, origin.z + size.z);
    float x = xDistribution(generator);
    float y = yDistribution(generator);
    float z = zDistribution(generator);

    return glm::vec3(x, y, z);
  }

  float get_random_between(glm::vec2 range) {
    std::uniform_real_distribution<float> distribution(range.x, range.y);
    return distribution(generator);
  }

  void print(glm::vec3 vector) {
    std::cout << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
  }

  void print(glm::vec4 vector) {
    std::cout << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << std::endl;
  }

  void print(glm::mat3 matrix) {
    std::cout << "START";
    for (int r = 0; r < 3; r++) {
      for (int c = 0; c < 3; c++) {
	std::cout << std::setw(5) << matrix[r][c];
      }
      std::cout << std::endl;
    }
    std::cout << "END";
  }

  void print(glm::mat4 matrix) {
    std::cout << "START" << std::endl;
    for (int r = 0; r < 4; r++) {
      for (int c = 0; c < 4; c++) {
	std::cout << std::setw(5) << matrix[r][c];
      }
      std::cout << std::endl;
    }
    std::cout << "END" << std::endl;
  }
}
