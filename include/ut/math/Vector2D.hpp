// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "VectorND.hpp"

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

namespace ut
{
    template <typename N> class vec<N, 2>
    {
    public:
        using scalar_type       = N;
        using vector_type       = vec<N,2>;
        using vector_ref        = vector_type&;
        using scalar_ref        = scalar_type&;
        using vector_param      = vector_type const&;
        using scalar_param      = scalar_type;
        using elements_type     = scalar_type[2];

        size_t static constexpr SIZE = 2;

        union
        {
            struct { scalar_type x,y; };
            elements_type elements;
        };

        M_DECL vec()
            : x{scalar_type(0)}, y{scalar_type(0)}
        {}

        M_DECL explicit vec(scalar_param n)
            : x{n}, y{n}
        {}

        M_DECL vec(scalar_param x, scalar_param y)
            : x{x}, y{y}
        {}

        M_DECL vec(elements_type const& e)
            : x{e[0]}, y{e[1]}
        {}

        M_DECL vec(vector_type const&)=default;
        M_DECL vec(vector_type&&) noexcept =default;

        M_DECL vec& operator=(vector_type const&)=default;
        M_DECL vec& operator=(vector_type&&) noexcept =default;

        template <typename T>
        M_DECL vec<T,SIZE> cast() const { return vec<T,SIZE>{T(x), T(y)}; }

        //
        // mutators
        //

        M_DECL void set(scalar_type s)
        { x = s; y = s; }

        M_DECL void set(scalar_type x, scalar_type y)
        { this->x = x; this->y = y; }

        M_DECL void set(elements_type const& e)
        { x = e[0]; y = e[1]; }

        M_DECL void add(scalar_type s) { x += s; y += s; }
        M_DECL void sub(scalar_type s) { x -= s; y -= s; }
        M_DECL void mul(scalar_type s) { x *= s; y *= s; }
        M_DECL void div(scalar_type s) { x /= s; y /= s; }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(scalar_type s) { x %= s; y %= s; }

        M_DECL void add(vector_param p) { x += p.x; y += p.y; }
        M_DECL void sub(vector_param p) { x -= p.x; y -= p.y; }
        M_DECL void mul(vector_param p) { x *= p.x; y *= p.y; }
        M_DECL void div(vector_param p) { x /= p.x; y /= p.y; }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(vector_param p) { x %= p.x; y %= p.y; }

        //
        // accessors
        //

        M_DECL_PURE vector_type withX(scalar_type s) { return vector_type{s,y}; }
        M_DECL_PURE vector_type withY(scalar_type s) { return vector_type{x,s}; }

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
        ENABLE_IF_FLOAT M_DECL_PURE scalar_type angle   (vector_param v) const { return (*this==v) ? scalar_type{0} : std::acos(dot(v) / (length()*v.length())); }
        ENABLE_IF_FLOAT M_DECL_PURE scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        ENABLE_IF_FLOAT M_DECL_PURE vector_type round() const { return vector_type(std::round(x), std::round(y)); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type floor() const { return vector_type(std::floor(x), std::floor(y)); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type ceil () const { return vector_type(std::ceil (x), std::ceil (y)); }

        M_DECL_PURE vector_type neg    () const { return vector_type(-x,-y); }
        M_DECL_PURE vector_type reverse() const { return vector_type(y, x); }

        ENABLE_IF_FLOAT M_DECL_PURE vector_type normal () const { return *this / length(); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type project(vector_param base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * scalar_type{2}); }

        ENABLE_IF_FLOAT M_DECL_PURE bool isNan() const { return std::isnan(x) || std::isnan(y); }
        ENABLE_IF_FLOAT M_DECL_PURE bool isInf() const { return std::isinf(x) || std::isinf(y); }

        M_DECL_PURE vector_type operator - () const { return neg(); }

        M_DECL_PURE vector_type operator+(scalar_type s) const { return vector_type(x + s, y + s); }
        M_DECL_PURE vector_type operator-(scalar_type s) const { return vector_type(x - s, y - s); }
        M_DECL_PURE vector_type operator*(scalar_type s) const { return vector_type(x * s, y * s); }
        M_DECL_PURE vector_type operator/(scalar_type s) const { return vector_type(x / s, y / s); }

        ENABLE_IF_INTEGRAL
        M_DECL_PURE vector_type operator%(scalar_type s) const { return vector_type(x % s, y % s); }

        M_DECL_PURE vector_type operator+(vector_type const& p) const { return vector_type(x + p.x, y + p.y); }
        M_DECL_PURE vector_type operator-(vector_type const& p) const { return vector_type(x - p.x, y - p.y); }
        M_DECL_PURE vector_type operator*(vector_type const& p) const { return vector_type(x * p.x, y * p.y); }
        M_DECL_PURE vector_type operator/(vector_type const& p) const { return vector_type(x / p.x, y / p.y); }

        ENABLE_IF_INTEGRAL
        M_DECL_PURE vector_type operator%(vector_type const& p) const { return vector_type{x % p.x, y % p.y}; }

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

        M_DECL_PURE bool operator== (vector_param v) const { return std::equal(begin(), end(), v.begin()); }
        M_DECL_PURE bool operator!= (vector_param v) const { return !(*this == v); }
        M_DECL_PURE bool operator<  (vector_param v) const { return std::lexicographical_compare(begin(), end(), v.begin(), v.end()); }
        M_DECL_PURE bool operator>  (vector_param v) const { return *this < v; }
        M_DECL_PURE bool operator<= (vector_param v) const { return !(*this < v); }
        M_DECL_PURE bool operator>= (vector_param v) const { return !(v < *this); }

        inline scalar_type  operator[] (size_t i) const { assert(i < SIZE); return elements[i]; }
        inline scalar_type& operator[] (size_t i)       { assert(i < SIZE); return elements[i]; }

        [[nodiscard]] inline auto begin()       { return std::begin(elements); }
        [[nodiscard]] inline auto begin() const { return std::begin(elements); }

        [[nodiscard]] inline auto end()       { return std::end(elements); }
        [[nodiscard]] inline auto end() const { return std::end(elements); }

        [[nodiscard]] inline size_t size() const { return SIZE; }

        [[nodiscard]] inline scalar_type const* data() const { return elements; }
        [[nodiscard]] inline scalar_type*       data()       { return elements; }
    };

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

#undef M_DECL_PURE
#undef M_DECL