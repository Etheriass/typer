#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

#include "word.h"
#include "colors.h"

std::vector<Word> create_words(std::vector<std::string>& text, SDL_Renderer *ren, TTF_Font *font)
{
    std::vector<Word> words;

    words.clear();
    words.reserve(text.size());
    for (size_t i = 0; i < text.size(); ++i)
        words.push_back(Word{text[i], colors::incoming});

    // pre-create textures once
    for (auto &r : words)
        create_word_texture(ren, font, r);

    return words;
}

bool create_word_texture(SDL_Renderer* ren, TTF_Font* font, Word& w) {
    if (w.tex) return true;
    SDL_Surface* surf = TTF_RenderText_Blended(font, w.text.c_str(), 0, w.color);
    if (!surf) return false;
    w.tex = SDL_CreateTextureFromSurface(ren, surf);
    w.w = surf->w; w.h = surf->h;
    SDL_DestroySurface(surf);
    return w.tex != nullptr;
}

static bool is_same_color(SDL_Color& a, SDL_Color& b){
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

void set_word_color(std::vector<Word>& words, size_t i, SDL_Color c){
    // if (i >= words.size()) return;
    if (is_same_color(words[i].color, c)) return;

    words[i].color = c;
    if (words[i].tex){
        SDL_DestroyTexture(words[i].tex);
        words[i].tex = nullptr;
    }
}

bool is_same_text(const Word& a, const Word& b) {
    return a.text == b.text;
}

void destroy_words(std::vector<Word>& words) {
    for (auto& w : words) {
        if (w.tex) SDL_DestroyTexture(w.tex);
        w.tex = nullptr;
    }
}
