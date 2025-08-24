#include <SDL3/SDL.h>
#include "Vector2.h"
#include "Constants.h"
#include "Bullet.h"

Bullet::Bullet(Vector2 startPos, float direction)
    : pos(startPos), speed(BULLET_SPEED), lifetime(BULLET_LIFE_SPAN), radius(BULLET_RADIUS), active(true) {
    float radians = DEG2RAD(direction);
    vel = Vector2(std::cos(radians), std::sin(radians)) * speed;
}

void Bullet::update(float dt) {
    pos += vel * dt;
    lifetime -= dt;
    if (lifetime <= 0.0f) active = false;

    // screen wrap
    if (pos.x < 0) pos.x += SCREEN_WIDTH;
    if (pos.x > SCREEN_WIDTH) pos.x -= SCREEN_WIDTH;
    if (pos.y < 0) pos.y += SCREEN_HEIGHT;
    if (pos.y > SCREEN_HEIGHT) pos.y -= SCREEN_HEIGHT;
}

void Bullet::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    // Draw filled circle
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius*radius)) {
                SDL_RenderPoint(renderer, pos.x + dx, pos.y + dy);
            }
        }
    }
}

bool Bullet::isAlive() const { return active; }
float Bullet::getRadius() const { return radius; }
Vector2 Bullet::getPosition() const { return pos; }