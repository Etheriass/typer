#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>

#include "text.h"
#include "vocabulary.h"
#include "displayer.h"
#include "theme.h"
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

void update_entry_texture(SDL_Renderer *ren, TTF_Font *font, Text &entry)
{
  generate_text_texture(ren, entry, font, entry.color);
}

static bool handle_event(const SDL_Event &e, Session &s, std::vector<Text> &words, Text &entry, SDL_Renderer *ren, TTF_Font *font, TTF_Font *font_active, ThemeState &theme_state, bool &running)
{
  switch (e.type)
  {
  case SDL_EVENT_QUIT:
    running = false;
    return true;

  case SDL_EVENT_KEY_DOWN:
  {
    if (e.key.key == SDLK_ESCAPE)
    {
      s.reset(false);
      words = generate_words(s.vocabulary, ren, font, theme_state.current);
      entry.text.clear();
      update_entry_texture(ren, font, entry);
      return true;
    }
    if (s.finished && e.key.key == SDLK_RETURN)
    {
      s.next();
      words = generate_words(s.vocabulary, ren, font, theme_state.current);
      entry.text.clear();
      update_entry_texture(ren, font, entry);
      return true;
    }

    if (s.finished)
      return false;

    if (!s.started && e.key.key == SDLK_UP && s.vocabulary.size() < MAX_WORD_COUNT)
    {
      s.vocabulary = load_vocabulary(s.vocab_path, s.vocabulary.size() * 1.5);
      words = generate_words(s.vocabulary, ren, font, theme_state.current);
      return true;
    }
    if (!s.started && e.key.key == SDLK_DOWN && s.vocabulary.size() > BASE_WORD_COUNT)
    {
      s.vocabulary = load_vocabulary(s.vocab_path, s.vocabulary.size() / 1.5);
      words = generate_words(s.vocabulary, ren, font, theme_state.current);
      return true;
    }

    if (e.key.key == SDLK_TAB)
    {
      theme_state.toggle();
      s.reset();
      words = generate_words(s.vocabulary, ren, font, theme_state.current);
      change_text_color(ren, font, entry, theme_state.current.word_active);
      update_entry_texture(ren, font, entry);
      return true;
    }

    if (e.key.key == SDLK_SPACE && s.index < words.size())
    {
      if (entry.text == words[s.index].text)
      {
        s.stats.correct_words++;
        change_text_color(ren, font, words[s.index], theme_state.current.word_correct);
      }
      else
      {
        s.stats.incorrect_words++;
        change_text_color(ren, font, words[s.index], theme_state.current.word_incorrect);
      }

      entry.text.clear();
      update_entry_texture(ren, font, entry);
      s.index++;
      if (s.index < words.size())
        change_text_color(ren, font_active, words[s.index], theme_state.current.word_active);
      return true;
    }
    if (e.key.key == SDLK_BACKSPACE && !entry.text.empty())
    {
      entry.text.pop_back();
      update_entry_texture(ren, font, entry);
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
    entry.text += e.text.text;
    update_entry_texture(ren, font, entry);
    return true;
  }

  default:
    return false;
  }
}

void loop(SDL_Window *&win, SDL_Renderer *&ren, TTF_Font *font, TTF_Font *font_active )
{
  bool running = true;
  SDL_StartTextInput(win); // enables SDL_EVENT_TEXT_INPUT
  ThemeState theme_state;

  Session session;
  session.init();

  Text entry{"", theme_state.current.word_active, font, ren};

  std::vector<Text> words = generate_words(session.vocabulary, ren, font, theme_state.current);

  int longest_word_w = 0, word_h = 0, space_w = 0;
  TTF_GetStringSize(font, "fire-extinguisher", 0, &longest_word_w, &word_h);
  TTF_GetStringSize(font, " ", 0, &space_w, &word_h);

  bool dirty = true;
  bool saw_resize = false;

  SDL_Event e;
  while (running)
  {
    while (SDL_WaitEventTimeout(&e, 5)) // wait for events with a timeout to reduce CPU usage
    {
      if (e.type == SDL_EVENT_WINDOW_RESIZED || e.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
      {
        saw_resize = true;
        continue;
      }
      dirty |= handle_event(e, session, words, entry, ren, font, font_active, theme_state, running);
    }

    if (saw_resize)
    {
      dirty = true;
      saw_resize = false;
    }

    if (session.index == words.size() && !session.finished)
      session.finish();

    if (dirty)
    {
      Layout L = compute_layout(ren, word_h, space_w, longest_word_w);
      render_frame(ren, font, session, words, entry, L, theme_state.current);
      dirty = false;
    }
  }

  SDL_StopTextInput(win);
  // destroy_textures(entry, words);
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

  if (!SDL_SetRenderVSync(ren, 1)) {
    SDL_Log("VSync request not honored: %s", SDL_GetError());
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

  std::string fontPath = FONT_PATH;
  if (const char *base = SDL_GetBasePath())
  { // absolute, cross-platform
    fontPath = std::string(base) + FONT_PATH;
  }

  TTF_Font *font = TTF_OpenFont(fontPath.c_str(), WORD_FONT_PX * scale);
  if (!font)
  {
    quit(win, ren, nullptr, true);
    return 1;
  }
  TTF_Font *font_active = TTF_OpenFont(fontPath.c_str(), (WORD_FONT_PX * scale) + 1);
  // else
  // {
  //   int dpi = int(16 * scale);
  //   TTF_SetFontSizeDPI(font, WORD_FONT_PX * scale, dpi, dpi);
  // }

  loop(win, ren, font, font_active);

  quit(win, ren, font, false);
  return 0;
}