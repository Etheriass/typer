#include <cstdint>
#include <fstream>
#include <vector>
#include <string>
#include <random>

std::vector<std::string> load_vocabulary(const std::string& path, size_t size, uint64_t seed = 0) {
    std::ifstream in(path);
    std::vector<std::string> vocabulary;
    std::string s;
    if (!in) return {
        "Error: Couldn't open vocabulary file"
    };
    while (in >> s) {
        if (!s.empty())
            vocabulary.push_back(s);
    }

    if (vocabulary.empty()) return {}; // no words to sample

    std::vector<std::string> sample;
    sample.reserve(size);
    std::mt19937_64 rng(seed ? seed : std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, vocabulary.size() - 1);
    for (size_t i = 0; i < size; ++i)
        sample.push_back(vocabulary[dist(rng)]);

    return sample;
}

