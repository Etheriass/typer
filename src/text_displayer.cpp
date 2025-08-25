#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

#include "word_run.h"
#include "colors.h"

#define FONT_PATH "../assets/UbuntuMono-R.ttf"

TTF_Font *get_font()
{
    TTF_Font *font = TTF_OpenFont("../assets/UbuntuMono-R.ttf", 24);
    if (!font)
    {
        SDL_Log("Failed to load font: %s", SDL_GetError());
    }
    return font;
}

std::vector<WordRun> init_text(std::vector<std::string>& words, SDL_Renderer *ren, TTF_Font *font)
{
    std::vector<WordRun> runs; // one per word, cached textures

    runs.clear();
    runs.reserve(words.size());
    for (size_t i = 0; i < words.size(); ++i)
        runs.push_back(WordRun{words[i], colors::incoming});

    // pre-create textures once
    for (auto &r : runs)
        ensure_word_texture(ren, font, r);

    return runs;
}

// void display_text(){

// }

void clear_text(std::vector<WordRun> runs, TTF_Font *font)
{
    for (auto &r : runs)
    {
        if (r.tex)
            SDL_DestroyTexture(r.tex);
    }
    runs.clear();
    TTF_CloseFont(font);
}


bool is_same_color(SDL_Color& a, SDL_Color& b){
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

void set_word_color(std::vector<WordRun>& runs, size_t i, SDL_Color c){
    if (i >= runs.size()) return;
    if (is_same_color(runs[i].color, c)) return;

    runs[i].color = c;
    if (runs[i].tex){
        SDL_DestroyTexture(runs[i].tex);
        runs[i].tex = nullptr;
    }
}

