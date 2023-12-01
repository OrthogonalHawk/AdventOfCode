#ifndef ADVENT_OF_CODE_CONVERSION_H
#define ADVENT_OF_CODE_CONVERSION_H

#include <cstdint>
#include <string>
#include <vector>

template <typename T>
std::vector<T> convert_string_vector_to_numbers(const std::vector<std::string>& strVec, const T& specialValue);

#endif /* ADVENT_OF_CODE_CONVERSION_H */