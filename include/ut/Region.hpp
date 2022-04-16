#pragma once

#include "Rect.hpp"

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define ENABLE_IF_INCLUSIVE template <bool I_ = I, typename = std::enable_if_t<I>>

namespace ut
{
    /// Similar to Rect. Represented by min and max coordinates instead of position and size.
    /// Makes some calculations more efficient.
    ///
    /// 'I' indicates the use of Inclusive Maximum for all size (width/height) calculations. (e.g. "(1,1), (2,2)" will
    /// have size "(2,2)", instead of "(1,1)". It is disallowed for non-integer types.
    template <typename N, bool I = false>
    struct regionx
    {
        static_assert(std::is_integral_v<N> || !I, "Inclusive Maximum only supported for integral types");

        using scalar_type       = N;
        using real_type         = real_t;
        using region_type       = regionx<N>;
        using rect_type         = rectx<N>;
        using point_type        = vec2x<N>;
        using split_type        = std::tuple<region_type,region_type>;
        using fit_type          = std::tuple<scalar_type,scalar_type>;

        constexpr static bool INCLUSIVE = I;

        point_type min, max;

        //
        // ctor
        //

        M_DECL regionx(scalar_type min_x, scalar_type min_y, scalar_type max_x, scalar_type max_y)
            : min{min_x, min_y}, max{max_x, max_y}
        { }

        M_DECL regionx(point_type const& min, point_type const& max)
            : min{min}, max{max}
        { }

        M_DECL explicit regionx(rect_type const& rect)
            : min{rect.x, rect.y}, max{rect.x+rect.w, rect.y+rect.h}
        { }

        M_DECL regionx()=default;
        M_DECL regionx(regionx const&)=default;
        M_DECL regionx(regionx&&) noexcept =default;

        M_DECL regionx& operator=(regionx const&)=default;
        M_DECL regionx& operator=(regionx&&) noexcept =default;

        M_DECL explicit operator rect_type() const { return rect(); }

        //
        // accessors
        //

        M_DECL_PURE bool inverted() const { return min.x > max.x || min.y > max.y; }

        M_DECL_PURE scalar_type area() const { return width() * height(); }

        M_DECL_PURE point_type size  () const { if constexpr(INCLUSIVE) return max - min + point_type{1,1}; else return max - min; }
        M_DECL_PURE point_type pos   () const { return min; }
        M_DECL_PURE point_type center() const { return min + (size() / 2); }

        M_DECL_PURE scalar_type x() const { return min.x; }
        M_DECL_PURE scalar_type y() const { return min.y; }

        M_DECL_PURE scalar_type width () const { if constexpr (INCLUSIVE) return max.x - min.x + 1; else return max.x - min.x; }
        M_DECL_PURE scalar_type height() const { if constexpr (INCLUSIVE) return max.y - min.y + 1; else return max.y - min.y; }

        M_DECL_PURE point_type tl() const { return min; }              /// Top Left
        M_DECL_PURE point_type tr() const { return {max.x, min.y }; }  /// Top Right
        M_DECL_PURE point_type bl() const { return {min.x, max.y }; }  /// Bottom Left
        M_DECL_PURE point_type br() const { return max; }              /// Bottom Right

        M_DECL_PURE point_type tc() const { return {min.x + (width()) / 2, min.y }; } /// Top Center
        M_DECL_PURE point_type bc() const { return {min.x + (width()) / 2, max.y }; } /// Bottom Center
        M_DECL_PURE point_type lc() const { return {min.x, min.y + (height()) / 2 }; } /// Left Center
        M_DECL_PURE point_type rc() const { return {max.x, min.y + (height()) / 2 }; } /// Right Center

        template <typename T>
        M_DECL_PURE regionx<T> cast() const { return regionx<T>(min.template cast<T>(), max.template cast<T>()); }

        template <typename T=N>
        M_DECL_PURE rectx<T> rect() const { return rectx<T>(T(min.x), T(min.y), T(width()), T(height())); }

        //
        // mutators
        //

        M_DECL void set(scalar_type min_x, scalar_type min_y, scalar_type max_x, scalar_type max_y)
        { min.set(min_x, min_y); max.set(max_x, max_y); }

        M_DECL void set(point_type const& min, point_type const& max)
        { this->min.set(min); this->max.set(max); }

        M_DECL void set(rect_type const& r)
        { min.set(r.x, r.y); max.set(r.x + r.w, r.y + r.h); }

        M_DECL void set(region_type const& r)
        { min = r.min; max = r.max; }

        M_DECL void pos(scalar_type x, scalar_type y) { pos({x,y}); }
        M_DECL void pos(point_type const& p) { offset(p - min); }

        M_DECL void posX(scalar_type s) { offsetX(s - min.x); }
        M_DECL void posY(scalar_type s) { offsetY(s - min.y); }

        M_DECL void offset (scalar_type x, scalar_type y) { point_type p{x,y}; min += p; max += p; }
        M_DECL void offset (point_type  p) { offset(p.x, p.y); }

        M_DECL void offsetX(scalar_type o) { min.x += o; max.x += o; }
        M_DECL void offsetY(scalar_type o) { min.y += o; max.y += o; }

        M_DECL void width(scaler sc)     { width(sc(width())); }
        M_DECL void width(scalar_type s) { if constexpr(INCLUSIVE) max.x = min.x + s - 1; else max.x = min.x + s; }

        M_DECL void height(scaler sc)     { height(sc(height())); }
        M_DECL void height(scalar_type s) { if constexpr(INCLUSIVE) max.y = min.y + s - 1; else max.y = min.y + s; }

        M_DECL void size(scaler scw, scaler sch)       { size(scw(width()), sch(height())); }
        M_DECL void size(scalar_type w, scalar_type h) { size({w,h}); }
        M_DECL void size(point_type const& p)          { if constexpr (INCLUSIVE) max = min + p - point_type{1,1}; else max = min + p; }

        //
        // copy mutators
        //

        M_DECL_PURE region_type withMin(point_type const& p) { auto tmp = *this; tmp.min = p; return tmp; }
        M_DECL_PURE region_type withMax(point_type const& p) { auto tmp = *this; tmp.max = p; return tmp; }

#define MUT(op) auto tmp = *this; tmp.op; return tmp;
        M_DECL_PURE region_type withPos(scalar_type x, scalar_type y) const { MUT(pos({x,y})); }
        M_DECL_PURE region_type withPos(point_type const& p)          const { MUT(pos(p)) }

        M_DECL_PURE region_type withPosX(scalar_type s) const { MUT(x(s)) }
        M_DECL_PURE region_type withPosY(scalar_type s) const { MUT(y(s)) }

        M_DECL_PURE region_type withOffsetX(scalar_type s)       const { MUT(translateX(s)) }
        M_DECL_PURE region_type withOffsetY(scalar_type s)       const { MUT(translateY(s)); }
        M_DECL_PURE region_type withOffset (point_type const& p) const { MUT(translate(p)) }

        M_DECL_PURE region_type withWidth (scaler sc)     const { MUT(width(sc)) }
        M_DECL_PURE region_type withWidth (scalar_type s) const { MUT(width(s)); }

        M_DECL_PURE region_type withHeight(scaler sc)     const { MUT(height(sc)) }
        M_DECL_PURE region_type withHeight(scalar_type s) const { MUT(height(s)); }

        M_DECL_PURE region_type withSize(scaler scw, scaler sch)       const { MUT(size(scw, sch)) }
        M_DECL_PURE region_type withSize(scalar_type w, scalar_type h) const { MUT(size(w,h)) }
        M_DECL_PURE region_type withSize(point_type const& p)          const { MUT(size(p)) }
#undef MUT

        //
        // set ops
        //

        M_DECL void add(region_type const& r)
        {
            if ((r.min.x < r.max.x) && (r.min.y < r.max.y))
            {
                if ((min.x < max.x) && (min.y < max.y))
                {
                    if (min.x > r.min.x) min.x = r.min.x;
                    if (min.y > r.min.y) min.y = r.min.y;
                    if (max.x < r.max.x) max.x = r.max.x;
                    if (max.y < r.max.y) max.y = r.max.y;
                }
                else
                {
                    min.x = r.min.x;
                    min.y = r.min.y;
                    max.x = r.max.x;
                    max.y = r.max.y;
                }
            }
        }

        M_DECL void add(point_type const& p)
        {
            if (p.x < min.x)
                min.x = p.x;
            else if (p.x > max.x)
                max.x = p.x;

            if (p.y < min.y)
                min.y = p.y;
            else if (p.y > max.y)
                max.y = p.y;
        }

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
        // alignment
        //

#define DECL_ALIGN(__name__) \
    M_DECL_PURE region_type __name__(point_type const& s) const { return __name__(s.x, s.y); } \
    M_DECL_PURE region_type __name__

        DECL_ALIGN(alignedTLtoTL) (scalar_type w, scalar_type h) const { return {{min.x    , min.y     }, {min.x + w, min.y + h } }; }
        DECL_ALIGN(alignedTLtoTR) (scalar_type w, scalar_type h) const { return {{min.x - w, min.y     }, {min.x    , min.y + h } }; }
        DECL_ALIGN(alignedTLtoBL) (scalar_type w, scalar_type h) const { return {{min.x    , min.y - h }, {min.x + w, min.y     } }; }
        DECL_ALIGN(alignedTLtoBR) (scalar_type w, scalar_type h) const { return {{min.x - w, min.y - h }, {min.x    , min.y     } }; }

        DECL_ALIGN(alignedTRtoTL) (scalar_type w, scalar_type h) const { return {{max.x    , min.y     }, {max.x + w, min.y + h } }; }
        DECL_ALIGN(alignedTRtoTR) (scalar_type w, scalar_type h) const { return {{max.x - w, min.y     }, {max.x    , min.y + h } }; }
        DECL_ALIGN(alignedTRtoBL) (scalar_type w, scalar_type h) const { return {{max.x    , min.y - h }, {max.x + w, min.y     } }; }
        DECL_ALIGN(alignedTRtoBR) (scalar_type w, scalar_type h) const { return {{max.x - w, min.y - h }, {max.x    , min.y     } }; }

        DECL_ALIGN(alignedBLtoTL) (scalar_type w, scalar_type h) const { return {{min.x    , max.y     }, {min.x + w, max.y + h } }; }
        DECL_ALIGN(alignedBLtoTR) (scalar_type w, scalar_type h) const { return {{min.x - w, max.y     }, {min.x    , max.y + h } }; }
        DECL_ALIGN(alignedBLtoBL) (scalar_type w, scalar_type h) const { return {{min.x    , max.y - h }, {min.x + w, max.y     } }; }
        DECL_ALIGN(alignedBLtoBR) (scalar_type w, scalar_type h) const { return {{min.x - w, max.y - h }, {min.x    , max.y     } }; }

        DECL_ALIGN(alignedBRtoTL) (scalar_type w, scalar_type h) const { return {{max.x    , max.y     }, {max.x + w, max.y + h } }; }
        DECL_ALIGN(alignedBRtoTR) (scalar_type w, scalar_type h) const { return {{max.x - w, max.y     }, {max.x    , max.y + h } }; }
        DECL_ALIGN(alignedBRtoBL) (scalar_type w, scalar_type h) const { return {{max.x    , max.y - h }, {max.x + w, max.y     } }; }
        DECL_ALIGN(alignedBRtoBR) (scalar_type w, scalar_type h) const { return {{max.x - w, max.y - h }, {max.x    , max.y     } }; }

#define PADX  w=(width()-w)/2;
#define PADY  h=(height()-h)/2;
#define HALFX w=w/2;
#define HALFY h=h/2;

        DECL_ALIGN(alignedTCtoTC) (scalar_type w, scalar_type h) const { PADX return {{min.x + w, min.y     }, {max.x - w, min.y + h } }; }
        DECL_ALIGN(alignedTCtoBC) (scalar_type w, scalar_type h) const { PADX return {{min.x + w, min.y - h }, {max.x - w, min.y     } }; }
        DECL_ALIGN(alignedBCtoTC) (scalar_type w, scalar_type h) const { PADX return {{min.x + w, max.y     }, {max.x - w, max.y + h } }; }
        DECL_ALIGN(alignedBCtoBC) (scalar_type w, scalar_type h) const { PADX return {{min.x + w, max.y - h }, {max.x - w, max.y     } }; }

        DECL_ALIGN(alignedLCtoLC) (scalar_type w, scalar_type h) const { PADY return {{min.x    , min.y + h }, {min.x + w, max.y - h } }; }
        DECL_ALIGN(alignedLCtoRC) (scalar_type w, scalar_type h) const { PADY return {{min.x - w, min.y + h }, {min.x    , max.y - h } }; }
        DECL_ALIGN(alignedRCtoLC) (scalar_type w, scalar_type h) const { PADY return {{max.x    , min.y + h }, {max.x + w, max.y - h } }; }
        DECL_ALIGN(alignedRCtoRC) (scalar_type w, scalar_type h) const { PADY return {{max.x - w, min.y + h }, {max.x    , max.y - h } }; }

        DECL_ALIGN(alignedCCtoCC) (scalar_type w, scalar_type h) const { PADY PADX return {{min.x + w, min.y + h }, {max.x - w, max.y - h } }; }

#undef PADX
#undef PADY
#undef HALFX
#undef HALFY

#undef DECL_ALIGN

        //
        // shrink
        //

        M_DECL_PURE region_type shrunk(point_type const& tl, point_type const& br) const
        { return { min + tl, max - br }; }

        M_DECL_PURE region_type shrunk(scalar_type left, scalar_type top, scalar_type right, scalar_type bottom) const
        { return shrunk({left,top}, {right,bottom}); }

        M_DECL_PURE region_type shrunk(scalar_type horz, scalar_type vert) const
        { return shrunk({horz,vert}, {horz,vert}); }

        M_DECL_PURE region_type shrunk(scalar_type sz) const
        { return shrunk({sz,sz}, {sz,sz}); }

        M_DECL_PURE region_type shrunk(scaler left, scaler top, scaler right, scaler bottom) const
        {
            assert(left.v + right.v <= .5);
            assert(top.v + bottom.v <= .5);
            return shrunk(left(width()), top(height()), right(width()), bottom(height()));
        }

        M_DECL_PURE region_type shrunk(scaler horz, scaler vert) const
        {
            assert(horz.v <= .5);
            assert(vert.v <= .5);
            return shrunk(horz(width()), vert(height()));
        }

        M_DECL_PURE region_type shrunk(scaler f) const
        { return shrunk(f, f); }

        //
        // expand
        //

        M_DECL_PURE region_type expanded(point_type const& tl, point_type const& br) const
        { return { min - tl, max + br }; }

        M_DECL_PURE region_type expanded(scalar_type left, scalar_type top, scalar_type right, scalar_type bottom) const
        { return expanded({left,top}, {right,bottom}); }

        M_DECL_PURE region_type expanded(scalar_type horz, scalar_type vert) const
        { return expanded({horz,vert}, {horz,vert}); }

        M_DECL_PURE region_type expanded(scalar_type sz) const
        { return expanded({sz,sz}, {sz,sz}); }

        M_DECL_PURE region_type expanded(scaler left, scaler top, scaler right, scaler bottom) const
        {
            assert(left.v + right.v <= .5);
            assert(top.v + bottom.v <= .5);
            return expanded(left(width()), top(height()), right(width()), bottom(height()));
        }

        M_DECL_PURE region_type expanded(scaler horz, scaler vert) const
        {
            assert(horz.v <= .5);
            assert(vert.v <= .5);
            return expanded(horz(width()), vert(height()));
        }

        M_DECL_PURE region_type expanded(scaler f) const
        { return expanded(f, f); }

        //
        // fit
        //

        M_DECL_PURE point_type fit(scalar_type dw, scalar_type dh) const
        {
            real_type scale = std::min((real_type)width() / dw, (real_type)height() / dh);
            return { dw*scale, dh*scale };
        }

        M_DECL_PURE point_type fit(point_type const& d) const
        { return fit(d.w, d.h); }

        M_DECL_PURE std::tuple<point_type, real_type> fitScale(scalar_type dw, scalar_type dh) const
        {
            real_type scale = std::min((real_type)width() / dw, (real_type)height() / dh);
            return { { dw*scale, dh*scale }, scale };
        }

        M_DECL_PURE std::tuple<point_type, real_type> fitScale(point_type const& d) const
        { return fitScale(d.w, d.h); }

        //
        // split
        //

        // TODO: add output parameter option (and maybe eliminate return value)
        M_DECL_PURE region_type splitV(scalar_type vmin, scalar_type vmax) const
        {
            assert(vmin <= vmax);
            assert(vmax >= min.y && vmax <= max.y);
            assert(vmin >= min.y && vmin <= max.y);
            return {{ min.x, vmin }, { max.x, vmax }};
        }

        M_DECL_PURE region_type splitH(scalar_type hmin, scalar_type hmax) const
        {
            assert(hmin <= hmax);
            assert(hmax >= min.x && hmax <= max.x);
            assert(hmin >= min.x && hmin <= max.x);
            return {{ min.x, hmin }, { max.x, hmax }};
        }

        M_DECL_PURE split_type splitTop(scalar_type dh) const
        {
            auto split = min.y + dh;
            return {splitV(min.y, split), splitV(split, max.y)};
        }

        M_DECL_PURE split_type splitBottom(scalar_type dh) const
        {
            auto split = max.y - dh;
            return {splitV(min.y, split), splitV(split, max.y)};
        }

        M_DECL_PURE split_type splitLeft(scalar_type dw) const
        {
            auto split = min.x + dw;
            return {splitH(min.x, split), splitV(split, max.x)};
        }

        M_DECL_PURE split_type splitRight(scalar_type dw) const
        {
            auto split = max.x - dw;
            return {splitH(min.x, split), splitV(split, max.x)};
        }
    };

    using region = regionx<int>;

    typedef regionx<float>        regionf;
    typedef regionx<double>       regiond;
    typedef regionx<int>          regioni;
    typedef regionx<unsigned>     regionu;
    typedef regionx<std::uint8_t> regionb;

#if defined(UT_STL_INTEROP)
    template <typename N, bool I>
    inline std::ostream& operator<<(std::ostream& os, regionx<N,I> const& r)
    {
        return os << r.min << ", " << r.max;
    }

    template <typename N, bool I>
    inline std::string to_string(regionx<N,I> const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }
#endif
}

#undef M_DECL_PURE
#undef M_DECL