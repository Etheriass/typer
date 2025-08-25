#ifndef STATS_H
#define STATS_H

#include <cstddef>
#include <chrono>

struct Stats
{
    size_t correct_words = 0;
    size_t incorrect_words = 0;
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point stop;
    std::chrono::duration<double> elapsed;
    float wpm;
    float accuracy;


    void reset() {
        correct_words = 0;
        incorrect_words = 0;
        start = {};
        stop = {};
        elapsed = {};
        wpm = 0.0f;
        accuracy = 0.0f;
    }

    void finish()
    {
        stop = std::chrono::system_clock::now();
        elapsed = stop - start;
        wpm = (correct_words + incorrect_words) / (elapsed.count() / 60.0f);
        accuracy = (float)correct_words / (float)(correct_words + incorrect_words);
    }

};

#endif
