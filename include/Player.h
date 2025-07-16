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
    int startX;
    int startY;
private:
    SDL_Texture* texture;
    SDL_FRect position;
    
    float velocityX;
    float acceleration;
    float maxSpeed;
    float drag;

    bool movingLeft;
    bool movingRight;
};
