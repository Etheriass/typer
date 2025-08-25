#ifndef TEXT_DISPLAYER_H
#define TEXT_DISPLAYER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

#include "word_run.h"

TTF_Font *get_font();

// Draw words left->right with wrap at maxWidth.
// Returns the y of the last line drawn (bottom).
inline float draw_colored_words(SDL_Renderer *ren, TTF_Font *font,
                                std::vector<WordRun> &runs,
                                float x0, float y0, float maxWidth,
                                float lineSpacing = 8.0f)
{
    int spaceW = 0, lineH = 0;
    TTF_GetStringSize(font, "-", 0, &spaceW, &lineH);

    float x = x0, y = y0;
    for (auto &r : runs)
    {
        if (!ensure_word_texture(ren, font, r))
            continue;

        // wrap if needed
        if (x + r.w > x0 + maxWidth)
        {
            x = x0;
            y += lineH + lineSpacing;
        }

        SDL_FRect dst{x, y, (float)r.w, (float)r.h};
        SDL_RenderTexture(ren, r.tex, nullptr, &dst);

        x += r.w + spaceW; // add a space after each word
    }
    return y + lineH;
}

std::vector<WordRun> init_text(std::vector<std::string>& words, SDL_Renderer *ren, TTF_Font *font);

// void display_text(){

// }

void clear_text(std::vector<WordRun> runs, TTF_Font *font);

void set_word_color(std::vector<WordRun>& runs, size_t i, SDL_Color c);

#endif