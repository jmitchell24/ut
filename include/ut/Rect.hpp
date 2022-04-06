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
    struct fraction
    {
        real_t v;

        inline constexpr explicit fraction(real_t v)
            : v{v}
        { assert(v >= 0 && v <= 1); }

        template <typename T>
        inline constexpr T operator() (T t) const
        { return T(t * v); }
    };

    template <typename N>
    struct size2x
    {
        using scalar_type   = N;
        using real_type     = real_t;
        using size_type     = size2x<N>;
        using rect_type     = rectx<N>;
        using vec_type      = vec2x<N>;

        scalar_type w,h;

        inline constexpr size2x(scalar_type w, scalar_type h)
            : w{w}, h{h}
        {}

        inline constexpr explicit size2x(vec_type const& v)
            : w{v.x}, h{v.y}
        {}

        constexpr explicit operator rect_type() const { return vec(); }

        //
        // accessors
        //

        M_DECL_PURE vec_type vec() const { return vec_type{w,h}; }

        //
        // fit
        //

        M_DECL_PURE size_type fit(scalar_type dw, scalar_type dh) const
        {
            auto scale = std::min((real_type)w / dw, (real_type)h / dh);
            return { dw*scale, dh*scale };
        }

        M_DECL_PURE size_type fit(size_type const& d) const
        { return fit(d.w, d.h); }

        M_DECL_PURE std::tuple<size_type, real_type> fitScale(scalar_type dw, scalar_type dh) const
        {
            auto scale = std::min((real_type)w / dw, (real_type)h / dh);
            return { { dw*scale, dh*scale }, scale };
        }

        M_DECL_PURE std::tuple<size_type, real_type> fitScale(size_type const& d) const
        { return fitScale(d.w, d.h); }
    };

    using size2 = size2x<int>;

    typedef size2x<float>        size2f;
    typedef size2x<double>       size2d;
    typedef size2x<int>          size2i;
    typedef size2x<unsigned>     size2u;
    typedef size2x<std::uint8_t> size2b;

    template <typename N>
    struct rectx
    {
        using scalar_type   = N;
        using rect_type     = rectx<N>;
        using point_type    = vec2x<N>;
        using split_type    = std::tuple<rect_type,rect_type>;
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
        M_DECL_PURE point_type size() const { return point_type(w,h); }

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

        M_DECL_PURE scalar_type  operator[] (size_t i) const { assert(i < 4); return elements[i]; }
        M_DECL_PURE scalar_type& operator[] (size_t i)       { assert(i < 4); return elements[i]; }

        M_DECL_PURE auto begin()       { return std::begin(elements); }
        M_DECL_PURE auto begin() const { return std::begin(elements); }

        M_DECL_PURE auto end()       { return std::end(elements); }
        M_DECL_PURE auto end() const { return std::end(elements); }

        M_DECL_PURE scalar_type const* data() const { return elements; }
        M_DECL_PURE scalar_type*       data()       { return elements; }
    };

    using rect = rectx<int>;

    typedef rectx<float>        rectf;
    typedef rectx<double>       rectd;
    typedef rectx<int>          recti;
    typedef rectx<unsigned>     rectu;
    typedef rectx<std::uint8_t> rectb;

#if defined(UT_STL_INTEROP)
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
