#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

#include "word.h"
#include "colors.h"

std::vector<Word> create_words(const std::vector<std::string>& text, SDL_Renderer *ren, TTF_Font *font)
{
    std::vector<Word> words;
    words.reserve(text.size());
    for (size_t i = 0; i < text.size(); ++i)
        words.push_back(Word{text[i], i == 0 ? colors::active : colors::incoming});

    // pre-create textures once
    for (Word &w : words)
        generate_word_texture(ren, font, w);

    return words;
}

bool generate_word_texture(SDL_Renderer* ren, TTF_Font* font, Word& w) {
    if (w.tex) return true;
    SDL_Surface* surf = TTF_RenderText_Blended(font, w.text.c_str(), 0, w.color);
    if (!surf) return false;
    w.tex = SDL_CreateTextureFromSurface(ren, surf);
    w.w = surf->w; w.h = surf->h;
    SDL_DestroySurface(surf);
    return w.tex != nullptr;
}

static bool is_same_color(const SDL_Color& a, const SDL_Color& b){
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

void set_word_color(Word &word, SDL_Color c){
    if (is_same_color(word.color, c)) return;

    word.color = c;
    if (word.tex){
        SDL_DestroyTexture(word.tex);
        word.tex = nullptr;
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
