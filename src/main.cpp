#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>

#include "word.h"
#include "text.h"
#include "displayer.h"
#include "colors.h"
#include "stats.h"

static constexpr int WINDOW_W = 1200;
static constexpr int WINDOW_H = 700;
static constexpr int DURATION_SECONDS = 60;
static constexpr size_t WORD_COUNT = 20;
static constexpr int BASE_FONT_PX = 28;
static constexpr int WORD_FONT_PX = 30;

static const char *FONT_PATH = "../assets/UbuntuMono-R.ttf";
static const char *TEXT_PATH = "../assets/words.txt";

bool init(SDL_Window *&win, SDL_Renderer *&ren)
{
  return SDL_Init(SDL_INIT_VIDEO) &&
         SDL_CreateWindowAndRenderer("Typer", WINDOW_W, WINDOW_H, SDL_WINDOW_HIGH_PIXEL_DENSITY, &win, &ren) &&
         TTF_Init();
}

void loop(SDL_Window *&win, SDL_Renderer *&ren, std::vector<Word> &words, TTF_Font *font, float scale)
{
  SDL_StartTextInput(win); // enables SDL_EVENT_TEXT_INPUT

  int longest_word_w = 0, word_h = 0, space_w = 0;
  TTF_GetStringSize(font, "fire-extinguisher", 0, &longest_word_w, &word_h);
  TTF_GetStringSize(font, " ", 0, &space_w, &word_h);


  bool running = true;
  size_t current_index = 0;
  set_word_color(words, current_index, colors::active);

  Word entry = Word{"", colors::active};
  create_word_texture(ren, font, entry);

  Stats stats = Stats{0, 0};
  bool started = false;
  bool finished = false;

  while (running)
  {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_EVENT_QUIT)
        running = false;
      else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
        running = false;
      else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE && current_index < words.size())
      {
        if (is_same_text(entry, words[current_index]))
        {
          stats.correct_words++;
          set_word_color(words, current_index, colors::correct);
        }
        else
        {
          stats.incorrect_words++;
          set_word_color(words, current_index, colors::incorrect);
        }

        SDL_DestroyTexture(entry.tex);
        entry.tex = nullptr;
        entry.text.clear();
        current_index++;
        if (current_index < words.size())
          set_word_color(words, current_index, colors::active);
      }
      else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_BACKSPACE && !entry.text.empty())
      {
        entry.text.pop_back();
        SDL_DestroyTexture(entry.tex);
        entry.tex = nullptr;
        create_word_texture(ren, font, entry);
      }
      else if (e.type == SDL_EVENT_TEXT_INPUT)
      {
        if (strcmp(e.text.text, " ") != 0)
        {
          if (!started)
          {
            started = true;
            stats.start = std::chrono::system_clock::now();
          }
          entry.text += e.text.text;
          SDL_DestroyTexture(entry.tex);
          entry.tex = nullptr;
          create_word_texture(ren, font, entry);
        }
      }
    }

    if (current_index == words.size() && !finished)
    {
      stats.finish();
      finished = true;
      SDL_Log("Elapsed time: %.2f seconds", stats.elapsed.count());
      SDL_Log("Words per minute: %.2f", stats.wpm);
      SDL_Log("Accuracy: %.2f%%", stats.accuracy * 100);
    }

    // draw
    int W, H;
    SDL_GetRenderOutputSize(ren, &W, &H);
    SDL_SetRenderDrawColor(ren, 18, 18, 18, 255);
    SDL_RenderClear(ren);

    // translucent overlay example
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_FRect overlay{0, 0, (float)W, (float)H};
    SDL_SetRenderDrawColor(ren, 18, 18, 18, 100);
    SDL_RenderFillRect(ren, &overlay);

    float x_words = 0.1 * W, y_words = 0.15 * H;
    display_words(ren, font, words, x_words, y_words, (float)W - x_words, space_w, word_h);

    float x_entry = (W / 2 - longest_word_w / 2), y_entry = (H / 2);
    SDL_FRect entry_line{x_entry - 10, y_entry + word_h + 10, longest_word_w + 20.0f, 10};
    SDL_SetRenderDrawColor(ren, 250, 250, 250, 255);
    SDL_RenderFillRect(ren, &entry_line);
    SDL_FRect dst{x_entry, y_entry, (float)entry.w, (float)entry.h};
    SDL_RenderTexture(ren, entry.tex, nullptr, &dst);

    SDL_RenderPresent(ren);
  }
  SDL_StopTextInput(win);
}

void quit(SDL_Window *win, SDL_Renderer *ren, TTF_Font *font, bool error)
{
  if (error)
    SDL_Log("An error occurred: %s", SDL_GetError());
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}

int main(int argc, char **argv)
{
  SDL_Window *win = nullptr;
  SDL_Renderer *ren = nullptr;

  if (!init(win, ren))
  {
    quit(win, ren, nullptr, true);
    return 1;
  }

  float scale = SDL_GetWindowDisplayScale(win);
  if (scale == 0.0f)
  {
    SDL_Log("Error getting window display scale: %s", SDL_GetError());
    scale = 1.0f;
  }

  std::vector<std::string> text = load_text(TEXT_PATH, WORD_COUNT);
  if (text.empty())
  {
    SDL_Log("Failed to load text from %s", TEXT_PATH);
    quit(win, ren, nullptr, false);
  }

  TTF_Font *font = TTF_OpenFont(FONT_PATH, WORD_FONT_PX * scale);
  if (!font)
  {
    SDL_Log("Failed to load font: %s", SDL_GetError());
    quit(win, ren, nullptr, false);
  }

  std::vector<Word> words = create_words(text, ren, font);

  loop(win, ren, words, font, scale);

  quit(win, ren, font, false);
  return 0;
}