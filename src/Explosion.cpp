#include "Explosion.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

// Static member definition
SDL_Texture* Explosion::texture = nullptr;

Explosion::Explosion(SDL_Renderer* ren, Vector2 pos, float size, float frameDuration, const std::string soundFilePath)
    : position(pos),
      renderer(ren),
      currentFrame(0),
      size(size),
      frameTime(frameDuration),
      elapsedTime(0.0f),
      finished(false),
      explosionSound(soundFilePath) {
        explosionSound.setVolume(0.3f);
      }

bool Explosion::LoadTexture(SDL_Renderer* ren) {
    texture = IMG_LoadTexture(ren, "assets/images/explosion_transparent.png");
    if (!texture) {
        SDL_Log("Failed to load explosion texture: %s", SDL_GetError());
        return false;
    } else {
        std::cout << "Loaded Explosion Texture" << std::endl;
    }
    return true;
}

void Explosion::UnloadTexture() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Explosion::update(float delta) {
    if (finished) return;

    if (!explosionSound.isPlaying()) {
        std::cout << "playing explosion" << std::endl;
        explosionSound.play();
    }

    elapsedTime += delta;
    while (elapsedTime >= frameTime) {
        elapsedTime -= frameTime;
        currentFrame++;
        if (currentFrame >= frameCount) {
            finished = true;
            break;
        }
    }
}

void Explosion::draw() {
    if (finished || !texture) return;

    float texW, texH;
    SDL_GetTextureSize(texture, &texW, &texH);
    
    int frameW = texW / frameCount;

    SDL_FRect src = { float(currentFrame * frameW), 0, float(frameW), float(texH) };
    SDL_FRect dst = { position.x - size / 2, position.y - size / 2, size, size };

    SDL_RenderTexture(renderer, texture, &src, &dst);
}