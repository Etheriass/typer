#pragma once // Variant of ifndef..

#include <SDL3/SDL.h>

struct Theme
{
    bool is_dark;
    SDL_Color background;
    SDL_Color word_incoming;
    SDL_Color word_active;
    SDL_Color word_correct;
    SDL_Color word_incorrect;
    SDL_Color text;
};

inline const Theme DARK_THEME = {
    .is_dark = true,
    .background = SDL_Color{40, 41, 54, 255},
    .word_incoming = SDL_Color{200, 200, 200, 255},
    .word_active = SDL_Color{255, 255, 255, 255},
    .word_correct = SDL_Color{90, 220, 120, 255},
    .word_incorrect = SDL_Color{240, 100, 100, 255},
    .text = SDL_Color{255, 255, 255, 255},
};

inline const Theme LIGHT_THEME = {
    .is_dark = false,
    .background = SDL_Color{247, 244, 233, 255},
    .word_incoming = SDL_Color{100, 100, 100, 255},
    .word_active = SDL_Color{41, 52, 56, 255},
    .word_correct = SDL_Color{40, 185, 40, 255},
    .word_incorrect = SDL_Color{245, 73, 39, 255},
    .text = SDL_Color{41, 52, 56, 255},
};

struct ThemeState
{
    Theme current = DARK_THEME;
    void toggle() { current = current.is_dark ? LIGHT_THEME : DARK_THEME; }
};

// namespace colors
// {
//     inline constexpr SDL_Color incoming = SDL_Color{200, 200, 200, 255};
//     inline constexpr SDL_Color active = SDL_Color{255, 255, 255, 255};
//     inline constexpr SDL_Color correct = SDL_Color{90, 220, 120, 255};
//     inline constexpr SDL_Color incorrect = SDL_Color{240, 100, 100, 255};
// }
