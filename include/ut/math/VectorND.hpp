// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#define ENABLE_IF_INTEGRAL template <typename N_ = N, typename = std::enable_if_t<std::is_integral_v<N_>>>
#define ENABLE_IF_FLOAT    template <typename N_ = N, typename = std::enable_if_t<std::is_floating_point_v<N_>>>


#include <array>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <type_traits>
#include <cmath>
#include <cstdint>

namespace ut
{
    using real_t = float;

    template <typename N, size_t D> struct vec
    {
        using scalar_type       = N;
        using vector_type       = vec<N,D>;
        using vector_ref        = vector_type&;
        using scalar_ref        = scalar_type&;
        using vector_param      = vector_type const&;
        using scalar_param      = scalar_type;
        using elements_type     = scalar_type[D];

        size_t static constexpr SIZE = D;

        elements_type elements;

        inline vec()
        {
            std::fill(std::begin(elements), std::end(elements), (scalar_type)0);
        }

        constexpr vec(vector_type&&) noexcept =default;
        constexpr vec(vector_type const&)=default;

        template <typename P>
        inline vec(vec<P,D> const& v)
        {
            for (size_t i = 0; i < SIZE; ++i)
                elements[i] = (scalar_type)v.elements[i];
        }

        inline explicit vec(scalar_param n)
        {
            std::fill(begin(), end(), n);
        }

//        inline vec(std::initializer_list<N>&& il)
//            : vec(make_vector<N,D>(il.begin(), il.end()))
//        {}

        inline explicit vec(elements_type const& components)
        {
            std::copy(std::begin(components), std::end(components), begin());
        }

        inline scalar_type sum       () const { return std::accumulate(begin(), end(), (scalar_type)0, std::plus<scalar_type>()); }
        inline scalar_type sumSquared() const { return std::inner_product(begin(), end(), begin(), (scalar_type)0); }
        inline scalar_type length    () const { return std::sqrt(sumSquared()); }
        inline scalar_type dot     (vector_param v) const { return std::inner_product(begin(), end(), v.begin(), (scalar_type)0); }
        inline scalar_type angle   (vector_param v) const { return (*this == v) ? (scalar_type)0 : std::acos(dot(v) / (length() * v.length())); }
        inline scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        inline vector_type round() const
        {
            elements_type temp;
            for (size_t i = 0; i < SIZE; ++i)
                temp[i] = std::round(this->elements[i]);
            return {temp};
        }

        inline vector_type floor() const
        {
            elements_type temp;
            for (size_t i = 0; i < SIZE; ++i)
                temp[i] = std::floor(this->elements[i]);
            return {temp};
        }

        inline vector_type ceil() const
        {
            elements_type temp;
            for (size_t i = 0; i < SIZE; ++i)
                temp[i] = std::ceil(this->elements[i]);
            return {temp};
        }

        inline vector_type reverse() const
        {
            elements_type temp;
            std::reverse_copy(begin(), end(), std::begin(temp));
            return vector_type(temp);
        }

        inline vector_type normal() const { return *this / length(); }
        inline vector_type project(vector_param base)   const { return base * ((*this * base) / base.sumSquared() ); }
        inline vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * (scalar_type)2); }

        [[nodiscard]] inline bool isNan() const
        {
            bool nan = false;
            for (size_t i = 0; i < SIZE; ++i)
                nan |= std::isnan(elements[i]);
            return nan;
        }

        [[nodiscard]] inline bool isInf() const
        {
            bool inf = false;
            for (size_t i = 0; i < SIZE; ++i)
                inf |= std::isinf(elements[i]);
            return inf;
        }

        inline vector_type operator- () const { return transform(std::negate<scalar_type>()); }

        inline vector_type operator+ (vector_param v) const { return transform(v, std::plus<scalar_type>()); }
        inline vector_type operator- (vector_param v) const { return transform(v, std::minus<scalar_type>()); }
        inline vector_type operator* (vector_param v) const { return transform(v, std::multiplies<scalar_type>()); }
        inline vector_type operator/ (vector_param v) const { return transform(v, std::divides<scalar_type>()); }

        inline vector_type operator+ (scalar_param n) const { return transform([n](scalar_param x){ return x + n; }); }
        inline vector_type operator- (scalar_param n) const { return transform([n](scalar_param x){ return x - n; }); }
        inline vector_type operator* (scalar_param n) const { return transform([n](scalar_param x){ return x * n; }); }
        inline vector_type operator/ (scalar_param n) const { return transform([n](scalar_param x){ return x / n; }); }

        inline vector_ref operator=  (vector_type&&) noexcept =default;
        inline vector_ref operator=  (vector_param v) { std::copy(v.begin(), v.end(), begin()); return *this; }
        inline vector_ref operator+= (vector_param v) { return (*this = *this + v); }
        inline vector_ref operator-= (vector_param v) { return (*this = *this - v); }
        inline vector_ref operator*= (vector_param v) { return (*this = *this * v); }
        inline vector_ref operator/= (vector_param v) { return (*this = *this / v); }

        inline vector_ref operator=  (scalar_param n) { std::fill(begin(), end(), n); return *this; }
        inline vector_ref operator+= (scalar_param n) { return (*this = *this + n); }
        inline vector_ref operator-= (scalar_param n) { return (*this = *this - n); }
        inline vector_ref operator*= (scalar_param n) { return (*this = *this * n); }
        inline vector_ref operator/= (scalar_param n) { return (*this = *this / n); }

        inline bool operator== (vector_param v) const { return std::equal(begin(), end(), v.begin()); }
        inline bool operator!= (vector_param v) const { return !(*this == v); }
        inline bool operator<  (vector_param v) const { return std::lexicographical_compare(begin(), end(), v.begin(), v.end()); }
        inline bool operator>  (vector_param v) const { return *this < v; }
        inline bool operator<= (vector_param v) const { return !(*this < v); }
        inline bool operator>= (vector_param v) const { return !(v < *this); }

        inline scalar_param operator[] (size_t i) const { assert(i < SIZE); return elements[i]; }
        inline scalar_type& operator[] (size_t i)       { assert(i < SIZE); return elements[i]; }

        inline auto begin()       { return std::begin(elements); }
        inline auto begin() const { return std::begin(elements); }

        inline auto end()       { return std::end(elements); }
        inline auto end() const { return std::end(elements); }

        [[nodiscard]] inline size_t size() const { return SIZE; }

        inline scalar_type const* data() const { return elements; }
        inline scalar_type*       data()       { return elements; }

        static_assert(D > 1, "D must be greater than one");

    private:
        template <typename P> inline vector_type transform(P predicate) const
        {
            vector_type copy = *this;
            std::transform(copy.begin(), copy.end(), copy.begin(), predicate);
            return copy;
        }

        template <typename P> inline vector_type transform(vector_param v, P predicate) const
        {
            vector_type copy = *this;
            std::transform(v.begin(), v.end(), copy.begin(), copy.end(), predicate);
            return copy;
        }
    };

    template <typename N, size_t D, typename It>
    inline vec<N,D> make_vec(It first, It last)
    {
        vec<N,D> v;
        auto e = v.end();
        auto c = v.begin();
        for (auto i = first; i != last && c != e; ++i, ++c)
            *c = (N)*i;
        return v;
    }
}
