// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "VectorND.hpp"
#include "Vector3D.hpp"

namespace ut
{
    template <typename N> class VecND<N, 4>
    {
    public:
        typedef N               scalar_type;
        typedef VecND<scalar_type,4>    vector_type;
        typedef vector_type&            vector_ref;
        typedef scalar_type&            scalar_ref;
        typedef vector_type const&      vector_param;
        typedef scalar_type             scalar_param;
        typedef N               components_type[4];

        size_t static constexpr SIZE = 4;

        union
        {
            struct { scalar_type x,y,z,w; };

            VecND<N,2> xy;
            VecND<N,3> xyz;

            components_type components;
        };

        inline constexpr VecND()
            : x((scalar_type)0), y((scalar_type)0), z((scalar_type)0), w((scalar_type)0)
        {}

        constexpr VecND(vector_type&&)=default;
        constexpr VecND(vector_type const&)=default;

        template <typename P>
        inline constexpr VecND(VecND<P,4> const& v)
            : x((scalar_type)v.x), y((scalar_type)v.y), z((scalar_type)v.z), w((scalar_type)v.w)
        {}

        inline constexpr explicit VecND(scalar_param n)
            : x(n), y(n), z(n), w(n)
        {}

        inline constexpr VecND(VecND<N,3> const& v, scalar_param w)
            : x(v.x), y(v.y), z(v.z), w(w)
        {}

        inline constexpr VecND(VecND<N,2> const& v, scalar_param z, scalar_param w)
            : x(v.x), y(v.y), z(z), w(w)
        {}

        inline constexpr VecND(scalar_param x, scalar_param y, scalar_param z, scalar_param w)
            : x(x), y(y), z(z), w(w)
        {}

        inline constexpr explicit VecND(components_type const& components)
            : x(components[0]), y(components[1]), z(components[2]), w(components[3])
        {}

        inline scalar_type sum       () const { return x + y + z + w; }
        inline scalar_type sumSquared() const { return x*x + y*y + z*z + w*w; }
        inline scalar_type length    () const { return std::sqrt(sumSquared()); }
        inline scalar_type dot     (vector_param v) const { return x*v.x + y*v.y + z*v.z + w*v.w; }
        inline scalar_type angle   (vector_param v) const { return (*this==v) ? (scalar_type)0 : std::acos(dot(v) / (length()*v.length())); }
        inline scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        inline vector_type round  () const { return vector_type(std::round(x), std::round(y), std::round(z), std::round(w)); }
        inline vector_type floor  () const { return vector_type(std::floor(x), std::floor(y), std::floor(z), std::floor(w)); }
        inline vector_type ceil   () const { return vector_type(std::ceil (x), std::ceil (y), std::ceil (z), std::ceil (w)); }

        inline vector_type reverse() const { return vector_type(w,z,y,x); }
        inline vector_type normal () const { return *this / length(); }
        inline vector_type project(vector_param base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        inline vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * (scalar_type)2); }

        inline bool isNan() const { return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w); }
        inline bool isInf() const { return std::isinf(x) || std::isinf(y) || std::isinf(z) || std::isinf(w); }

        inline vector_type operator - () const { return vector_type(-x,-y,-z,-w); }

        inline vector_type operator + (vector_param v) const { return vector_type(x + v.x, y + v.y, z + v.z, w + v.w); }
        inline vector_type operator - (vector_param v) const { return vector_type(x - v.x, y - v.y, z - v.z, w - v.w); }
        inline vector_type operator * (vector_param v) const { return vector_type(x * v.x, y * v.y, z * v.z, w * v.w); }
        inline vector_type operator / (vector_param v) const { return vector_type(x / v.x, y / v.y, z / v.z, w / v.w); }

        inline vector_ref operator =  (vector_type&&)=default;
        inline vector_ref operator =  (vector_param v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
        inline vector_ref operator += (vector_param v) { return (*this = *this + v); }
        inline vector_ref operator -= (vector_param v) { return (*this = *this - v); }
        inline vector_ref operator *= (vector_param v) { return (*this = *this * v); }
        inline vector_ref operator /= (vector_param v) { return (*this = *this / v); }

        inline vector_type operator + (scalar_param n) const { return vector_type(x + n, y + n, z + n, w + n); }
        inline vector_type operator - (scalar_param n) const { return vector_type(x - n, y - n, z - n, w - n); }
        inline vector_type operator * (scalar_param n) const { return vector_type(x * n, y * n, z * n, w * n); }
        inline vector_type operator / (scalar_param n) const { return vector_type(x / n, y / n, z / n, w / n); }

        inline vector_ref operator =  (scalar_param n) { x = n; y = n; z = n; w = n; return *this; }
        inline vector_ref operator += (scalar_param n) { return (*this = *this + n); }
        inline vector_ref operator -= (scalar_param n) { return (*this = *this - n); }
        inline vector_ref operator *= (scalar_param n) { return (*this = *this * n); }
        inline vector_ref operator /= (scalar_param n) { return (*this = *this / n); }

        inline bool operator== (vector_param v) const { return std::equal(begin(), end(), v.begin()); }
        inline bool operator!= (vector_param v) const { return !(*this == v); }
        inline bool operator<  (vector_param v) const { return std::lexicographical_compare(begin(), end(), v.begin(), v.end()); }
        inline bool operator>  (vector_param v) const { return *this < v; }
        inline bool operator<= (vector_param v) const { return !(*this < v); }
        inline bool operator>= (vector_param v) const { return !(v < *this); }

        inline scalar_param operator[] (size_t const i) const { assert(i < SIZE); return components[i]; }
        inline scalar_type    & operator[] (size_t const i)       { assert(i < SIZE); return components[i]; }

        inline scalar_type*       begin()       { return std::begin(components); }
        inline scalar_type const* begin() const { return std::begin(components); }

        inline scalar_type*       end()       { return std::end(components); }
        inline scalar_type const* end() const { return std::end(components); }
    };

    template <typename N> using Vec4N = VecND<N, 4>;

    typedef Vec4N<real_t>        Vec4;
    typedef Vec4N<float>         Vec4f;
    typedef Vec4N<double>        Vec4d;
    typedef Vec4N<int>           Vec4i;
    typedef Vec4N<unsigned>      Vec4u;
    typedef Vec4N<unsigned char> Vec4b;

    typedef Vec4N<real_t>        Color4;
    typedef Vec4N<float>         Color4f;
    typedef Vec4N<double>        Color4d;
    typedef Vec4N<int>           Color4i;
    typedef Vec4N<unsigned>      Color4u;
    typedef Vec4N<unsigned char> Color4b;

    extern template struct VecND<float        , 4>;
    extern template struct VecND<double       , 4>;
    extern template struct VecND<int          , 4>;
    extern template struct VecND<unsigned     , 4>;
    extern template struct VecND<unsigned char, 4>;
}
