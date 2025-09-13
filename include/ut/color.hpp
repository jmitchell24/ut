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
        struct oklch;

        /// \brief https://evilmartians.com/chronicles/oklch-in-css-why-quit-rgb-hsl
        ///
        struct oklch
        {
            /// \brief Perceived lightness [ 0, 1 ]
            real_t l;

            /// \brief Chroma, gray to most saturated color [ 0, 1 ]
            real_t c;

            /// \brief Hue angle [ 0, 360 ]
            real_t h;

            /// \brief Opacity [ 0, 1 ]
            real_t a;

            constexpr oklch()
                : l{ 0 }, c{ 0 }, h{ 0 }, a{ 0 }
            { }

            constexpr oklch(real_t l, real_t c, real_t h, real_t a = 1)
                : l{ l }, c{ c }, h{ h }, a{ a }
            { }

            explicit oklch(vec3 const& v, real_t a = 1)
                : oklch(v.x, v.y, v.z, a)
            { }

            explicit oklch(vec4 const& v)
                : oklch(v.x, v.y, v.z, v.w)
            { }

            explicit oklch(color const& c)
                : oklch(RGBtoOKLCH(c))
            { }

            oklch(oklch const&)=default;
            oklch(oklch&&) noexcept =default;

            oklch& operator=(oklch const&)=default;
            oklch& operator=(oklch&&) noexcept =default;

            [[nodiscard]] bool clamped() const
            {
                return !(
                    NCLAMPED(l, 0,1) ||
                    NCLAMPED(c, 0,1) ||
                    NCLAMPED(h, 0,360) ||
                    NCLAMPED(a, 0,1) );
            }

            void clamp()
            {
                l = CLAMP(l, 0,1);
                c = CLAMP(c, 0,1);
                h = CLAMP(h, 0,360);
                a = CLAMP(a, 0,1);
            }

            [[nodiscard]] oklch withL(real_t x) const { x = CLAMP(x, 0, 1);   return {x, c, h, a}; }
            [[nodiscard]] oklch withC(real_t x) const { x = CLAMP(x, 0, 1);   return {l, x, h, a}; }
            [[nodiscard]] oklch withH(real_t x) const { x = CLAMP(x, 0, 360); return {l, c, x, a}; }
            [[nodiscard]] oklch withA(real_t x) const { x = CLAMP(x, 0, 1);   return {l, c, h, x}; }

            [[nodiscard]] oklch invL() const { return {1.0f-l, c     , h       , a     }; }
            [[nodiscard]] oklch invC() const { return {l     , 1.0f-c, h       , a     }; }
            [[nodiscard]] oklch invH() const { return {l     , c     , 360.0f-h, a     }; }
            [[nodiscard]] oklch invA() const { return {l     , c     , h       , 1.0f-a}; }

            [[nodiscard]] oklch inv   () const { return { 1.0f-l, 1.0f-c, 360.0f-h, 1.0f-a }; }
            [[nodiscard]] oklch invLCH() const { return { 1.0f-l, 1.0f-c, 360.0f-h, a }; }
            [[nodiscard]] oklch opaque() const { return { l, c, h, 1.0f }; }

            [[nodiscard]] vec4   toVec4  () const { return { l,c,h,a }; }
            [[nodiscard]] vec3   toVec3  () const { return { l,c,h }; }
            [[nodiscard]] color  toColor () const { return OKLCHtoRGB(*this); }

            void rotate(real_t x)
            { h = std::fmod(h + (x < 0 ? 360 + x : x), 360); }



            [[nodiscard]] static oklch gradientX(oklch const& left, oklch const& right, real_t x)
            {
                return
                {
                    left.l + (right.l - left.l) * x,
                    left.c + (right.c - left.c) * x,
                    left.h + (right.h - left.h) * x,
                    left.a + (right.a - left.a) * x
                };
            }

            [[nodiscard]] static oklch gradientXY(oklch const& tl, oklch const& tr, oklch const& br, oklch const& bl, real_t x, real_t y)
            {
                oklch t = gradientX(tl, tr, x); // top
                oklch b = gradientX(bl, br, x); // bottom
                return gradientX(t, b, y);
            }
        };

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

            inline explicit normal(color const& c)
                : normal(RGBtoNORMAL(c))
            {}

            constexpr normal(normal const&)=default;
            constexpr normal(normal&&) noexcept =default;

            constexpr normal& operator=(normal const&)=default;
            constexpr normal& operator=(normal&&) noexcept =default;

            [[nodiscard]]  bool clamped() const
            {
                return !(
                    NCLAMPED(r, 0,1) ||
                    NCLAMPED(g, 0,1) ||
                    NCLAMPED(b, 0,1) ||
                    NCLAMPED(a, 0,1) );
            }

            void clamp()
            {
                r = CLAMP(r, 0,1);
                g = CLAMP(g, 0,1);
                b = CLAMP(b, 0,1);
                a = CLAMP(a, 0,1);
            }

            [[nodiscard]] inline operator vec4() const { return toVec4(); }
            [[nodiscard]] inline operator vec3() const { return toVec3(); }

            [[nodiscard]] inline explicit operator color() const { return toColor(); }

            [[nodiscard]] inline vec4  toVec4 () const { return { r,g,b,a }; }
            [[nodiscard]] inline vec3  toVec3 () const { return { r,g,b }; }
            [[nodiscard]] inline color toColor() const { return NORMALtoRGB(*this); }
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

        inline explicit color(oklch const& c)
            : color{OKLCHtoRGB(c)}
        {}

        [[nodiscard]] inline normal toNormal()     const { return RGBtoNORMAL(*this); }
        [[nodiscard]] inline normal toNormal(b8 a) const { return withA(a).toNormal(); }

        [[nodiscard]] inline oklch toOKLCH()        const { return RGBtoOKLCH(*this); }
        [[nodiscard]] inline oklch toOKLCH(b8 a)    const { return withA(a).toOKLCH(); }

        /// \brief      Create the background ANSI escape sequence for this color.
        /// \return     A string of the escape sequence.
        [[nodiscard]] std::string toFgEscCode() const;
        [[nodiscard]] std::string toBgEscCode() const;

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
        [[nodiscard]] inline explicit operator oklch () const { return RGBtoOKLCH(*this); }

        M_DECL_PURE bool operator== (color const& c) const { return same(*this, c); }
        M_DECL_PURE bool operator!= (color const& c) const { return !(*this == c);  }
        M_DECL_PURE bool operator<  (color const& c) const { return less(*this, c); }
        M_DECL_PURE bool operator>  (color const& c) const { return *this < c;      }
        M_DECL_PURE bool operator<= (color const& c) const { return !(*this < c);   }
        M_DECL_PURE bool operator>= (color const& c) const { return !(c < *this);   }

        M_DECL static color grayscale(b8 x) { return { x,x,x,x }; }
        M_DECL static color grayscale(b8 x, b8 a) { return { x,x,x,a }; }

        static color   parseRGBA(char const* s);
        static bool tryParseRGBA(char const* s, color& c);

        static color   parseARGB(char const* s);
        static bool tryParseARGB(char const* s, color& c);

        template <size_t Count>
        [[nodiscard]] static std::array<color, Count> swatch(color const& col)
        {
            static_assert(Count > 0);

            auto step = 100.0f / Count;
            auto c = col.toOKLCH();

            std::array<color, Count> tmp;
            for (size_t i = 0; i < Count; ++i)
                tmp[i] = c.withL(step * (i+1)).toColor();
            return tmp;
        }

        template <size_t Count>
        [[nodiscard]] static std::array<color, Count> scheme(color const& col)
        {
            static_assert(Count > 0);

            auto step = 360.0f / Count;
            auto c = col.toOKLCH();

            std::array<color, Count> tmp;
            for (size_t i = 0; i < Count; ++i)
            {
                tmp[i] = c.toColor();
                c.rotate(step);
            }

            return tmp;
        }

        [[nodiscard]] static color gradientX(color const& left, color const& right, real_t x)
        {
            return oklch::gradientX(left.toOKLCH(), right.toOKLCH(), x).toColor();
        }

        [[nodiscard]] static color gradientXY(color const& tl, color const& tr, color const& br, color const& bl, real_t x, real_t y)
        {
            return oklch::gradientXY(tl.toOKLCH(), tr.toOKLCH(), br.toOKLCH(), bl.toOKLCH(), x, y).toColor();
        }

        template <size_t Count>
        [[nodiscard]] static std::array<color, Count> gradient(color const& left, color const& right)
        {
            static_assert(Count > 1);

            auto cl=left.toOKLCH();
            auto cr=right.toOKLCH();

            std::array<color, Count> tmp;
            for (size_t i = 0; i < Count; ++i)
            {
                real_t t = static_cast<real_t>(i) / static_cast<real_t>(Count - 1);
                tmp[i] = oklch::gradientX(cl, cr, t).toColor();
            }

            return tmp;
        }

        template <size_t Width, size_t Height>
        [[nodiscard]] static std::array<color, Width*Height> gradient(color const& tl, color const& tr, color const& br, color const& bl)
        {
            static_assert(Width*Height > 2);

            auto ctl = tl.toOKLCH();
            auto ctr = tr.toOKLCH();
            auto cbl = bl.toOKLCH();
            auto cbr = br.toOKLCH();

            std::array<color, Width*Height> tmp;
            for (size_t y = 0; y < Height; ++y)
            {
                for (size_t x = 0; x < Width; ++x)
                {
                    real_t u = static_cast<real_t>(x) / static_cast<real_t>(Width - 1);
                    real_t v = static_cast<real_t>(y) / static_cast<real_t>(Height - 1);
                    tmp[y * Width + x] = oklch::gradientXY(ctl, ctr, cbl, cbr, u, v).toColor();
                }
            }

            return tmp;
        }

    private:
        inline static normal RGBtoNORMAL(color c)
        {
            return {real_t(c.r) / 255, real_t(c.g) / 255, real_t(c.b) / 255, real_t(c.a) / 255 };
        }

        inline static color NORMALtoRGB(normal n)
        {
            n.clamp();
            return { b8(n.r * 255), b8(n.g * 255), b8(n.b * 255), b8(n.a * 255)  };
        }

        static oklch RGBtoOKLCH(color c);
        static color OKLCHtoRGB(oklch c);
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

    inline std::string to_string(color const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }

#define COLOR_VAR(_name, _value) static constexpr color _name{_value};

namespace colors
{
    UT_EXPAND_COLORS(COLOR_VAR)
}

#undef COLOR_VAR

}

#undef CLAMP
#undef NCLAMPED

#undef M_DECL_PURE
#undef M_DECL