// Player.h
#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>

class Player {
public:
    Player(SDL_Renderer* renderer);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void handleInput(const bool* keystates);
    SDL_FRect getBounds() const;

private:
    SDL_Texture* texture;
    SDL_FRect position;
    SDL_FPoint velocity;
    float angle; 
    float speed;
    float rotationSpeed;
    float thrust;
    float friction;

    bool rotatingLeft;
    bool rotatingRight;
    bool thrusting;
    bool braking;
};
