#ifndef RECT_HPP
#define RECT_HPP

#include "math/Vector2D.hpp"

#include <utility>

namespace ut
{
    template <typename N>
    struct rectx
    {
        using scalar_type   = N;
        using real_type     = real_t;
        using rect_type     = rectx<N>;
        using point_type    = vec2x<N>;
        using split_type    = std::pair<rect_type,rect_type>;
        using elements_type = scalar_type[4];

        union
        {
            struct { scalar_type x,y,w,h; };
            elements_type elements;
        };

        constexpr rectx()
            : x{0}, y{0}, w{0}, h{0}
        {}

        constexpr rectx(scalar_type x, scalar_type y, scalar_type width, scalar_type height)
            : x{x}, y{y}, w{width}, h{height}
        {}

        constexpr rectx(point_type const& p, point_type const& s)
            : x{p.x}, y{p.y}, w{s.x}, h{s.y}
        {}

        constexpr rectx(rectx const&)=default;
        constexpr rectx(rectx&&) noexcept =default;

        constexpr rectx& operator=(rectx const&)=default;
        constexpr rectx& operator=(rectx&&) noexcept =default;

        template <typename T>
        [[nodiscard]] inline rectx<T> cast() const { return rectx<T>(T(x), T(y), T(w), T(h)); }

        [[nodiscard]] inline constexpr scalar_type minX() const { return x; }
        [[nodiscard]] inline constexpr scalar_type minY() const { return y; }
        [[nodiscard]] inline constexpr scalar_type maxX() const { return x + w; }
        [[nodiscard]] inline constexpr scalar_type maxY() const { return y + h; }

        ENABLE_IF_INTEGRAL [[nodiscard]] inline constexpr scalar_type maxIncX() const { return x + w - 1; }      // inclusive
        ENABLE_IF_INTEGRAL [[nodiscard]] inline constexpr scalar_type maxIncY() const { return y + h - 1; }      // inclusive

        [[nodiscard]] inline constexpr point_type min() const { return point_type(x,y); }
        [[nodiscard]] inline constexpr point_type max() const { return point_type(x + w, y + h); }

        ENABLE_IF_INTEGRAL [[nodiscard]] inline constexpr point_type maxInc() const { return point_type{x + w - 1, y + h - 1}; }

        ENABLE_IF_FLOAT [[nodiscard]] inline constexpr rect_type round() const { return rect_type(std::round(x), std::round(y), std::round(w), std::round(h)); }
        ENABLE_IF_FLOAT [[nodiscard]] inline constexpr rect_type floor() const { return rect_type(std::floor(x), std::floor(y), std::floor(w), std::floor(h)); }
        ENABLE_IF_FLOAT [[nodiscard]] inline constexpr rect_type ceil () const { return rect_type(std::ceil (x), std::ceil (y), std::ceil (w), std::ceil (h)); }

        [[nodiscard]] inline constexpr point_type pos () const { return point_type(x,y); }
        [[nodiscard]] inline constexpr point_type size() const { return point_type(w,h); }

        [[nodiscard]] inline constexpr point_type topLeft    (scalar_type dx, scalar_type dy) const { return point_type(x+dx      , y+dy       ); }
        [[nodiscard]] inline constexpr point_type topRight   (scalar_type dx, scalar_type dy) const { return point_type(x + w - dx, y + dy       ); }
        [[nodiscard]] inline constexpr point_type bottomLeft (scalar_type dx, scalar_type dy) const { return point_type(x+dx      , y + h - dy); }
        [[nodiscard]] inline constexpr point_type bottomRight(scalar_type dx, scalar_type dy) const { return point_type(x + w - dx, y + h - dy); }

        [[nodiscard]] inline constexpr point_type topLeft    (point_type const& p) const { return topLeft    (p.x, p.y); }
        [[nodiscard]] inline constexpr point_type topRight   (point_type const& p) const { return topRight   (p.x, p.y); }
        [[nodiscard]] inline constexpr point_type bottomLeft (point_type const& p) const { return bottomLeft (p.x, p.y); }
        [[nodiscard]] inline constexpr point_type bottomRight(point_type const& p) const { return bottomRight(p.x, p.y); }

        [[nodiscard]] inline constexpr rect_type topLeft    (scalar_type dx, scalar_type dy, scalar_type dw, scalar_type dh) const { return rect_type(x + dx         , y + dy         , dw, dh); }
        [[nodiscard]] inline constexpr rect_type topRight   (scalar_type dx, scalar_type dy, scalar_type dw, scalar_type dh) const { return rect_type(x + w - dx - dw, y + dy         , dw, dh); }
        [[nodiscard]] inline constexpr rect_type bottomLeft (scalar_type dx, scalar_type dy, scalar_type dw, scalar_type dh) const { return rect_type(x + dx         , y + h - dy - dh, dw, dh); }
        [[nodiscard]] inline constexpr rect_type bottomRight(scalar_type dx, scalar_type dy, scalar_type dw, scalar_type dh) const { return rect_type(x + w - dx - dw, y + h - dy - dh, dw, dh); }

        [[nodiscard]] inline constexpr rect_type topLeft    (rect_type const& r) const { return topLeft    (r.x, r.y, r.w, r.h); }
        [[nodiscard]] inline constexpr rect_type topRight   (rect_type const& r) const { return topRight   (r.x, r.y, r.w, r.h); }
        [[nodiscard]] inline constexpr rect_type bottomLeft (rect_type const& r) const { return bottomLeft (r.x, r.y, r.w, r.h); }
        [[nodiscard]] inline constexpr rect_type bottomRight(rect_type const& r) const { return bottomRight(r.x, r.y, r.w, r.h); }

        [[nodiscard]] inline constexpr rect_type top   (scalar_type dy, scalar_type dh) const { return rect_type(x              , y + dy            , w , dh); }
        [[nodiscard]] inline constexpr rect_type bottom(scalar_type dy, scalar_type dh) const { return rect_type(x              , y + h - dy - dh   , w , dh); }
        [[nodiscard]] inline constexpr rect_type left  (scalar_type dx, scalar_type dw) const { return rect_type(x + dx         , y                 , dw, h ); }
        [[nodiscard]] inline constexpr rect_type right (scalar_type dx, scalar_type dw) const { return rect_type(x + w - dx - dw, y                 , dw, h ); }



        [[nodiscard]] inline constexpr rect_type fit(scalar_type dw, scalar_type dh) const
        {
            real_type scale = std::min(real_type(this->w) / dw, real_type(this->h) / dh);
            dw *= scale;
            dh *= scale;

            return
            {
                this->x + (this->w - dw) / 2,
                this->y + (this->h - dh) / 2,
                dw,
                dh
            };
        }

        [[nodiscard]] inline constexpr rect_type fit(scalar_type dw, scalar_type dh, real_type& scale) const
        {
            scale = std::min(real_type(this->w) / dw, real_type(this->h) / dh);
            dw *= scale;
            dh *= scale;

            return
            {
                this->x + (this->w - dw) / 2,
                this->y + (this->h - dh) / 2,
                dw,
                dh
            };
        }

        [[nodiscard]] inline constexpr rect_type pad(scalar_type left, scalar_type top, scalar_type right, scalar_type bottom) const
        {
            return { x + left, y + top, w - left - right, h - top - bottom };
        }

        [[nodiscard]] inline constexpr rect_type pad(scalar_type horz, scalar_type vert) const
        {
            return { x + horz, y + vert, w - horz - horz, h - vert - vert };
        }

        [[nodiscard]] inline constexpr rect_type pad(scalar_type pad) const
        {
            return { x + pad, y + pad, w - pad - pad, h - pad - pad };
        }

        [[nodiscard]] inline constexpr split_type splitTop(scalar_type dh) const
        {
            return
            {
                {x, y, w, dh},
                {x, y + dh, w, h - dh}
            };
        }

        [[nodiscard]] inline constexpr split_type splitLeft(scalar_type dw) const
        {
            return
            {
                rect_type(x, y, dw, h),
                rect_type(x + dw, y, w - dw, h)
            };
        }

        [[nodiscard]] inline constexpr split_type splitBottom(scalar_type dh) const
        {
            return
            {
                rect_type(x, y + h - dh, w, dh),
                rect_type(x, y, w, h - dh),
            };
        }

        [[nodiscard]] inline static constexpr rect_type fromBound(point_type const& min, point_type const& max)
        {
            scalar_type x = min.x;
            scalar_type y = min.y;
            scalar_type w = max.x - min.x;
            scalar_type h = max.y - min.y;
            return {x, y, w, h};
        }

        [[nodiscard]] inline static constexpr rect_type fromBound(scalar_type min_x, scalar_type min_y, scalar_type max_x, scalar_type max_y)
        {
            scalar_type x = min_x;
            scalar_type y = min_y;
            scalar_type w = max_x - min_x;
            scalar_type h = max_y - min_y;
            return {x, y, w, h};
        }

        [[nodiscard]] inline constexpr scalar_type  operator[] (size_t i) const { assert(i < 4); return elements[i]; }
        [[nodiscard]] inline constexpr scalar_type& operator[] (size_t i)       { assert(i < 4); return elements[i]; }

        [[nodiscard]] inline constexpr auto begin()       { return std::begin(elements); }
        [[nodiscard]] inline constexpr auto begin() const { return std::begin(elements); }

        [[nodiscard]] inline constexpr auto end()       { return std::end(elements); }
        [[nodiscard]] inline constexpr auto end() const { return std::end(elements); }

        [[nodiscard]] inline constexpr scalar_type const* data() const { return elements; }
        [[nodiscard]] inline constexpr scalar_type*       data()       { return elements; }
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
#endif // RECT_HPP
