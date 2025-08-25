#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>

#include "word_run.h"
#include "text_displayer.h"
#include "colors.h"


bool init(SDL_Window*& win, SDL_Renderer*& ren) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return false;
  }
  if (!SDL_CreateWindowAndRenderer("Typer", 700, 300, 0, &win, &ren)){
    SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
    return false;
  }
  if (!TTF_Init()) return false;
  return true;
}



void loop(SDL_Window*& win, SDL_Renderer*& ren, std::vector<WordRun> runs, TTF_Font * font) {
  SDL_StartTextInput(win); // enables SDL_EVENT_TEXT_INPUT
  bool running = true;
  std::string buffer;
  std::string space = " ";
  size_t current_index = 0;
  set_word_color(runs, current_index, colors::active);

  WordRun current_word = WordRun{"", colors::active};
  ensure_word_texture(ren, font, current_word);


  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) running = false;
      else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) running = false;
      else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE && current_index < runs.size()) {
        if (buffer.compare(runs[current_index].text) == 0)
            set_word_color(runs, current_index, colors::good);
        else
            set_word_color(runs, current_index, colors::bad);

        buffer.clear();
        current_index++;
        set_word_color(runs, current_index, colors::active);
        SDL_DestroyTexture(current_word.tex);
        current_word.tex = nullptr;
        current_word.text = buffer;
        ensure_word_texture(ren, font, current_word);
      }
      else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_BACKSPACE && !buffer.empty()) {
        buffer.pop_back();
        SDL_DestroyTexture(current_word.tex);
        current_word.tex = nullptr;
        current_word.text = buffer;
        ensure_word_texture(ren, font, current_word);
    }
      else if (e.type == SDL_EVENT_TEXT_INPUT) {
        if (space.compare(e.text.text) != 0) {
            buffer += e.text.text; // UTF-8 characters
            SDL_DestroyTexture(current_word.tex);
            current_word.tex = nullptr;
            current_word.text = buffer;
            ensure_word_texture(ren, font, current_word);
            }
        } 
    }

    // draw
    int W,H; SDL_GetRenderOutputSize(ren, &W, &H);
    SDL_SetRenderDrawColor(ren, 18,18,18,255);
    SDL_RenderClear(ren);

    // translucent overlay example
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_FRect overlay{0,0,(float)W,(float)H};
    SDL_SetRenderDrawColor(ren, 18,18,18,100);
    SDL_RenderFillRect(ren, &overlay);

    

    draw_colored_words(ren, font, runs, 100.f, 120.f, (float)W - 40.f);

    float x = 150, y = 250;
    // SDL_FRect word_box{x,y,100,30};
    // SDL_SetRenderDrawColor(ren, 50,50,50,200);
    // SDL_RenderFillRect(ren, &word_box);
    SDL_FRect dst{x, y, (float)current_word.w, (float)current_word.h};
    SDL_RenderTexture(ren, current_word.tex, nullptr, &dst);

    SDL_RenderPresent(ren);
  }
  SDL_StopTextInput(win);
}



int main(int argc, char** argv){
    SDL_Window*   win = nullptr;
    SDL_Renderer* ren = nullptr;

    if (!init(win, ren)) {
        SDL_Log("Initialization failed");
        return 1;
    }

    std::vector<std::string> words = {"hello", "world", "this", "is", "a", "test"};

    TTF_Font * font = get_font();
    std::vector<WordRun> runs = init_text(words, ren, font);

    loop(win, ren, runs, font);

    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}