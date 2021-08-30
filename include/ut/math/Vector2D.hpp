// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "VectorND.hpp"

namespace ut
{
    using real_t = float;

    template <typename N> class VecND<N, 2>
    {
    public:
        using scalar_type       = N;
        using vector_type       = VecND<N,2>;
        using vector_ref        = vector_type&;
        using scalar_ref        = scalar_type&;
        using vector_param      = vector_type const&;
        using scalar_param      = scalar_type;
        using components_type   = scalar_type[2];

        size_t static constexpr SIZE = 2;

        union
        {
            struct { scalar_type x,y; };
            components_type components;
        };

        inline constexpr VecND()
            : x{scalar_type(0)}, y{scalar_type(0)}
        {}

        inline constexpr explicit VecND(scalar_param n)
            : x{n}, y{n}
        {}

        inline constexpr VecND(scalar_param x, scalar_param y)
            : x{x}, y{y}
        {}

        inline constexpr VecND(components_type const& components)
            : x{components[0]}, y{components[1]}
        {}

        VecND(vector_type const&)=default;
        VecND(vector_type&&)=default;

        VecND& operator=(vector_type const&)=default;
        VecND& operator=(vector_type&&)=default;

        template <typename T>
        inline VecND<T,2> cast() const { return VecND<T,2>{T(x), T(y)}; }

        inline void add(scalar_type s) { x += s, y += s; }
        inline void sub(scalar_type s) { x += s, y += s; }
        inline void mul(scalar_type s) { x += s, y += s; }
        inline void div(scalar_type s) { x += s, y += s; }

        ENABLE_IF_INTEGRAL
        inline void mod(scalar_type s) { x += s, y += s; }

        inline void add(vector_param p) { x += p.x, y += p.y; }
        inline void sub(vector_param p) { x -= p.x, y -= p.y; }
        inline void mul(vector_param p) { x *= p.x, y *= p.y; }
        inline void div(vector_param p) { x /= p.x, y /= p.y; }

        ENABLE_IF_INTEGRAL
        inline void mod(vector_param p) { x %= p.x, y %= p.y; }

        inline scalar_type sum       () const { return x + y; }
        inline scalar_type sumSquared() const { return x*x + y*y; }
        inline scalar_type length    () const { return std::sqrt(sumSquared()); }

        // I try to use brace-initialization everywhere I can, here's why I'm not using it within this class:
        // https://stackoverflow.com/questions/60580647/narrowing-conversion-from-int-to-unsigned-char
        // Negating a vector like this 'vector_type{static_cast<scalar_type>(-x), static_cast<scalar_type>(-y)}' to avoid the warning is retarded
        // In this instance I'll accept the implicit narrowing

        ENABLE_IF_FLOAT inline scalar_type dot     (vector_param v) const { return x*v.x + y*v.y; }
        ENABLE_IF_FLOAT inline scalar_type angle   (vector_param v) const { return (*this==v) ? scalar_type{0} : std::acos(dot(v) / (length()*v.length())); }
        ENABLE_IF_FLOAT inline scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        ENABLE_IF_FLOAT inline vector_type round() const { return vector_type(std::round(x), std::round(y)); }
        ENABLE_IF_FLOAT inline vector_type floor() const { return vector_type(std::floor(x), std::floor(y)); }
        ENABLE_IF_FLOAT inline vector_type ceil () const { return vector_type(std::ceil (x), std::ceil (y)); }

        inline vector_type neg    () const { return vector_type(-x,-y); }
        inline vector_type reverse() const { return vector_type(y, x); }

        ENABLE_IF_FLOAT inline vector_type normal () const { return *this / length(); }
        ENABLE_IF_FLOAT inline vector_type project(vector_param base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        ENABLE_IF_FLOAT inline vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * scalar_type{2}); }

        ENABLE_IF_FLOAT inline bool isNan() const { return std::isnan(x) || std::isnan(y); }
        ENABLE_IF_FLOAT inline bool isInf() const { return std::isinf(x) || std::isinf(y); }

        inline scalar_type*       begin()       { return std::begin(components); }
        inline scalar_type const* begin() const { return std::begin(components); }

        inline scalar_type*       end()       { return std::end(components); }
        inline scalar_type const* end() const { return std::end(components); }

        inline vector_type operator - () const { return neg(); }

        inline vector_type operator+(scalar_type s) const { return vector_type(x + s, y + s); }
        inline vector_type operator-(scalar_type s) const { return vector_type(x - s, y - s); }
        inline vector_type operator*(scalar_type s) const { return vector_type(x * s, y * s); }
        inline vector_type operator/(scalar_type s) const { return vector_type(x / s, y / s); }

        ENABLE_IF_INTEGRAL
        inline vector_type operator%(scalar_type s) const { return vector_type(x % s, y % s); }

        inline vector_type operator+(vector_type const& p) const { return vector_type(x + p.x, y + p.y); }
        inline vector_type operator-(vector_type const& p) const { return vector_type(x - p.x, y - p.y); }
        inline vector_type operator*(vector_type const& p) const { return vector_type(x * p.x, y * p.y); }
        inline vector_type operator/(vector_type const& p) const { return vector_type(x / p.x, y / p.y); }

        ENABLE_IF_INTEGRAL
        inline vector_type operator%(vector_type const& p) const { return vector_type{x % p.x, y % p.y}; }

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

        inline scalar_type  operator[] (size_t i) const { assert(i < SIZE); return components[i]; }
        inline scalar_type& operator[] (size_t i)       { assert(i < SIZE); return components[i]; }
    };

    template <typename N> using Vec2N = VecND<N, 2>;

    typedef Vec2N<real_t>        Vec2;
    typedef Vec2N<float>         Vec2f;
    typedef Vec2N<double>        Vec2d;
    typedef Vec2N<int>           Vec2i;
    typedef Vec2N<unsigned>      Vec2u;
    typedef Vec2N<unsigned char> Vec2b;



    extern template struct VecND<float        , 2>;
    extern template struct VecND<double       , 2>;
    extern template struct VecND<int          , 2>;
    extern template struct VecND<unsigned     , 2>;
    extern template struct VecND<unsigned char, 2>;


}
