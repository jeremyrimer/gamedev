#pragma once
#include <SDL3/SDL.h>
#include "Vector2.h" // Your own Vector2 struct/class

class Explosion {
public:
    Explosion(SDL_Renderer* renderer, const Vector2& position, SDL_Texture* spriteSheet, int frameW, int frameH, int totalFrames, float frameTime);

    void Update(float deltaTime);
    void Draw(SDL_Renderer* renderer) const;
    bool IsFinished() const { return finished; }

private:
    SDL_Renderer* renderer;
    Vector2 pos;
    SDL_Texture* texture;
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int totalFrames;
    float frameTime;   // seconds per frame
    float timer;
    bool finished;
};
