#include <SDL3/SDL.h>
#include "Player.h"
#include <string>
#include <SDL3_ttf/SDL_ttf.h>

class DebugHUD {
public:
    DebugHUD(SDL_Renderer* renderer);
    void handleInput(const bool* keystates);
    void update(float deltaTime, const Player& player);
    void render(SDL_Renderer* renderer);

private:
    void renderText(SDL_Renderer* renderer, const std::string &text, int x, int y);
    bool visible = false;
    // SDL_ttf stuff: font, textures, etc.
    // Data to display: FPS, coords, etc.
    SDL_FRect playerBounds;
    TTF_Font* font;  // ← this line was missing!
};