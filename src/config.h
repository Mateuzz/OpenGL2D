#pragma once

#include <SDL2/SDL.h>

namespace config {
    constexpr unsigned RepeatTestNumber = 100;
    constexpr int MaxTextureSlots = 32;
    inline int ActualTexturesSlots = 16; // 16 is the minimum, we can up this during runtime using glGetIntegerv
    constexpr size_t TexturesCount = 32;
    inline SDL_Window* gWindow;
};
