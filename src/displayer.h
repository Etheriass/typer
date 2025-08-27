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

static void display_text(SDL_Renderer *ren, TTF_Font *font, const std::string &text,
                         float x0, float y0, float x_max, float space_w, float line_h, const ThemeState &theme,
                         float lineSpacing = 8.0f)
{
    float x = x0, y = y0;

    int text_w = 0, text_h = 0;
    TTF_GetStringSize(font, text.c_str(), 0, &text_w, &text_h);

    SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), 0, theme.get().text);
    if (!surf)
        return;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_DestroySurface(surf);
    if (!tex)
        return;

    if (x + text_w > x_max)
    {
        x = x0;
        y += line_h + lineSpacing;
    }

    SDL_FRect dst{x, y, (float)text_w, (float)text_h};
    SDL_RenderTexture(ren, tex, nullptr, &dst);

    SDL_DestroyTexture(tex);
}

static void display_results(SDL_Renderer *ren, TTF_Font *font, Session s,
                     float x0, float y0, float x_max, float space_w, float line_h, const ThemeState &theme,
                     float lineSpacing = 8.0f)
{

    const std::string time = "Time elapsed: " + std::format("{:.1f}", s.stats.elapsed.count());
    const std::string wpm = "Word per minutes: " + std::format("{:.1f}", s.stats.wpm);
    const std::string accuracy = "Accuracy: " + std::format("{:.1f}", s.stats.accuracy);

    display_text(ren, font, time, x0, y0, x_max, space_w, line_h, theme);
    display_text(ren, font, wpm, x0, y0 + line_h + lineSpacing, x_max, space_w, line_h, theme);
    display_text(ren, font, accuracy, x0, y0 + 2 * (line_h + lineSpacing), x_max, space_w, line_h, theme);
}

inline void render_frame(SDL_Renderer *ren, TTF_Font *font, const Session &s, std::vector<Word> &words, const Layout &L, const ThemeState &theme)
{
    Theme current_theme = theme.get();
    SDL_SetRenderDrawColor(ren, current_theme.background.r, current_theme.background.g, current_theme.background.b, current_theme.background.a);
    SDL_RenderClear(ren);

    display_words(ren, font, words, L.x_words, L.y_words, L.max_w, L.space_w, L.word_h);

    if (s.finished)
    {
        display_results(ren, font, s, L.x_entry, L.y_entry, L.max_w, L.space_w, L.word_h, theme);
    }
    else
    {
        SDL_SetRenderDrawColor(ren, current_theme.text.r, current_theme.text.g, current_theme.text.b, current_theme.text.a);
        SDL_FRect entry_line{L.x_entry - 10, L.y_entry + L.word_h + 10, L.longest_w + 20.f, 10};
        SDL_RenderFillRect(ren, &entry_line);

        if (s.entry.tex)
        {
            SDL_FRect dst{L.x_entry, L.y_entry, (float)s.entry.w, (float)s.entry.h};
            SDL_RenderTexture(ren, s.entry.tex, nullptr, &dst);
        }
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