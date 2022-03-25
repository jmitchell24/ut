// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "VectorND.hpp"

namespace ut
{
    template <typename N> class vec<N, 3>
    {
    public:
        using scalar_type       = N;
        using vector_type       = vec<N,3>;
        using vector_ref        = vector_type&;
        using scalar_ref        = scalar_type&;
        using vector_param      = vector_type const&;
        using scalar_param      = scalar_type;
        using elements_type     = scalar_type[3];

        size_t static constexpr SIZE = 3;

        union
        {
            struct { scalar_type x,y,z; };
            elements_type elements;
        };

        inline constexpr vec()
                : x{scalar_type(0)}, y{scalar_type(0)}, z{scalar_type(0)}
        {}

        inline constexpr explicit vec(scalar_param n)
                : x{n}, y{n}, z{n}
        {}

        inline constexpr vec(scalar_param x, scalar_param y, scalar_param z)
                : x{x}, y{y}, z{z}
        {}

        inline constexpr vec(elements_type const& e)
                : x{e[0]}, y{e[1]}, z{e[2]}
        {}

        vec(vector_type const&)=default;
        vec(vector_type&&) noexcept =default;

        vec& operator=(vector_type const&)=default;
        vec& operator=(vector_type&&) noexcept =default;

        template <typename T>
        inline vec<T,SIZE> cast() const { return vec<T,SIZE>{T(x), T(y), T(z)}; }

        inline void add(scalar_type s) { x += s, y += s; z += s; }
        inline void sub(scalar_type s) { x += s, y += s; z += s; }
        inline void mul(scalar_type s) { x += s, y += s; z += s; }
        inline void div(scalar_type s) { x += s, y += s; z += s; }

        ENABLE_IF_INTEGRAL
        inline void mod(scalar_type s) { x %= s; y %= s; z %= s; }

        inline void add(vector_param p) { x += p.x; y += p.y; z += p.z; }
        inline void sub(vector_param p) { x -= p.x; y -= p.y; z -= p.z; }
        inline void mul(vector_param p) { x *= p.x; y *= p.y; z *= p.z; }
        inline void div(vector_param p) { x /= p.x; y /= p.y; z /= p.z; }

        ENABLE_IF_INTEGRAL
        inline void mod(vector_param p) { x %= p.x, y %= p.y; z %= p.z; }

        [[nodiscard]] inline scalar_type sum       () const { return x + y; }
        [[nodiscard]] inline scalar_type sumSquared() const { return x*x + y*y; }
        [[nodiscard]] inline scalar_type length    () const { return std::sqrt(sumSquared()); }

        ENABLE_IF_FLOAT [[nodiscard]] inline scalar_type dot     (vector_param v) const { return x*v.x + y*v.y + z*v.z; }
        ENABLE_IF_FLOAT [[nodiscard]] inline scalar_type angle   (vector_param v) const { return (*this==v) ? scalar_type{0} : std::acos(dot(v) / (length()*v.length())); }
        ENABLE_IF_FLOAT [[nodiscard]] inline scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type round() const { return vector_type(std::round(x), std::round(y)); }
        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type floor() const { return vector_type(std::floor(x), std::floor(y)); }
        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type ceil () const { return vector_type(std::ceil (x), std::ceil (y)); }

        [[nodiscard]] inline vector_type neg    () const { return vector_type(-x,-y,-z); }
        [[nodiscard]] inline vector_type reverse() const { return vector_type(z, y, x); }

        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type normal () const { return *this / length(); }
        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type project(vector_param base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * scalar_type{2}); }
        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type cross  (vector_param v)      const { return vector_type((y * v.z) - (v.y * z), (z * v.x) - (v.z * x), (x * v.y) - (v.x * y)); }

        ENABLE_IF_FLOAT [[nodiscard]] inline bool isNan() const { return std::isnan(x) || std::isnan(y); }
        ENABLE_IF_FLOAT [[nodiscard]] inline bool isInf() const { return std::isinf(x) || std::isinf(y); }

        inline vector_type operator - () const { return neg(); }

        inline vector_type operator+(scalar_type s) const { return vector_type(x + s, y + s, z + s); }
        inline vector_type operator-(scalar_type s) const { return vector_type(x - s, y - s, z - s); }
        inline vector_type operator*(scalar_type s) const { return vector_type(x * s, y * s, z * s); }
        inline vector_type operator/(scalar_type s) const { return vector_type(x / s, y / s, z / s); }

        ENABLE_IF_INTEGRAL
        inline vector_type operator%(scalar_type s) const { return vector_type(x % s, y % s, z % s); }

        inline vector_type operator+(vector_type const& p) const { return vector_type(x + p.x, y + p.y, z + p.z); }
        inline vector_type operator-(vector_type const& p) const { return vector_type(x - p.x, y - p.y, z - p.z); }
        inline vector_type operator*(vector_type const& p) const { return vector_type(x * p.x, y * p.y, z * p.z); }
        inline vector_type operator/(vector_type const& p) const { return vector_type(x / p.x, y / p.y, z / p.z); }

        ENABLE_IF_INTEGRAL
        inline vector_type operator%(vector_type const& p) const { return vector_type(x % p.x, y % p.y, z % p.z); }

        inline vector_type& operator += (vector_type const& p) { add(p); return *this; }
        inline vector_type& operator -= (vector_type const& p) { sub(p); return *this; }
        inline vector_type& operator *= (vector_type const& p) { mul(p); return *this; }
        inline vector_type& operator /= (vector_type const& p) { div(p); return *this; }

        ENABLE_IF_INTEGRAL
        inline vector_type& operator %= (vector_type const& p) { mod(p); return *this; }

        inline vector_type& operator += (scalar_type s) { add(s); return *this; }
        inline vector_type& operator -= (scalar_type s) { sub(s); return *this; }
        inline vector_type& operator *= (scalar_type s) { mul(s); return *this; }
        inline vector_type& operator /= (scalar_type s) { div(s); return *this; }

        ENABLE_IF_INTEGRAL
        inline vector_type& operator %= (scalar_type s) { mod(s); return *this; }

        inline bool operator== (vector_param v) const { return std::equal(begin(), end(), v.begin()); }
        inline bool operator!= (vector_param v) const { return !(*this == v); }
        inline bool operator<  (vector_param v) const { return std::lexicographical_compare(begin(), end(), v.begin(), v.end()); }
        inline bool operator>  (vector_param v) const { return *this < v; }
        inline bool operator<= (vector_param v) const { return !(*this < v); }
        inline bool operator>= (vector_param v) const { return !(v < *this); }

        [[nodiscard]] inline scalar_type  operator[] (size_t i) const { assert(i < SIZE); return elements[i]; }
        [[nodiscard]] inline scalar_type& operator[] (size_t i)       { assert(i < SIZE); return elements[i]; }

        [[nodiscard]] inline auto begin()       { return std::begin(elements); }
        [[nodiscard]] inline auto begin() const { return std::begin(elements); }

        [[nodiscard]] inline auto end()       { return std::end(elements); }
        [[nodiscard]] inline auto end() const { return std::end(elements); }

        [[nodiscard]] inline size_t size() const { return SIZE; }

        [[nodiscard]] inline scalar_type const* data() const { return elements; }
        [[nodiscard]] inline scalar_type*       data()       { return elements; }
    };

    template <typename N> using vec3x = vec<N, 3>;

    typedef vec3x<real_t>       vec3;
    typedef vec3x<float>        vec3f;
    typedef vec3x<double>       vec3d;
    typedef vec3x<int>          vec3i;
    typedef vec3x<unsigned>     vec3u;
    typedef vec3x<std::uint8_t> vec3b;

    extern template struct vec<float       , 3>;
    extern template struct vec<double      , 3>;
    extern template struct vec<int         , 3>;
    extern template struct vec<unsigned    , 3>;
    extern template struct vec<std::uint8_t, 3>;
}
