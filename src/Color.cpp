//
// Created by james on 29/03/22.
//

#include "ut/Color.hpp"

#include <cmath>
using namespace std;

using namespace ut;

inline bool try_parse(char const* s, uint8_t& x1, uint8_t& x2, uint8_t& x3, uint8_t& x4)
{
    if (s == nullptr)
        return false;

    char const* fmt = s[0] == '#' ? "#%02X%02X%02X%02X" : "%02X%02X%02X%02X";

    unsigned t[4];
    if (auto res = sscanf(s, fmt, t+0, t+1, t+2, t+3); res == 4)
    {
        x1 = t[0]; x2 = t[1]; x3 = t[2]; x4 = t[3];
        return true;
    }
    return false;
}

inline bool try_parse(char const* s, uint8_t& x1, uint8_t& x2, uint8_t& x3)
{
    if (s == nullptr)
        return false;

    char const* fmt = s[0] == '#' ? "#%02X%02X%02X" : "%02X%02X%02X";

    unsigned t[3];
    if (auto res = sscanf(s, fmt, t+0, t+1, t+2); res == 4)
    {
        x1 = t[0]; x2 = t[1]; x3 = t[2];
        return true;
    }
    return false;
}

//
// color
//

color color::parseRGBA(char const* s) { color c; try_parse(s, c.r, c.g, c.b, c.a); return c; }
bool color::tryParseRGBA(char const* s, color& c) { return try_parse(s, c.r, c.g, c.b, c.a); }

color color::parseARGB(char const* s) { color c; try_parse(s, c.a, c.r, c.g, c.b); return c; }
bool color::tryParseARGB(char const* s, color& c) { return try_parse(s, c.a, c.r, c.g, c.b); }



