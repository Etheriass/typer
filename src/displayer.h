#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

#include "word.h"
#include "session.h"
#include "layout.h"

// Draw words left->right with wrap at maxWidth.
inline void display_words(SDL_Renderer *ren, TTF_Font *font, std::vector<Word> &words,
                          float x0, float y0, float x_max, float space_w, float line_h,
                          float lineSpacing = 8.0f)
{
    float x = x0, y = y0;
    for (Word &word : words)
    {
        if (!generate_word_texture(ren, font, word))
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

inline void render_frame(SDL_Renderer *ren, TTF_Font *font, const Session &s, std::vector<Word> &words, const Layout &L)
{
    SDL_SetRenderDrawColor(ren, 18, 18, 18, 255);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_FRect overlay{0, 0, (float)L.W, (float)L.H};
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 100);
    SDL_RenderFillRect(ren, &overlay);

    display_words(ren, font, words, L.x_words, L.y_words, L.max_w, L.space_w, L.word_h);

    SDL_SetRenderDrawColor(ren, 250, 250, 250, 255);
    SDL_FRect entry_line{L.x_entry - 10, L.y_entry + L.word_h + 10, L.longest_w + 20.f, 10};
    SDL_RenderFillRect(ren, &entry_line);

    if (s.entry.tex)
    {
        SDL_FRect dst{L.x_entry, L.y_entry, (float)s.entry.w, (float)s.entry.h};
        SDL_RenderTexture(ren, s.entry.tex, nullptr, &dst);
    }

    SDL_RenderPresent(ren);
}

void destroy_textures(Session &s, std::vector<Word> &words)
{
    if (s.entry.tex)
    {
        SDL_DestroyTexture(s.entry.tex);
        s.entry.tex = nullptr;
    }
    for (auto &w : words)
        if (w.tex)
        {
            SDL_DestroyTexture(w.tex);
            w.tex = nullptr;
        }
}

#endif