#include "loop.h"
#include <GLFW/glfw3.h>

int main() {
    GLFWwindow* window = initWindow();
    if (!window) return -1;

    // Main game loop
    while (!glfwWindowShouldClose(window)) {
        // Handle input
        processInput(window);

        // Update the game state
        updateGame();

        // Render the scene
        render(window);

        // Swap buffers (to show the rendered frame)
        glfwSwapBuffers(window);

        // Poll for events (keyboard, mouse, etc.)
        glfwPollEvents();
    }

    // Cleanup and terminate
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}