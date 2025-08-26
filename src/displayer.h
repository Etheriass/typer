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
                          float space_w, float line_h,
                          float lineSpacing = 8.0f)
{
    float x = x0, y = y0;
    for (Word &word : words)
    {
        if (!create_word_texture(ren, font, word))
            continue;
        
        if (x + word.w > x_max) // wrap if needed
        {
            x = x0;
            y += line_h + lineSpacing;
        }

        SDL_FRect dst{x, y, (float)word.w, (float)word.h};
        SDL_RenderTexture(ren, word.tex, nullptr, &dst);

        x += word.w + space_w; // add a space after each word
    }
}

#endif