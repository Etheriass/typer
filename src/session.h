#ifndef SESSION_H
#define SESSION_H

#include <SDL3/SDL_filesystem.h>
#include <vector>

#include "text.h"
#include "stats.h"
#include "vocabulary.h"
#include "theme.h"
#include "const.h"

struct Session
{
    std::vector<std::string> vocabulary;
    std::string vocab_path;
    Stats stats;
    size_t id;
    size_t index;
    bool started;
    bool finished;

    void init()
    {
        vocab_path = std::string(SDL_GetBasePath()) + VOCABULARY_PATH;
        vocabulary = load_vocabulary(vocab_path, BASE_WORD_COUNT);
        stats = Stats{0, 0};
        id = 0;
        index = 0;
        started = false;
        finished = false;
    }

    void reset()
    {
        stats = Stats{0, 0};
        index = 0;
        started = false;
        finished = false;
    }

    void finish()
    {
        if (!finished)
        {
            stats.finish();
            finished = true;
        }
    }

    void next()
    {
        vocabulary = load_vocabulary(vocab_path, BASE_WORD_COUNT);
        stats = Stats{0, 0};
        id++;
        index = 0;
        started = false;
        finished = false;
    }
};

#endif
