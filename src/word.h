#ifndef WORD_H
#define WORD_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

#include "theme.h"

struct Word {
    std::string text;
    SDL_Color   color;
    SDL_Texture* tex = nullptr;
    int w = 0, h = 0;          // texture size in pixels
};

std::vector<Word> create_words(const std::vector<std::string>& text, SDL_Renderer *ren, TTF_Font *font, const Theme &theme);

bool generate_word_texture(SDL_Renderer* ren, TTF_Font* font, Word& w);


void set_word_color(Word &word, SDL_Color c);

bool is_same_text(const Word& a, const Word& b);

void destroy_words(std::vector<Word>& words);

#endif