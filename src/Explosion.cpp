#include "Explosion.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

Explosion::Explosion(SDL_Renderer* renderer, const Vector2& position, SDL_Texture* spriteSheet, int frameW, int frameH, int totalFrames, float frameTime)
    : renderer(renderer),
      pos(position),
      texture(spriteSheet),
      frameWidth(frameW),
      frameHeight(frameH),
      currentFrame(0),
      totalFrames(totalFrames),
      frameTime(frameTime),
      timer(0.0f),
      finished(false)
{
    SDL_Texture* explosionTexture = IMG_LoadTexture(renderer, "assets/imagesexplosion.png");
    if (!explosionTexture) {
        SDL_Log("Failed to load explosion texture: %s", SDL_GetError());
    } else {
        std::cout << "Explosion Texture Loaded" << std::endl;
    }

    SDL_Surface* surface = IMG_Load("assets/explosion.png");
    if (!surface) {
        SDL_Log("Failed to load image: %s", SDL_GetError());
    }
}

void Explosion::Update(float deltaTime) {
    if (finished) return;

    timer += deltaTime;
    if (timer >= frameTime) {
        timer -= frameTime;
        currentFrame++;
        if (currentFrame >= totalFrames) {
            finished = true;
        }
    }
}

void Explosion::Draw(SDL_Renderer* renderer) const {
    if (finished) return;

    SDL_FRect src = {
        static_cast<float>(currentFrame * frameWidth),
        0.0f,
        static_cast<float>(frameWidth),
        static_cast<float>(frameHeight)
    };

    SDL_FRect dst = {
        pos.x - frameWidth / 2.0f,
        pos.y - frameHeight / 2.0f,
        static_cast<float>(frameWidth),
        static_cast<float>(frameHeight)
    };

    SDL_RenderTexture(renderer, texture, &src, &dst);
}
