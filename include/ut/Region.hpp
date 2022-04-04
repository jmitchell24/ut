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

        DECO_PURE bool inverted() const { return min.x > max.x || min.y > max.y; }

        DECO_PURE scalar_type area() const { return width() * height(); }

        DECO_PURE rect_type rect() const { return { min.x, min.y, width(), height() }; }

        DECO_PURE point_type pos   () const { return min; }
        DECO_PURE point_type size  () const { return max - min; }
        DECO_PURE point_type center() const { return min + (size() / 2); }

        DECO_PURE scalar_type width () const { return max.x - min.x; }
        DECO_PURE scalar_type height() const { return max.y - min.y; }

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

        DECO_PURE region_type alignTLtoTL(scalar_type w, scalar_type h) { return { { min.x  , min.y   }, { min.x+w, min.y+h } }; }
        DECO_PURE region_type alignTLtoTR(scalar_type w, scalar_type h) { return { { min.x-w, min.y   }, { min.x  , min.y+h } }; }
        DECO_PURE region_type alignTLtoBL(scalar_type w, scalar_type h) { return { { min.x  , min.y-h }, { min.x+w, min.y   } }; }
        DECO_PURE region_type alignTLtoBR(scalar_type w, scalar_type h) { return { { min.x-w, min.y-h }, { min.x  , min.y   } }; }

        DECO_PURE region_type alignTRtoTL(scalar_type w, scalar_type h) { return { { max.x  , min.y   }, { max.x+w, min.y+h } }; }
        DECO_PURE region_type alignTRtoTR(scalar_type w, scalar_type h) { return { { max.x-w, min.y   }, { max.x  , min.y+h } }; }
        DECO_PURE region_type alignTRtoBL(scalar_type w, scalar_type h) { return { { max.x  , min.y-h }, { max.x+w, min.y   } }; }
        DECO_PURE region_type alignTRtoBR(scalar_type w, scalar_type h) { return { { max.x-w, min.y-h }, { max.x  , min.y   } }; }

        DECO_PURE region_type alignBLtoTL(scalar_type w, scalar_type h) { return { { min.x  , max.y   }, { min.x+w, max.y+h } }; }
        DECO_PURE region_type alignBLtoTR(scalar_type w, scalar_type h) { return { { min.x-w, max.y   }, { min.x  , max.y+h } }; }
        DECO_PURE region_type alignBLtoBL(scalar_type w, scalar_type h) { return { { min.x  , max.y-h }, { min.x+w, max.y   } }; }
        DECO_PURE region_type alignBLtoBR(scalar_type w, scalar_type h) { return { { min.x-w, max.y-h }, { min.x  , max.y   } }; }

        DECO_PURE region_type alignBRtoTL(scalar_type w, scalar_type h) { return { { max.x  , max.y   }, { max.x+w, max.y+h } }; }
        DECO_PURE region_type alignBRtoTR(scalar_type w, scalar_type h) { return { { max.x-w, max.y   }, { max.x  , max.y+h } }; }
        DECO_PURE region_type alignBRtoBL(scalar_type w, scalar_type h) { return { { max.x  , max.y-h }, { max.x+w, max.y   } }; }
        DECO_PURE region_type alignBRtoBR(scalar_type w, scalar_type h) { return { { max.x-w, max.y-h }, { max.x  , max.y   } }; }

#define PADX w=(width()-w)/2;
#define PADY h=(height()-h)/2;
#define HALFX w=w/2;
#define HALFY h=h/2;

        DECO_PURE region_type alignTCtoTC(scalar_type w, scalar_type h) { PADX return { { min.x+w, min.y   }, { max.x-w, min.y+h } }; }
        DECO_PURE region_type alignTCtoBC(scalar_type w, scalar_type h) { PADX return { { min.x+w, min.y-h }, { max.x-w, min.y   } }; }
        DECO_PURE region_type alignBCtoTC(scalar_type w, scalar_type h) { PADX return { { min.x+w, max.y   }, { max.x-w, max.y+h } }; }
        DECO_PURE region_type alignBCtoBC(scalar_type w, scalar_type h) { PADX return { { min.x+w, max.y-h }, { max.x-w, max.y   } }; }

        DECO_PURE region_type alignLCtoLC(scalar_type w, scalar_type h) { PADY return { { min.x  , min.y+h }, { min.x+w, max.y-h } }; }
        DECO_PURE region_type alignLCtoRC(scalar_type w, scalar_type h) { PADY return { { min.x-w, min.y+h }, { min.x  , max.y-h } }; }
        //DECO_PURE region_type alignRCtoLC(scalar_type w, scalar_type h) { PADY return { { min.x, min.y }, { max.x, min.y } }; }
        //DECO_PURE region_type alignRCtoRC(scalar_type w, scalar_type h) { PADY return { { min.x, min.y }, { max.x, min.y } }; }


        //DECO_PURE region_type alignTCtoCC(scalar_type w, scalar_type h) { PADX HALFY return { { min.x+w,

#undef PADX
#undef PADY
#undef HALFX
#undef HALFY

//
//        DECO_PURE region_type alignTCtoBC(scalar_type w, scalar_type h) { /*return { { }, { } };*/ assert(!"not implemented");return {};  };
//        DECO_PURE region_type alignBCtoTC(scalar_type w, scalar_type h) { /*return { { }, { } };*/ assert(!"not implemented");return {};  };
//        DECO_PURE region_type alignBCtoBC(scalar_type w, scalar_type h) { /*return { { }, { } };*/ assert(!"not implemented");return {};  };
//
//        DECO_PURE region_type alignLCtoLC(scalar_type w, scalar_type h) { /*return { { }, { } };*/ assert(!"not implemented");return {};  };
//        DECO_PURE region_type alignLCtoRC(scalar_type w, scalar_type h) { /*return { { }, { } };*/ assert(!"not implemented");return {};  };
//        DECO_PURE region_type alignRCtoLC(scalar_type w, scalar_type h) { /*return { { }, { } };*/ assert(!"not implemented");return {};  };
//        DECO_PURE region_type alignRCtoRC(scalar_type w, scalar_type h) { /*return { { }, { } };*/ assert(!"not implemented");return {};  };


        //DECO_PURE region_type alignTLtoTR(scalar_type w, scalar_type h) { return { {min.x-w, min.y  }, {min.x  ,min.y+h} }; } /// Align Top Right to parent Top Left




        //DECO_PURE region_type alignOLB(scalar_type w, scalar_type h) { return { {min.x-w, max.y-h}, {min.x  ,max.y-h} }; } /// Align Outside, Left, Bottom

        //DECO_PURE region_type alignORT(scalar_type w, scalar_type h) { return { {max.x  , min.y  }, {max.x+w,min.y+h} }; } /// Align Outside, Right, Top
        //DECO_PURE region_type alignORB(scalar_type w, scalar_type h) { return { {max.x  , max.y  }, {max.x+w,max.y-h} }; } /// Align Outside, Right, Bottom

        //DECO_PURE region_type alignORT(scalar_type w, scalar_type h) { return { {max.x  , min.y  }, {max.x+w,min.y+h} }; } /// Align Outside, Right, Top
        //DECO_PURE region_type alignORB(scalar_type w, scalar_type h) { return { {max.x  , max.y  }, {max.x+w,max.y-h} }; } /// Align Outside, Right, Bottom



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

#undef DECO_PURE