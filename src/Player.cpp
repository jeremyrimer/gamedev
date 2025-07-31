#define _USE_MATH_DEFINES
#include <cmath>
#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include "Constants.h"
#include <iostream>

using std::cos;
using std::sin;

// convert degrees to radians, shifting the calc 90 degrees to the left
#define DEG2RAD(angleDegrees) ( (angleDegrees-90) * M_PI / 180.0f )

Player::Player(SDL_Renderer* renderer)
    : velocity{0, 0}, 
      angle(PLAYER_STARTING_ANGLE), 
      speed(PLAYER_STARTING_SPEED), 
      rotationSpeed(PLAYER_STARTING_ROTATION_SPEED), 
      thrust(PLAYER_THRUST), 
      friction(PLAYER_FRICTION) {
    SDL_Surface* surface = IMG_Load("assets/player-ship.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    SDL_AudioSpec spec;
    if (!SDL_LoadWAV("assets/sound/thrusters.wav", &spec, &thrusterBuffer, &thrusterLength)) {
        SDL_Log("Failed to load thruster sound: %s", SDL_GetError());
    } else {
        SDL_AudioSpec desiredSpec = {};
        desiredSpec.format = spec.format;
        desiredSpec.channels = spec.channels;
        desiredSpec.freq = spec.freq;
        thrusterDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desiredSpec);
        if (thrusterDevice == 0) {
            SDL_Log("Failed to open audio device: %s", SDL_GetError());
        } else {
            thrusterStream = SDL_CreateAudioStream(&spec, &desiredSpec);
            SDL_BindAudioStream(thrusterDevice, thrusterStream);
            if (SDL_PutAudioStreamData(thrusterStream, thrusterBuffer, thrusterLength) < 0) {
                SDL_Log("Failed to put audio data into stream: %s", SDL_GetError());
            } else {
                SDL_PauseAudioDevice(thrusterDevice);
            }
        }
        // std::cout << "Opening audio device with freq=" << desiredSpec.freq
        //     << ", format=" << desiredSpec.format
        //     << ", channels=" << (int)desiredSpec.channels << std::endl;
                
    }
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
        if (!thrusterAudioData.playing) {
            thrusterAudioData.playing = true;
            // std::cout << "THRUST AUDIO RESUMING" << std::endl;
            SDL_ResumeAudioDevice(thrusterDevice);
        }

        // Refill audio stream if needed
        if (SDL_GetAudioStreamAvailable(thrusterStream) < (int)thrusterLength) {
            if (SDL_PutAudioStreamData(thrusterStream, thrusterBuffer, thrusterLength) < 0) {
                SDL_Log("Failed to queue audio: %s", SDL_GetError());
            }
        }
    } else {
        if (thrusterAudioData.playing) {
            thrusterAudioData.playing = false;
            // std::cout << "THRUST AUDIO PAUSING" << std::endl;
            SDL_PauseAudioDevice(thrusterDevice);
        }
    }
}

// render a player on screen
void Player::render(SDL_Renderer* renderer) {
    SDL_FRect dest = position;
    SDL_FPoint center = {dest.w / 2, dest.h / 2};
    SDL_RenderTextureRotated(renderer, texture, nullptr, &dest, angle, &center, SDL_FLIP_NONE);

    if (thrusting) renderThruster(renderer);    
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

void Player::renderThruster(SDL_Renderer* renderer) {
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