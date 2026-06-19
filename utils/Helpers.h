// Note: Comments are only for understanding—do not modify the code itself.
#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <algorithm>
#include <cctype>

inline std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

#endif // HELPERS_H
