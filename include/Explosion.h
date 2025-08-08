#pragma once
#include <SDL3/SDL.h>
#include "Vector2.h"
#include "Audio.h"

class Explosion {
public:
    SDL_Renderer* renderer;
    Vector2 position;      // Center position
    float size;            // Width & height
    int currentFrame;      // Current frame index
    float frameTime;       // Seconds per frame
    float elapsedTime;     // Time accumulated for frame switching
    bool finished;         // Whether animation is done
    Audio explosionSound;  // sound to play when blowing up

    Explosion(SDL_Renderer* renderer, Vector2 pos, float size, float frameDuration, const std::string soundFilePath);

    void update(float delta);
    void draw();

    static constexpr int leftMargin = 50;
    static constexpr int rightMargin = 50;
    static constexpr int spacing = 30;    
    static constexpr int frameCount = 7;
    static SDL_Texture* texture;
    static bool LoadTexture(SDL_Renderer* renderer);
    static void UnloadTexture();
};
