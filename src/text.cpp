#include <fstream>
#include <vector>
#include <string>
#include <random>

std::vector<std::string> load_text(const std::string& path, size_t size, u_int64_t seed = 0) {
    std::ifstream in(path);
    std::vector<std::string> text;
    std::string s;
    while (in >> s) {
        if (!s.empty())
            text.push_back(s);
    }
    std::vector<std::string> sample;
    sample.reserve(size);
    std::mt19937_64 rng(seed ? seed : std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, text.size() - 1);
    for (size_t i = 0; i < size; ++i)
        sample.push_back(text[dist(rng)]);

    return sample;
}

