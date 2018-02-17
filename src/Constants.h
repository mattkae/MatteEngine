#ifndef CONSTANTS_H
#define CONSTANTS_H

struct ApplicationConstants {
  int width = 800;
  int height = 600;
  float aspectRatio = (float)width / (float)height;;
  const char* title = "Flooids";

  float near = 0.1f;
  float far = 1000.f;
};

static ApplicationConstants Constants;

#endif
