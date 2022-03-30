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

color color::parseRGBA(const char *s) { color c; try_parse(s, c.r, c.g, c.b, c.a); return c; }
bool color::tryParseRGBA(const char *s, color& c) { return try_parse(s, c.r, c.g, c.b, c.a); }

color color::parseARGB(char const* s) { color c; try_parse(s, c.a, c.r, c.g, c.b); return c; }
bool color::tryParseARGB(char const* s, color& c) { return try_parse(s, c.a, c.r, c.g, c.b); }

color::normal color::RGBtoNORMAL(color c)
{
    return {real_t(c.r) / 255, real_t(c.g) / 255, real_t(c.b) / 255, real_t(c.a) / 255 };
}

color color::NORMALtoRGB(normal n)
{
    return { b8(n.r * 255), b8(n.g * 255), b8(n.b * 255), b8(n.a * 255)  };
}

// Convert color3 floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
color::hsv color::NORMALtoHSV(normal c)
{
    auto K = real_t(0);

    if (c.g < c.b)
    {
        swap(c.g, c.b);
        K = -real_t(1);
    }

    if (c.r < c.g)
    {
        swap(c.r, c.g);
        K = -real_t(2) / real_t(6) - K;
    }

    real_t const chroma = c.r - (c.g < c.b ? c.g : c.b);

    return
    {
    fabs(K + (c.g - c.b) / (real_t(6) * chroma + real_t(1e-20))),
    chroma / (c.r + real_t(1e-20)),
    c.r,
    c.a
    };
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to color3 floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
color::normal color::HSVtoNORMAL(hsv c)
{
    if (c.s == real_t(0.0))
    {
        return { c.v, c.v, c.v, c.a };
    }

    c.h = fmod(c.h, real_t(1.0)) / (real_t(60.0) / real_t(360.0));
    int    i = (int)c.h;
    real_t f = c.h - (float)i;
    real_t p = c.v * (real_t(1.0) - c.s);
    real_t q = c.v * (real_t(1.0) - c.s * f);
    real_t t = c.v * (real_t(1.0) - c.s * (real_t(1.0) - f));

    switch (i)
    {
        case 0: return { c.v, t  , p  , c.a };
        case 1: return { q  , c.v, p  , c.a };
        case 2: return { p  , c.v, t  , c.a };
        case 3: return { p  , q  , c.v, c.a };
        case 4: return { t  , p  , c.v, c.a };
        case 5: default: return { c.v, p, q, c.a };
    }
}