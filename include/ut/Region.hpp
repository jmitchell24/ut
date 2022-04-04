#pragma once

#include "Rect.hpp"

#define DECO_PURE [[nodiscard]] inline constexpr

namespace ut
{
    enum gravity
    {
        GRAVITY_NONE,
        GRAVITY_LEFT,
        GRAVITY_RIGHT,
        GRAVITY_TOP,
        GRAVITY_BOTTOM,
        GRAVITY_CENTER
    };

    /// Similar to Rect. Represented by min and max coordinates instead of position and size.
    /// Makes some calculations more efficient.
    template <typename N>
    struct regionx
    {
        using scalar_type       = N;
        using real_type         = real_t;
        using region_type       = rectx<N>;
        using rect_type         = rectx<N>;
        using point_type        = vec2x<N>;
        using split_type        = std::pair<region_type,region_type>;
        using elements_type     = scalar_type[4];

        point_type min, max;

        //
        // ctor
        //

        constexpr regionx()
            : min{0,0}, max{0,0}
        { }

        constexpr regionx(scalar_type min_x, scalar_type min_y, scalar_type max_x, scalar_type max_y)
            : min{min_x, min_y}, max{max_x, max_y}
        { }

        constexpr regionx(point_type const& min, point_type const& max)
            : min{min}, max{max}
        { }

        constexpr explicit regionx(rect_type const& rect)
                : min{rect.x, rect.y}, max{rect.x+rect.w, rect.y+rect.y}
        { }

        constexpr regionx(regionx const&)=default;
        constexpr regionx(regionx&&) noexcept =default;

        constexpr regionx& operator=(regionx const&)=default;
        constexpr regionx& operator=(regionx&&) noexcept =default;

        constexpr explicit operator rect_type() const { return rect(); }

        //
        // accessors
        //

        DECO_PURE bool inverted() const { return min.x > max.x || min.y > max.y; }

        DECO_PURE scalar_type area() const { return width() * height(); }

        DECO_PURE rect_type rect() const { return { min.x, min.y, width(), height() }; }

        DECO_PURE point_type pos   () const { return min; }
        DECO_PURE point_type size  () const { return max - min; }
        DECO_PURE point_type width () const { return max.x - min.x; }
        DECO_PURE point_type height() const { return max.y - min.y; }
        DECO_PURE point_type center() const { return min + (size() / 2); }

        DECO_PURE point_type tl() const { return min; }               /// Top Left
        DECO_PURE point_type tr() const { return { max.x, min.y }; }  /// Top Right
        DECO_PURE point_type bl() const { return { min.x, max.y }; }  /// Bottom Left
        DECO_PURE point_type br() const { return max; }               /// Bottom Right

        template <typename T>
        DECO_PURE regionx<T> cast() const { return regionx<T>(min.template cast<T>(), max.template cast<T>()); }

        //
        // utility
        //

        ENABLE_IF_FLOAT DECO_PURE region_type round() const { return region_type(min.round(), max.round()); }
        ENABLE_IF_FLOAT DECO_PURE region_type floor() const { return region_type(min.floor(), max.floor()); }
        ENABLE_IF_FLOAT DECO_PURE region_type ceil () const { return region_type(min.ceil (), max.ceil ()); }

        //
        // bound checks
        //

        DECO_PURE bool contains(point_type  const& p) const { return p.x     >= min.x && p.y     >= min.y && p.x     <  max.x && p.y     <  max.y; }
        DECO_PURE bool contains(region_type const& r) const { return r.min.x >= min.x && r.min.y >= min.y && r.max.x <= max.x && r.max.y <= max.y; }
        DECO_PURE bool overlaps(region_type const& r) const { return r.min.y <  max.y && r.max.y >  min.y && r.Min.x <  max.x && r.max.x >  min.x; }


        //
        // alignment helpers
        //


        DECO_PURE region_type alignOLT(scalar_type w, scalar_type h) { return { {min.x-w, min.y  }, {min.x,min.y+h} }; } /// Align Outside, Left, Top
        DECO_PURE region_type alignOLB(scalar_type w, scalar_type h) { return { {min.x-w, max.y-h}, {min.x,max.y-h} }; } /// Align Outside, Left, Bottom



        DECO_PURE region_type pad(point_type const& tl, point_type const& br) const
        {
            return { min + tl, max - br };
        }

        DECO_PURE region_type pad(scalar_type left, scalar_type top, scalar_type right, scalar_type bottom) const
        {
            return pad({left,top}, {right,bottom});
        }

        DECO_PURE region_type pad(scalar_type horz, scalar_type vert) const
        {
            return pad({horz,vert}, {horz,vert});
        }

        DECO_PURE region_type pad(scalar_type sz) const
        {
            return pad({sz,sz}, {sz,sz});
        }

        DECO_PURE region_type pad(fraction left, fraction top, fraction right, fraction bottom) const
        {
            assert(left.v + right.v <= .5);
            assert(top.v + bottom.v <= .5);
            return pad(left(width()), top(height()), right(width()), bottom(height()));
        }

        DECO_PURE region_type pad(fraction horz, fraction vert) const
        {
            assert(horz.v <= .5);
            assert(vert.v <= .5);
            return pad(horz(width()), vert(height()));
        }

        DECO_PURE region_type pad(fraction f) const
        {
            return pad(f, f);
        }
    };
}

#undef DECO_PURE