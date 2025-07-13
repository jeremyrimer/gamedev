// Engine.h
#pragma once
#include <SDL3/SDL.h>

void update();
void render(SDL_Renderer* renderer);
void handleInput(const SDL_Event& event);
