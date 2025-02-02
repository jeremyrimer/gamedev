cmake_minimum_required(VERSION 3.10)
project(GameDev)

set(CMAKE_CXX_STANDARD 17)

# Set the path to the GLFW library
set(GLFW_PATH "C:/programming/libs/glfw-3.4")

# Add include directories
include_directories(${GLFW_PATH}/include)

# Add source files
set(SOURCES
    src/game.cpp
    src/loop.cpp
)

# Add the executable target
add_executable(game ${SOURCES})

# Link GLFW and OpenGL libraries
target_link_libraries(game
    ${GLFW_PATH}/build/src/libglfw3.a
    opengl32
    gdi32
)