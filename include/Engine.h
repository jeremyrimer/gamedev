// Engine.h
#pragma once
#include <SDL3/SDL.h>

void update(const bool* keystates);
void render(SDL_Renderer* renderer);
void handleInput(const SDL_Event& event);
void initPlayer(SDL_Renderer* renderer);