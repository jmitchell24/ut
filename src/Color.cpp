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

// Convert color3 floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
template <typename T>
static void RGBtoHSV(T r, T g, T b, T& out_h, T& out_s, T& out_v)
{
    T K = T(0);
    if (g < b)
    {
        swap(g, b);
        K = -T(1);
    }
    if (r < g)
    {
        swap(r, g);
        K = -T(2) / T(6) - K;
    }

    const T chroma = r - (g < b ? g : b);
    out_h = fabs(K + (g - b) / (T(6) * chroma + T(1e-20)));
    out_s = chroma / (r + T(1e-20));
    out_v = r;
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to color3 floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
template <typename T>
void HSVtoRGB(T h, T s, T v, T& out_r, T& out_g, T& out_b)
{
    if (s == T(0.0))
    {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = fmod(h, T(1.0)) / (T(60.0) / T(360.0));
    int   i = (int)h;
    T f = h - (float)i;
    T p = v * (T(1.0) - s);
    T q = v * (T(1.0) - s * f);
    T t = v * (T(1.0) - s * (T(1.0) - f));

    switch (i)
    {
        case 0: out_r = v; out_g = t; out_b = p; break;
        case 1: out_r = q; out_g = v; out_b = p; break;
        case 2: out_r = p; out_g = v; out_b = t; break;
        case 3: out_r = p; out_g = q; out_b = v; break;
        case 4: out_r = t; out_g = p; out_b = v; break;
        case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}

//
// color3
//

color4 color3::rgba(uint8_t a) const { return {r, g, b, a}; }

vec3  color3::normal () const { return {real_t(r) / 255, real_t(g) / 255, real_t(b) / 255 }; }
vec3f color3::normalf() const { return {float (r) / 255, float (g) / 255, float (b) / 255 }; }
vec3d color3::normald() const { return {double(r) / 255, double(g) / 255, double(b) / 255 }; }

vec3  color3::hsv () const { vec3  c, n = normal (); RGBtoHSV(n.x, n.y, n.z, c.x, c.y, c.z); return c; }
vec3f color3::hsvf() const { vec3f c, n = normalf(); RGBtoHSV(n.x, n.y, n.z, c.x, c.y, c.z); return c; }
vec3d color3::hsvd() const { vec3d c, n = normald(); RGBtoHSV(n.x, n.y, n.z, c.x, c.y, c.z); return c; }


color3 color3::fromNormal (vec3  v) { return { uint8_t(v.x * 255), uint8_t(v.y * 255), uint8_t(v.z * 255)  }; }
color3 color3::fromNormalf(vec3f v) { return { uint8_t(v.x * 255), uint8_t(v.y * 255), uint8_t(v.z * 255)  }; }
color3 color3::fromNormald(vec3d v) { return { uint8_t(v.x * 255), uint8_t(v.y * 255), uint8_t(v.z * 255)  }; }

color3 color3::fromHSV (vec3  v) { vec3  c; HSVtoRGB(v.x, v.y, v.z, c.x, c.y, c.z); return fromNormal (c); }
color3 color3::fromHSVf(vec3f v) { vec3f c; HSVtoRGB(v.x, v.y, v.z, c.x, c.y, c.z); return fromNormalf(c); }
color3 color3::fromHSVd(vec3d v) { vec3d c; HSVtoRGB(v.x, v.y, v.z, c.x, c.y, c.z); return fromNormald(c); }

color3 color3::parse(const char *s) { color3 c; try_parse(s, c.r, c.g, c.b); return c; }
bool color3::tryParse(const char *s, color3& c) { return try_parse(s, c.r, c.g, c.b); }

//
// color4
//

color3 color4::rgb() const { return {r, g, b }; }

vec4  color4::normal () const { return {real_t(r) / 255, real_t(g) / 255, real_t(b) / 255, real_t(a) / 255 }; }
vec4f color4::normalf() const { return {float (r) / 255, float (g) / 255, float (b) / 255, float (a) / 255 }; }
vec4d color4::normald() const { return {double(r) / 255, double(g) / 255, double(b) / 255, double(a) / 255 }; }

vec4  color4::hsv () const { vec4  c, n = normal (); RGBtoHSV(n.x, n.y, n.z, c.x, c.y, c.z); c.w = n.w; return c; }
vec4f color4::hsvf() const { vec4f c, n = normalf(); RGBtoHSV(n.x, n.y, n.z, c.x, c.y, c.z); c.w = n.w; return c; }
vec4d color4::hsvd() const { vec4d c, n = normald(); RGBtoHSV(n.x, n.y, n.z, c.x, c.y, c.z); c.w = n.w; return c; }

color4 color4::fromNormal (vec4  v) { return { uint8_t(v.x * 255), uint8_t(v.y * 255), uint8_t(v.z * 255), uint8_t(v.w * 255)  }; }
color4 color4::fromNormalf(vec4f v) { return { uint8_t(v.x * 255), uint8_t(v.y * 255), uint8_t(v.z * 255), uint8_t(v.w * 255)  }; }
color4 color4::fromNormald(vec4d v) { return { uint8_t(v.x * 255), uint8_t(v.y * 255), uint8_t(v.z * 255), uint8_t(v.w * 255)  }; }

color4 color4::fromHSV (vec3  v, real_t a) { vec4  c; HSVtoRGB(v.x, v.y, v.z, c.x, c.y, c.z); c.w = a; return fromNormal (c); }
color4 color4::fromHSVf(vec3f v, float  a) { vec4f c; HSVtoRGB(v.x, v.y, v.z, c.x, c.y, c.z); c.w = a; return fromNormalf(c); }
color4 color4::fromHSVd(vec3d v, double a) { vec4d c; HSVtoRGB(v.x, v.y, v.z, c.x, c.y, c.z); c.w = a; return fromNormald(c); }

color4 color4::parseRGBA(const char *s) { color4 c; try_parse(s, c.r, c.g, c.b, c.a); return c; }
bool color4::tryParseRGBA(const char *s, color4& c) { return try_parse(s, c.r, c.g, c.b, c.a); }

color4 color4::parseARGB(const char *s) { color4 c; try_parse(s, c.a, c.r, c.g, c.b); return c; }
bool color4::tryParseARGB(const char *s, color4& c) { return try_parse(s, c.a, c.r, c.g, c.b); }