#pragma once

#include "Rect.hpp"

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr


namespace ut
{
    /// Similar to Rect. Represented by min and max coordinates instead of position and size.
    /// Makes some calculations more efficient.
    template <typename N>
    struct regionx
    {
        using scalar_type       = N;
        using real_type         = real_t;
        using region_type       = regionx<N>;
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
                : min{rect.x, rect.y}, max{rect.x+rect.w, rect.y+rect.h}
        { }

        constexpr regionx(regionx const&)=default;
        constexpr regionx(regionx&&) noexcept =default;

        constexpr regionx& operator=(regionx const&)=default;
        constexpr regionx& operator=(regionx&&) noexcept =default;

        constexpr explicit operator rect_type() const { return rect(); }

        //
        // accessors
        //

        M_DECL_PURE bool inverted() const { return min.x > max.x || min.y > max.y; }

        M_DECL_PURE scalar_type area() const { return width() * height(); }

        M_DECL_PURE point_type pos   () const { return min; }
        M_DECL_PURE point_type size  () const { return max - min; }
        M_DECL_PURE point_type center() const { return min + (size() / 2); }

        M_DECL_PURE scalar_type x() const { return min.x; }
        M_DECL_PURE scalar_type y() const { return min.y; }

        M_DECL_PURE scalar_type width () const { return max.x - min.x; }
        M_DECL_PURE scalar_type height() const { return max.y - min.y; }

        M_DECL_PURE point_type tl() const { return min; }              /// Top Left
        M_DECL_PURE point_type tr() const { return {max.x, min.y }; }  /// Top Right
        M_DECL_PURE point_type bl() const { return {min.x, max.y }; }  /// Bottom Left
        M_DECL_PURE point_type br() const { return max; }              /// Bottom Right

        M_DECL_PURE point_type tc() const { return {min.x + (max.x - min.x) / 2, min.y }; } /// Top Center
        M_DECL_PURE point_type bc() const { return {min.x + (max.x - min.x) / 2, max.y }; } /// Bottom Center
        M_DECL_PURE point_type lc() const { return {min.x, min.y + (max.y - min.y) / 2 }; } /// Left Center
        M_DECL_PURE point_type rc() const { return {max.x, min.y + (max.y - min.y) / 2 }; } /// Right Center

        template <typename T>
        M_DECL_PURE regionx<T> cast() const { return regionx<T>(min.template cast<T>(), max.template cast<T>()); }

        template <typename T=N>
        M_DECL_PURE rectx<T> rect() const { return rectx<T>(T(min.x), T(min.y), T(width()), T(height())); }

        //
        // mutators
        //

        M_DECL void pos(scalar_type x, scalar_type y) { pos({x,y}); }
        M_DECL void pos(point_type const& p) { auto tmp = max - min; min = p; max = min + tmp; }

        M_DECL void translate (point_type  p) { min += p; max += p; }
        M_DECL void translateX(scalar_type o) { min.x += o; max.x += o; }
        M_DECL void translateY(scalar_type o) { min.y += o; max.y += o; }

        M_DECL void size(scalar_type w, scalar_type h) { size({w,h}); }
        M_DECL void size(point_type const& p) { max = min + p; }

        M_DECL void width(scalar_type s) { max.x = min.x + s; }
        M_DECL void height(scalar_type s) { max.y = min.y + s; }

        //
        // utility
        //

        ENABLE_IF_FLOAT M_DECL_PURE region_type round() const { return region_type(min.round(), max.round()); }
        ENABLE_IF_FLOAT M_DECL_PURE region_type floor() const { return region_type(min.floor(), max.floor()); }
        ENABLE_IF_FLOAT M_DECL_PURE region_type ceil () const { return region_type(min.ceil (), max.ceil ()); }

        //
        // bound checks
        //

        M_DECL_PURE bool contains(point_type  const& p) const { return p.x >= min.x && p.y >= min.y && p.x < max.x && p.y < max.y; }
        M_DECL_PURE bool contains(region_type const& r) const { return r.min.x >= min.x && r.min.y >= min.y && r.max.x <= max.x && r.max.y <= max.y; }
        M_DECL_PURE bool overlaps(region_type const& r) const { return r.min.y < max.y && r.max.y > min.y && r.Min.x < max.x && r.max.x > min.x; }

        //
        // alignment helpers
        //

        M_DECL_PURE region_type alignTLtoTL(scalar_type w, scalar_type h) { return {{min.x  , min.y   }, {min.x + w, min.y + h } }; }
        M_DECL_PURE region_type alignTLtoTR(scalar_type w, scalar_type h) { return {{min.x - w, min.y   }, {min.x  , min.y + h } }; }
        M_DECL_PURE region_type alignTLtoBL(scalar_type w, scalar_type h) { return {{min.x  , min.y - h }, {min.x + w, min.y   } }; }
        M_DECL_PURE region_type alignTLtoBR(scalar_type w, scalar_type h) { return {{min.x - w, min.y - h }, {min.x  , min.y   } }; }

        M_DECL_PURE region_type alignTRtoTL(scalar_type w, scalar_type h) { return {{max.x  , min.y   }, {max.x + w, min.y + h } }; }
        M_DECL_PURE region_type alignTRtoTR(scalar_type w, scalar_type h) { return {{max.x - w, min.y   }, {max.x  , min.y + h } }; }
        M_DECL_PURE region_type alignTRtoBL(scalar_type w, scalar_type h) { return {{max.x  , min.y - h }, {max.x + w, min.y   } }; }
        M_DECL_PURE region_type alignTRtoBR(scalar_type w, scalar_type h) { return {{max.x - w, min.y - h }, {max.x  , min.y   } }; }

        M_DECL_PURE region_type alignBLtoTL(scalar_type w, scalar_type h) { return {{min.x  , max.y   }, {min.x + w, max.y + h } }; }
        M_DECL_PURE region_type alignBLtoTR(scalar_type w, scalar_type h) { return {{min.x - w, max.y   }, {min.x  , max.y + h } }; }
        M_DECL_PURE region_type alignBLtoBL(scalar_type w, scalar_type h) { return {{min.x  , max.y - h }, {min.x + w, max.y   } }; }
        M_DECL_PURE region_type alignBLtoBR(scalar_type w, scalar_type h) { return {{min.x - w, max.y - h }, {min.x  , max.y   } }; }

        M_DECL_PURE region_type alignBRtoTL(scalar_type w, scalar_type h) { return {{max.x  , max.y   }, {max.x + w, max.y + h } }; }
        M_DECL_PURE region_type alignBRtoTR(scalar_type w, scalar_type h) { return {{max.x - w, max.y   }, {max.x  , max.y + h } }; }
        M_DECL_PURE region_type alignBRtoBL(scalar_type w, scalar_type h) { return {{max.x  , max.y - h }, {max.x + w, max.y   } }; }
        M_DECL_PURE region_type alignBRtoBR(scalar_type w, scalar_type h) { return {{max.x - w, max.y - h }, {max.x  , max.y   } }; }

#define PADX  w=(width()-w)/2;
#define PADY  h=(height()-h)/2;
#define HALFX w=w/2;
#define HALFY h=h/2;

        M_DECL_PURE region_type alignTCtoTC(scalar_type w, scalar_type h) { PADX return {{min.x + w, min.y   }, {max.x - w, min.y + h } }; }
        M_DECL_PURE region_type alignTCtoBC(scalar_type w, scalar_type h) { PADX return {{min.x + w, min.y - h }, {max.x - w, min.y   } }; }
        M_DECL_PURE region_type alignBCtoTC(scalar_type w, scalar_type h) { PADX return {{min.x + w, max.y   }, {max.x - w, max.y + h } }; }
        M_DECL_PURE region_type alignBCtoBC(scalar_type w, scalar_type h) { PADX return {{min.x + w, max.y - h }, {max.x - w, max.y   } }; }

        M_DECL_PURE region_type alignLCtoLC(scalar_type w, scalar_type h) { PADY return {{min.x  , min.y + h }, {min.x + w, max.y - h } }; }
        M_DECL_PURE region_type alignLCtoRC(scalar_type w, scalar_type h) { PADY return {{min.x - w, min.y + h }, {min.x  , max.y - h } }; }
        M_DECL_PURE region_type alignRCtoLC(scalar_type w, scalar_type h) { PADY return {{max.x  , min.y + h }, {max.x + w, max.y - h } }; }
        M_DECL_PURE region_type alignRCtoRC(scalar_type w, scalar_type h) { PADY return {{max.x - w, min.y + h }, {max.x  , max.y - h } }; }

        M_DECL_PURE region_type alignCCtoCC(scalar_type w, scalar_type h) { PADY PADX return {{min.x + w, min.y + h }, {max.x - w, max.y - h } }; }

#undef PADX
#undef PADY
#undef HALFX
#undef HALFY

        M_DECL_PURE region_type pad(point_type const& tl, point_type const& br) const
        {
            return { min + tl, max - br };
        }

        M_DECL_PURE region_type pad(scalar_type left, scalar_type top, scalar_type right, scalar_type bottom) const
        {
            return pad({left,top}, {right,bottom});
        }

        M_DECL_PURE region_type pad(scalar_type horz, scalar_type vert) const
        {
            return pad({horz,vert}, {horz,vert});
        }

        M_DECL_PURE region_type pad(scalar_type sz) const
        {
            return pad({sz,sz}, {sz,sz});
        }

        M_DECL_PURE region_type pad(fraction left, fraction top, fraction right, fraction bottom) const
        {
            assert(left.v + right.v <= .5);
            assert(top.v + bottom.v <= .5);
            return pad(left(width()), top(height()), right(width()), bottom(height()));
        }

        M_DECL_PURE region_type pad(fraction horz, fraction vert) const
        {
            assert(horz.v <= .5);
            assert(vert.v <= .5);
            return pad(horz(width()), vert(height()));
        }

        M_DECL_PURE region_type pad(fraction f) const
        {
            return pad(f, f);
        }

        M_DECL_PURE region_type fit(scalar_type dw, scalar_type dh) const
        {
            auto w = (real_type)width();
            auto h = (real_type)height();
            auto scale = std::min(w / dw, h / dh);

            dw *= scale;
            dh *= scale;

            auto x = min.x + (w - dw) / 2;
            auto y = min.y + (h - dh) / 2;

            return { x, y, x+dw, y+dh };
        }

        M_DECL_PURE region_type fit(scalar_type dw, scalar_type dh, real_type& scale) const
        {
            auto w = (real_type)width();
            auto h = (real_type)height();
            scale = std::min(w / dw, h / dh);

            dw *= scale;
            dh *= scale;

            auto x = min.x + (w - dw) / 2;
            auto y = min.y + (h - dh) / 2;

            return { x, y, x+dw, y+dh };
        }
    };

    using region = regionx<int>;

    typedef regionx<float>        regionf;
    typedef regionx<double>       regiond;
    typedef regionx<int>          regioni;
    typedef regionx<unsigned>     regionu;
    typedef regionx<std::uint8_t> regionb;

#if defined(UT_STL_INTEROP)
    template <typename N>
    inline std::ostream& operator<<(std::ostream& os, regionx<N> const& r)
    {
        return os << r.min << ", " << r.max;
    }

    template <typename N, size_t D>
    inline std::string to_string(regionx<N> const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }
#endif
}

#undef M_DECL_PURE