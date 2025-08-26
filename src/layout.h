#ifndef LAYOUT_H
#define LAYOUT_H

#include <SDL3/SDL.h>

struct Layout
{
    int W = 0, H = 0;
    int word_h = 0, space_w = 0, longest_w = 0;
    float x_words = 0.f, y_words = 0.f, max_w = 0.f;
    float x_entry = 0.f, y_entry = 0.f;
};


inline Layout compute_layout(SDL_Renderer *ren, int word_h, int space_w, int longest_w) {
  Layout L;
  SDL_GetRenderOutputSize(ren, &L.W, &L.H);
  L.word_h = word_h; L.space_w = space_w; L.longest_w = longest_w;
  L.x_words = 0.10f * L.W;  L.y_words = 0.15f * L.H;
  L.max_w   = (float)L.W - 2.f * L.x_words;               // symmetric margins
  L.x_entry = (L.W*0.5f - longest_w * 0.5f); L.y_entry = (L.H * 0.5f);
  return L;
}

#endif