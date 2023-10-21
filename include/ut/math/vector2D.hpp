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
    template <typename N> class vec<N, 2>
    {
    public:
        using scalar_type   = N;
        using vector_type   = vec<N, 2>;
        using vector_param  = vector_type const&;
        using scalar_param  = scalar_type;
        using pack_type     = N[2];

        size_t static constexpr PACK_SIZE = 2;

        union
        {
            struct { scalar_type x,y; };
            pack_type pack;
        };

        //
        // ctor
        //

        M_DECL vec()
            : x{zero_}, y{zero_}
        {}

        M_DECL explicit vec(scalar_param n)
            : x{n}, y{n}
        {}

        M_DECL vec(scalar_param x, scalar_param y)
            : x{x}, y{y}
        {}

        M_DECL explicit vec(pack_type const& p)
            : x{p[0]}, y{p[1]}
        {}

        M_DECL vec(vector_type const&)=default;
        M_DECL vec(vector_type&&) noexcept =default;

        M_DECL vec& operator=(vector_type const&)=default;
        M_DECL vec& operator=(vector_type&&) noexcept =default;

        template <typename T>
        M_DECL_PURE vec<T,2> cast() const { return vec<T,2>{T(x), T(y)}; }

        //
        // mutators
        //

        M_DECL void set(scalar_param s)
        { x = s; y = s; }

        M_DECL void set(scalar_param x, scalar_param y)
        { this->x = x; this->y = y; }

        M_DECL void set(pack_type const& p)
        { x = p[0]; y = p[1]; }

        M_DECL void set(vector_param p)
        { x = p.x; y = p.y; }

        M_DECL void add(scalar_param s) { x += s; y += s; }
        M_DECL void sub(scalar_param s) { x -= s; y -= s; }
        M_DECL void mul(scalar_param s) { x *= s; y *= s; }
        M_DECL void div(scalar_param s) { x /= s; y /= s; }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(scalar_param s) { x %= s; y %= s; }

        M_DECL void add(scalar_param x, scalar_param y) { this->x += x; this->y += y; }
        M_DECL void sub(scalar_param x, scalar_param y) { this->x -= x; this->y -= y; }
        M_DECL void mul(scalar_param x, scalar_param y) { this->x *= x; this->y *= y; }
        M_DECL void div(scalar_param x, scalar_param y) { this->x /= x; this->y /= y; }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(scalar_param x, scalar_param y) { this->x %= x; this->y %= y; }

        M_DECL void add(vector_param p) { x += p.x; y += p.y; }
        M_DECL void sub(vector_param p) { x -= p.x; y -= p.y; }
        M_DECL void mul(vector_param p) { x *= p.x; y *= p.y; }
        M_DECL void div(vector_param p) { x /= p.x; y /= p.y; }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(vector_param p) { x %= p.x; y %= p.y; }

        M_DECL void offset (vector_param p) { x += p.x; y += p.y; }
        M_DECL void offsetX(scalar_param s) { x += s; }
        M_DECL void offsetY(scalar_param s) { y += s; }

        M_DECL void scale (vector_param p) { x *= p.x; y *= p.y; }
        M_DECL void scaleX(scalar_param s) { x *= s; }
        M_DECL void scaleY(scalar_param s) { y *= s; }

        //
        // copy mutators
        //

        M_DECL_PURE vector_type withX(scalar_param s) const { return vector_type{s,y}; }
        M_DECL_PURE vector_type withY(scalar_param s) const { return vector_type{x,s}; }

        M_DECL_PURE vector_type withOffsetX(scalar_param s) const { return vector_type(x+s, y); }
        M_DECL_PURE vector_type withOffsetY(scalar_param s) const { return vector_type(x, y+s); }

        M_DECL_PURE vector_type withScaleX(scalar_param s) const { return vector_type(x*s, y); }
        M_DECL_PURE vector_type withScaleY(scalar_param s) const { return vector_type(x, y*s); }

        //
        // utilities
        //

        M_DECL_PURE scalar_type sum       () const { return x + y; }
        M_DECL_PURE scalar_type sumSquared() const { return x*x + y*y; }
        M_DECL_PURE scalar_type length    () const { return std::sqrt(sumSquared()); }

        // I try to use brace-initialization everywhere I can, here's why I'm not using it within this class:
        // https://stackoverflow.com/questions/60580647/narrowing-conversion-from-int-to-unsigned-char
        // Negating a vector like this 'vector_type{static_cast<scalar_type>(-x), static_cast<scalar_type>(-y)}' to avoid the warning is retarded
        // In this instance I'll accept the implicit narrowing

        ENABLE_IF_FLOAT M_DECL_PURE scalar_type dot     (vector_param v) const { return x*v.x + y*v.y; }
        ENABLE_IF_FLOAT M_DECL_PURE scalar_type angle   (vector_param v) const { return (*this==v) ? zero_ : std::acos(dot(v) / (length()*v.length())); }
        ENABLE_IF_FLOAT M_DECL_PURE scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        ENABLE_IF_FLOAT M_DECL_PURE vector_type round() const { return vector_type(std::round(x), std::round(y)); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type floor() const { return vector_type(std::floor(x), std::floor(y)); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type ceil () const { return vector_type(std::ceil (x), std::ceil (y)); }


        M_DECL_PURE vector_type neg    () const { return vector_type(-x,-y); }
        M_DECL_PURE vector_type reverse() const { return vector_type( y, x); }
        M_DECL_PURE vector_type inverse() const { return vector_type(one_ / x, one_ / y); }

        ENABLE_IF_FLOAT M_DECL_PURE vector_type normal () const { return *this / length(); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type project(vector_param base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * two_); }

        ENABLE_IF_FLOAT M_DECL_PURE bool isFltNan   () const { return std::isnan   (x) || std::isnan   (y); }
        ENABLE_IF_FLOAT M_DECL_PURE bool isFltInf   () const { return std::isinf   (x) || std::isinf   (y); }
        ENABLE_IF_FLOAT M_DECL_PURE bool isFltNormal() const { return std::isnormal(x) && std::isnormal(y); }

        //
        // operators
        //

        M_DECL_PURE vector_type operator - () const { return neg(); }

        M_DECL_PURE vector_type operator+(scalar_param s) const { return vector_type(x + s, y + s); }
        M_DECL_PURE vector_type operator-(scalar_param s) const { return vector_type(x - s, y - s); }
        M_DECL_PURE vector_type operator*(scalar_param s) const { return vector_type(x * s, y * s); }
        M_DECL_PURE vector_type operator/(scalar_param s) const { return vector_type(x / s, y / s); }

        ENABLE_IF_INTEGRAL
        M_DECL_PURE vector_type operator%(scalar_param s) const { return vector_type(x % s, y % s); }

        M_DECL_PURE vector_type operator+(vector_param p) const { return vector_type(x + p.x, y + p.y); }
        M_DECL_PURE vector_type operator-(vector_param p) const { return vector_type(x - p.x, y - p.y); }
        M_DECL_PURE vector_type operator*(vector_param p) const { return vector_type(x * p.x, y * p.y); }
        M_DECL_PURE vector_type operator/(vector_param p) const { return vector_type(x / p.x, y / p.y); }

        ENABLE_IF_INTEGRAL
        M_DECL_PURE vector_type operator%(vector_param p) const { return vector_type(x % p.x, y % p.y); }

        M_DECL vector_type& operator += (vector_param p) { add(p); return *this; }
        M_DECL vector_type& operator -= (vector_param p) { sub(p); return *this; }
        M_DECL vector_type& operator *= (vector_param p) { mul(p); return *this; }
        M_DECL vector_type& operator /= (vector_param p) { div(p); return *this; }

        ENABLE_IF_INTEGRAL
        M_DECL vector_type& operator %= (vector_param p) { mod(p); return *this; }

        M_DECL vector_type& operator += (scalar_param s) { add(s); return *this; }
        M_DECL vector_type& operator -= (scalar_param s) { sub(s); return *this; }
        M_DECL vector_type& operator *= (scalar_param s) { mul(s); return *this; }
        M_DECL vector_type& operator /= (scalar_param s) { div(s); return *this; }

        ENABLE_IF_INTEGRAL
        M_DECL vector_type& operator %= (scalar_param s) { mod(s); return *this; }

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
        M_DECL vec(std::nullptr_t);
    };

    template <typename N> constexpr vec<N,2>::vec(std::nullptr_t)
    {
        using vec_t = vec<N, 2>;
        static_assert(offsetof(vec_t, x) == offsetof(vec_t, pack) + sizeof(N)*0, "wrong 'x' layout in pack");
        static_assert(offsetof(vec_t, y) == offsetof(vec_t, pack) + sizeof(N)*1, "wrong 'y' layout in pack");
    }

    template <typename N> using vec2x = vec<N, 2>;

    typedef vec2x<real_t>        vec2;
    typedef vec2x<float>         vec2f;
    typedef vec2x<double>        vec2d;
    typedef vec2x<int>           vec2i;
    typedef vec2x<unsigned>      vec2u;
    typedef vec2x<unsigned char> vec2b;

    extern template struct vec<float        , 2>;
    extern template struct vec<double       , 2>;
    extern template struct vec<int          , 2>;
    extern template struct vec<unsigned     , 2>;
    extern template struct vec<unsigned char, 2>;
}

#undef zero_
#undef one_
#undef two_

#undef M_DECL_PURE
#undef M_DECL
