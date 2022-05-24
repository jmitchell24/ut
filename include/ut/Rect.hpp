#ifndef RECT_HPP
#define RECT_HPP

#include "math/Vector2D.hpp"

#include <utility>

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

namespace ut
{
    template <typename N>
    struct rectx;

    /// strongly typed fractional value
    struct scaler
    {
        real_t v;

        inline constexpr explicit scaler(real_t v)
            : v{v}
        {}

        template <typename T>
        inline constexpr T operator() (T t) const
        { return T(t * v); }
    };

    inline constexpr scaler operator "" _sc(long double x) noexcept
    { return scaler{(real_t)x}; }

    inline constexpr scaler operator "" _pc(unsigned long long i) noexcept
    { return scaler((real_t)i / (real_t)100); }

    template <typename T1, typename T2>
    struct duo { T1 first; T2 second; };

    template <typename T1, typename T2, typename T3>
    struct trio { T1 first; T2 second; T3 third; };

    template <typename T1, typename T2, typename T3, typename T4>
    struct quadro { T1 first; T2 second; T3 third; T4 fourth; };

    template <typename N>
    struct rectx
    {
        using scalar_type   = N;
        using rect_type     = rectx<N>;
        using point_type    = vec2x<N>;
        using split_type    = duo<rect_type,rect_type>;
        using elements_type = scalar_type[4];

        union
        {
            struct { scalar_type x,y,w,h; };
            elements_type elements;
        };

        //
        // ctor
        //

        constexpr rectx()
            : x{0}, y{0}, w{0}, h{0}
        {}

        constexpr rectx(scalar_type x, scalar_type y, scalar_type w, scalar_type h)
            : x{x}, y{y}, w{w}, h{h}
        { assert(w>=0); assert(h>=0); }

        constexpr rectx(point_type const& p, point_type const& s)
            : rectx{p.x, p.y, s.x, s.y}
        {}

        constexpr rectx(rectx const&)=default;
        constexpr rectx(rectx&&) noexcept =default;

        constexpr rectx& operator=(rectx const&)=default;
        constexpr rectx& operator=(rectx&&) noexcept =default;

        //
        // mutators
        //

        M_DECL void pos (scalar_type px, scalar_type py) { x = px; y = py; }
        M_DECL void size(scalar_type sw, scalar_type sh) { w = sw; h = sh; assert(w>=0); assert(h>=0); }

        M_DECL void pos (point_type const& p) { x = p.x; y = p.y; }
        M_DECL void size(point_type const& s) { w = s.x; h = s.y; }

        //
        // accessors
        //

        M_DECL_PURE point_type pos () const { return point_type(x,y); }
        //M_DECL_PURE point_type size() const { return point_type(w,h); }

        template <typename T>
        M_DECL_PURE rectx<T> cast() const { return rectx<T>(T(x), T(y), T(w), T(h)); }

        M_DECL_PURE scalar_type minX() const { return x; }
        M_DECL_PURE scalar_type minY() const { return y; }
        M_DECL_PURE scalar_type maxX() const { return x + w; }
        M_DECL_PURE scalar_type maxY() const { return y + h; }

        ENABLE_IF_INTEGRAL M_DECL_PURE scalar_type maxIncX() const { return x + w - 1; }      // inclusive
        ENABLE_IF_INTEGRAL M_DECL_PURE scalar_type maxIncY() const { return y + h - 1; }      // inclusive

        M_DECL_PURE point_type min() const { return point_type(x,y); }
        M_DECL_PURE point_type max() const { return point_type(x + w, y + h); }

        ENABLE_IF_INTEGRAL [[nodiscard]] inline constexpr point_type maxInc() const { return point_type{x + w - 1, y + h - 1}; }

        //
        // utility
        //

        ENABLE_IF_FLOAT M_DECL_PURE rect_type round() const { return rect_type(std::round(x), std::round(y), std::round(w), std::round(h)); }
        ENABLE_IF_FLOAT M_DECL_PURE rect_type floor() const { return rect_type(std::floor(x), std::floor(y), std::floor(w), std::floor(h)); }
        ENABLE_IF_FLOAT M_DECL_PURE rect_type ceil () const { return rect_type(std::ceil (x), std::ceil (y), std::ceil (w), std::ceil (h)); }

        //
        // split
        //

        M_DECL_PURE split_type splitTop(scalar_type dh) const
        {
            return
            {
                {x, y, w, dh},
                {x, y + dh, w, h - dh}
            };
        }

        M_DECL_PURE split_type splitLeft(scalar_type dw) const
        {
            return
            {
                rect_type(x, y, dw, h),
                rect_type(x + dw, y, w - dw, h)
            };
        }

        M_DECL_PURE split_type splitBottom(scalar_type dh) const
        {
            return
            {
                rect_type(x, y + h - dh, w, dh),
                rect_type(x, y, w, h - dh),
            };
        }

        //
        // container utilities
        //

        M_DECL_PURE scalar_type  operator[] (size_t i) const { assert(i < 4); return elements[i]; }
        M_DECL      scalar_type& operator[] (size_t i)       { assert(i < 4); return elements[i]; }

        M_DECL_PURE scalar_type const* begin() const { return &elements[0]; }
        M_DECL      scalar_type*       begin()       { return &elements[0]; }

        M_DECL_PURE scalar_type const* end() const { return &elements[3]; }
        M_DECL      scalar_type*       end()       { return &elements[3]; }

        M_DECL_PURE scalar_type const* data() const { return elements; }
        M_DECL      scalar_type*       data()       { return elements; }

        M_DECL_PURE size_t size() const { return 4; }
    };

    using rect = rectx<int>;

    typedef rectx<float>        rectf;
    typedef rectx<double>       rectd;
    typedef rectx<int>          recti;
    typedef rectx<unsigned>     rectu;
    typedef rectx<std::uint8_t> rectb;

#if !defined(UT_NO_STL_INTEROP)
    template <typename N>
    inline std::ostream& operator<<(std::ostream& os, rectx<N> const& r)
    {
        return os << r.x << "x" << r.y << " [" << r.w << ", " << r.h << "]";
    }

    template <typename N, size_t D>
    inline std::string to_string(rectx<N> const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }
#endif
}

#undef M_DECL_PURE
#undef M_DECL

#endif // RECT_HPP
