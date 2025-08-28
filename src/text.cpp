#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

#include "text.h"
#include "theme.h"
#include "displayer.h"

std::vector<Text> generate_words(const std::vector<std::string> &sample, SDL_Renderer *ren, TTF_Font *font, const Theme &theme)
{
    std::vector<Text> words;
    words.reserve(sample.size());
    for (size_t i = 0; i < sample.size(); ++i){
        Text t{sample[i], i == 0 ? theme.word_active : theme.word_incoming, font, ren};
        words.push_back(t);
    }
    return words;
}

void change_text_color(SDL_Renderer *ren, TTF_Font *font, Text &text, SDL_Color c){
    if (text.color.r == c.r && text.color.g == c.g && text.color.b == c.b && text.color.a == c.a) return;
    text.color = c;
    generate_text_texture(ren, text, font, c);
}
