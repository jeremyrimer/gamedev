#define _USE_MATH_DEFINES
#include "Vector2.h"
#include "Asteroid.h"
#include <vector>
#include <cstdlib>  // for rand()
#include <cmath>
#include "Constants.h"

Asteroid::Asteroid(SDL_Renderer* renderer, Vector2 pos, Vector2 vel, AsteroidSize sz)
    : position(pos), velocity(vel), size(sz), renderer(renderer)
{
    switch (size) {
        case AsteroidSize::LARGE:  radius = 40; break;
        case AsteroidSize::MEDIUM: radius = 25; break;
        case AsteroidSize::SMALL:  radius = 15; break;
    }

    rotation = static_cast<float>(rand() % 360);
    rotationSpeed = ((rand() % 200) - 100) / 100.0f; // -1.0 to 1.0
}

Asteroid::Asteroid(SDL_Renderer* renderer)
    : Asteroid(
        renderer,
        Vector2(                           // random position
            static_cast<float>(rand() % SCREEN_WIDTH),
            static_cast<float>(rand() % SCREEN_HEIGHT)
        ),
        Vector2(                           // random velocity
            std::cos((rand() % 360) * (M_PI / 180.0f)) * (50.0f + rand() % 50),
            std::sin((rand() % 360) * (M_PI / 180.0f)) * (50.0f + rand() % 50)
        ),
        static_cast<AsteroidSize>((rand() % 3) + 1) // random size
      )
{
    // Nothing else needed here!
}


void Asteroid::update(float deltaTime) {
    position += velocity * deltaTime;
    rotation += rotationSpeed * deltaTime;
    wrapAroundScreen();
}

bool Asteroid::isSmallest() const {
    return size == AsteroidSize::SMALL;
}

// Split this asteroid into smaller ones
std::vector<Asteroid> Asteroid::split(SDL_Renderer* newRenderer) {
    std::vector<Asteroid> pieces;
    if (isSmallest()) return pieces;

    AsteroidSize newSize = static_cast<AsteroidSize>(static_cast<int>(size) - 1);

    for (int i = 0; i < 2 + rand() % 2; ++i) {
        float angle = static_cast<float>(rand() % 360) * (3.14159f / 180.0f);
        float speed = 50.0f + rand() % 50;
        Vector2 newVel(std::cos(angle) * speed, std::sin(angle) * speed);

        pieces.emplace_back(newRenderer, position, newVel, newSize);
    }

    return pieces;
}

void Asteroid::wrapAroundScreen() {
    if (position.x + radius < 0) position.x = static_cast<float>(SCREEN_WIDTH) + radius;
    if (position.x - radius > static_cast<float>(SCREEN_WIDTH)) position.x = -radius ;
    if (position.y + radius < 0) position.y = static_cast<float>(SCREEN_HEIGHT) + radius;
    if (position.y - radius > static_cast<float>(SCREEN_HEIGHT)) position.y = -radius;
}

void Asteroid::render() const {
    if (!renderer) {
        SDL_Log("ERROR: Asteroid has null renderer pointer!");
        return; // avoid crash
    }
    int centerX = static_cast<int>(position.x);
    int centerY = static_cast<int>(position.y);

    // Placeholder: use SDL_RenderPoint or your own drawCircle()
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
                SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

Vector2 Asteroid::getPosition() const {
    return position;
}

int Asteroid::getRadius() const {
    return radius;
}