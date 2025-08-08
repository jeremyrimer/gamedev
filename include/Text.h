#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>


class Text {
public:
    /**
     * Constructor takes relative file path to a WAV file
     * e.g. assets/sound/thrusters.wav
     */
    Text(SDL_Renderer* renderer, const char* fontFilePath, int fontSize);
    ~Text();
    void display(std::string text, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;


private:
    void init();
    SDL_Color color;
    SDL_Renderer* renderer;
    TTF_Font* font;
    int fontSize;
    const char* fontFilePath;
};
