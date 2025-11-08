#pragma once

#include "xywh.hpp"

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define ENABLE_IF_INCLUSIVE     template <bool I_ = I, typename = std::enable_if_t<I>>

#define UT_EXPAND_RECT_ANCHORS \
    X(TL, tl) \
    X(TR, tr) \
    X(BL, bl) \
    X(BR, br) \
    X(TC, tc) \
    X(BC, bc) \
    X(LC, lc) \
    X(RC, rc) \
    X(CC, cc)

#define UT_EXPAND_RECT_ALIGNMENTS \
    X(TLtoTL) \
    X(TLtoTR) \
    X(TLtoBL) \
    X(TLtoBR) \
    \
    X(TRtoTL) \
    X(TRtoTR) \
    X(TRtoBL) \
    X(TRtoBR) \
    \
    X(BLtoTL) \
    X(BLtoTR) \
    X(BLtoBL) \
    X(BLtoBR) \
    \
    X(BRtoTL) \
    X(BRtoTR) \
    X(BRtoBL) \
    X(BRtoBR) \
    \
    X(TCtoTC) \
    X(TCtoBC) \
    X(BCtoTC) \
    X(BCtoBC) \
    \
    X(LCtoLC) \
    X(LCtoRC) \
    X(RCtoLC) \
    X(RCtoRC) \
    \
    X(CCtoCC)


namespace ut
{
    template<typename N, bool I>
    constexpr bool less(rectx<N,I> const& a, rectx<N,I> const& b);

    template<typename N, bool I>
    constexpr bool same(rectx<N,I> const& a, rectx<N,I> const& b);

    /// 'I' indicates the use of Inclusive Maximum for all size (width/height) calculations. (e.g. "(1,1), (2,2)" will
    /// have size "(2,2)", instead of "(1,1)". It is disallowed for non-integer types.
    template <typename N, bool I = false>
    struct rectx
    {
        static_assert(std::is_integral_v<N> || !I, "Inclusive Maximum only supported for integral types");

        using scalar_type   = N;
        using real_type     = real_t;
        using rect_type     = rectx<N,I>;
        using xywh_type     = xywhx<N>;
        using point_type    = vec2x<N>;
        using split_type    = std::tuple<rect_type,rect_type>;
        //using fit_type    = std::tuple<scalar_type,scalar_type>;

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
        // class extras (if any)
        //

        #ifdef UT_RECT_CLASS_EXTRA
        UT_RECT_CLASS_EXTRA
        #endif

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

        M_DECL explicit rectx(xywh_type const& xywh)
            : left{xywh.minX()}, top{xywh.minY()}, right{xywh.maxX()}, bottom{xywh.maxY()}
        { }

        M_DECL rectx(rectx const&)=default;
        M_DECL rectx(rectx&&) noexcept =default;

        M_DECL rectx& operator=(rectx const&)=default;
        M_DECL rectx& operator=(rectx&&) noexcept =default;

        M_DECL explicit operator xywh_type() const { return xywh(); }

        //
        // accessors
        //

        M_DECL_PURE bool inverted() const { return min.x > max.x || min.y > max.y; }
        M_DECL_PURE bool empty() const { return max.x - min.x == 0 || max.y - min.x == 0; }

        M_DECL_PURE scalar_type area() const { return width() * height(); }

        M_DECL_PURE point_type size  () const { if constexpr(INCLUSIVE) return max - min + point_type{1,1}; else return max - min; }
        M_DECL_PURE point_type pos   () const { return min; }
        M_DECL_PURE point_type center() const { return min + (size() / 2); }

        M_DECL_PURE scalar_type x() const { return min.x; }
        M_DECL_PURE scalar_type y() const { return min.y; }

        M_DECL_PURE scalar_type width () const { if constexpr (INCLUSIVE) return max.x - min.x + 1; else return max.x - min.x; }
        M_DECL_PURE scalar_type height() const { if constexpr (INCLUSIVE) return max.y - min.y + 1; else return max.y - min.y; }

        M_DECL_PURE point_type tl() const { return min; }              ///< Top Left
        M_DECL_PURE point_type tr() const { return {max.x, min.y }; }  ///< Top Right
        M_DECL_PURE point_type bl() const { return {min.x, max.y }; }  ///< Bottom Left
        M_DECL_PURE point_type br() const { return max; }              ///< Bottom Right

        M_DECL_PURE point_type tc() const { return point_type(min.x + (width()) / 2, min.y); } ///< Top Center
        M_DECL_PURE point_type bc() const { return point_type(min.x + (width()) / 2, max.y); } ///< Bottom Center
        M_DECL_PURE point_type lc() const { return point_type(min.x, min.y + (height()) / 2); } ///< Left Center
        M_DECL_PURE point_type rc() const { return point_type(max.x, min.y + (height()) / 2); } ///< Right Center

        template <typename T>
        M_DECL_PURE rectx<T> cast() const
        { return rectx<T>(static_cast<T>(left), static_cast<T>(top), static_cast<T>(right), static_cast<T>(bottom)); }


        template <typename T=N>
        M_DECL_PURE xywhx<T> xywh() const
        { return xywhx<T>(static_cast<T>(min.x), static_cast<T>(min.y), static_cast<T>(width()), static_cast<T>(height())); }



        //
        // mutators
        //

        M_DECL void set(scalar_type min_x, scalar_type min_y, scalar_type max_x, scalar_type max_y)
        { min.set(min_x, min_y); max.set(max_x, max_y); }

        M_DECL void set(point_type const& min, point_type const& max)
        { this->min.set(min); this->max.set(max); }

        M_DECL void set(rect_type const& r)
        { min = r.min; max = r.max; }

        M_DECL void setPos(scalar_type x, scalar_type y) { setPos({x,y}); }
        M_DECL void setPos(point_type const& p) { setOffset(p - min); }

        M_DECL void setPosX(scalar_type s) { setOffsetX(s - min.x); }
        M_DECL void setPosY(scalar_type s) { setOffsetY(s - min.y); }

        M_DECL void setOffset (scalar_type x, scalar_type y) { point_type p{x,y}; min += p; max += p; }
        M_DECL void setOffset (point_type  p) { setOffset(p.x, p.y); }

        M_DECL void setOffsetX(scalar_type o) { min.x += o; max.x += o; }
        M_DECL void setOffsetY(scalar_type o) { min.y += o; max.y += o; }

        M_DECL void setWidth(perc pc)       { setWidth(pc(width())); }
        M_DECL void setWidth(scalar_type s) { if constexpr(INCLUSIVE) max.x = min.x + s - 1; else max.x = min.x + s; }

        M_DECL void setHeight(perc pc)       { setHeight(pc(height())); }
        M_DECL void setHeight(scalar_type s) { if constexpr(INCLUSIVE) max.y = min.y + s - 1; else max.y = min.y + s; }

        M_DECL void setSize(perc pc_w, perc pc_h)           { setSize(pc_w(width()), pc_h(height())); }
        M_DECL void setSize(scalar_type w, scalar_type h)   { setSize({w,h}); }
        M_DECL void setSize(point_type const& p)            { if constexpr (INCLUSIVE) max = min + p - point_type{1,1}; else max = min + p; }

        M_DECL void normalize()
        {
            if (min.x > max.x) std::swap(min.x, max.x);
            if (min.y > max.y) std::swap(min.y, max.y);
        }

        //
        // copy mutators
        //

        M_DECL_PURE rect_type withMin(point_type const& p) const { auto tmp = *this; tmp.min = p; return tmp; }
        M_DECL_PURE rect_type withMax(point_type const& p) const { auto tmp = *this; tmp.max = p; return tmp; }

#define MUT(op_) auto tmp = *this; tmp.op_; return tmp;
        M_DECL_PURE rect_type withPos(scalar_type x, scalar_type y) const { MUT(setPos({x,y})); }
        M_DECL_PURE rect_type withPos(point_type const& p)          const { MUT(setPos(p)) }

        M_DECL_PURE rect_type withPosX(scalar_type s) const { MUT(setPosX(s)) }
        M_DECL_PURE rect_type withPosY(scalar_type s) const { MUT(setPosY(s)) }

        M_DECL_PURE rect_type withOffsetX(scalar_type s)       const { MUT(setOffsetX(s)) }
        M_DECL_PURE rect_type withOffsetY(scalar_type s)       const { MUT(setOffsetY(s)); }
        M_DECL_PURE rect_type withOffset (point_type const& p) const { MUT(setOffset(p)) }

        M_DECL_PURE rect_type withWidth (perc pc)       const { MUT(setWidth(pc)) }
        M_DECL_PURE rect_type withWidth (scalar_type s) const { MUT(setWidth(s)); }

        M_DECL_PURE rect_type withHeight(perc pc)       const { MUT(setHeight(pc)) }
        M_DECL_PURE rect_type withHeight(scalar_type s) const { MUT(setHeight(s)); }

        M_DECL_PURE rect_type withSize(perc pc_w, perc pc_h)         const { MUT(setSize(pc_w, pc_h)) }
        M_DECL_PURE rect_type withSize(scalar_type w, scalar_type h) const { MUT(setSize(w,h)) }
        M_DECL_PURE rect_type withSize(point_type const& p)          const { MUT(setSize(p)) }
#undef MUT

        //
        // set ops
        //

        M_DECL void add(rect_type const& r)
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

        ENABLE_IF_FLOAT M_DECL_PURE rect_type round() const { return rect_type(min.round(), max.round()); }
        ENABLE_IF_FLOAT M_DECL_PURE rect_type floor() const { return rect_type(min.floor(), max.floor()); }
        ENABLE_IF_FLOAT M_DECL_PURE rect_type ceil () const { return rect_type(min.ceil (), max.ceil ()); }

        //
        // bound checks
        //

        M_DECL_PURE bool contains(point_type  const& p) const { return p.x >= min.x && p.y >= min.y && p.x < max.x && p.y < max.y; }
        M_DECL_PURE bool contains(rect_type const& r) const { return r.min.x >= min.x && r.min.y >= min.y && r.max.x <= max.x && r.max.y <= max.y; }
        M_DECL_PURE bool overlaps(rect_type const& r) const { return r.min.y < max.y && r.max.y > min.y && r.min.x < max.x && r.max.x > min.x; }

        //
        // alignment
        //

#define DECL_ANCHORS(alignment_) \
    M_DECL_PURE rect_type anchor##alignment_##_W(perc pc_w) const { return anchor##alignment_##_W(pc_w(width())); } \
    M_DECL_PURE rect_type anchor##alignment_##_H(perc pc_h) const { return anchor##alignment_##_H(pc_h(height())); } \
    M_DECL_PURE rect_type anchor##alignment_##_W(scalar_type w) const { return anchor##alignment_(w, (w/width()) * height()); } \
    M_DECL_PURE rect_type anchor##alignment_##_H(scalar_type h) const { return anchor##alignment_((h/height()) * width(), h); } \
    M_DECL_PURE rect_type anchor##alignment_(point_type const& s) const { return anchor##alignment_(s.x, s.y); } \
    M_DECL_PURE rect_type anchor##alignment_

        DECL_ANCHORS(TLtoTL) (scalar_type w, scalar_type h) const { return { point_type(min.x    , min.y    ), point_type(min.x + w, min.y + h) }; }
        DECL_ANCHORS(TLtoTR) (scalar_type w, scalar_type h) const { return { point_type(min.x - w, min.y    ), point_type(min.x    , min.y + h) }; }
        DECL_ANCHORS(TLtoBL) (scalar_type w, scalar_type h) const { return { point_type(min.x    , min.y - h), point_type(min.x + w, min.y    ) }; }
        DECL_ANCHORS(TLtoBR) (scalar_type w, scalar_type h) const { return { point_type(min.x - w, min.y - h), point_type(min.x    , min.y    ) }; }

        DECL_ANCHORS(TRtoTL) (scalar_type w, scalar_type h) const { return { point_type(max.x    , min.y    ), point_type(max.x + w, min.y + h) }; }
        DECL_ANCHORS(TRtoTR) (scalar_type w, scalar_type h) const { return { point_type(max.x - w, min.y    ), point_type(max.x    , min.y + h) }; }
        DECL_ANCHORS(TRtoBL) (scalar_type w, scalar_type h) const { return { point_type(max.x    , min.y - h), point_type(max.x + w, min.y    ) }; }
        DECL_ANCHORS(TRtoBR) (scalar_type w, scalar_type h) const { return { point_type(max.x - w, min.y - h), point_type(max.x    , min.y    ) }; }

        DECL_ANCHORS(BLtoTL) (scalar_type w, scalar_type h) const { return { point_type(min.x    , max.y    ), point_type(min.x + w, max.y + h) }; }
        DECL_ANCHORS(BLtoTR) (scalar_type w, scalar_type h) const { return { point_type(min.x - w, max.y    ), point_type(min.x    , max.y + h) }; }
        DECL_ANCHORS(BLtoBL) (scalar_type w, scalar_type h) const { return { point_type(min.x    , max.y - h), point_type(min.x + w, max.y    ) }; }
        DECL_ANCHORS(BLtoBR) (scalar_type w, scalar_type h) const { return { point_type(min.x - w, max.y - h), point_type(min.x    , max.y    ) }; }

        DECL_ANCHORS(BRtoTL) (scalar_type w, scalar_type h) const { return { point_type(max.x    , max.y    ), point_type(max.x + w, max.y + h) }; }
        DECL_ANCHORS(BRtoTR) (scalar_type w, scalar_type h) const { return { point_type(max.x - w, max.y    ), point_type(max.x    , max.y + h) }; }
        DECL_ANCHORS(BRtoBL) (scalar_type w, scalar_type h) const { return { point_type(max.x    , max.y - h), point_type(max.x + w, max.y    ) }; }
        DECL_ANCHORS(BRtoBR) (scalar_type w, scalar_type h) const { return { point_type(max.x - w, max.y - h), point_type(max.x    , max.y    ) }; }

#define PADX  w=(width()-w)/2;
#define PADY  h=(height()-h)/2;

        DECL_ANCHORS(TCtoTC) (scalar_type w, scalar_type h) const { PADX return { point_type(min.x + w, min.y    ), point_type(max.x - w, min.y + h) }; }
        DECL_ANCHORS(TCtoBC) (scalar_type w, scalar_type h) const { PADX return { point_type(min.x + w, min.y - h), point_type(max.x - w, min.y    ) }; }
        DECL_ANCHORS(BCtoTC) (scalar_type w, scalar_type h) const { PADX return { point_type(min.x + w, max.y    ), point_type(max.x - w, max.y + h) }; }
        DECL_ANCHORS(BCtoBC) (scalar_type w, scalar_type h) const { PADX return { point_type(min.x + w, max.y - h), point_type(max.x - w, max.y    ) }; }

        DECL_ANCHORS(LCtoLC) (scalar_type w, scalar_type h) const { PADY return { point_type(min.x    , min.y + h), point_type(min.x + w, max.y - h) }; }
        DECL_ANCHORS(LCtoRC) (scalar_type w, scalar_type h) const { PADY return { point_type(min.x - w, min.y + h), point_type(min.x    , max.y - h) }; }
        DECL_ANCHORS(RCtoLC) (scalar_type w, scalar_type h) const { PADY return { point_type(max.x    , min.y + h), point_type(max.x + w, max.y - h) }; }
        DECL_ANCHORS(RCtoRC) (scalar_type w, scalar_type h) const { PADY return { point_type(max.x - w, min.y + h), point_type(max.x    , max.y - h) }; }

        DECL_ANCHORS(CCtoCC) (scalar_type w, scalar_type h) const { PADY PADX return { point_type(min.x + w, min.y + h), point_type(max.x - w, max.y - h) }; }

#undef PADX
#undef PADY

#undef DECL_ALIGN

        //
        // deflated
        //

        M_DECL_PURE rect_type deflated(scalar_type l, scalar_type t, scalar_type r, scalar_type b) const
        { return { left+l, top+t, right-r, bottom-b }; }

        M_DECL_PURE rect_type deflated(scalar_type h, scalar_type v) const
        { return deflated( h, v, h, v ); }

        M_DECL_PURE rect_type deflated(scalar_type sz) const
        { return deflated( sz, sz, sz, sz ); }

        M_DECL_PURE rect_type deflated(rect_type const& r) const
        { return deflated( r.left, r.top, r.right, r.bottom ); }

        M_DECL_PURE rect_type deflated(point_type const& tl, point_type const& br) const
        { return deflated( tl.x, tl.y, br.x, br.y ); }

        M_DECL_PURE rect_type deflated(point_type const& hv) const
        { return deflated( hv.x, hv.y, hv.x, hv.y ); }

        //
        // inflated
        //

        M_DECL_PURE rect_type inflated(scalar_type l, scalar_type t, scalar_type r, scalar_type b) const
        { return { left-l, top-t, right+r, bottom+b }; }

        M_DECL_PURE rect_type inflated(scalar_type h, scalar_type v) const
        { return inflated( h, v, h, v ); }

        M_DECL_PURE rect_type inflated(scalar_type sz) const
        { return inflated( sz, sz, sz, sz ); }

        M_DECL_PURE rect_type inflated(rect_type const& r) const
        { return inflated( r.left, r.top, r.right, r.bottom ); }

        M_DECL_PURE rect_type inflated(point_type const& tl, point_type const& br) const
        { return inflated( tl.x, tl.y, br.x, br.y ); }

        M_DECL_PURE rect_type inflated(point_type const& hv) const
        { return inflated( hv.x, hv.y, hv.x, hv.y ); }

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

        M_DECL_PURE point_type fitAspectSize(real_type ratio) const
        {
            auto w = real_type( width() );
            auto h = real_type( height() );
            auto r = w/h;

            if (r > ratio)
                return { scalar_type( h*ratio ), scalar_type( h ) };
            return { scalar_type( w ),scalar_type( w/ratio ) };
        }

        M_DECL_PURE rect_type fitAspect(real_type ratio) const
        {
            return anchorCCtoCC(fitAspectSize(ratio));
        }

        //
        // split
        //

        M_DECL_PURE rect_type splitV(scalar_type vmin, scalar_type vmax) const
        {
            assert(vmin <= vmax);
            assert(vmax >= min.y && vmax <= max.y);
            assert(vmin >= min.y && vmin <= max.y);
            return {{ min.x, vmin }, { max.x, vmax }};
        }

        M_DECL_PURE rect_type splitH(scalar_type hmin, scalar_type hmax) const
        {
            assert(hmin <= hmax);
            assert(hmax >= min.x && hmax <= max.x);
            assert(hmin >= min.x && hmin <= max.x);
            return {{ hmin, min.y }, { hmax, max.y }};
        }

        template <size_t Count>
        M_DECL_PURE std::array<rect_type, Count> splitNV(scalar_type inner_margin = 0) const
        {
            static_assert(Count > 0);

            auto step   = (height() - (inner_margin * (Count - 1)) ) / Count;
            auto vmin   = min.y;
            auto vmax   = vmin + step;

            assert(step > 0 && "total inner margin must be less than height");

            std::array<rect_type, Count> tmp;
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
        M_DECL_PURE std::array<rect_type, Count> splitNH(scalar_type inner_margin = 0) const
        {
            static_assert(Count > 0);

            auto step   = (width() - (inner_margin * (Count - 1)) ) / Count;
            auto hmin   = min.x;
            auto hmax   = hmin + step;

            assert(step > 0 && "total inner margin must be less than height");

            std::array<rect_type, Count> tmp;
            //tmp[0] = {{ min.x, hmin }, { max.x, hmax }};
            tmp[0] = {{ hmin, min.y }, { hmax, max.y }};

            step += inner_margin;

            for (size_t i = 1; i < Count; ++i)
            {
                hmin += step;
                hmax += step;
                //tmp[i] = {{ min.x, hmin }, { max.x, hmax }};
                tmp[i] = {{ hmin, min.y }, { hmax, max.y }};
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
        // grid
        //

        struct cellopt
        {
            size_t w=1, h=1; scalar_type inner_pad = scalar_type(0), outer_pad = scalar_type(0);
        };

        M_DECL_PURE rect_type cell(size_t w, size_t h, size_t x, size_t y, cellopt const& opt = {}) const
        {
            assert(w > 0);
            assert(x + opt.w <= w);

            assert(h > 0);
            assert(y + opt.h <= h);

            assert(opt.inner_pad >= scalar_type(0));
            assert(opt.outer_pad >= scalar_type(0));

            auto cw = (width() - (w - 1) * opt.inner_pad - 2 * opt.outer_pad) / scalar_type(w);
            auto ch = (height() - (h - 1) * opt.inner_pad - 2 * opt.outer_pad) / scalar_type(h);

            xywh_type p;
            p.x = scalar_type( pos().x + opt.outer_pad + scalar_type(x) * (cw + opt.inner_pad)  );
            p.y = scalar_type( pos().y + opt.outer_pad + scalar_type(y) * (ch + opt.inner_pad)  );
            p.w = scalar_type( cw * scalar_type(opt.w) + scalar_type(opt.w - 1) * opt.inner_pad );
            p.h = scalar_type( ch * scalar_type(opt.h) + scalar_type(opt.h - 1) * opt.inner_pad );

            return rect_type{p};
        }

        struct rowopt
        {
            size_t w=1; scalar_type inner_pad = scalar_type(0), outer_pad = scalar_type(0);
        };

        M_DECL_PURE rect_type row(size_t w, size_t x, rowopt const& opt = {}) const
        {
            assert(w > 0);
            assert(x + opt.w <= w);

            assert(opt.inner_pad >= 0.0f);
            assert(opt.outer_pad >= 0.0f);

            auto cw = (width() - (w - 1) * opt.inner_pad - 2 * opt.outer_pad) / scalar_type(w);
            auto ch = (height() - 2 * opt.outer_pad);

            xywh_type p;
            p.x = scalar_type( pos().x + opt.outer_pad + scalar_type(x) * (cw + opt.inner_pad)  );
            p.y = scalar_type( pos().y + opt.outer_pad                                          );
            p.w = scalar_type( cw * scalar_type(opt.w) + scalar_type(opt.w - 1) * opt.inner_pad );
            p.h = scalar_type( ch                                                               );

            return rect_type{p};
        }

        template<typename... Args>
        M_DECL void rowTie(rowopt const& opt, Args&... args) const
        {
            size_t w = sizeof...(Args) * opt.w;
            size_t i = 0;
            (..., (args = row(w, i, opt), i+=opt.w));
        }

        struct colopt
        {
            size_t h=1; scalar_type inner_pad = scalar_type(0), outer_pad = scalar_type(0);
        };

        M_DECL_PURE rect_type col(size_t h, size_t y, colopt const& opt = {}) const
        {
            assert(h > 0);
            assert(y + opt.h <= h);

            assert(opt.inner_pad >= scalar_type(0));
            assert(opt.outer_pad >= scalar_type(0));

            auto cw = (width() - 2 * opt.outer_pad);
            auto ch = (height() - (h - 1) * opt.inner_pad - 2 * opt.outer_pad) / scalar_type(h);

            xywh_type p;
            p.x = scalar_type( pos().x + opt.outer_pad                                            );
            p.y = scalar_type( pos().y + opt.outer_pad + scalar_type(y) * (ch + opt.inner_pad)    );
            p.w = scalar_type( cw                                                                 );
            p.h = scalar_type( ch * scalar_type(opt.h) + scalar_type(opt.h - 1) * opt.inner_pad   );

            return rect_type{p};
        }

        template<typename... Args>
        M_DECL void colTie(colopt const& opt, Args&... args) const
        {
            size_t h = sizeof...(Args) * opt.h;
            size_t i = 0;
            (..., (args = col(h, i, opt), i+=opt.h));
        }

        //
        // transform
        //

        M_DECL_PURE point_type transformPoint(rect_type const& to, point_type const& p) const
        { return transformPoint(*this, to, p); }

        M_DECL_PURE rect_type transformRect(rect_type const& to, rect_type const& r) const
        { return transformRect(*this, to, r); }

        M_DECL static point_type transformPoint(rect_type const& from, rect_type const& to, point_type const& p)
        {
            auto scale = (to.max - to.min) / (from.max - from.min);
            return to.min + (p - from.min) * scale;
        }

        M_DECL static rect_type transformRect(rect_type const& from, rect_type const& to, rect_type const& r)
        {
            return { transformPoint(from,to,r.min), transformPoint(from,to,r.max) };
        }

        //
        // operator overloads
        //

        M_DECL_PURE bool operator== (rect_type const& r) const { return same(*this, r); }
        M_DECL_PURE bool operator!= (rect_type const& r) const { return !(*this == r);  }
        M_DECL_PURE bool operator<  (rect_type const& r) const { return less(*this, r); }
        M_DECL_PURE bool operator>  (rect_type const& r) const { return *this < r;      }
        M_DECL_PURE bool operator<= (rect_type const& r) const { return !(*this < r);   }
        M_DECL_PURE bool operator>= (rect_type const& r) const { return !(r < *this);   }

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

    template <typename N> template <typename T, bool I> constexpr rectx<T,I> xywhx<N>::rect() const
    { return { minX(), minY(), maxX(), maxY() }; }

    using rect = rectx<real_t>;

    typedef rectx<float>        rectf;
    typedef rectx<double>       rectd;
    typedef rectx<int>          recti;
    typedef rectx<unsigned>     rectu;
    typedef rectx<std::uint8_t> rectb;

    template<typename N, bool I>
    constexpr bool less(rectx<N,I> const& a, rectx<N,I> const& b)
    {
        // https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
        auto first1 = a.begin(), last1 = a.end();
        auto first2 = b.begin(), last2 = b.end();
        for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 )
        {
            if (*first1 < *first2) return true;
            if (*first2 < *first1) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template<typename N, bool I>
    constexpr bool same(rectx<N,I> const& a, rectx<N,I> const& b)
    {
        return
            a.pack[0] == b.pack[0] &&
            a.pack[1] == b.pack[1] &&
            a.pack[2] == b.pack[2] &&
            a.pack[3] == b.pack[3];
    }

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

//
// std::format compatibility
//

namespace std
{
    template<typename N, bool I>
    struct formatter<ut::rectx<N,I>> : std::formatter<std::string>
    {
        auto format(ut::rectx<N,I> const& obj, std::format_context& ctx) const
        {
            std::ostringstream oss;
            oss << obj;
            return std::formatter<std::string>::format(oss.str(), ctx);
        }
    };
}

#undef M_DECL_PURE
#undef M_DECL
