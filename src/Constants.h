#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>

struct ApplicationConstants {
  int width = 800;
  int height = 600;
  float aspectRatio = (float)width / (float)height;
  std::string title = "Flooids";

  float near = 1.f;
  float far = 1000.f;
};

static ApplicationConstants Constants;

#endif
