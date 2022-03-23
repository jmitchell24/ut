#ifndef RECT_HPP
#define RECT_HPP

#include "math/Vector2D.hpp"

#include <utility>

namespace ut
{
    template <typename N>
    struct RectN
    {
        using scalar_type   = N;
        using real_type     = real_t;
        using rect_type     = RectN<N>;
        using point_type    = Vec2N<N>;
        using split_type    = std::pair<rect_type,rect_type>;

        scalar_type x, y;
        scalar_type w, h;

        RectN()
            : x{0}, y{0}, w{0}, h{0}
        {}

        RectN(scalar_type x, scalar_type y, scalar_type width, scalar_type height)
            : x{x}, y{y}, w{width}, h{height}
        {}

        RectN(point_type const& p, point_type const& s)
            : x{p.x}, y{p.y}, w{s.x}, h{s.y}
        {}

        RectN(RectN const&)=default;
        RectN(RectN&&) noexcept =default;

        RectN& operator=(RectN const&)=default;
        RectN& operator=(RectN&&) noexcept =default;

        template <typename T>
        inline RectN<T> cast() const { return RectN<T>(T(x), T(y), T(w), T(h)); }

        inline scalar_type minX() const { return x; }
        inline scalar_type minY() const { return y; }
        inline scalar_type maxX() const { return x + w; }
        inline scalar_type maxY() const { return y + h; }

        ENABLE_IF_INTEGRAL inline scalar_type maxIncX() const { return x + w - 1; }      // inclusive
        ENABLE_IF_INTEGRAL inline scalar_type maxIncY() const { return y + h - 1; }      // inclusive

        inline point_type min() const { return point_type(x,y); }
        inline point_type max() const { return point_type(x + w, y + h); }

        ENABLE_IF_INTEGRAL inline point_type maxInc() const { return point_type{x + w - 1, y + h - 1}; }


        inline point_type xy  () const { return point_type(x,y); }
        inline point_type size() const { return point_type(w,h); }

        inline point_type topLeft    (scalar_type dx, scalar_type dy) const { return point_type(x+dx      , y+dy       ); }
        inline point_type topRight   (scalar_type dx, scalar_type dy) const { return point_type(x + w - dx, y + dy       ); }
        inline point_type bottomLeft (scalar_type dx, scalar_type dy) const { return point_type(x+dx      , y + h - dy); }
        inline point_type bottomRight(scalar_type dx, scalar_type dy) const { return point_type(x + w - dx, y + h - dy); }

        inline point_type topLeft    (point_type const& p) const { return topLeft    (p.x, p.y); }
        inline point_type topRight   (point_type const& p) const { return topRight   (p.x, p.y); }
        inline point_type bottomLeft (point_type const& p) const { return bottomLeft (p.x, p.y); }
        inline point_type bottomRight(point_type const& p) const { return bottomRight(p.x, p.y); }

        inline rect_type topLeft    (scalar_type dx, scalar_type dy, scalar_type dw, scalar_type dh) const { return rect_type(x + dx             , y + dy              , dw, dh); }
        inline rect_type topRight   (scalar_type dx, scalar_type dy, scalar_type dw, scalar_type dh) const { return rect_type(x + w - dx - dw, y + dy              , dw, dh); }
        inline rect_type bottomLeft (scalar_type dx, scalar_type dy, scalar_type dw, scalar_type dh) const { return rect_type(x + dx             , y + h - dy - dh, dw, dh); }
        inline rect_type bottomRight(scalar_type dx, scalar_type dy, scalar_type dw, scalar_type dh) const { return rect_type(x + w - dx - dw, y + h - dy - dh, dw, dh); }

        inline rect_type topLeft    (rect_type const& r) const { return topLeft    (r.x, r.y, r.w, r.h); }
        inline rect_type topRight   (rect_type const& r) const { return topRight   (r.x, r.y, r.w, r.h); }
        inline rect_type bottomLeft (rect_type const& r) const { return bottomLeft (r.x, r.y, r.w, r.h); }
        inline rect_type bottomRight(rect_type const& r) const { return bottomRight(r.x, r.y, r.w, r.h); }

        inline rect_type top   (scalar_type dy, scalar_type dh) const { return rect_type(x              , y + dy            , w , dh); }
        inline rect_type bottom(scalar_type dy, scalar_type dh) const { return rect_type(x              , y + h - dy - dh   , w , dh); }
        inline rect_type left  (scalar_type dx, scalar_type dw) const { return rect_type(x + dx         , y                 , dw, h ); }
        inline rect_type right (scalar_type dx, scalar_type dw) const { return rect_type(x + w - dx - dw, y                 , dw, h ); }



        inline rect_type fit(scalar_type dw, scalar_type dh) const
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

        inline rect_type fit(scalar_type dw, scalar_type dh, real_type& scale) const
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

        inline rect_type pad(scalar_type left, scalar_type top, scalar_type right, scalar_type bottom) const
        {
            return { x + left, y + top, w - left - right, h - top - bottom };
        }

        inline rect_type pad(scalar_type horz, scalar_type vert) const
        {
            return { x + horz, y + vert, w - horz - horz, h - vert - vert };
        }

        inline rect_type pad(scalar_type pad) const
        {
            return { x + pad, y + pad, w - pad - pad, h - pad - pad };
        }

        inline split_type splitTop(scalar_type dh) const
        {
            return
            {
                {x, y, w, dh},
                {x, y + dh, w, h - dh}
            };
        }

        inline split_type splitLeft(scalar_type dw) const
        {
            return
            {
                rect_type(x, y, dw, h),
                rect_type(x + dw, y, w - dw, h)
            };
        }

        inline split_type splitBottom(scalar_type dh) const
        {
            return
            {
                rect_type(x, y + h - dh, w, dh),
                rect_type(x, y, w, h - dh),
            };
        }

        inline static rect_type fromBound(point_type const& min, point_type const& max)
        {
            scalar_type x = min.x;
            scalar_type y = min.y;
            scalar_type w = max.x - min.x;
            scalar_type h = max.y - min.y;
            return {x, y, w, h};
        }

        inline static rect_type fromBound(scalar_type min_x, scalar_type min_y, scalar_type max_x, scalar_type max_y)
        {
            scalar_type x = min_x;
            scalar_type y = min_y;
            scalar_type w = max_x - min_x;
            scalar_type h = max_y - min_y;
            return {x, y, w, h};
        }
    };

    using Rect = RectN<int>;
    using Point = Vec2N<int>;

    typedef RectN<float>         Rectf;
    typedef RectN<double>        Rectd;
    typedef RectN<int>           Recti;
    typedef RectN<unsigned>      Rectu;
    typedef RectN<unsigned char> Rectb;
}
#endif // RECT_HPP
