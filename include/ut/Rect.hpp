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
        using rect_type     = RectN<N>;
        using point_type    = Vec2N<N>;
        using split_type    = std::pair<rect_type,rect_type>;

        scalar_type x, y;
        scalar_type width, height;

        RectN()
            : x{0}, y{0}, width{0}, height{0}
        {}

        RectN(scalar_type x, scalar_type y, scalar_type width, scalar_type height)
            : x{x}, y{y}, width{width}, height{height}
        {}

        RectN(point_type const& p, point_type const& s)
            : x{p.x}, y{p.y}, width{s.x}, height{s.y}
        {}

//        RectN(scalar_type width, scalar_type height)
//            : x{0}, y{0}, width{width}, height{height}
//        {}

        RectN(RectN const&)=default;
        RectN(RectN&&)=default;

        RectN& operator=(RectN const&)=default;
        RectN& operator=(RectN&&)=default;

        template <typename T>
        inline RectN<T> cast() const { return RectN<T>(T(x), T(y), T(width), T(height)); }

        inline scalar_type minX() const { return x; }
        inline scalar_type minY() const { return y; }
        inline scalar_type maxX() const { return x+width; }
        inline scalar_type maxY() const { return y+height; }

        ENABLE_IF_INTEGRAL inline scalar_type maxIncX() const { return x+width-1; }       // inclusive
        ENABLE_IF_INTEGRAL inline scalar_type maxIncY() const { return y+height-1; }      // inclusive

        inline point_type min() const { return point_type(x,y); }
        inline point_type max() const { return point_type(x+width, y+height); }

        ENABLE_IF_INTEGRAL inline point_type maxInc() const { return point_type{x+width-1, y+height-1}; }


        inline point_type xy  () const { return point_type(x,y); }
        inline point_type size() const { return point_type(width,height); }

        inline point_type topLeft    (scalar_type dx, scalar_type dy) const { return point_type(x+dx      , y+dy       ); }
        inline point_type topRight   (scalar_type dx, scalar_type dy) const { return point_type(x+width-dx, y+dy       ); }
        inline point_type bottomLeft (scalar_type dx, scalar_type dy) const { return point_type(x+dx      , y+height-dy); }
        inline point_type bottomRight(scalar_type dx, scalar_type dy) const { return point_type(x+width-dx, y+height-dy); }

        inline point_type topLeft    (point_type const& p) const { return topLeft    (p.x, p.y); }
        inline point_type topRight   (point_type const& p) const { return topRight   (p.x, p.y); }
        inline point_type bottomLeft (point_type const& p) const { return bottomLeft (p.x, p.y); }
        inline point_type bottomRight(point_type const& p) const { return bottomRight(p.x, p.y); }

        inline rect_type topLeft    (scalar_type dx, scalar_type dy, scalar_type w, scalar_type h) const { return rect_type(x+dx        , y+dy         , w, h); }
        inline rect_type topRight   (scalar_type dx, scalar_type dy, scalar_type w, scalar_type h) const { return rect_type(x+width-dx-w, y+dy         , w, h); }
        inline rect_type bottomLeft (scalar_type dx, scalar_type dy, scalar_type w, scalar_type h) const { return rect_type(x+dx        , y+height-dy-h, w, h); }
        inline rect_type bottomRight(scalar_type dx, scalar_type dy, scalar_type w, scalar_type h) const { return rect_type(x+width-dx-w, y+height-dy-h, w, h); }

        inline rect_type topLeft    (rect_type const& r) const { return topLeft    (r.x, r.y, r.width, r.height); }
        inline rect_type topRight   (rect_type const& r) const { return topRight   (r.x, r.y, r.width, r.height); }
        inline rect_type bottomLeft (rect_type const& r) const { return bottomLeft (r.x, r.y, r.width, r.height); }
        inline rect_type bottomRight(rect_type const& r) const { return bottomRight(r.x, r.y, r.width, r.height); }

        inline rect_type top   (scalar_type dy, scalar_type h) const { return rect_type( x           , y+dy         , width, h      ); }
        inline rect_type bottom(scalar_type dy, scalar_type h) const { return rect_type( x           , y+height-dy-h, width, h      ); }
        inline rect_type left  (scalar_type dx, scalar_type w) const { return rect_type( x+dx        , y            , w    , height ); }
        inline rect_type right (scalar_type dx, scalar_type w) const { return rect_type( x+width-dx-w, y            , w    , height ); }

        inline rect_type fit(scalar_type width, scalar_type height) const
        {
            double scale = std::min(
                double(this->width) / width, double(this->height) / height);
            width *= scale;
            height *= scale;

            scalar_type x = this->x + (this->width - width) / 2;
            scalar_type y = this->y + (this->height - height) / 2;
            return rect_type(x,y,width,height);
        }

        inline rect_type fit(scalar_type width, scalar_type height, double& scale) const
        {
            scale = std::min(
                double(this->width) / width, double(this->height) / height);
            width *= scale;
            height *= scale;

            scalar_type x = this->x + (this->width - width) / 2;
            scalar_type y = this->y + (this->height - height) / 2;
            return rect_type(x,y,width,height);
        }

        inline rect_type pad(scalar_type left, scalar_type top, scalar_type right, scalar_type bottom) const
        {
            return rect_type { x + left, y + top, width - left - right, height - top - bottom };
        }

        inline rect_type pad(scalar_type horz, scalar_type vert) const
        {
            return rect_type { x + horz, y + vert, width - horz - horz, height - vert - vert };
        }

        inline rect_type pad(scalar_type pad) const
        {
            return rect_type { x + pad, y + pad, width - pad - pad, height - pad - pad };
        }

        inline split_type splitTop(scalar_type h) const
        {
            return split_type
            {
                rect_type{x,y,width,h},
                rect_type(x,y+h,width,height-h)
            };
        }

        inline split_type splitLeft(scalar_type w) const
        {
            return split_type
            {
                rect_type(x,y,w,height),
                rect_type(x+w,y,width-w,height)
            };
        }

        inline split_type splitBottom(scalar_type h) const
        {
            return split_type
            {
                rect_type(x,y+height-h,width,h),
                rect_type(x,y,width,height-h),
            };
        }

        inline static rect_type fromBound(point_type const& min, point_type const& max)
        {
            scalar_type x = min.x;
            scalar_type y = min.y;
            scalar_type width  = max.x - min.x;
            scalar_type height = max.y - min.y;
            return rect_type(x,y,width,height);
        }

        inline static rect_type fromBound(scalar_type min_x, scalar_type min_y, scalar_type max_x, scalar_type max_y)
        {
            scalar_type x = min_x;
            scalar_type y = min_y;
            scalar_type width  = max_x - min_x;
            scalar_type height = max_y - min_y;
            return rect_type(x,y,width,height);
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
