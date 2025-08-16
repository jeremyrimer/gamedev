#pragma once
#include "Vector2.h"
#include <SDL3/SDL.h>
#include <vector>
#include <cmath>

enum class AsteroidSize {
    LARGE = 3,
    MEDIUM = 2,
    SMALL = 1
};

class Asteroid {
    public:
        Asteroid(SDL_Renderer* renderer, Vector2 pos, Vector2 vel, AsteroidSize sz);
        Asteroid(SDL_Renderer* renderer);
        void update(float deltaTime);
        void render() const;
        bool isSmallest() const;
        Vector2 getPosition() const;
        int getRadius() const;

        // Split this asteroid into smaller ones
        std::vector<Asteroid> split(SDL_Renderer* newRenderer);

    private:
        SDL_Renderer* renderer;
        void wrapAroundScreen();
        int radius;
        float rotation;
        float rotationSpeed;
        AsteroidSize size;
        Vector2 position;
        Vector2 velocity;
};
