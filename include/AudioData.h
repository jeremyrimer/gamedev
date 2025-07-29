#pragma once
#include <SDL3/SDL.h>

struct AudioData {
    Uint8* buffer = nullptr;
    Uint32 length = 0;
    Uint32 position = 0;
    bool playing = false;
};
