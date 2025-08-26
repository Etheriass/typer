#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <string>

std::vector<std::string> load_text(const std::string& path, size_t size, uint64_t seed = 0);

#endif