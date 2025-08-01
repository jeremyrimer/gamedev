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
    rotation = static_cast<float>(rand() % 360);
    rotationSpeed = ((rand() % 200) - 100) / 100.0f; // -1.0 to 1.0
}

Asteroid::Asteroid(SDL_Renderer* renderer) : renderer(renderer) {
     // Random position anywhere on screen
    float px = static_cast<float>(rand() / RAND_MAX * SCREEN_WIDTH);
    float py = static_cast<float>(rand() / RAND_MAX * SCREEN_HEIGHT);
    position = Vector2(px, py);
    
    // Random angle and speed for velocity
    float angle = static_cast<float>(rand() % 360) * (M_PI / 180.0f);
    float speed = 50.0f + rand() % 50; // 50–100
    velocity = Vector2(std::cos(angle) * speed, std::sin(angle) * speed);

    // Random size: large, medium, or small (weighted if you want)
    int r = rand() % 3;
    size = static_cast<AsteroidSize>(r + 1); // enum values 1 = SMALL to 3 = LARGE

    switch (size) {
        case AsteroidSize::LARGE:  radius = 40; break;
        case AsteroidSize::MEDIUM: radius = 25; break;
        case AsteroidSize::SMALL:  radius = 15; break;
    }

    rotation = static_cast<float>(rand() % 360);
    rotationSpeed = ((rand() % 200) - 100) / 100.0f; // -1.0 to 1.0
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
std::vector<Asteroid> Asteroid::split() {
    std::vector<Asteroid> pieces;
    if (isSmallest()) return pieces;

    AsteroidSize newSize = static_cast<AsteroidSize>(static_cast<int>(size) - 1);

    for (int i = 0; i < 2 + rand() % 2; ++i) {
        float angle = static_cast<float>(rand() % 360) * (3.14159f / 180.0f);
        float speed = 50.0f + rand() % 50;
        Vector2 newVel(std::cos(angle) * speed, std::sin(angle) * speed);

        pieces.emplace_back(renderer, position, newVel, newSize);
    }

    return pieces;
}

void Asteroid::wrapAroundScreen() {
    if (position.x + radius < 0) position.x = static_cast<float>(SCREEN_WIDTH) + radius;
    if (position.x - radius > static_cast<float>(SCREEN_WIDTH)) position.x = -radius ;
    if (position.y + radius < 0) position.y += static_cast<float>(SCREEN_HEIGHT) + radius;
    if (position.y - radius > static_cast<float>(SCREEN_HEIGHT)) position.y = -radius;
}

void Asteroid::render() const {


    int centerX = static_cast<int>(position.x);
    int centerY = static_cast<int>(position.y);

    // Draw filled or outline circle (assuming your drawCircle function)
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
