#include <SDL3/SDL.h>
#include "Player.h"
#include <string>
#include <SDL3_ttf/SDL_ttf.h>

class DebugHUD {
public:
    DebugHUD(SDL_Renderer* renderer);
    void handleInput(const bool* keystates);
    void update(float deltaTime, const Player& player);
    void render();

private:
    SDL_Renderer* renderer;
    void renderText(const std::string &text, int x, int y);
    bool lastF3State = false;
    bool visible = false;
    SDL_FRect playerBounds;
    float playerAngle;
    SDL_FPoint playerVelocity;
    TTF_Font* font;
};