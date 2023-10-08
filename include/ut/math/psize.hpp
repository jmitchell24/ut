#ifndef RECT_HPP
#define RECT_HPP

#include "vector2D.hpp"

#include <utility>

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define ASSERT_SIZE(w_, h_) assert((w_) >= 0); assert((h_) >= 0)

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

        inline constexpr explicit perc(real_t v)
            : v{v}
        { assert(v > static_cast<real_t>(0) ); }

        template <typename T>
        inline constexpr T operator() (T t) const
        {
            auto x = v * static_cast<real_t>(t);
            return static_cast<T>(x);
        }
    };

    inline constexpr perc operator "" _pc(unsigned long long i) noexcept
    { return perc((real_t)i / (real_t)100); }

    template <typename N>
    struct psizex
    {
        using scalar_type   = N;
        using psize_type    = psizex<N>;
        using point_type    = vec2x<N>;

        using pack_type = N[4];

        size_t static constexpr PACK_SIZE = 4;

        union
        {            
            struct { scalar_type width,height,x,y; };
            struct { point_type size,pos; };
            pack_type pack;
        };

        //
        // ctor
        //

        M_DECL psizex()
            : width{0}, height{0}, x{0}, y{0}
        {}

        M_DECL psizex(scalar_type width, scalar_type height, scalar_type x=scalar_type(0), scalar_type y=scalar_type(0))
            : width{width}, height{height}, x{x}, y{y}
        { ASSERT_SIZE(width,height); }

        M_DECL psizex(point_type const& size, point_type const& pos={})
            : psizex{size.x, size.y, pos.x, pos.y}
        { ASSERT_SIZE(size.x, size.y); }

        M_DECL psizex(psizex const&)=default;
        M_DECL psizex(psizex&&) noexcept =default;

        M_DECL psizex& operator=(psizex const&)=default;
        M_DECL psizex& operator=(psizex&&) noexcept =default;

        //
        // accessors
        //

        M_DECL_PURE point_type min() const { return { x, y }; }
        M_DECL_PURE point_type max() const { ASSERT_SIZE(width,height); return { x + width, y + height }; }

        M_DECL_PURE scalar_type minX() const { return x; }
        M_DECL_PURE scalar_type minY() const { return y; }
        M_DECL_PURE scalar_type maxX() const { assert(width >= 0); return x + width; }
        M_DECL_PURE scalar_type maxY() const { assert(height >= 0); return y + height; }

        template <typename T>
        M_DECL_PURE psizex<T> cast() const
        { return psizex<T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(width), static_cast<T>(height)); }

        template <typename T=N, bool I=false>
        M_DECL_PURE rectx<T,I> rect() const;

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

        M_DECL psizex(std::nullptr_t);
    };

    template <typename N> constexpr psizex<N>::psizex(std::nullptr_t)
    {
        using psize_t = psizex<N>;
        auto constexpr off0 = offsetof(psize_t, size) + offsetof(psize_t::point_type, x);
        auto constexpr off1 = offsetof(psize_t, size) + offsetof(psize_t::point_type, y);
        auto constexpr off2 = offsetof(psize_t, pos)  + offsetof(psize_t::point_type, x);
        auto constexpr off3 = offsetof(psize_t, pos)  + offsetof(psize_t::point_type, y);

        static_assert(off0 == offsetof(psize_t, pack) + sizeof(N) * 0, "wrong 'size.x' layout in pack");
        static_assert(off1 == offsetof(psize_t, pack) + sizeof(N) * 1, "wrong 'size.y' layout in pack");
        static_assert(off2 == offsetof(psize_t, pack) + sizeof(N) * 2, "wrong 'pos.x' layout in pack");
        static_assert(off3 == offsetof(psize_t, pack) + sizeof(N) * 3, "wrong 'pos.y' layout in pack");

        static_assert(offsetof(psize_t, width)  == offsetof(psize_t, pack) + sizeof(N) * 0, "wrong 'width' layout in pack");
        static_assert(offsetof(psize_t, height) == offsetof(psize_t, pack) + sizeof(N) * 1, "wrong 'height' layout in pack");
        static_assert(offsetof(psize_t, x)      == offsetof(psize_t, pack) + sizeof(N) * 2, "wrong 'x' layout in pack");
        static_assert(offsetof(psize_t, y)      == offsetof(psize_t, pack) + sizeof(N) * 3, "wrong 'y' layout in pack");
    }

    using psize = psizex<float>;

    typedef psizex<float>        psizef;
    typedef psizex<double>       psized;
    typedef psizex<int>          psizei;
    typedef psizex<unsigned>     psizeu;
    typedef psizex<std::uint8_t> psizeb;

    template <typename N>
    inline std::ostream& operator<<(std::ostream& os, psizex<N> const& r)
    {
        return os << r.x << "x" << r.y << " [" << r.width << ", " << r.height << "]";
    }

    template <typename N>
    inline std::string to_string(psizex<N> const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }
}

#undef ASSERT_SIZE

#undef M_DECL_PURE
#undef M_DECL

#endif // RECT_HPP
