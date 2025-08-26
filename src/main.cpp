#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>

#include "word.h"
#include "vocabulary.h"
#include "displayer.h"
#include "colors.h"
#include "stats.h"
#include "session.h"
#include "const.h"
#include "layout.h"

bool init(SDL_Window *&win, SDL_Renderer *&ren)
{
  return SDL_Init(SDL_INIT_VIDEO) &&
         SDL_CreateWindowAndRenderer("Typer", WINDOW_W, WINDOW_H, SDL_WINDOW_HIGH_PIXEL_DENSITY, &win, &ren) &&
         TTF_Init();
}

static inline void redraw_entry(SDL_Renderer *ren, TTF_Font *font, Word &entry)
{
  if (entry.tex)
  {
    SDL_DestroyTexture(entry.tex);
    entry.tex = nullptr;
  }
  generate_word_texture(ren, font, entry);
}

static bool handle_event(const SDL_Event &e, Session &s, std::vector<Word> &words, SDL_Renderer *ren, TTF_Font *font, bool &running)
{
  switch (e.type)
  {
  case SDL_EVENT_QUIT:
    running = false;
    return true;
  case SDL_EVENT_WINDOW_RESIZED:
  case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    return true;

  case SDL_EVENT_KEY_DOWN:
  {
    if (e.key.key == SDLK_ESCAPE)
    {
      running = false;
      return true;
    }

    if (s.finished)
      return false;

    if (e.key.key == SDLK_SPACE && s.index < words.size())
    {
      if (is_same_text(s.entry, words[s.index]))
      {
        s.stats.correct_words++;
        set_word_color(words[s.index], colors::correct);
      }
      else
      {
        s.stats.incorrect_words++;
        set_word_color(words[s.index], colors::incorrect);
      }

      s.entry.text.clear();
      redraw_entry(ren, font, s.entry);
      s.index++;
      if (s.index < words.size())
        set_word_color(words[s.index], colors::active);
      return true;
    }
    if (e.key.key == SDLK_BACKSPACE && !s.entry.text.empty())
    {
      s.entry.text.pop_back();
      redraw_entry(ren, font, s.entry);
      return true;
    }
    return false;
  }

  case SDL_EVENT_TEXT_INPUT:
  {
    if (s.finished || s.index >= words.size())
      return false;
    if (strcmp(e.text.text, " ") == 0)
      return false; // space handled in KEY_DOWN

    if (!s.started)
    {
      s.started = true;
      s.stats.start = std::chrono::steady_clock::now();
    }
    s.entry.text += e.text.text;
    redraw_entry(ren, font, s.entry);
    return true;
  }

  default:
    return false;
  }
}

void loop(SDL_Window *&win, SDL_Renderer *&ren, TTF_Font *font)
{
  bool running = true;
  SDL_StartTextInput(win); // enables SDL_EVENT_TEXT_INPUT

  Session session;
  session.init();

  std::vector<Word> words = create_words(session.vocabulary, ren, font);

  int longest_word_w = 0, word_h = 0, space_w = 0;
  TTF_GetStringSize(font, "fire-extinguisher", 0, &longest_word_w, &word_h);
  TTF_GetStringSize(font, " ", 0, &space_w, &word_h);

  bool dirty = true;

  while (running)
  {
    for (SDL_Event e; SDL_PollEvent(&e);)
      dirty |= handle_event(e, session, words, ren, font, running);

    if (session.index == words.size() && !session.finished)
    {
      session.stats.finish();
      session.finished = true;
      SDL_Log("Elapsed time: %.2f seconds", session.stats.elapsed.count());
      SDL_Log("Words per minute: %.2f", session.stats.wpm);
      SDL_Log("Accuracy: %.2f%%", session.stats.accuracy * 100);
    }

    if (dirty)
    {
      Layout L = compute_layout(ren, word_h, space_w, longest_word_w);
      render_frame(ren, font, session, words, L);
      dirty = false;
    }
  }

  SDL_StopTextInput(win);
  destroy_textures(session, words);
}

void quit(SDL_Window *win, SDL_Renderer *ren, TTF_Font *font, bool sdl_error)
{
  if (sdl_error)
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

  if (!SDL_SetWindowResizable(win, true))
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

  TTF_Font *font = TTF_OpenFont(FONT_PATH, WORD_FONT_PX * scale);
  if (!font)
  {
    quit(win, ren, nullptr, true);
    return 1;
  }

  loop(win, ren, font);

  quit(win, ren, font, false);
  return 0;
}