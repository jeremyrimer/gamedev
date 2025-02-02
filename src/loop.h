#ifndef LOOP_H
#define LOOP_H

#include <GLFW/glfw3.h>

// Game variables
extern float xPos, yPos, speed;

// Function declarations
GLFWwindow* initWindow();
void processInput(GLFWwindow* window);
void updateGame();
void render(GLFWwindow* window);

#endif