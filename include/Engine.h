// Engine.h
#pragma once
#include <SDL3/SDL.h>

void update(float deltaTime);
void render(SDL_Renderer* renderer);
void handleGlobalInput(const SDL_Event& event, const bool* keyboardState);
void init(SDL_Renderer* renderer);
