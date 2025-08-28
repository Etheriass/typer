#ifndef TEXT_H
#define TEXT_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>

#include "theme.h"

struct Text
{
    std::string text;
    SDL_Color color;
    SDL_Texture *tex = nullptr;
    int w = 0, h = 0; // texture size in pixels

    Text(const std::string &s, SDL_Color color, TTF_Font *font, SDL_Renderer *ren)
        : text(s), color(color)
    {
        if (s.empty()) return;
        SDL_Surface *surf = TTF_RenderText_Blended(font, this->text.c_str(), 0, color);
        w = surf->w;
        h = surf->h;
        tex = SDL_CreateTextureFromSurface(ren, surf);
        SDL_DestroySurface(surf);
    }

    void destroy_texture()
    {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
};

inline void generate_text_texture(SDL_Renderer *ren, Text &text, TTF_Font *font, const SDL_Color c)
{
    if (text.text.empty()){
        text.w = 0;
        text.h = 0;
        text.destroy_texture();
        return;
    }
    SDL_Surface *surf = TTF_RenderText_Blended(font, text.text.c_str(), 0, c);
    text.w = surf->w;
    text.h = surf->h;
    text.tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_DestroySurface(surf);

}

std::vector<Text> generate_words(const std::vector<std::string> &sample, SDL_Renderer *ren, TTF_Font *font, const Theme &theme);

void change_text_color(SDL_Renderer *ren, TTF_Font *font, Text &text, SDL_Color c);


#endif