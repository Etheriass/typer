#pragma once // Variant of ifndef..

#include <SDL3/SDL.h>

namespace colors
{
    inline constexpr SDL_Color incoming = SDL_Color{200, 200, 200, 255};
    inline constexpr SDL_Color active = SDL_Color{255, 255, 255, 255};
    inline constexpr SDL_Color correct = SDL_Color{90, 220, 120, 255};
    inline constexpr SDL_Color incorrect = SDL_Color{240, 100, 100, 255};
}

