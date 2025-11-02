#pragma once

#include "vector2D.hpp"

#include <utility>

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

namespace ut
{
    template <typename N>
    struct psizex;

    template <typename N, bool I>
    struct rectx;

    /// strongly typed fractional value
    struct perc
    {
        real_t v;

        constexpr explicit perc(real_t v)
            : v{v}
        { assert(v > static_cast<real_t>(0) ); }

        template <typename T>
        constexpr T operator() (T t) const
        {
            auto x = v * static_cast<real_t>(t);
            return static_cast<T>(x);
        }
    };

    inline constexpr perc operator ""_pc (unsigned long long i) noexcept
    { return perc((real_t)i / (real_t)100); }

    template <typename N>
    struct xywhx
    {
        using scalar_type   = N;
        using xywh_type     = xywhx<N>;
        using point_type    = vec2x<N>;

        using pack_type = N[4];

        size_t static constexpr PACK_SIZE = 4;

        union
        {            
            struct { scalar_type x,y,w,h; };
            struct { point_type pos,size; };
            pack_type pack;
        };

        //
        // class extras (if any)
        //

        #ifdef UT_XYWH_CLASS_EXTRA
        UT_XYWH_CLASS_EXTRA
        #endif

        //
        // ctor
        //

        M_DECL xywhx()
            : x{0}, y{0}, w{0}, h{0}
        { }

        M_DECL xywhx(scalar_type x, scalar_type y, scalar_type w, scalar_type h)
            : x{x}, y{y}, w{w}, h{h}
        { }

        M_DECL xywhx(point_type const& pos, point_type const& size)
            : xywhx{pos.x, pos.y, size.x, size.y}
        { }

        M_DECL xywhx(xywhx const&)=default;
        M_DECL xywhx(xywhx&&) noexcept =default;

        M_DECL xywhx& operator=(xywhx const&)=default;
        M_DECL xywhx& operator=(xywhx&&) noexcept =default;

        //
        // accessors
        //

        M_DECL_PURE bool inverted() const { return w < 0 || h < 0; }
        M_DECL_PURE bool empty() const { return w == 0 || h == 0; }

        M_DECL_PURE point_type min() const { return { x, y }; }
        M_DECL_PURE point_type max() const { return { x + w, y + h }; }

        M_DECL_PURE scalar_type minX() const { return x; }
        M_DECL_PURE scalar_type minY() const { return y; }
        M_DECL_PURE scalar_type maxX() const { return x + w; }
        M_DECL_PURE scalar_type maxY() const { return y + h; }

        template <typename T>
        M_DECL_PURE xywhx<T> cast() const
        { return xywhx<T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(w), static_cast<T>(h)); }

        template <typename T=N, bool I=false>
        M_DECL_PURE rectx<T,I> rect() const;

        //
        // mutators
        //

        M_DECL void normalize()
        {
            if (w < 0) { w = -w; x -= w;  }
            if (h < 0) { h = -h; y -= h; }
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

        M_DECL xywhx(std::nullptr_t);
    };

    template <typename N> constexpr xywhx<N>::xywhx(std::nullptr_t)
    {
        using xywh_t = xywhx<N>;
        auto constexpr off0 = offsetof(xywh_t, pos)  + offsetof(xywh_t::point_type, x);
        auto constexpr off1 = offsetof(xywh_t, pos)  + offsetof(xywh_t::point_type, y);
        auto constexpr off2 = offsetof(xywh_t, size) + offsetof(xywh_t::point_type, x);
        auto constexpr off3 = offsetof(xywh_t, size) + offsetof(xywh_t::point_type, y);


        static_assert(off0 == offsetof(xywh_t, pack) + sizeof(N) * 0, "wrong 'size.x' layout in pack");
        static_assert(off1 == offsetof(xywh_t, pack) + sizeof(N) * 1, "wrong 'size.y' layout in pack");
        static_assert(off2 == offsetof(xywh_t, pack) + sizeof(N) * 2, "wrong 'pos.x' layout in pack");
        static_assert(off3 == offsetof(xywh_t, pack) + sizeof(N) * 3, "wrong 'pos.y' layout in pack");

        static_assert(offsetof(xywh_t, x)   == offsetof(xywh_t, pack) + sizeof(N) * 0, "wrong 'x' layout in pack");
        static_assert(offsetof(xywh_t, y)   == offsetof(xywh_t, pack) + sizeof(N) * 1, "wrong 'y' layout in pack");
        static_assert(offsetof(xywh_t, w)   == offsetof(xywh_t, pack) + sizeof(N) * 2, "wrong 'w' layout in pack");
        static_assert(offsetof(xywh_t, h)   == offsetof(xywh_t, pack) + sizeof(N) * 3, "wrong 'h' layout in pack");
    }

    using xywh = xywhx<float>;

    typedef xywhx<float>        xywhf;
    typedef xywhx<double>       xywhd;
    typedef xywhx<int>          xywhi;
    typedef xywhx<unsigned>     xywhu;
    typedef xywhx<std::uint8_t> xywhb;

    template <typename N>
    std::ostream& operator<<(std::ostream& os, xywhx<N> const& r)
    {
        return os << r.x << "x" << r.y << " [" << r.w << ", " << r.h << "]";
    }

    template <typename N>
    std::string to_string(psizex<N> const& v)
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
    template <typename N>
    struct formatter<ut::xywhx<N>> : std::formatter<std::string>
    {
        auto format(ut::xywhx<N> const& obj, std::format_context& ctx) const
        {
            std::ostringstream oss;
            oss << obj;
            return std::formatter<std::string>::format(oss.str(), ctx);
        }
    };
}

#undef ASSERT_SIZE

#undef M_DECL_PURE
#undef M_DECL