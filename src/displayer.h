#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

#include "word.h"


// Draw words left->right with wrap at maxWidth.
inline void display_words(SDL_Renderer *ren, TTF_Font *font,
                                std::vector<Word> &words,
                                float x0, float y0, float x_max,
                                float lineSpacing = 8.0f)
{
    int spaceW = 0, lineH = 0;
    TTF_GetStringSize(font, ".", 0, &spaceW, &lineH);

    float x = x0, y = y0;
    for (auto &word : words)
    {
        if (!create_word_texture(ren, font, word))
            continue;

        // wrap if needed
        if (x + word.w > x_max)
        {
            x = x0;
            y += lineH + lineSpacing;
        }

        SDL_FRect dst{x, y, (float)word.w, (float)word.h};
        SDL_RenderTexture(ren, word.tex, nullptr, &dst);

        x += word.w + spaceW; // add a space after each word
    }
}


void clear_text(std::vector<Word> words);

#endif