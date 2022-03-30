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
    COLOR(black,                0x00, 0x00, 0x00) \
    COLOR(silver,               0xc0, 0xc0, 0xc0) \
    COLOR(gray,                 0x80, 0x80, 0x80) \
    COLOR(white,                0xff, 0xff, 0xff) \
    COLOR(maroon,               0x80, 0x00, 0x00) \
    COLOR(red,                  0xff, 0x00, 0x00) \
    COLOR(purple,               0x80, 0x00, 0x80) \
    COLOR(green,                0x00, 0x80, 0x00) \
    COLOR(lime,                 0x00, 0xff, 0x00) \
    COLOR(olive,                0x80, 0x80, 0x00) \
    COLOR(yellow,               0xff, 0xff, 0x00) \
    COLOR(navy,                 0x00, 0x00, 0x80) \
    COLOR(blue,                 0x00, 0x00, 0xff) \
    COLOR(teal,                 0x00, 0x80, 0x80) \
    COLOR(aqua,                 0x00, 0xff, 0xff) \
    COLOR(orange,               0xff, 0xa5, 0x00) \
    COLOR(aliceblue,            0xf0, 0xf8, 0xff) \
    COLOR(antiquewhite,         0xfa, 0xeb, 0xd7) \
    COLOR(aquamarine,           0x7f, 0xff, 0xd4) \
    COLOR(azure,                0xf0, 0xff, 0xff) \
    COLOR(beige,                0xf5, 0xf5, 0xdc) \
    COLOR(bisque,               0xff, 0xe4, 0xc4) \
    COLOR(blanchedalmond,       0xff, 0xeb, 0xcd) \
    COLOR(blueviolet,           0x8a, 0x2b, 0xe2) \
    COLOR(brown,                0xa5, 0x2a, 0x2a) \
    COLOR(burlywood,            0xde, 0xb8, 0x87) \
    COLOR(cadetblue,            0x5f, 0x9e, 0xa0) \
    COLOR(chartreuse,           0x7f, 0xff, 0x00) \
    COLOR(chocolate,            0xd2, 0x69, 0x1e) \
    COLOR(coral,                0xff, 0x7f, 0x50) \
    COLOR(cornflowerblue,       0x64, 0x95, 0xed) \
    COLOR(cornsilk,             0xff, 0xf8, 0xdc) \
    COLOR(crimson,              0xdc, 0x14, 0x3c) \
    COLOR(cyan,                 0x00, 0xff, 0xff) \
    COLOR(darkblue,             0x00, 0x00, 0x8b) \
    COLOR(darkcyan,             0x00, 0x8b, 0x8b) \
    COLOR(darkgoldenrod,        0xb8, 0x86, 0x0b) \
    COLOR(darkgray,             0xa9, 0xa9, 0xa9) \
    COLOR(darkgreen,            0x00, 0x64, 0x00) \
    COLOR(darkgrey,             0xa9, 0xa9, 0xa9) \
    COLOR(darkkhaki,            0xbd, 0xb7, 0x6b) \
    COLOR(darkmagenta,          0x8b, 0x00, 0x8b) \
    COLOR(darkolivegreen,       0x55, 0x6b, 0x2f) \
    COLOR(darkorange,           0xff, 0x8c, 0x00) \
    COLOR(darkorchid,           0x99, 0x32, 0xcc) \
    COLOR(darkred,              0x8b, 0x00, 0x00) \
    COLOR(darksalmon,           0xe9, 0x96, 0x7a) \
    COLOR(darkseagreen,         0x8f, 0xbc, 0x8f) \
    COLOR(darkslateblue,        0x48, 0x3d, 0x8b) \
    COLOR(darkslategray,        0x2f, 0x4f, 0x4f) \
    COLOR(darkslategrey,        0x2f, 0x4f, 0x4f) \
    COLOR(darkturquoise,        0x00, 0xce, 0xd1) \
    COLOR(darkviolet,           0x94, 0x00, 0xd3) \
    COLOR(deeppink,             0xff, 0x14, 0x93) \
    COLOR(deepskyblue,          0x00, 0xbf, 0xff) \
    COLOR(dimgray,              0x69, 0x69, 0x69) \
    COLOR(dimgrey,              0x69, 0x69, 0x69) \
    COLOR(dodgerblue,           0x1e, 0x90, 0xff) \
    COLOR(firebrick,            0xb2, 0x22, 0x22) \
    COLOR(floralwhite,          0xff, 0xfa, 0xf0) \
    COLOR(forestgreen,          0x22, 0x8b, 0x22) \
    COLOR(gainsboro,            0xdc, 0xdc, 0xdc) \
    COLOR(ghostwhite,           0xf8, 0xf8, 0xff) \
    COLOR(gold,                 0xff, 0xd7, 0x00) \
    COLOR(goldenrod,            0xda, 0xa5, 0x20) \
    COLOR(greenyellow,          0xad, 0xff, 0x2f) \
    COLOR(grey,                 0x80, 0x80, 0x80) \
    COLOR(honeydew,             0xf0, 0xff, 0xf0) \
    COLOR(hotpink,              0xff, 0x69, 0xb4) \
    COLOR(indianred,            0xcd, 0x5c, 0x5c) \
    COLOR(indigo,               0x4b, 0x00, 0x82) \
    COLOR(ivory,                0xff, 0xff, 0xf0) \
    COLOR(khaki,                0xf0, 0xe6, 0x8c) \
    COLOR(lavender,             0xe6, 0xe6, 0xfa) \
    COLOR(lavenderblush,        0xff, 0xf0, 0xf5) \
    COLOR(lawngreen,            0x7c, 0xfc, 0x00) \
    COLOR(lemonchiffon,         0xff, 0xfa, 0xcd) \
    COLOR(lightblue,            0xad, 0xd8, 0xe6) \
    COLOR(lightcoral,           0xf0, 0x80, 0x80) \
    COLOR(lightcyan,            0xe0, 0xff, 0xff) \
    COLOR(lightgoldenrodyellow, 0xfa, 0xfa, 0xd2) \
    COLOR(lightgray,            0xd3, 0xd3, 0xd3) \
    COLOR(lightgreen,           0x90, 0xee, 0x90) \
    COLOR(lightgrey,            0xd3, 0xd3, 0xd3) \
    COLOR(lightpink,            0xff, 0xb6, 0xc1) \
    COLOR(lightsalmon,          0xff, 0xa0, 0x7a) \
    COLOR(lightseagreen,        0x20, 0xb2, 0xaa) \
    COLOR(lightskyblue,         0x87, 0xce, 0xfa) \
    COLOR(lightslategray,       0x77, 0x88, 0x99) \
    COLOR(lightslategrey,       0x77, 0x88, 0x99) \
    COLOR(lightsteelblue,       0xb0, 0xc4, 0xde) \
    COLOR(lightyellow,          0xff, 0xff, 0xe0) \
    COLOR(limegreen,            0x32, 0xcd, 0x32) \
    COLOR(linen,                0xfa, 0xf0, 0xe6) \
    COLOR(magenta,              0xff, 0x00, 0xff) \
    COLOR(fuchsia,              0xff, 0x00, 0xff) \
    COLOR(mediumaquamarine,     0x66, 0xcd, 0xaa) \
    COLOR(mediumblue,           0x00, 0x00, 0xcd) \
    COLOR(mediumorchid,         0xba, 0x55, 0xd3) \
    COLOR(mediumpurple,         0x93, 0x70, 0xdb) \
    COLOR(mediumseagreen,       0x3c, 0xb3, 0x71) \
    COLOR(mediumslateblue,      0x7b, 0x68, 0xee) \
    COLOR(mediumspringgreen,    0x00, 0xfa, 0x9a) \
    COLOR(mediumturquoise,      0x48, 0xd1, 0xcc) \
    COLOR(mediumvioletred,      0xc7, 0x15, 0x85) \
    COLOR(midnightblue,         0x19, 0x19, 0x70) \
    COLOR(mintcream,            0xf5, 0xff, 0xfa) \
    COLOR(mistyrose,            0xff, 0xe4, 0xe1) \
    COLOR(moccasin,             0xff, 0xe4, 0xb5) \
    COLOR(navajowhite,          0xff, 0xde, 0xad) \
    COLOR(oldlace,              0xfd, 0xf5, 0xe6) \
    COLOR(olivedrab,            0x6b, 0x8e, 0x23) \
    COLOR(orangered,            0xff, 0x45, 0x00) \
    COLOR(orchid,               0xda, 0x70, 0xd6) \
    COLOR(palegoldenrod,        0xee, 0xe8, 0xaa) \
    COLOR(palegreen,            0x98, 0xfb, 0x98) \
    COLOR(paleturquoise,        0xaf, 0xee, 0xee) \
    COLOR(palevioletred,        0xdb, 0x70, 0x93) \
    COLOR(papayawhip,           0xff, 0xef, 0xd5) \
    COLOR(peachpuff,            0xff, 0xda, 0xb9) \
    COLOR(peru,                 0xcd, 0x85, 0x3f) \
    COLOR(pink,                 0xff, 0xc0, 0xcb) \
    COLOR(plum,                 0xdd, 0xa0, 0xdd) \
    COLOR(powderblue,           0xb0, 0xe0, 0xe6) \
    COLOR(rosybrown,            0xbc, 0x8f, 0x8f) \
    COLOR(royalblue,            0x41, 0x69, 0xe1) \
    COLOR(saddlebrown,          0x8b, 0x45, 0x13) \
    COLOR(salmon,               0xfa, 0x80, 0x72) \
    COLOR(sandybrown,           0xf4, 0xa4, 0x60) \
    COLOR(seagreen,             0x2e, 0x8b, 0x57) \
    COLOR(seashell,             0xff, 0xf5, 0xee) \
    COLOR(sienna,               0xa0, 0x52, 0x2d) \
    COLOR(skyblue,              0x87, 0xce, 0xeb) \
    COLOR(slateblue,            0x6a, 0x5a, 0xcd) \
    COLOR(slategray,            0x70, 0x80, 0x90) \
    COLOR(slategrey,            0x70, 0x80, 0x90) \
    COLOR(snow,                 0xff, 0xfa, 0xfa) \
    COLOR(springgreen,          0x00, 0xff, 0x7f) \
    COLOR(steelblue,            0x46, 0x82, 0xb4) \
    COLOR(tan,                  0xd2, 0xb4, 0x8c) \
    COLOR(thistle,              0xd8, 0xbf, 0xd8) \
    COLOR(tomato,               0xff, 0x63, 0x47) \
    COLOR(turquoise,            0x40, 0xe0, 0xd0) \
    COLOR(violet,               0xee, 0x82, 0xee) \
    COLOR(wheat,                0xf5, 0xde, 0xb3) \
    COLOR(whitesmoke,           0xf5, 0xf5, 0xf5) \
    COLOR(yellowgreen,          0x9a, 0xcd, 0x32) \
    COLOR(rebeccapurple,        0x66, 0x33, 0x99)

namespace ut
{
    struct color3;
    struct color4;

    struct color3
    {
        std::uint8_t r=0,g=0,b=0;

        inline operator vec3b() const
        { return {r,g,b}; }

        [[nodiscard]] color4 rgba(std::uint8_t a = 255) const;

        [[nodiscard]] vec3  normal () const;
        [[nodiscard]] vec3f normalf() const;
        [[nodiscard]] vec3d normald() const;

        [[nodiscard]] vec3  hsv () const;
        [[nodiscard]] vec3f hsvf() const;
        [[nodiscard]] vec3d hsvd() const;

        static color3 fromNormal (vec3 v);
        static color3 fromNormalf(vec3f v);
        static color3 fromNormald(vec3d v);

        static color3 fromHSV(vec3 v);
        static color3 fromHSVf(vec3f v);
        static color3 fromHSVd(vec3d v);

        static color3 parse(char const* s);
        static bool tryParse(char const* s, color3& c);
    };

    struct color4
    {
        std::uint8_t r=0,g=0,b=0,a=0;

        inline operator vec4b() const
        { return {r,g,b,a}; }

        [[nodiscard]] color3 rgb() const;

        [[nodiscard]] vec4  normal () const;
        [[nodiscard]] vec4f normalf() const;
        [[nodiscard]] vec4d normald() const;

        [[nodiscard]] vec4  hsv () const;
        [[nodiscard]] vec4f hsvf() const;
        [[nodiscard]] vec4d hsvd() const;

        static color4 fromNormal(vec4 v);
        static color4 fromNormalf(vec4f v);
        static color4 fromNormald(vec4d v);

        static color4 fromHSV (vec3  v, real_t a = real_t(1));
        static color4 fromHSVf(vec3f v, float  a = 1);
        static color4 fromHSVd(vec3d v, double a = 1);

        static color4 parseRGBA(char const* s);
        static bool tryParseRGBA(char const* s, color4& c);

        static color4 parseARGB(char const* s);
        static bool tryParseARGB(char const* s, color4& c);
    };

#if defined(UT_STL_INTEROP)
    inline std::ostream& operator<<(std::ostream& os, color3 const& c)
    { return os << (vec3b)c; }

    inline std::ostream& operator<<(std::ostream& os, color4 const& c)
    { return os << (vec4b)c; }
#endif

namespace colors
{
#define COLOR(_name, _r, _g, _b) static const color3 _name{_r, _g, _b};
    UT_COLORS
#undef COLOR
}
}


#endif //UT_COLOR_HPP
