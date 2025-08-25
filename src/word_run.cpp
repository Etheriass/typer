#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "word_run.h"

// build texture once for the word+color
bool ensure_word_texture(SDL_Renderer* ren, TTF_Font* font, WordRun& r) {
    if (r.tex) return true;
    SDL_Surface* surf = TTF_RenderText_Blended(font, r.text.c_str(), 0, r.color);
    if (!surf) return false;
    r.tex = SDL_CreateTextureFromSurface(ren, surf);
    r.w = surf->w; r.h = surf->h;
    SDL_DestroySurface(surf);
    return r.tex != nullptr;
}

// free when done
void destroy_word_runs(std::vector<WordRun>& runs) {
    for (auto& r : runs) {
        if (r.tex) SDL_DestroyTexture(r.tex);
        r.tex = nullptr;
    }
}
