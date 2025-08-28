#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

#include "text.h"
#include "session.h"
#include "layout.h"

inline void display_text(SDL_Renderer *ren, const TTF_Font *font, const Text &text, const float x, const float y)
{
    SDL_FRect dst{x, y, (float)text.w, (float)text.h};
    SDL_RenderTexture(ren, text.tex, nullptr, &dst);
}

inline void display_words(SDL_Renderer *ren, TTF_Font *font, std::vector<Text> &words, const Layout &L,
                          float lineSpacing = 8.0f)
{
    float x = L.x_words, y = L.y_words;
    for (Text &word : words)
    {
        if (x + word.w > L.max_w) // wrap if needed
        {
            x = L.x_words;
            y += L.text_h + lineSpacing;
        }
        display_text(ren, font, word, x, y);
        x += word.w + L.space_w; // add a space after each word
    }
}

static void display_results(SDL_Renderer *ren, TTF_Font *font, Session s, const Layout &L, const Theme &theme,
                            float lineSpacing = 8.0f)
{

    const std::string time = "Time elapsed: " + std::format("{:.1f}", s.stats.elapsed.count());
    const std::string wpm = "Word per minutes: " + std::format("{:.1f}", s.stats.wpm);
    const std::string accuracy = "Accuracy: " + std::format("{:.1f}", s.stats.accuracy);

    SDL_Color text_color = theme.text;
    display_text(ren, font, Text{time, text_color, font, ren}, L.x_entry, L.y_entry);
    display_text(ren, font, Text{wpm, text_color, font, ren}, L.x_entry, L.y_entry + L.text_h + lineSpacing);
    display_text(ren, font, Text{accuracy, text_color, font, ren}, L.x_entry, L.y_entry + 2 * (L.text_h + lineSpacing));
}

inline void render_frame(SDL_Renderer *ren, TTF_Font *font, const Session &s, std::vector<Text> &words, Text &entry, const Layout &L, const Theme &theme)
{
    SDL_SetRenderDrawColor(ren, theme.background.r, theme.background.g, theme.background.b, theme.background.a);
    SDL_RenderClear(ren);

    display_words(ren, font, words, L);

    if (s.finished)
    {
        display_results(ren, font, s, L, theme);
    }
    else
    {
        SDL_SetRenderDrawColor(ren, theme.text.r, theme.text.g, theme.text.b, theme.text.a);
        SDL_FRect entry_line{L.x_entry - 10, L.y_entry + L.text_h + 10, L.longest_w + 20.f, 10};
        SDL_RenderFillRect(ren, &entry_line);

        if (entry.tex)
        {
            display_text(ren, font, entry, L.x_entry, L.y_entry);
        }
    }

    SDL_RenderPresent(ren);
}

inline void destroy_textures(Text &entry, std::vector<Text> &words)
{
    if (entry.tex)
        entry.destroy_texture();
    for (Text &w : words)
        w.destroy_texture();
}

#endif