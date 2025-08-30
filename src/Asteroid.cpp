#define _USE_MATH_DEFINES
#include "Vector2.h"
#include "Asteroid.h"
#include <vector>
#include <cstdlib>
#include <cmath>
#include "Constants.h"

// Main constructor (handles crater generation)
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

    // Generate craters
    int craterCount = 3 + rand() % 4; // 3-6 craters
    for (int i = 0; i < craterCount; ++i) {
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
        float dist = static_cast<float>(rand()) / RAND_MAX * radius * 0.75f;
        Crater c;
        c.offset = Vector2(std::cos(angle) * dist, std::sin(angle) * dist);
        c.radius = radius * (0.1f + static_cast<float>(rand()) / RAND_MAX * 0.15f);
        craters.push_back(c);
    }
}

// Spawn away from player constructor
Asteroid::Asteroid(SDL_Renderer* renderer, const Vector2& playerPos)
    : Asteroid(
        renderer,
        generateSpawnPosition(playerPos),
        Vector2(
            std::cos((rand() % 360) * (M_PI / 180.0f)) * (50.0f + rand() % 50),
            std::sin((rand() % 360) * (M_PI / 180.0f)) * (50.0f + rand() % 50)
        ),
        static_cast<AsteroidSize>((rand() % 3) + 1)
      )
{}

// Spawn logic
Vector2 Asteroid::generateSpawnPosition(const Vector2& playerPos) {
    while (true) {
        float px = static_cast<float>(rand() % SCREEN_WIDTH);
        float py = static_cast<float>(rand() % SCREEN_HEIGHT);
        Vector2 pos(px, py);

        float dx = pos.x - playerPos.x;
        float dy = pos.y - playerPos.y;
        if ((dx*dx + dy*dy) >= SAFE_SPAWN_RADIUS * SAFE_SPAWN_RADIUS) {
            return pos;
        }
    }
}

void Asteroid::update(float deltaTime) {
    position += velocity * deltaTime;
    rotation += rotationSpeed * deltaTime;
    wrapAroundScreen();
}

bool Asteroid::isSmallest() const { return size == AsteroidSize::SMALL; }

// Split asteroid
std::vector<Asteroid> Asteroid::split(SDL_Renderer* newRenderer) {
    std::vector<Asteroid> pieces;
    if (isSmallest()) return pieces;

    AsteroidSize newSize = static_cast<AsteroidSize>(static_cast<int>(size) - 1);

    for (int i = 0; i < 2 + rand() % 2; ++i) {
        float angle = static_cast<float>(rand() % 360) * (M_PI / 180.0f);
        float speed = 50.0f + rand() % 50;
        Vector2 newVel(std::cos(angle) * speed, std::sin(angle) * speed);
        pieces.emplace_back(newRenderer, position, newVel, newSize);
    }
    return pieces;
}

void Asteroid::wrapAroundScreen() {
    if (position.x + radius < 0) position.x = static_cast<float>(SCREEN_WIDTH) + radius;
    if (position.x - radius > static_cast<float>(SCREEN_WIDTH)) position.x = -radius;
    if (position.y + radius < 0) position.y = static_cast<float>(SCREEN_HEIGHT) + radius;
    if (position.y - radius > static_cast<float>(SCREEN_HEIGHT)) position.y = -radius;
}

void Asteroid::render() const {
    if (!renderer) return;

    int centerX = static_cast<int>(position.x);
    int centerY = static_cast<int>(position.y);

    SDL_Color backing = {130, 130, 130, 255}; // outer/backing circle
    SDL_Color main    = {150, 150, 150, 255}; // inner/main circle
    SDL_Color shade   = {180, 180, 180, 255}; // overlay shading
    SDL_Color crater  = {90, 90, 90, 255};

    // ---- Draw backing circle ----
    for (int w = 0; w < radius*2; w++) {
        for (int h = 0; h < radius*2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius*radius)) {
                SDL_SetRenderDrawColor(renderer, backing.r, backing.g, backing.b, backing.a);
                SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }

    // ---- Draw inner/main circle ----
    int innerRadius = static_cast<int>(radius * 0.9f);
    for (int w = 0; w < innerRadius*2; w++) {
        for (int h = 0; h < innerRadius*2; h++) {
            int dx = innerRadius - w;
            int dy = innerRadius - h;
            if ((dx*dx + dy*dy) <= (innerRadius*innerRadius)) {
                SDL_SetRenderDrawColor(renderer, main.r, main.g, main.b, main.a);
                SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }

    // ---- Draw shading circle (offset) ----
    int offsetX = static_cast<int>(radius * 0.08f);
    int offsetY = static_cast<int>(radius * 0.08f);
    for (int w = 0; w < innerRadius*2; w++) {
        for (int h = 0; h < innerRadius*2; h++) {
            int dx = innerRadius - w;
            int dy = innerRadius - h;
            if ((dx*dx + dy*dy) <= (innerRadius*innerRadius)) {
                SDL_SetRenderDrawColor(renderer, shade.r, shade.g, shade.b, shade.a);
                SDL_RenderPoint(renderer, centerX + dx + offsetX, centerY + dy + offsetY);
            }
        }
    }

    // ---- Draw craters with rotation ----
    float radRot = rotation * (M_PI / 180.0f);
    float cosA = std::cos(radRot);
    float sinA = std::sin(radRot);

    for (auto& c : craters) {
        float rx = c.offset.x * cosA - c.offset.y * sinA;
        float ry = c.offset.x * sinA + c.offset.y * cosA;

        int cx = static_cast<int>(centerX + rx);
        int cy = static_cast<int>(centerY + ry);
        int cr = static_cast<int>(c.radius);

        for (int w = 0; w < cr*2; w++) {
            for (int h = 0; h < cr*2; h++) {
                int dx = cr - w;
                int dy = cr - h;
                if ((dx*dx + dy*dy) <= (cr*cr)) {
                    SDL_SetRenderDrawColor(renderer, crater.r, crater.g, crater.b, crater.a);
                    SDL_RenderPoint(renderer, cx + dx, cy + dy);
                }
            }
        }
    }
}

std::vector<Vector2> Asteroid::generateJaggedPoints(int numPoints) const {
    std::vector<Vector2> points;
    points.reserve(numPoints);
    for (int i = 0; i < numPoints; i++) {
        float angle = i * (2.0f * M_PI / numPoints);
        float noise = ((rand() % 20) - 10) / 100.0f;
        float r = radius * (1.0f + noise);
        points.push_back({std::cos(angle) * r, std::sin(angle) * r});
    }
    return points;
}

Vector2 Asteroid::getPosition() const { return position; }
int Asteroid::getRadius() const { return radius; }
