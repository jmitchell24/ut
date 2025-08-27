//
// Created by james on 29/03/22.
//

#include "ut/color.hpp"

extern "C"
{
#include "hsluv.c"
}

//
// std
//

#include <cmath>
using namespace std;

//
// ut
//
using namespace ut;

//
// oklch
//

// https://claude.ai/chat/eb9cdb22-825d-489e-8291-95ab38d5a072
// https://evilmartians.com/chronicles/oklch-in-css-why-quit-rgb-hsl

// Helper function to convert sRGB to linear RGB
float sRGB_to_linear(float c)
{
    if (c <= 0.04045f)
        return c / 12.92f;
    return powf((c + 0.055f) / 1.055f, 2.4f);
}

// Helper function to convert linear RGB to sRGB
float linear_to_sRGB(float c)
{
    if (c <= 0.0031308f)
        return c * 12.92f;
    return 1.055f * powf(c, 1.0f / 2.4f) - 0.055f;
}

// Helper function to convert linear RGB to OKLab
void linear_RGB_to_OKLab(float r, float g, float b, float* L, float* a, float* b_out)
{
    // Matrix multiplication for RGB to LMS
    float l = 0.4122214708f * r + 0.5363325363f * g + 0.0514459929f * b;
    float m = 0.2119034982f * r + 0.6806995451f * g + 0.1073969566f * b;
    float s = 0.0883024619f * r + 0.2817188376f * g + 0.6299787005f * b;

    // Cube root
    l = cbrtf(l);
    m = cbrtf(m);
    s = cbrtf(s);

    // Matrix multiplication for LMS to OKLab
    *L = 0.2104542553f * l + 0.7936177850f * m - 0.0040720468f * s;
    *a = 1.9779984951f * l - 2.4285922050f * m + 0.4505937099f * s;
    *b_out = 0.0259040371f * l + 0.7827717662f * m - 0.8086757660f * s;
}

// Helper function to convert OKLab to linear RGB
void OKLab_to_linear_RGB(float L, float a, float b_in, float* r, float* g, float* b)
{
    // Matrix multiplication for OKLab to LMS
    float l = L + 0.3963377774f * a + 0.2158037573f * b_in;
    float m = L - 0.1055613458f * a - 0.0638541728f * b_in;
    float s = L - 0.0894841775f * a - 1.2914855480f * b_in;

    // Cube
    l = l * l * l;
    m = m * m * m;
    s = s * s * s;

    // Matrix multiplication for LMS to RGB
    *r = +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
    *g = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
    *b = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;
}

void RGB_to_OKLCH(color c, color::oklch& out)
{
    // Convert 8-bit RGB to 0-1 range
    float rf = c.r / 255.0f;
    float gf = c.g / 255.0f;
    float bf = c.b / 255.0f;
    float af = c.a / 255.0f;

    // Convert sRGB to linear RGB
    rf = sRGB_to_linear(rf);
    gf = sRGB_to_linear(gf);
    bf = sRGB_to_linear(bf);

    // Convert to OKLab
    float lab_a, lab_b;
    linear_RGB_to_OKLab(rf, gf, bf, &out.l, &lab_a, &lab_b);

    // Convert OKLab to OKLCH
    out.c = sqrtf(lab_a * lab_a + lab_b * lab_b);
    out.h = atan2f(lab_b, lab_a) * 180.0f / M_PI;
    out.a = af;

    if (out.h < 0)
        out.h += 360.0f;
}

void OKLCH_to_RGB(color::oklch c, color& out)
{
    // Convert OKLCH to OKLab
    float h_rad = c.h * M_PI / 180.0f;
    float lab_a = c.c * cosf(h_rad);
    float lab_b = c.c * sinf(h_rad);

    // Convert OKLab to linear RGB
    float rf, gf, bf;
    OKLab_to_linear_RGB(c.l, lab_a, lab_b, &rf, &gf, &bf);

    // Convert linear RGB to sRGB
    rf = linear_to_sRGB(rf);
    gf = linear_to_sRGB(gf);
    bf = linear_to_sRGB(bf);

    // Clamp and convert to 8-bit
    rf = std::max(0.0f, std::min(1.0f, rf));
    gf = std::max(0.0f, std::min(1.0f, gf));
    bf = std::max(0.0f, std::min(1.0f, bf));
    float af = std::max(0.0f, std::min(1.0f, c.a));

    out.r = (int)roundf(rf * 255.0f);
    out.g = (int)roundf(gf * 255.0f);
    out.b = (int)roundf(bf * 255.0f);
    out.a = (int)roundf(af * 255.0f);
}

//
// color parsing
//

bool try_parse(char const* s, uint8_t& x1, uint8_t& x2, uint8_t& x3, uint8_t& x4)
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

bool try_parse(char const* s, uint8_t& x1, uint8_t& x2, uint8_t& x3)
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

string color::toFgEscCode() const
{
    return
        "\033[38;2;"
        + std::to_string(r)
        + ";"
        + std::to_string(g)
        + ";"
        + std::to_string(b)
        + "m";
}

string color::toBgEscCode() const
{
    return
        "\033[48;2;"
        + std::to_string(r)
        + ";"
        + std::to_string(g)
        + ";"
        + std::to_string(b)
        + "m";
}

color::oklch color::RGBtoOKLCH(color c)
{
    oklch tmp;
    RGB_to_OKLCH(c, tmp);
    return tmp;
}

color color::OKLCHtoRGB(oklch c)
{
    color tmp;
    OKLCH_to_RGB(c, tmp);
    return tmp;
}




