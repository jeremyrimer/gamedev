#define _USE_MATH_DEFINES
#include <cmath>
#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include "Constants.h"
#include <iostream>
#include "Audio.h"

using std::cos;
using std::sin;

// convert degrees to radians, shifting the calc 90 degrees to the left
#define DEG2RAD(angleDegrees) ( (angleDegrees-90) * M_PI / 180.0f )

Player::Player(SDL_Renderer* renderer)
    : renderer(renderer),
      velocity{0, 0}, 
      angle(PLAYER_STARTING_ANGLE), 
      speed(PLAYER_STARTING_SPEED), 
      rotationSpeed(PLAYER_STARTING_ROTATION_SPEED), 
      thrust(PLAYER_THRUST), 
      friction(PLAYER_FRICTION),
      thrusterSound("assets/sound/thrusters.wav") {
    SDL_Surface* surface = IMG_Load("assets/player-ship.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    position = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 32, 32};
}

// handle input for a player
void Player::handleInput(const bool* keystates) {
    rotatingLeft = keystates[SDL_SCANCODE_LEFT];
    rotatingRight = keystates[SDL_SCANCODE_RIGHT];
    thrusting = keystates[SDL_SCANCODE_UP];
    braking = keystates[SDL_SCANCODE_DOWN];
}

// update the player's properties
void Player::update(float deltaTime) {
    if (rotatingLeft) {
        angle -= rotationSpeed * deltaTime;
    }
    if (rotatingRight) {
        angle += rotationSpeed * deltaTime;
    }

    float radians = DEG2RAD(angle);

    if (thrusting) {
        velocity.x += cos(radians) * thrust * deltaTime;
        velocity.y += sin(radians) * thrust * deltaTime;
    }
    if (braking) {
        velocity.x -= cos(radians) * thrust * deltaTime;
        velocity.y -= sin(radians) * thrust * deltaTime;
    }

    // Apply velocity to position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    float centerX = position.x + position.w / 2;
    float centerY = position.y + position.h / 2;

    // player hits the left side of screen
    if (centerX < -position.w / 2) {
      // std::cout << "SETTING PLAYER TO RIGHT SIDE OF SCREEN" << std::endl;
      position.x = static_cast<float>(SCREEN_WIDTH);
    }
    // player hits the right side of the screen
    else if (centerX > static_cast<float>(SCREEN_WIDTH) + position.w / 2) {
      // std::cout << "SETTING PLAYER TO LEWFT SIDE OF SCREEN" << std::endl;
      position.x = -position.w;
    }
    // player hits the top of the screen
    if (centerY < -position.h / 2) {
      // std::cout << "SETTING PLAYER TO BOTTOM OF SCREEN" << std::endl;
      position.y = static_cast<float>(SCREEN_HEIGHT);
    }
    // player hits the bottom of the screen
    else if (centerY > static_cast<float>(SCREEN_HEIGHT) + position.h / 2) {
      // std::cout << "SETTING PLAYER TO TOP OF SCREEN" << std::endl;
      position.y = -position.h;
    }

    // Apply friction to position but use deltaTime to make framerate sync a non-issue
    velocity.x -= velocity.x * friction * deltaTime;
    velocity.y -= velocity.y * friction * deltaTime;

    // SOUND
    if (thrusting) {
        thrusterSound.play();
        thrusterSound.updateForLooping();
    } else {
        thrusterSound.stop();
    }
}

// render a player on screen
void Player::render() {
    SDL_FRect dest = position;
    SDL_FPoint center = {dest.w / 2, dest.h / 2};
    SDL_RenderTextureRotated(renderer, texture, nullptr, &dest, angle, &center, SDL_FLIP_NONE);

    if (thrusting) renderThruster();    
}

SDL_FRect Player::getPosition() const {
    return position;
}

float Player::getAngle() const {
    return angle;
}

SDL_FPoint Player::getVelocity() const {
    return velocity;
}

void Player::renderThruster() {
    if (rand() % 100 < 90) { // 90% chance to show flame this frame
        float flameLength = 18.0f + (rand() % 6);
        float flameSpread = 0.5f + (rand() % 3);

        float rad = DEG2RAD(angle);
        float centerX = position.x + position.w / 2;
        float centerY = position.y + position.h / 2;

        float rearX = centerX - cos(rad) * (position.w / 2);
        float rearY = centerY - sin(rad) * (position.h / 2);

        // Slight randomization in color to simulate flicker
        int r = 240 + rand() % 16; // 240–255
        int g = 80 + rand() % 30;  // 80–110
        int b = 0;

        SDL_Vertex verts[3];
        verts[0].position = { rearX - cos(rad) * flameLength, rearY - sin(rad) * flameLength };
        verts[1].position = { rearX + sin(rad) * flameSpread, rearY - cos(rad) * flameSpread };
        verts[2].position = { rearX - sin(rad) * flameSpread, rearY + cos(rad) * flameSpread };

        for (int i = 0; i < 3; ++i)
            verts[i].color = { static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), 255 };

        SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);
    }
}