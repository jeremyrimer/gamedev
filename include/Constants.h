#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

// helpers
#define DEG2RAD(angleDegrees) (((angleDegrees) - 90.0f) * M_PI / 180.0f)

// Screen Settings
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

// Player Settings
constexpr float PLAYER_STARTING_ANGLE = 0.0f;
constexpr float PLAYER_STARTING_ROTATION_SPEED = 180.0f;
constexpr float PLAYER_STARTING_SPEED = 0.0f;
constexpr float PLAYER_THRUST = 300.0f;
constexpr float PLAYER_FRICTION = 0.996f;
constexpr int   PLAYER_STARTING_LIVES = 2;
constexpr float RESPAWN_DELAY = 3.0f; // seconds
constexpr float BULLET_SPEED = 600.0f;
constexpr float BULLET_LIFE_SPAN = 1.0f; // seconds