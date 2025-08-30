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

struct Crater {
    Vector2 offset;
    float radius;
};

static const float SAFE_SPAWN_RADIUS = 300.0f;

class Asteroid {
    public:
        Asteroid(SDL_Renderer* renderer, Vector2 pos, Vector2 vel, AsteroidSize sz);
        Asteroid(SDL_Renderer* renderer, const Vector2& playerPos);
        void update(float deltaTime);
        void render() const;
        bool isSmallest() const;
        Vector2 getPosition() const;
        int getRadius() const;
        AsteroidSize getSize() const;
        void setVelocity(Vector2 vel);
        Vector2 getVelocity() const;

        // Split this asteroid into smaller ones
        std::vector<Asteroid> split(SDL_Renderer* newRenderer);

    private:
        SDL_Renderer* renderer;
        void wrapAroundScreen();
        Vector2 generateSpawnPosition(const Vector2& playerPos);
        int radius;
        float rotation;
        float rotationSpeed;
        AsteroidSize size;
        Vector2 position;
        Vector2 velocity;
        std::vector<Crater> craters;
        std::vector<Vector2> generateJaggedPoints(int numPoints) const;
};
