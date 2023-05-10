#pragma once

#include "psize.hpp"

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define ENABLE_IF_INCLUSIVE template <bool I_ = I, typename = std::enable_if_t<I>>

#define UT_REGION_ALIGNMENTS \
    REGION_ALIGN(TLtoTL) \
    REGION_ALIGN(TLtoTR) \
    REGION_ALIGN(TLtoBL) \
    REGION_ALIGN(TLtoBR) \
    \
    REGION_ALIGN(TRtoTL) \
    REGION_ALIGN(TRtoTR) \
    REGION_ALIGN(TRtoBL) \
    REGION_ALIGN(TRtoBR) \
    \
    REGION_ALIGN(BLtoTL) \
    REGION_ALIGN(BLtoTR) \
    REGION_ALIGN(BLtoBL) \
    REGION_ALIGN(BLtoBR) \
    \
    REGION_ALIGN(BRtoTL) \
    REGION_ALIGN(BRtoTR) \
    REGION_ALIGN(BRtoBL) \
    REGION_ALIGN(BRtoBR) \
    \
    REGION_ALIGN(TCtoTC) \
    REGION_ALIGN(TCtoBC) \
    REGION_ALIGN(BCtoTC) \
    REGION_ALIGN(BCtoBC) \
    \
    REGION_ALIGN(LCtoLC) \
    REGION_ALIGN(LCtoRC) \
    REGION_ALIGN(RCtoLC) \
    REGION_ALIGN(RCtoRC) \
    \
    REGION_ALIGN(CCtoCC)


namespace ut
{
    /// Similar to Rect. Represented by min and max coordinates instead of position and size.
    /// Makes some calculations more efficient.
    ///
    /// 'I' indicates the use of Inclusive Maximum for all size (width/height) calculations. (e.g. "(1,1), (2,2)" will
    /// have size "(2,2)", instead of "(1,1)". It is disallowed for non-integer types.
    template <typename N, bool I = false>
    struct rectx
    {
        static_assert(std::is_integral_v<N> || !I, "Inclusive Maximum only supported for integral types");

        using scalar_type       = N;
        using real_type         = real_t;
        using region_type       = rectx<N,I>;
        using psize_type        = psizex<N>;
        using point_type        = vec2x<N>;
        using split_type        = std::tuple<region_type,region_type>;
        //using fit_type          = std::tuple<scalar_type,scalar_type>;

        using pack_type = N[4];

        size_t static constexpr PACK_SIZE = 4;

        bool constexpr static INCLUSIVE = I;

        union
        {
            struct { scalar_type left, top, right, bottom; };
            struct { point_type min, max; };
            pack_type pack;
        };

        //
        // ctor
        //

        M_DECL rectx()
            : left{}, top{}, right{}, bottom{}
        {}

        M_DECL rectx(scalar_type left, scalar_type top, scalar_type right, scalar_type bottom)
            : left{left}, top{top}, right{right}, bottom{bottom}
        { }

        M_DECL rectx(point_type const& min, point_type const& max)
            : left{min.x}, top{min.y}, right{max.x}, bottom{max.y}
        { }

        M_DECL explicit rectx(psize_type const& psize)
            : left{psize.minX()}, top{psize.minY()}, right{psize.maxX()}, bottom{psize.maxY()}
        { }

        M_DECL rectx(rectx const&)=default;
        M_DECL rectx(rectx&&) noexcept =default;

        M_DECL rectx& operator=(rectx const&)=default;
        M_DECL rectx& operator=(rectx&&) noexcept =default;

        M_DECL explicit operator psize_type() const { return psize(); }

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

        M_DECL_PURE point_type tc() const { return point_type(min.x + (width()) / 2, min.y); } /// Top Center
        M_DECL_PURE point_type bc() const { return point_type(min.x + (width()) / 2, max.y); } /// Bottom Center
        M_DECL_PURE point_type lc() const { return point_type(min.x, min.y + (height()) / 2); } /// Left Center
        M_DECL_PURE point_type rc() const { return point_type(max.x, min.y + (height()) / 2); } /// Right Center

//        M_DECL_PURE scalar_type left    () const { return min.x; }
//        M_DECL_PURE scalar_type right   () const { return max.x; }
//        M_DECL_PURE scalar_type top     () const { return min.y; }
//        M_DECL_PURE scalar_type bottom  () const { return max.y; }

        template <typename T>
        M_DECL_PURE rectx<T> cast() const
        { return rectx<T>(static_cast<T>(left), static_cast<T>(top), static_cast<T>(right), static_cast<T>(bottom)); }


        template <typename T=N>
        M_DECL_PURE psizex<T> psize() const
        { return psizex<T>(static_cast<T>(width()), static_cast<T>(height()), static_cast<T>(min.x), static_cast<T>(min.y)); }

        //
        // mutators
        //

        M_DECL void set(scalar_type min_x, scalar_type min_y, scalar_type max_x, scalar_type max_y)
        { min.set(min_x, min_y); max.set(max_x, max_y); }

        M_DECL void set(point_type const& min, point_type const& max)
        { this->min.set(min); this->max.set(max); }

//        M_DECL void set(rect_type const& r)
//        { min.set(r.x, r.y); max.set(r.x + r.w, r.y + r.h); }

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

#define MUT(op_) auto tmp = *this; tmp.op_; return tmp;
        M_DECL_PURE region_type withPos(scalar_type x, scalar_type y) const { MUT(pos({x,y})); }
        M_DECL_PURE region_type withPos(point_type const& p)          const { MUT(pos(p)) }

        M_DECL_PURE region_type withPosX(scalar_type s) const { MUT(posX(s)) }
        M_DECL_PURE region_type withPosY(scalar_type s) const { MUT(posY(s)) }

        M_DECL_PURE region_type withOffsetX(scalar_type s)       const { MUT(offsetX(s)) }
        M_DECL_PURE region_type withOffsetY(scalar_type s)       const { MUT(offsetY(s)); }
        M_DECL_PURE region_type withOffset (point_type const& p) const { MUT(offset(p)) }

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
        M_DECL_PURE bool overlaps(region_type const& r) const { return r.min.y < max.y && r.max.y > min.y && r.min.x < max.x && r.max.x > min.x; }

        //
        // alignment
        //

#define DECL_ALIGN(__name__) \
    M_DECL_PURE region_type __name__(point_type const& s) const { return __name__(s.x, s.y); } \
    M_DECL_PURE region_type __name__

        DECL_ALIGN(alignedTLtoTL) (scalar_type w, scalar_type h) const { return { point_type(min.x    , min.y    ), point_type(min.x + w, min.y + h) }; }
        DECL_ALIGN(alignedTLtoTR) (scalar_type w, scalar_type h) const { return { point_type(min.x - w, min.y    ), point_type(min.x    , min.y + h) }; }
        DECL_ALIGN(alignedTLtoBL) (scalar_type w, scalar_type h) const { return { point_type(min.x    , min.y - h), point_type(min.x + w, min.y    ) }; }
        DECL_ALIGN(alignedTLtoBR) (scalar_type w, scalar_type h) const { return { point_type(min.x - w, min.y - h), point_type(min.x    , min.y    ) }; }

        DECL_ALIGN(alignedTRtoTL) (scalar_type w, scalar_type h) const { return { point_type(max.x    , min.y    ), point_type(max.x + w, min.y + h) }; }
        DECL_ALIGN(alignedTRtoTR) (scalar_type w, scalar_type h) const { return { point_type(max.x - w, min.y    ), point_type(max.x    , min.y + h) }; }
        DECL_ALIGN(alignedTRtoBL) (scalar_type w, scalar_type h) const { return { point_type(max.x    , min.y - h), point_type(max.x + w, min.y    ) }; }
        DECL_ALIGN(alignedTRtoBR) (scalar_type w, scalar_type h) const { return { point_type(max.x - w, min.y - h), point_type(max.x    , min.y    ) }; }

        DECL_ALIGN(alignedBLtoTL) (scalar_type w, scalar_type h) const { return { point_type(min.x    , max.y    ), point_type(min.x + w, max.y + h) }; }
        DECL_ALIGN(alignedBLtoTR) (scalar_type w, scalar_type h) const { return { point_type(min.x - w, max.y    ), point_type(min.x    , max.y + h) }; }
        DECL_ALIGN(alignedBLtoBL) (scalar_type w, scalar_type h) const { return { point_type(min.x    , max.y - h), point_type(min.x + w, max.y    ) }; }
        DECL_ALIGN(alignedBLtoBR) (scalar_type w, scalar_type h) const { return { point_type(min.x - w, max.y - h), point_type(min.x    , max.y    ) }; }

        DECL_ALIGN(alignedBRtoTL) (scalar_type w, scalar_type h) const { return { point_type(max.x    , max.y    ), point_type(max.x + w, max.y + h) }; }
        DECL_ALIGN(alignedBRtoTR) (scalar_type w, scalar_type h) const { return { point_type(max.x - w, max.y    ), point_type(max.x    , max.y + h) }; }
        DECL_ALIGN(alignedBRtoBL) (scalar_type w, scalar_type h) const { return { point_type(max.x    , max.y - h), point_type(max.x + w, max.y    ) }; }
        DECL_ALIGN(alignedBRtoBR) (scalar_type w, scalar_type h) const { return { point_type(max.x - w, max.y - h), point_type(max.x    , max.y    ) }; }

#define PADX  w=(width()-w)/2;
#define PADY  h=(height()-h)/2;
#define HALFX w=w/2;
#define HALFY h=h/2;

        DECL_ALIGN(alignedTCtoTC) (scalar_type w, scalar_type h) const { PADX return { point_type(min.x + w, min.y    ), point_type(max.x - w, min.y + h) }; }
        DECL_ALIGN(alignedTCtoBC) (scalar_type w, scalar_type h) const { PADX return { point_type(min.x + w, min.y - h), point_type(max.x - w, min.y    ) }; }
        DECL_ALIGN(alignedBCtoTC) (scalar_type w, scalar_type h) const { PADX return { point_type(min.x + w, max.y    ), point_type(max.x - w, max.y + h) }; }
        DECL_ALIGN(alignedBCtoBC) (scalar_type w, scalar_type h) const { PADX return { point_type(min.x + w, max.y - h), point_type(max.x - w, max.y    ) }; }

        DECL_ALIGN(alignedLCtoLC) (scalar_type w, scalar_type h) const { PADY return { point_type(min.x    , min.y + h), point_type(min.x + w, max.y - h) }; }
        DECL_ALIGN(alignedLCtoRC) (scalar_type w, scalar_type h) const { PADY return { point_type(min.x - w, min.y + h), point_type(min.x    , max.y - h) }; }
        DECL_ALIGN(alignedRCtoLC) (scalar_type w, scalar_type h) const { PADY return { point_type(max.x    , min.y + h), point_type(max.x + w, max.y - h) }; }
        DECL_ALIGN(alignedRCtoRC) (scalar_type w, scalar_type h) const { PADY return { point_type(max.x - w, min.y + h), point_type(max.x    , max.y - h) }; }

        DECL_ALIGN(alignedCCtoCC) (scalar_type w, scalar_type h) const { PADY PADX return { point_type(min.x + w, min.y + h), point_type(max.x - w, max.y - h) }; }

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
            return point_type( dw*scale, dh*scale );
        }

        M_DECL_PURE point_type fit(point_type const& d) const
        { return fit(d.x, d.y); }

        M_DECL_PURE std::tuple<point_type, real_type> fitScale(scalar_type dw, scalar_type dh) const
        {
            real_type scale = std::min((real_type)width() / dw, (real_type)height() / dh);
            return { point_type( dw*scale, dh*scale ), scale };
        }

        M_DECL_PURE std::tuple<point_type, real_type> fitScale(point_type const& d) const
        { return fitScale(d.x, d.y); }

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
            return {{ hmin, min.y }, { hmax, max.y }};
        }

        template <size_t Count>
        M_DECL_PURE std::array<region_type, Count> splitNV(scalar_type inner_margin = 0) const
        {
            static_assert(Count > 0);

            auto step   = (height() - (inner_margin * (Count - 1)) ) / Count;
            auto vmin   = min.y;
            auto vmax   = vmin + step;

            assert(step > 0 && "total inner margin must be less than height");

            std::array<region_type, Count> tmp;
            tmp[0] = {{ min.x, vmin }, { max.x, vmax }};

            step += inner_margin;

            for (size_t i = 1; i < Count; ++i)
            {
                vmin += step;
                vmax += step;
                tmp[i] = {{ min.x, vmin }, { max.x, vmax }};
            }
            return tmp;
        }

        template <size_t Count>
        M_DECL_PURE std::array<region_type, Count> splitNH(scalar_type inner_margin = 0) const
        {
            static_assert(Count > 0);

            auto step   = (width() - (inner_margin * (Count - 1)) ) / Count;
            auto hmin   = min.x;
            auto hmax   = hmin + step;

            assert(step > 0 && "total inner margin must be less than height");

            std::array<region_type, Count> tmp;
            tmp[0] = {{ min.x, hmin }, { max.x, hmax }};

            step += inner_margin;

            for (size_t i = 1; i < Count; ++i)
            {
                hmin += step;
                hmax += step;
                tmp[i] = {{ min.x, hmin }, { max.x, hmax }};
            }
            return tmp;
        }

        M_DECL_PURE split_type splitTop(scalar_type dh, scalar_type inner_margin = 0) const
        {
            auto split = min.y + dh;
            return { splitV(min.y, split), splitV(split+inner_margin, max.y) };
        }

        M_DECL_PURE split_type splitBottom(scalar_type dh, scalar_type inner_margin = 0) const
        {
            auto split = max.y - dh;
            return {splitV(min.y, split-inner_margin), splitV(split, max.y)};
        }

        M_DECL_PURE split_type splitLeft(scalar_type dw, scalar_type inner_margin = 0) const
        {
            auto split = min.x + dw;
            return {splitH(min.x, split), splitH(split+inner_margin, max.x)};
        }

        M_DECL_PURE split_type splitRight(scalar_type dw, scalar_type inner_margin = 0) const
        {
            auto split = max.x - dw;
            return {splitH(min.x, split-inner_margin), splitH(split, max.x)};
        }

        //
        // container utilities
        //

        M_DECL_PURE scalar_type  operator[] (size_t i) const { assert(i < PACK_SIZE); return pack[i]; }
        M_DECL      scalar_type& operator[] (size_t i)       { assert(i < PACK_SIZE); return pack[i]; }

        M_DECL_PURE scalar_type const* begin() const { return pack; }
        M_DECL      scalar_type*       begin()       { return pack; }

        M_DECL_PURE scalar_type const* end() const { return pack+PACK_SIZE; }
        M_DECL      scalar_type*       end()       { return pack+PACK_SIZE; }

        M_DECL_PURE scalar_type const* data() const { return pack; }
        M_DECL      scalar_type*       data()       { return pack; }

    private:
        //
        // pack layout check
        //

        M_DECL rectx(std::nullptr_t);
    };

    template <typename N, bool I> constexpr rectx<N,I>::rectx(std::nullptr_t)
    {
        using rect_t = rectx<N, I>;

        auto constexpr off0 = offsetof(rect_t, min) + offsetof(rect_t::point_type, x);
        auto constexpr off1 = offsetof(rect_t, min) + offsetof(rect_t::point_type, y);
        auto constexpr off2 = offsetof(rect_t, max) + offsetof(rect_t::point_type, x);
        auto constexpr off3 = offsetof(rect_t, max) + offsetof(rect_t::point_type, y);

        static_assert(off0 == offsetof(rect_t, pack) + sizeof(N) * 0, "wrong 'min.x' layout in pack");
        static_assert(off1 == offsetof(rect_t, pack) + sizeof(N) * 1, "wrong 'min.y' layout in pack");
        static_assert(off2 == offsetof(rect_t, pack) + sizeof(N) * 2, "wrong 'max.x' layout in pack");
        static_assert(off3 == offsetof(rect_t, pack) + sizeof(N) * 3, "wrong 'max.y' layout in pack");

        static_assert(offsetof(rect_t, left)   == offsetof(rect_t, pack) + sizeof(N) * 0, "wrong 'left' layout in pack");
        static_assert(offsetof(rect_t, top)    == offsetof(rect_t, pack) + sizeof(N) * 1, "wrong 'top' layout in pack");
        static_assert(offsetof(rect_t, right)  == offsetof(rect_t, pack) + sizeof(N) * 2, "wrong 'right' layout in pack");
        static_assert(offsetof(rect_t, bottom) == offsetof(rect_t, pack) + sizeof(N) * 3, "wrong 'bottom' layout in pack");
    }

    using rect = rectx<int>;

    typedef rectx<float>        rectf;
    typedef rectx<double>       rectd;
    typedef rectx<int>          recti;
    typedef rectx<unsigned>     rectu;
    typedef rectx<std::uint8_t> rectb;

    template <typename N, bool I>
    inline std::ostream& operator<<(std::ostream& os, rectx<N,I> const& r)
    {
        return os << r.min << ", " << r.max;
    }

    template <typename N, bool I>
    inline std::string to_string(rectx<N,I> const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }
}

#undef M_DECL_PURE
#undef M_DECL
