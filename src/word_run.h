#ifndef WORD_RUN_H
#define WORD_RUN_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

struct WordRun {
    std::string text;          // e.g., "hello"
    SDL_Color   color;         // per-word color
    SDL_Texture* tex = nullptr;
    int w = 0, h = 0;          // texture size in pixels
};

// build texture once for the word+color
bool ensure_word_texture(SDL_Renderer* ren, TTF_Font* font, WordRun& r);

// free when done
void destroy_word_runs(std::vector<WordRun>& runs);

#endif