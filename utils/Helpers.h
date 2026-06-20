// utils/Helpers.h
// Purpose: Header file containing simple utility string conversion helpers.

#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <algorithm>
#include <cctype>

/**
 * @brief Transforms a string to lowercase to support case-insensitive lookups/searches.
 * @param s Input string.
 * @return Transformed lowercase copy of the string.
 */
inline std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

#endif // HELPERS_H
