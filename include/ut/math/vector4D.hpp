// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "vectorND.hpp"

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define zero_                   scalar_type{0}
#define one_                    scalar_type{1}
#define two_                    scalar_type{2}

namespace ut
{
    template <typename N> class vec<N, 4>
    {
    public:
        using scalar_type   = N;
        using vector_type   = vec<N, 4>;
        using vector_param  = vector_type const&;
        using scalar_param  = scalar_type;
        using pack_type     = N[4];

        size_t static constexpr PACK_SIZE = 4;

        union
        {
            struct { scalar_type x,y,z,w; };
            pack_type pack;
        };

        //
        // class extras (if any)
        //

        #ifdef UT_VEC4_CLASS_EXTRA
        UT_VEC4_CLASS_EXTRA
        #endif

        M_DECL vec()
            : x{zero_}, y{zero_}, z{zero_}, w{zero_}
        {}

        M_DECL explicit vec(scalar_param n)
            : x{n}, y{n}, z{n}, w{n}
        {}

        M_DECL vec(scalar_param x, scalar_param y, scalar_param z, scalar_param w)
            : x{x}, y{y}, z{z}, w{w}
        {}

        M_DECL explicit vec(pack_type const& p)
            : x{p[0]}, y{p[1]}, z{p[2]}, w{p[3]}
        {}

        M_DECL vec(vector_type const&)=default;
        M_DECL vec(vector_type&&) noexcept =default;

        M_DECL vec& operator=(vector_type const&)=default;
        M_DECL vec& operator=(vector_type&&) noexcept =default;

        template <typename T>
        inline vec<T,4> cast() const { return vec<T,4>{T(x), T(y), T(z), T(w)}; }

        //
        // mutators
        //

        M_DECL void set(scalar_type s)
        { x = s; y = s; z = s; w = s;  }

        M_DECL void set(scalar_type x, scalar_type y, scalar_type z, scalar_type w)
        { this->x = x; this->y = y; this->z = z; this->w = w; }

        M_DECL void set(pack_type const& p)
        { x = p[0]; y = p[1]; z = p[2]; w = p[3]; }

        M_DECL void set(vector_type const& p)
        { x = p.x; y = p.y; z = p.z; w = p.w; }

        M_DECL void add(scalar_type s) { x += s, y += s; z += s; w += s; }
        M_DECL void sub(scalar_type s) { x += s, y += s; z += s; w += s; }
        M_DECL void mul(scalar_type s) { x += s, y += s; z += s; w += s; }
        M_DECL void div(scalar_type s) { x += s, y += s; z += s; w += s; }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(scalar_type s) { x %= s; y %= s; z %= s; w %= s; }

        M_DECL void add(vector_param p) { x += p.x; y += p.y; z += p.z; w += p.w; }
        M_DECL void sub(vector_param p) { x -= p.x; y -= p.y; z -= p.z; w -= p.w; }
        M_DECL void mul(vector_param p) { x *= p.x; y *= p.y; z *= p.z; w *= p.w; }
        M_DECL void div(vector_param p) { x /= p.x; y /= p.y; z /= p.z; w /= p.w; }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(vector_param p) { x %= p.x, y %= p.y; z %= p.z; w %= p.w; }

        //
        // copy mutators
        //

        M_DECL_PURE vector_type withX(scalar_param s) const { return vector_type{ s,y,z,w }; }
        M_DECL_PURE vector_type withY(scalar_param s) const { return vector_type{ x,s,z,w }; }
        M_DECL_PURE vector_type withZ(scalar_param s) const { return vector_type{ x,y,s,w }; }
        M_DECL_PURE vector_type withW(scalar_param s) const { return vector_type{ x,y,z,s }; }

        M_DECL_PURE vector_type withOffsetX(scalar_param s) const { return vector_type(x + s, y, z, w); }
        M_DECL_PURE vector_type withOffsetY(scalar_param s) const { return vector_type(x, y + s, z, w); }
        M_DECL_PURE vector_type withOffsetZ(scalar_param s) const { return vector_type(x, y, z + s, w); }
        M_DECL_PURE vector_type withOffsetW(scalar_param s) const { return vector_type(x, y, z, w + s); }

        M_DECL_PURE vector_type withScaleX(scalar_param s) const { return vector_type(x * s, y, z, w); }
        M_DECL_PURE vector_type withScaleY(scalar_param s) const { return vector_type(x, y * s, z, w); }
        M_DECL_PURE vector_type withScaleZ(scalar_param s) const { return vector_type(x, y, z * s, w); }
        M_DECL_PURE vector_type withScaleW(scalar_param s) const { return vector_type(x, y, z, w * s); }

        //
        // utilities
        //

        M_DECL_PURE scalar_type sum       () const { return x + y + z + w; }
        M_DECL_PURE scalar_type sumSquared() const { return x*x + y*y + z*z + w*w; }
        M_DECL_PURE scalar_type length    () const { return std::sqrt(sumSquared()); }

        ENABLE_IF_FLOAT M_DECL_PURE scalar_type dot     (vector_param v) const { return x*v.x + y*v.y + z*v.z + w*v.w; }
        ENABLE_IF_FLOAT M_DECL_PURE scalar_type angle   (vector_param v) const { return (*this == v) ? zero_ : std::acos(dot(v) / (length() * v.length())); }
        ENABLE_IF_FLOAT M_DECL_PURE scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        ENABLE_IF_FLOAT M_DECL_PURE vector_type round() const { return vector_type(std::round(x), std::round(y)); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type floor() const { return vector_type(std::floor(x), std::floor(y)); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type ceil () const { return vector_type(std::ceil(x), std::ceil(y)); }

        M_DECL_PURE vector_type neg    () const { return vector_type(      -x,       -y,       -z,       -w); }
        M_DECL_PURE vector_type reverse() const { return vector_type(       w,        z,        y,        x); }
        M_DECL_PURE vector_type inverse() const { return vector_type(one_ / x, one_ / y, one_ / z, one_ / w); }

        ENABLE_IF_FLOAT M_DECL_PURE vector_type normal() const { return *this / length(); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type project(vector_param base)   const { return base * ((*this * base) / base.sumSquared()); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * two_); }
        //ENABLE_IF_FLOAT M_DECL_PURE vector_type cross  (vector_param v)      const { return vector_type((y * v.z) - (v.y * z), (z * v.x) - (v.z * x), (x * v.y) - (v.x * y)); }

        ENABLE_IF_FLOAT M_DECL_PURE bool isFltNan   () const { return std::isnan   (x) || std::isnan   (y) || std::isnan   (z) || std::isnan   (w); }
        ENABLE_IF_FLOAT M_DECL_PURE bool isFltInf   () const { return std::isinf   (x) || std::isinf   (y) || std::isinf   (z) || std::isinf   (w); }
        ENABLE_IF_FLOAT M_DECL_PURE bool isFltNormal() const { return std::isnormal(x) && std::isnormal(y) && std::isnormal(z) && std::isnormal(w); }

        //
        // operators
        //

        M_DECL_PURE vector_type operator - () const { return neg(); }

        M_DECL_PURE vector_type operator+(scalar_type s) const { return vector_type(x + s, y + s, z + s, w + s); }
        M_DECL_PURE vector_type operator-(scalar_type s) const { return vector_type(x - s, y - s, z - s, w - s); }
        M_DECL_PURE vector_type operator*(scalar_type s) const { return vector_type(x * s, y * s, z * s, w * s); }
        M_DECL_PURE vector_type operator/(scalar_type s) const { return vector_type(x / s, y / s, z / s, w / s); }

        ENABLE_IF_INTEGRAL
        M_DECL_PURE vector_type operator%(scalar_type s) const { return vector_type(x % s, y % s, z % s, w % s); }

        M_DECL_PURE vector_type operator+(vector_type const& p) const { return vector_type(x + p.x, y + p.y, z + p.z, w + p.w); }
        M_DECL_PURE vector_type operator-(vector_type const& p) const { return vector_type(x - p.x, y - p.y, z - p.z, w - p.w); }
        M_DECL_PURE vector_type operator*(vector_type const& p) const { return vector_type(x * p.x, y * p.y, z * p.z, w * p.w); }
        M_DECL_PURE vector_type operator/(vector_type const& p) const { return vector_type(x / p.x, y / p.y, z / p.z, w / p.w); }

        ENABLE_IF_INTEGRAL
        M_DECL_PURE vector_type operator%(vector_type const& p) const { return vector_type(x % p.x, y % p.y, z % p.z, w % p.w); }

        M_DECL vector_type& operator += (vector_type const& p) { add(p); return *this; }
        M_DECL vector_type& operator -= (vector_type const& p) { sub(p); return *this; }
        M_DECL vector_type& operator *= (vector_type const& p) { mul(p); return *this; }
        M_DECL vector_type& operator /= (vector_type const& p) { div(p); return *this; }

        ENABLE_IF_INTEGRAL
        M_DECL vector_type& operator %= (vector_type const& p) { mod(p); return *this; }

        M_DECL vector_type& operator += (scalar_type s) { add(s); return *this; }
        M_DECL vector_type& operator -= (scalar_type s) { sub(s); return *this; }
        M_DECL vector_type& operator *= (scalar_type s) { mul(s); return *this; }
        M_DECL vector_type& operator /= (scalar_type s) { div(s); return *this; }

        ENABLE_IF_INTEGRAL
        M_DECL vector_type& operator %= (scalar_type s) { mod(s); return *this; }

        M_DECL_PURE bool operator== (vector_param v) const { return same(*this, v); }
        M_DECL_PURE bool operator!= (vector_param v) const { return !(*this == v); }
        M_DECL_PURE bool operator<  (vector_param v) const { return less(*this, v); }
        M_DECL_PURE bool operator>  (vector_param v) const { return *this < v; }
        M_DECL_PURE bool operator<= (vector_param v) const { return !(*this < v); }
        M_DECL_PURE bool operator>= (vector_param v) const { return !(v < *this); }

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

        M_DECL_PURE size_t size() const { return PACK_SIZE; }

    private:
        //
        // pack layout check
        //

        M_DECL vec(std::nullptr_t);
    };

    template <typename N> constexpr vec<N,4>::vec(std::nullptr_t)
    {
        using vec_t = vec<N, 4>;
        static_assert(offsetof(vec_t, x) == offsetof(vec_t, pack) + sizeof(N)*0, "wrong 'x' layout in pack");
        static_assert(offsetof(vec_t, y) == offsetof(vec_t, pack) + sizeof(N)*1, "wrong 'y' layout in pack");
        static_assert(offsetof(vec_t, z) == offsetof(vec_t, pack) + sizeof(N)*2, "wrong 'z' layout in pack");
        static_assert(offsetof(vec_t, w) == offsetof(vec_t, pack) + sizeof(N)*3, "wrong 'w' layout in pack");
    }

    template <typename N> using vec4x = vec<N, 4>;

    typedef vec4x<real_t>       vec4;
    typedef vec4x<float>        vec4f;
    typedef vec4x<double>       vec4d;
    typedef vec4x<int>          vec4i;
    typedef vec4x<unsigned>     vec4u;
    typedef vec4x<std::uint8_t> vec4b;

    extern template struct vec<float       , 4>;
    extern template struct vec<double      , 4>;
    extern template struct vec<int         , 4>;
    extern template struct vec<unsigned    , 4>;
    extern template struct vec<std::uint8_t, 4>;
}

#undef zero_
#undef one_
#undef two_

#undef M_DECL_PURE
#undef M_DECL
