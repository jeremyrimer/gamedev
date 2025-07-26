#define _USE_MATH_DEFINES
#include <cmath>
#include "Player.h"
#include <SDL3_image/SDL_image.h>

using std::cos;
using std::sin;

// convert degrees to radians, shifting the calc 90 degrees to the left
#define DEG2RAD(angleDegrees) ( (angleDegrees-90) * M_PI / 180.0f )

// player constructor
Player::Player(SDL_Renderer* renderer)
    : velocity{0, 0}, angle(0), speed(0), rotationSpeed(180.0f), thrust(200.0f), friction(0.996f) {
    SDL_Surface* surface = IMG_Load("assets/player-ship.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    position = {400, 300, 64, 64};
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

    // Apply friction to position
    velocity.x *= friction;
    velocity.y *= friction;
}

// render a player on screen
void Player::render(SDL_Renderer* renderer) {
    SDL_FRect dest = position;
    SDL_FPoint center = {dest.w / 2, dest.h / 2};
    SDL_RenderTextureRotated(renderer, texture, nullptr, &dest, angle, &center, SDL_FLIP_NONE);
}

// get a player's position
SDL_FRect Player::getBounds() const {
    return position;
}
