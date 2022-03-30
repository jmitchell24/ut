//
// Created by james on 29/03/22.
//

#ifndef UT_COLOR_HPP
#define UT_COLOR_HPP

#include "math/Vector4D.hpp"
#include "math/Vector3D.hpp"

#include <ut/math/Vector3D.hpp>
#include <ut/math/Vector4D.hpp>
#include <ut/Bit.hpp>

#define UT_COLORS \
    COLOR(black,                0x000000ff) \
    COLOR(silver,               0xc0c0c0ff) \
    COLOR(gray,                 0x808080ff) \
    COLOR(white,                0xffffffff) \
    COLOR(maroon,               0x800000ff) \
    COLOR(red,                  0xff0000ff) \
    COLOR(purple,               0x800080ff) \
    COLOR(green,                0x008000ff) \
    COLOR(lime,                 0x00ff00ff) \
    COLOR(olive,                0x808000ff) \
    COLOR(yellow,               0xffff00ff) \
    COLOR(navy,                 0x000080ff) \
    COLOR(blue,                 0x0000ffff) \
    COLOR(teal,                 0x008080ff) \
    COLOR(aqua,                 0x00ffffff) \
    COLOR(orange,               0xffa500ff) \
    COLOR(aliceblue,            0xf0f8ffff) \
    COLOR(antiquewhite,         0xfaebd7ff) \
    COLOR(aquamarine,           0x7fffd4ff) \
    COLOR(azure,                0xf0ffffff) \
    COLOR(beige,                0xf5f5dcff) \
    COLOR(bisque,               0xffe4c4ff) \
    COLOR(blanchedalmond,       0xffebcdff) \
    COLOR(blueviolet,           0x8a2be2ff) \
    COLOR(brown,                0xa52a2aff) \
    COLOR(burlywood,            0xdeb887ff) \
    COLOR(cadetblue,            0x5f9ea0ff) \
    COLOR(chartreuse,           0x7fff00ff) \
    COLOR(chocolate,            0xd2691eff) \
    COLOR(coral,                0xff7f50ff) \
    COLOR(cornflowerblue,       0x6495edff) \
    COLOR(cornsilk,             0xfff8dcff) \
    COLOR(crimson,              0xdc143cff) \
    COLOR(cyan,                 0x00ffffff) \
    COLOR(darkblue,             0x00008bff) \
    COLOR(darkcyan,             0x008b8bff) \
    COLOR(darkgoldenrod,        0xb8860bff) \
    COLOR(darkgray,             0xa9a9a9ff) \
    COLOR(darkgreen,            0x006400ff) \
    COLOR(darkgrey,             0xa9a9a9ff) \
    COLOR(darkkhaki,            0xbdb76bff) \
    COLOR(darkmagenta,          0x8b008bff) \
    COLOR(darkolivegreen,       0x556b2fff) \
    COLOR(darkorange,           0xff8c00ff) \
    COLOR(darkorchid,           0x9932ccff) \
    COLOR(darkred,              0x8b0000ff) \
    COLOR(darksalmon,           0xe9967aff) \
    COLOR(darkseagreen,         0x8fbc8fff) \
    COLOR(darkslateblue,        0x483d8bff) \
    COLOR(darkslategray,        0x2f4f4fff) \
    COLOR(darkslategrey,        0x2f4f4fff) \
    COLOR(darkturquoise,        0x00ced1ff) \
    COLOR(darkviolet,           0x9400d3ff) \
    COLOR(deeppink,             0xff1493ff) \
    COLOR(deepskyblue,          0x00bfffff) \
    COLOR(dimgray,              0x696969ff) \
    COLOR(dimgrey,              0x696969ff) \
    COLOR(dodgerblue,           0x1e90ffff) \
    COLOR(firebrick,            0xb22222ff) \
    COLOR(floralwhite,          0xfffaf0ff) \
    COLOR(forestgreen,          0x228b22ff) \
    COLOR(gainsboro,            0xdcdcdcff) \
    COLOR(ghostwhite,           0xf8f8ffff) \
    COLOR(gold,                 0xffd700ff) \
    COLOR(goldenrod,            0xdaa520ff) \
    COLOR(greenyellow,          0xadff2fff) \
    COLOR(grey,                 0x808080ff) \
    COLOR(honeydew,             0xf0fff0ff) \
    COLOR(hotpink,              0xff69b4ff) \
    COLOR(indianred,            0xcd5c5cff) \
    COLOR(indigo,               0x4b0082ff) \
    COLOR(ivory,                0xfffff0ff) \
    COLOR(khaki,                0xf0e68cff) \
    COLOR(lavender,             0xe6e6faff) \
    COLOR(lavenderblush,        0xfff0f5ff) \
    COLOR(lawngreen,            0x7cfc00ff) \
    COLOR(lemonchiffon,         0xfffacdff) \
    COLOR(lightblue,            0xadd8e6ff) \
    COLOR(lightcoral,           0xf08080ff) \
    COLOR(lightcyan,            0xe0ffffff) \
    COLOR(lightgoldenrodyellow, 0xfafad2ff) \
    COLOR(lightgray,            0xd3d3d3ff) \
    COLOR(lightgreen,           0x90ee90ff) \
    COLOR(lightgrey,            0xd3d3d3ff) \
    COLOR(lightpink,            0xffb6c1ff) \
    COLOR(lightsalmon,          0xffa07aff) \
    COLOR(lightseagreen,        0x20b2aaff) \
    COLOR(lightskyblue,         0x87cefaff) \
    COLOR(lightslategray,       0x778899ff) \
    COLOR(lightslategrey,       0x778899ff) \
    COLOR(lightsteelblue,       0xb0c4deff) \
    COLOR(lightyellow,          0xffffe0ff) \
    COLOR(limegreen,            0x32cd32ff) \
    COLOR(linen,                0xfaf0e6ff) \
    COLOR(magenta,              0xff00ffff) \
    COLOR(fuchsia,              0xff00ffff) \
    COLOR(mediumaquamarine,     0x66cdaaff) \
    COLOR(mediumblue,           0x0000cdff) \
    COLOR(mediumorchid,         0xba55d3ff) \
    COLOR(mediumpurple,         0x9370dbff) \
    COLOR(mediumseagreen,       0x3cb371ff) \
    COLOR(mediumslateblue,      0x7b68eeff) \
    COLOR(mediumspringgreen,    0x00fa9aff) \
    COLOR(mediumturquoise,      0x48d1ccff) \
    COLOR(mediumvioletred,      0xc71585ff) \
    COLOR(midnightblue,         0x191970ff) \
    COLOR(mintcream,            0xf5fffaff) \
    COLOR(mistyrose,            0xffe4e1ff) \
    COLOR(moccasin,             0xffe4b5ff) \
    COLOR(navajowhite,          0xffdeadff) \
    COLOR(oldlace,              0xfdf5e6ff) \
    COLOR(olivedrab,            0x6b8e23ff) \
    COLOR(orangered,            0xff4500ff) \
    COLOR(orchid,               0xda70d6ff) \
    COLOR(palegoldenrod,        0xeee8aaff) \
    COLOR(palegreen,            0x98fb98ff) \
    COLOR(paleturquoise,        0xafeeeeff) \
    COLOR(palevioletred,        0xdb7093ff) \
    COLOR(papayawhip,           0xffefd5ff) \
    COLOR(peachpuff,            0xffdab9ff) \
    COLOR(peru,                 0xcd853fff) \
    COLOR(pink,                 0xffc0cbff) \
    COLOR(plum,                 0xdda0ddff) \
    COLOR(powderblue,           0xb0e0e6ff) \
    COLOR(rosybrown,            0xbc8f8fff) \
    COLOR(royalblue,            0x4169e1ff) \
    COLOR(saddlebrown,          0x8b4513ff) \
    COLOR(salmon,               0xfa8072ff) \
    COLOR(sandybrown,           0xf4a460ff) \
    COLOR(seagreen,             0x2e8b57ff) \
    COLOR(seashell,             0xfff5eeff) \
    COLOR(sienna,               0xa0522dff) \
    COLOR(skyblue,              0x87ceebff) \
    COLOR(slateblue,            0x6a5acdff) \
    COLOR(slategray,            0x708090ff) \
    COLOR(slategrey,            0x708090ff) \
    COLOR(snow,                 0xfffafaff) \
    COLOR(springgreen,          0x00ff7fff) \
    COLOR(steelblue,            0x4682b4ff) \
    COLOR(tan,                  0xd2b48cff) \
    COLOR(thistle,              0xd8bfd8ff) \
    COLOR(tomato,               0xff6347ff) \
    COLOR(turquoise,            0x40e0d0ff) \
    COLOR(violet,               0xee82eeff) \
    COLOR(wheat,                0xf5deb3ff) \
    COLOR(whitesmoke,           0xf5f5f5ff) \
    COLOR(yellowgreen,          0x9acd32ff) \
    COLOR(rebeccapurple,        0x663399ff)

namespace ut
{
    /// color (RGBA) class with standard functionality.
    /// Stores color components as a 32-bit integer, with little endian byte order
    union color
    {
        struct normal
        {
            real_t r,g,b,a;
            inline explicit operator vec4() const
            { return { r,g,b,a }; }
        };

        struct hsv
        {
            real_t h,s,v,a;

            inline hsv withH(real_t x) { return {x, s, v, a}; }
            inline hsv withS(real_t x) { return {h, x, v, a}; }
            inline hsv withV(real_t x) { return {h, s, x, a}; }
            inline hsv withA(real_t x) { return {h, s, v, x}; }

            inline explicit operator vec4() const
            { return { h,s,v,a }; }
        };

        b32 i;
        struct { b8 a,b,g,r; };

        inline constexpr color()
            : i{0}
        {}

        inline constexpr color(b8 r, b8 g, b8 b, b8 a = 255)
            : r{r}, g{g}, b{b}, a{a}
        {}

        inline explicit constexpr color(b32 i)
            : i{i}
        {}

        inline explicit constexpr color(vec4b const& v)
            : r{v.x}, g{v.y}, b{v.z}, a{v.w}
        {}

        inline explicit constexpr color(normal const& n)
            : color{NORMALtoRGB(n)}
        {}

        inline explicit constexpr color(hsv const& h)
            : color{NORMALtoRGB(HSVtoNORMAL(h))}
        {}

        inline explicit constexpr operator vec4b () const { return {r,g,b,a}; }
        inline explicit constexpr operator normal() const { return RGBtoNORMAL(*this); }
        inline explicit constexpr operator hsv   () const { return NORMALtoHSV(RGBtoNORMAL(*this)); }

        inline constexpr bool operator== (color const& c) const { return i == c.i; }
        inline constexpr bool operator!= (color const& c) const { return !(*this == c); }
        inline constexpr bool operator<  (color const& c) const { return i < c.i; }
        inline constexpr bool operator>  (color const& c) const { return *this < c; }
        inline constexpr bool operator<= (color const& c) const { return !(*this < c); }
        inline constexpr bool operator>= (color const& c) const { return !(c < *this); }

        static color parseRGBA(char const* s);
        static bool tryParseRGBA(char const* s, color& c);

        static color parseARGB(char const* s);
        static bool tryParseARGB(char const* s, color& c);

    private:
        static constexpr color::normal RGBtoNORMAL(color c)
        {
            return {real_t(c.r) / 255, real_t(c.g) / 255, real_t(c.b) / 255, real_t(c.a) / 255 };
        }

        static constexpr color NORMALtoRGB(normal n)
        {
            return { b8(n.r * 255), b8(n.g * 255), b8(n.b * 255), b8(n.a * 255)  };
        }

        // Convert color3 floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
        // Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
        static constexpr color::hsv NORMALtoHSV(normal c)
        {
            auto K = real_t(0);

            if (c.g < c.b)
            {
                std::swap(c.g, c.b);
                K = -real_t(1);
            }

            if (c.r < c.g)
            {
                std::swap(c.r, c.g);
                K = -real_t(2) / real_t(6) - K;
            }

            real_t const chroma = c.r - (c.g < c.b ? c.g : c.b);

            return
            {
            std::fabs(K + (c.g - c.b) / (real_t(6) * chroma + real_t(1e-20))),
            chroma / (c.r + real_t(1e-20)),
            c.r,
            c.a
            };
        }

        // Convert hsv floats ([0-1],[0-1],[0-1]) to color3 floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
        // also http://en.wikipedia.org/wiki/HSL_and_HSV
        static constexpr normal HSVtoNORMAL(hsv c)
        {
            if (c.s == real_t(0.0))
            {
                return { c.v, c.v, c.v, c.a };
            }

            c.h = std::fmod(c.h, real_t(1.0)) / (real_t(60.0) / real_t(360.0));
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
    };

#if defined(UT_STL_INTEROP)
    inline std::ostream& operator<<(std::ostream& os, color const& c)
    { return os << (vec4b)c; }

    inline std::ostream& operator<<(std::ostream& os, color::normal const& c)
    { return os << (vec4)c; }

    inline std::ostream& operator<<(std::ostream& os, color::hsv const& c)
    { return os << (vec4)c; }
#endif

namespace colors
{
#define COLOR(_name, _i) static constexpr color _name{_i};
    UT_COLORS
#undef COLOR
}
}


#endif //UT_COLOR_HPP
