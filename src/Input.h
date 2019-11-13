#ifndef INPUT_H
#define INPUT_H
#include "Primitives/Point.h"

const int DEFAULT_FOCUS_TOKEN = 0;

struct GLFWwindow;
void initializeInputSystem(GLFWwindow* window);
bool isKeyUp(int key, int focusToken);
bool isKeyJustDown(int key, int focusToken);
bool isKeyDown(int key, int focusToken);
bool isKeyJustUp(int key, int focusToken);
bool isLeftClickDown();
bool isRightClickDown();
Point getCursorPosition();
int grabFocus();
void returnFocus(int focusToken);
int getInputFocus();
int getCurrentKeyDown(int focusToken);
int getCurrentScancode(int focusToken);
void getWindowDimensions(int& width, int& height);
bool isDefaultFocused();

#endif
