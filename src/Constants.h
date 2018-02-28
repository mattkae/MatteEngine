#ifndef CONSTANTS_H
#define CONSTANTS_H

struct ApplicationConstants {
  int width = 800;
  int height = 600;
  float aspectRatio = (float)width / (float)height;;
  const char* title = "Flooids";

  float near = 1.f;
  float far = 2000.f;
};

static ApplicationConstants Constants;

#endif
