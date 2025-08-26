#ifndef WORD_H
#define WORD_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

struct Word {
    std::string text;
    SDL_Color   color;
    SDL_Texture* tex = nullptr;
    int w = 0, h = 0;          // texture size in pixels
};


bool create_word_texture(SDL_Renderer* ren, TTF_Font* font, Word& w);

std::vector<Word> create_words(const std::vector<std::string>& text, SDL_Renderer *ren, TTF_Font *font);

void set_word_color(std::vector<Word>& words, size_t i, SDL_Color c);

bool is_same_text(const Word& a, const Word& b);

void destroy_words(std::vector<Word>& words);

#endif