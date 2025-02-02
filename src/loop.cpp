#include <GLFW/glfw3.h>
#include <iostream>

// Game variables
float xPos = 0.0f; // X position of the object
float yPos = 0.0f; // Y position of the object
float speed = 0.05f; // Speed of the object

// Initialize the GLFW window
GLFWwindow* initWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return nullptr;
    }

    // Create the windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Game Loop Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable V-Sync
    return window;
}

// Handle input (e.g., move object)
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);  // Close window on ESC key press
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        yPos += speed; // Move up
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        yPos -= speed; // Move down
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        xPos -= speed; // Move left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        xPos += speed; // Move right
    }
}

// Update the game state (this is where you handle game logic)
void updateGame() {
    // For now, nothing to update, just move the object
}

// Render the scene
void render(GLFWwindow* window) {
    // Clear the screen to a color (e.g., dark gray)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Simple rendering (just a point here for illustration)
    glPointSize(10.0f);
    glBegin(GL_POINTS);
        glVertex2f(xPos, yPos); // Draw a point at the current position
    glEnd();
}
