#ifndef SESSION_H
#define SESSION_H

#include <vector>

#include "word.h"
#include "stats.h"
#include "vocabulary.h"
#include "colors.h"
#include "const.h"

struct Session {
    std::vector<std::string> vocabulary;
    Word entry;
    Stats stats;
    size_t id;
    size_t index;
    bool started;
    bool finished;

    void init(){
        vocabulary = load_vocabulary(VOCABULARY_PATH, WORD_COUNT);
        entry = Word{"", colors::active};
        stats = Stats{0, 0};
        id = 0;
        index = 0;
        started = false;
        finished = false;
    }

    void next(){
        vocabulary = load_vocabulary(VOCABULARY_PATH, WORD_COUNT);
        entry = Word{"", colors::active};
        stats = Stats{0, 0};
        id++;
        index = 0;
        started = false;
        finished = false;
    }
};



#endif
