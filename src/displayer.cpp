#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

#include "word.h"

// void display_text(SDL_Renderer* ren, TTF_Font* font, const std::string s){

// }

void clear_words(std::vector<Word> words)
{
    for (auto &w : words)
    {
        if (w.tex)
            SDL_DestroyTexture(w.tex);
    }
    words.clear();
}



