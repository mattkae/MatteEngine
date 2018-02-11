#ifndef CONSTANTS_H
#define CONSTANTS_H

struct ApplicationConstants {
  int width = 1024;
  int height = 1024;
  float aspectRatio = 1.f;
  const char* title = "Flooids";
};

static ApplicationConstants Constants;

#endif
