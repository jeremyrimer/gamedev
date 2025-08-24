#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet(Vector2 position, float angle)
    : pos(position), life(BULLET_LIFE_SPAN) 
{
    // Calculate velocity based on ship angle
    vel.x = cosf(DEG2RAD(angle)) * BULLET_SPEED;
    vel.y = sinf(DEG2RAD(angle)) * BULLET_SPEED;
}

void Bullet::update(float deltaTime) {
    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;
    life -= deltaTime;

    // Wrap around screen like player
    if (pos.x < 0) pos.x += SCREEN_WIDTH;
    if (pos.x > SCREEN_WIDTH) pos.x -= SCREEN_WIDTH;
    if (pos.y < 0) pos.y += SCREEN_HEIGHT;
    if (pos.y > SCREEN_HEIGHT) pos.y -= SCREEN_HEIGHT;
}

void Bullet::render(SDL_Renderer* renderer) {
    SDL_FRect rect = { pos.x - 2, pos.y - 2, 4, 4 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
