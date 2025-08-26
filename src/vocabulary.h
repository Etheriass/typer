#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <vector>
#include <string>

std::vector<std::string> load_vocabulary(const std::string& path, size_t size, uint64_t seed = 0);

#endif