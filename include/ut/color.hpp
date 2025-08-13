//
// Created by james on 29/03/22.
//

#pragma once

#include <ut/math/vector3D.hpp>
#include <ut/math/vector4D.hpp>
#include <ut/bit.hpp>

#include <cstdio>

/// COLOR( NAME , HEXCODE )
#define UT_EXPAND_COLORS(COLOR) \
    COLOR(transparent,          0x00000000) \
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

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define CLAMP(x_, min_, max_) ( (x_) < (min_) ? (min_) : (x_) > (max_) ? (max_) : (x_) )
#define NCLAMPED(x_, min_, max_) ( (x_) < (min_) || (x_) > (max_) )

namespace ut
{
    union color;

    constexpr bool less(color const& a, color const& b);
    constexpr bool same(color const& a, color const& b);

    /// color (RGBA) class with standard functionality.
    /// Stores color components as a 32-bit integer, with little endian byte order
    union color
    {
        struct normal;
        struct hsv;
        struct hsluv;

        struct normal
        {
            real_t r,g,b,a;

            inline constexpr normal()
                : r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 }
            { }

            inline constexpr normal(real_t r, real_t g, real_t b, real_t a = 1)
                : r{ r }, g{ g }, b{ b }, a{ a }
            { }

            inline constexpr normal(vec4 const& v)
                : r{ v.x }, g{ v.y }, b{ v.z }, a{ v.w }
            { }

            inline constexpr normal(vec3 const& v, real_t a = 1)
                : r{ v.x }, g{ v.y }, b{ v.z }, a{ a }
            { }

            inline explicit normal(hsv const& hsv)
                : normal(HSVtoNORMAL(hsv))
            {}

            inline explicit normal(hsluv const& hsluv)
                : normal(HSLUVtoNORMAL(hsluv))
            {}

            inline explicit normal(color const& c)
                : normal(RGBtoNORMAL(c))
            {}

            constexpr normal(normal const&)=default;
            constexpr normal(normal&&) noexcept =default;

            constexpr normal& operator=(normal const&)=default;
            constexpr normal& operator=(normal&&) noexcept =default;

            [[nodiscard]] inline bool clamped() const
            {
                return !(
                    NCLAMPED(r, 0,1) ||
                    NCLAMPED(g, 0,1) ||
                    NCLAMPED(b, 0,1) ||
                    NCLAMPED(a, 0,1) );
            }

            inline void clamp()
            {
                r = CLAMP(r, 0,1);
                g = CLAMP(g, 0,1);
                b = CLAMP(b, 0,1);
                a = CLAMP(a, 0,1);
            }

            [[nodiscard]] inline operator vec4() const { return toVec4(); }
            [[nodiscard]] inline operator vec3() const { return toVec3(); }

            [[nodiscard]] inline explicit operator hsv  () const { return toHSV(); }
            [[nodiscard]] inline explicit operator hsluv() const { return toHSLUV(); }
            [[nodiscard]] inline explicit operator color() const { return toColor(); }

            [[nodiscard]] inline vec4  toVec4 () const { return { r,g,b,a }; }
            [[nodiscard]] inline vec3  toVec3 () const { return { r,g,b }; }
            [[nodiscard]] inline color toColor() const { return NORMALtoRGB  (*this); }
            [[nodiscard]] inline hsv   toHSV  () const { return NORMALtoHSV  (*this); }
            [[nodiscard]] inline hsluv toHSLUV() const { return NORMALtoHSLUV(*this); }
        };

        struct hsv
        {
            real_t h,s,v,a;

            inline constexpr hsv()
                : h{ 0 }, s{ 0 }, v{ 0 }, a{ 0 }
            {}

            inline constexpr hsv(real_t h, real_t s, real_t v, real_t a = 1)
                : h{ h }, s{ s }, v{ v }, a{ a }
            { }

            inline constexpr explicit hsv(vec4 const& v)
                : hsv(v.x, v.y, v.z, v.w)
            { }

            inline constexpr hsv(vec3 const& v, real_t a = 1)
                : hsv(v.x, v.y, v.z, a)
            { }

            inline explicit hsv(normal const& n)
                : hsv(NORMALtoHSV(n))
            { }

            inline explicit hsv(color const& c)
                : hsv(NORMALtoHSV(RGBtoNORMAL(c)))
            {}

            hsv(hsv const&)=default;
            hsv(hsv&&) noexcept =default;

            hsv& operator=(hsv const&)=default;
            hsv& operator=(hsv&&) noexcept =default;

            [[nodiscard]] inline bool clamped() const
            {
                return !(
                    NCLAMPED(h, 0,1) ||
                    NCLAMPED(s, 0,1) ||
                    NCLAMPED(v, 0,1) ||
                    NCLAMPED(a, 0,1) );
            }

            inline void clamp()
            {
                h = CLAMP(h, 0,1);
                s = CLAMP(s, 0,1);
                v = CLAMP(v, 0,1);
                a = CLAMP(a, 0,1);
            }

            [[nodiscard]] inline hsv withH(real_t x) const { x=CLAMP(x, 0,1); return {x, s, v, a}; }
            [[nodiscard]] inline hsv withS(real_t x) const { x=CLAMP(x, 0,1); return {h, x, v, a}; }
            [[nodiscard]] inline hsv withV(real_t x) const { x=CLAMP(x, 0,1); return {h, s, x, a}; }
            [[nodiscard]] inline hsv withA(real_t x) const { x=CLAMP(x, 0,1); return {h, s, v, x}; }

            [[nodiscard]] inline hsv invH() const { return {1.0f-h, s, v, a}; }
            [[nodiscard]] inline hsv invS() const { return {h, 1.0f-s, v, a}; }
            [[nodiscard]] inline hsv invV() const { return {h, s, 1.0f-v, a}; }
            [[nodiscard]] inline hsv invA() const { return {h, s, v, 1.0f-a}; }

            [[nodiscard]] inline hsv inv   () const { return {1.0f-h, 1.0f-s, 1.0f-v, 1.0f-a }; }
            [[nodiscard]] inline hsv invHSV() const { return {1.0f-h, 1.0f-s, 1.0f-v, a }; }
            [[nodiscard]] inline hsv opaque() const { return { h, s, v, 1.0f }; }

            [[nodiscard]] inline vec4   toVec4  () const { return { h,s,v,a }; }
            [[nodiscard]] inline vec3   toVec3  () const { return { h,s,v }; }
            [[nodiscard]] inline color  toColor () const { return NORMALtoRGB(HSVtoNORMAL(*this)); }
            [[nodiscard]] inline normal toNormal() const { return HSVtoNORMAL(*this); }
        };

        struct hsluv
        {
            /// hue [ 0, 360 ]
            real_t h;

            /// saturation [ 0, 100 ]
            real_t s;

            /// lightness [ 0, 100 ]
            real_t l;

            /// alpha [ 0, 1 ]
            real_t a;

            inline constexpr hsluv()
                : h{ 0 }, s{ 0 }, l{ 0 }, a{ 0 }
            {}

            inline constexpr hsluv(real_t h, real_t s, real_t l, real_t a = 1)
                : h{ h }, s{ s }, l{ l }, a{ a }
            { }

            inline explicit hsluv(vec3 const& v, real_t a = 1)
                : hsluv(v.x, v.y, v.z, a)
            {}

            inline explicit hsluv(normal const& n)
                : hsluv(NORMALtoHSLUV(n))
            {}

            inline explicit hsluv(color const& c)
                : hsluv(NORMALtoHSLUV(RGBtoNORMAL(c)))
            {}

            hsluv(hsluv const&)=default;
            hsluv(hsluv&&) noexcept =default;

            hsluv& operator=(hsluv const&)=default;
            hsluv& operator=(hsluv&&) noexcept =default;

            [[nodiscard]] inline bool clamped() const
            {
                return !(
                    NCLAMPED(h, 0,360) ||
                    NCLAMPED(s, 0,100) ||
                    NCLAMPED(l, 0,100) ||
                    NCLAMPED(a, 0,1) );
            }

            inline void clamp()
            {
                h = CLAMP(h, 0,360);
                s = CLAMP(s, 0,100);
                l = CLAMP(l, 0,100);
                a = CLAMP(a, 0,1);
            }

            [[nodiscard]] inline hsluv withH(real_t x) const { x = CLAMP(x, 0, 360); return {x, s, l, a}; }
            [[nodiscard]] inline hsluv withS(real_t x) const { x = CLAMP(x, 0, 100); return {h, x, l, a}; }
            [[nodiscard]] inline hsluv withL(real_t x) const { x = CLAMP(x, 0, 100); return {h, s, x, a}; }
            [[nodiscard]] inline hsluv withA(real_t x) const { x = CLAMP(x, 0, 1  ); return {h, s, l, x}; }

            [[nodiscard]] inline hsluv invH() const { return {360.0f-h, s, l, a}; }
            [[nodiscard]] inline hsluv invS() const { return {h, 100.0f-s, l, a}; }
            [[nodiscard]] inline hsluv invL() const { return {h, s, 100.0f-l, a}; }
            [[nodiscard]] inline hsluv invA() const { return {h, s, l, 1.0f-a}; }

            [[nodiscard]] inline hsluv inv   () const { return {360.0f-h, 100.0f-s, 100.0f-l, 1.0f-a }; }
            [[nodiscard]] inline hsluv invHSL() const { return {360.0f-h, 100.0f-s, 100.0f-l, a }; }
            [[nodiscard]] inline hsluv opaque() const { return { h, s, l, 1.0f }; }

            inline void rotate(real_t x)
            { h = std::fmod(h + (x < 0 ? 360 + x : x), 360); }

            template <size_t Count>
            [[nodiscard]] inline std::array<hsluv, Count> swatch() const
            {
                static_assert(Count > 0);

                auto step = 100.0f / Count;

                std::array<hsluv, Count> tmp;
                for (size_t i = 0; i < Count; ++i)
                    tmp[i] = withL(step * (i+1));

                return tmp;
            }

            template <size_t Count>
            [[nodiscard]] inline std::array<hsluv, Count> scheme() const
            {
                static_assert(Count > 0);

                auto step = 360.0f / Count;
                hsluv c = *this;

                std::array<hsluv, Count> tmp;
                for (size_t i = 0; i < Count; ++i)
                {
                    tmp[i] = c;
                    c.rotate(step);
                }

                return tmp;

            }

            [[nodiscard]] inline vec4   toVec4  () const { return { h,s,l,a }; }
            [[nodiscard]] inline vec3   toVec3  () const { return { h,s,l }; }
            [[nodiscard]] inline color  toColor () const { return NORMALtoRGB(HSLUVtoNORMAL(*this)); }
            [[nodiscard]] inline normal toNormal() const { return HSLUVtoNORMAL(*this); }
        };

        using pack_type = b8[4];


        b32         i;
        pack_type   pack;
        struct { b8 a,b,g,r; };

        M_DECL color()
            : i{0x000000ff}
        {}

        M_DECL color(b8 r, b8 g, b8 b, b8 a = 255)
            : a{a}, b{b}, g{g}, r{r}
        {}

        M_DECL explicit color(b32 i)
            : i{i}
        {}

        M_DECL explicit color(vec4b const& v)
            : a{v.w}, b{v.z}, g{v.y}, r{v.x}
        {}

        inline explicit color(normal const& n)
            : color{NORMALtoRGB(n)}
        {}

        inline explicit color(hsv const& h)
            : color{NORMALtoRGB(HSVtoNORMAL(h))}
        {}

        inline explicit color(hsluv const& h)
            : color{NORMALtoRGB(HSLUVtoNORMAL(h))}
        {}

        [[nodiscard]] inline normal toNormal()     const { return RGBtoNORMAL(*this); }
        [[nodiscard]] inline normal toNormal(b8 a) const { return withA(a).toNormal(); }

        [[nodiscard]] inline hsv    toHSV()     const { return NORMALtoHSV(RGBtoNORMAL(*this)); }
        [[nodiscard]] inline hsv    toHSV(b8 a) const { return withA(a).toHSV(); }

        [[nodiscard]] inline hsluv  toHSLUV()     const { return NORMALtoHSLUV(RGBtoNORMAL(*this)); }
        [[nodiscard]] inline hsluv  toHSLUV(b8 a) const { return withA(a).toHSLUV(); }

        [[nodiscard]] std::string toTrueColorFgTermString() const;
        [[nodiscard]] std::string toTrueColorBgTermString() const;

        M_DECL_PURE color inv()    const { return color(255-r,255-g,255-b,255-a); }
        M_DECL_PURE color invRGB() const { return color(255-r,255-g,255-b, a); }

        M_DECL_PURE color withR(b8 x) const { return color( x, g, b, a ); }
        M_DECL_PURE color withG(b8 x) const { return color( r, x, b, a ); }
        M_DECL_PURE color withB(b8 x) const { return color( r, g, x, a ); }
        M_DECL_PURE color withA(b8 x) const { return color( r, g, b, x ); }

        M_DECL_PURE color channelR() const { return color( r,r,r, 255 ); };
        M_DECL_PURE color channelG() const { return color( g,g,g, 255 ); };
        M_DECL_PURE color channelB() const { return color( b,b,b, 255 ); };
        M_DECL_PURE color channelA() const { return color( a,a,a, 255 ); };

        M_DECL_PURE color withNormalR(real_t x) const { b8 y = b8(CLAMP(x, 0,1)*255); return color(y, g, b, a); }
        M_DECL_PURE color withNormalG(real_t x) const { b8 y = b8(CLAMP(x, 0,1)*255); return color(r, y, b, a); }
        M_DECL_PURE color withNormalB(real_t x) const { b8 y = b8(CLAMP(x, 0,1)*255); return color(r, g, y, a); }
        M_DECL_PURE color withNormalA(real_t x) const { b8 y = b8(CLAMP(x, 0,1)*255); return color(r, g, b, y); }

        M_DECL_PURE color opaque() const { return { r, g, b, 255 }; }

        M_DECL_PURE          explicit operator vec4b () const { return {r,g,b,a}; }
        [[nodiscard]] inline explicit operator normal() const { return RGBtoNORMAL(*this); }
        [[nodiscard]] inline explicit operator hsv   () const { return NORMALtoHSV(RGBtoNORMAL(*this)); }
        [[nodiscard]] inline explicit operator hsluv () const { return NORMALtoHSLUV(RGBtoNORMAL(*this)); }

        M_DECL_PURE bool operator== (color const& c) const { return same(*this, c); }
        M_DECL_PURE bool operator!= (color const& c) const { return !(*this == c);  }
        M_DECL_PURE bool operator<  (color const& c) const { return less(*this, c); }
        M_DECL_PURE bool operator>  (color const& c) const { return *this < c;      }
        M_DECL_PURE bool operator<= (color const& c) const { return !(*this < c);   }
        M_DECL_PURE bool operator>= (color const& c) const { return !(c < *this);   }

        M_DECL static color grayscale(b8 x) { return { x,x,x,x }; }
        M_DECL static color grayscale(b8 x, b8 a) { return { x,x,x,a }; }

//        M_DECL static color fromNormal(normal const& n) { return NORMALtoRGB(n); }
//        M_DECL static color fromNormal(real_t r, real_t g, real_t b, real_t a = 1.0f) { return fromNormal({r,g,b,a}); }

//        M_DECL static color fromHSV(hsv const& hsv) { return NORMALtoRGB(HSVtoNORMAL(hsv)); }
//        M_DECL static color fromHSV(real_t h, real_t s, real_t v, real_t a = 1.0f) { return fromHSV({h,s,v,a}); }

        static color   parseRGBA(char const* s);
        static bool tryParseRGBA(char const* s, color& c);

        static color   parseARGB(char const* s);
        static bool tryParseARGB(char const* s, color& c);

    private:
        inline static color::normal RGBtoNORMAL(color c)
        {
            return {real_t(c.r) / 255, real_t(c.g) / 255, real_t(c.b) / 255, real_t(c.a) / 255 };
        }

        inline static color NORMALtoRGB(normal n)
        {
            n.clamp();
            return { b8(n.r * 255), b8(n.g * 255), b8(n.b * 255), b8(n.a * 255)  };
        }

        // Convert color3 floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
        // Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
        inline static color::hsv NORMALtoHSV(normal c)
        {
            auto K = real_t(0);

            c.clamp();

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
        inline static color::normal HSVtoNORMAL(hsv c)
        {
            c.clamp();

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

        static color::normal HSLUVtoNORMAL(hsluv c);
        static color::hsluv  NORMALtoHSLUV(normal c);
    };

    constexpr bool less(color const& a, color const& b) { return a.i <  b.i; }
    constexpr bool same(color const& a, color const& b) { return a.i == b.i; }

    inline std::ostream& operator<<(std::ostream& os, color const& c)
    {
        std::array<char, 128> buffer;

        if (int res = snprintf(buffer.data(), buffer.size(), "#%.8x", c.i); res > 0)
        {
            int cnt = buffer.size()-1;
            os.write(buffer.data(), res > cnt ? cnt : res);
        }
        else
        {
            os << "???";
        }
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, color::hsluv const& c)
    {
        std::array<char, 128> buffer;

        if (int res = snprintf(buffer.data(), buffer.size(), "hsluv(%.0fdeg %.0f%% %.0f%% / %.0f%%)",
                               roundf(c.h), roundf(c.s), roundf(c.l), roundf(c.a*100)); res > 0)
        {
            int cnt = buffer.size()-1;
            os.write(buffer.data(), res > cnt ? cnt : res);
        }
        else
        {
            os << "???";
        }
        return os;
    }

    inline std::string to_string(color const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }

#define COLOR_VAR(name_, i_) static constexpr color name_{i_};
#define COLOR_HSLUV(name_, i_)  inline static color::hsluv  name_() { return color{i_}.toHSLUV(); }
#define COLOR_HSV(name_, i_)    inline static color::hsv    name_() { return color{i_}.toHSV(); }
#define COLOR_NORMAL(name_, i_) inline static color::normal name_() { return color{i_}.toNormal(); }

namespace colors
{
    UT_EXPAND_COLORS(COLOR_VAR)

    namespace hsluv     { UT_EXPAND_COLORS(COLOR_HSLUV)     }
    namespace hsv       { UT_EXPAND_COLORS(COLOR_HSV)       }
    namespace normal    { UT_EXPAND_COLORS(COLOR_NORMAL)    }
}

#undef COLOR_VAR
#undef COLOR_HSLUV
#undef COLOR_HSL
#undef COLOR_NORMAL

}

#undef CLAMP
#undef NCLAMPED

#undef M_DECL_PURE
#undef M_DECL