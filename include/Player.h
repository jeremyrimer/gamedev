// Player.h
#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>

class Player {
public:
    Player(SDL_Renderer* renderer); // Constructor
    void update(float deltaTime); // update a player's properties based on current state
    void render(SDL_Renderer* renderer); // display a player on the screen
    void handleInput(const bool* keystates); // apply player states based on input
    SDL_FRect getBounds() const; // unsure but seems to get a player's position

private:
    SDL_Texture* texture; // texture to render for the player
    SDL_FRect position; // where to render the texture on screen
    SDL_FPoint velocity; // how fast the player is moving represented by the x/y values of a point
    
    float angle; // angle the right side of the player is facing
    float speed;
    float rotationSpeed;
    float thrust;
    float friction;

    bool rotatingLeft;
    bool rotatingRight;
    bool thrusting;
    bool braking;
};
