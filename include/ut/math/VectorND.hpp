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
#include <execution>

#if defined(UT_STL_INTEROP)
#include <sstream>
#endif

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

        inline constexpr vec()
        { std::fill(std::begin(elements), std::end(elements), (scalar_type)0); }

        inline constexpr explicit vec(scalar_param n)
        { std::fill(std::begin(elements), std::end(elements), n); }

        inline explicit vec(elements_type const& components)
        { std::copy(std::begin(components), std::end(components), begin()); }

        vec(vector_type const&)=default;
        vec(vector_type&&) noexcept =default;

        vec& operator=(vector_type const&)=default;
        vec& operator=(vector_type&&) noexcept =default;

        template <typename T>
        inline vec<T,SIZE> cast() const
        {
            T temp[SIZE];
            for (size_t i = 0; i < SIZE; ++i)
                temp[i] = static_cast<T>(elements[i]);
            return vec<T,SIZE>{temp};
        }





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







        inline void add(scalar_type s) { transform([s](scalar_param x){ return x + s; }); }
        inline void sub(scalar_type s) { transform([s](scalar_param x){ return x - s; }); }
        inline void mul(scalar_type s) { transform([s](scalar_param x){ return x * s; }); }
        inline void div(scalar_type s) { transform([s](scalar_param x){ return x / s; }); }

        ENABLE_IF_INTEGRAL
        inline void mod(scalar_type s) { transform([s](scalar_param x){ return x % s; }); }

        inline void add(vector_param p) { transform(std::plus      <scalar_type>()); }
        inline void sub(vector_param p) { transform(std::minus     <scalar_type>()); }
        inline void mul(vector_param p) { transform(std::multiplies<scalar_type>()); }
        inline void div(vector_param p) { transform(std::divides   <scalar_type>()); }

        ENABLE_IF_INTEGRAL
        inline void mod(vector_param p) { transform(std::modulus   <scalar_type>()); }

        [[nodiscard]] inline scalar_type sum       () const { return std::accumulate(begin(), end(), (scalar_type)0, std::plus<scalar_type>()); }
        [[nodiscard]] inline scalar_type sumSquared() const { return std::inner_product(begin(), end(), begin(), (scalar_type)0); }
        [[nodiscard]] inline scalar_type length    () const { return std::sqrt(sumSquared()); }

        ENABLE_IF_FLOAT [[nodiscard]] inline scalar_type dot     (vector_param v) const { return std::inner_product(begin(), end(), v.begin(), (scalar_type)0); }
        ENABLE_IF_FLOAT [[nodiscard]] inline scalar_type angle   (vector_param v) const { return (*this == v) ? (scalar_type)0 : std::acos(dot(v) / (length() * v.length())); }
        ENABLE_IF_FLOAT [[nodiscard]] inline scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        [[nodiscard]] inline vector_type neg() const
        { return transform(std::negate<scalar_type>()); }

        [[nodiscard]] inline vector_type reverse() const
        {
            elements_type temp;
            std::reverse_copy(begin(), end(), std::begin(temp));
            return vector_type(temp);
        }

        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type normal() const { return *this / length(); }
        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type project(vector_param base)   const { return base * ((*this * base) / base.sumSquared() ); }
        ENABLE_IF_FLOAT [[nodiscard]] inline vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * (scalar_type)2); }

        [[nodiscard]] inline bool isNan() const
        { return std::accumulate(begin(), end(), false, [](auto a, auto b){ return a|std::isnan(b); }); }

        [[nodiscard]] inline bool isInf() const
        { return std::accumulate(begin(), end(), false, [](auto a, auto b){ return a|std::isinf(b); }); }


        inline vector_type operator - () const { return neg(); }

        inline vector_type operator+ (scalar_param s) const { return transform([s](scalar_param x){ return x + s; }); }
        inline vector_type operator- (scalar_param s) const { return transform([s](scalar_param x){ return x - s; }); }
        inline vector_type operator* (scalar_param s) const { return transform([s](scalar_param x){ return x * s; }); }
        inline vector_type operator/ (scalar_param s) const { return transform([s](scalar_param x){ return x / s; }); }

        ENABLE_IF_INTEGRAL
        inline vector_type operator% (scalar_param s) const { return transform([s](scalar_param x){ return x % s; }); }

        inline vector_type operator+ (vector_param v) const { return transform(v, std::plus<scalar_type>()); }
        inline vector_type operator- (vector_param v) const { return transform(v, std::minus<scalar_type>()); }
        inline vector_type operator* (vector_param v) const { return transform(v, std::multiplies<scalar_type>()); }
        inline vector_type operator/ (vector_param v) const { return transform(v, std::divides<scalar_type>()); }

        ENABLE_IF_INTEGRAL
        inline vector_type operator% (vector_param v) const { return transform(v, std::modulus<scalar_type>()); }

        inline vector_type& operator += (vector_param p) { add(p); return *this; }
        inline vector_type& operator -= (vector_param p) { sub(p); return *this; }
        inline vector_type& operator *= (vector_param p) { mul(p); return *this; }
        inline vector_type& operator /= (vector_param p) { div(p); return *this; }

        ENABLE_IF_INTEGRAL
        inline vector_type& operator %= (vector_param p) { mod(p); return *this; }

        inline vector_type& operator += (scalar_param s) { add(s); return *this; }
        inline vector_type& operator -= (scalar_param s) { sub(s); return *this; }
        inline vector_type& operator *= (scalar_param s) { mul(s); return *this; }
        inline vector_type& operator /= (scalar_param s) { div(s); return *this; }

        ENABLE_IF_INTEGRAL
        inline vector_type& operator %= (scalar_param s) { mod(s); return *this; }

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

        static_assert(D > 1, "D must be greater than one");

    private:
        template <typename P> inline void transform(P predicate)
        {
            std::transform(begin(), end(), begin(), predicate);
        }

        template <typename P> inline vector_type transform(P predicate) const
        {
            vector_type copy = *this;
            std::transform(begin(), end(), copy.begin(), predicate);
            return copy;
        }

        template <typename P> inline vector_type transform(vector_param v, P predicate) const
        {
            vector_type copy = *this;
            std::transform(v.begin(), v.end(), copy.begin(), copy.begin(), predicate);
            return copy;
        }
    };

    template <size_t D> using vecn   = vec<real_t, D>;
    template <size_t D> using vecnf  = vec<float, D>;
    template <size_t D> using vecnd  = vec<double, D>;
    template <size_t D> using vecni  = vec<int, D>;
    template <size_t D> using vecnu  = vec<unsigned, D>;
    template <size_t D> using vecnb  = vec<std::uint8_t, D>;

    template <typename N, typename... T>
    inline vec<N,sizeof...(T)> make_vec(T&&... t)
    {
        return vec<N,sizeof...(T)>( {t...} );
    }

#if defined(UT_STL_INTEROP)
    template <typename N, size_t D>
    inline std::ostream& operator<<(std::ostream& os, vec<N,D> const& v)
    {
        static_assert(D > 0, "seriously?");

        os << '(' << v[0];
        for (size_t i = 1; i < v.size(); ++i)
            os << ", " << v[i];
        os << ')';
        return os;
    }

    template <size_t D>
    inline std::ostream& operator<<(std::ostream& os, vec<char,D> const& v)
    {
        static_assert(D > 0, "seriously?");
        return os << v.template cast<int>();
    }

    template <size_t D>
    inline std::ostream& operator<<(std::ostream& os, vec<unsigned char,D> const& v)
    {
        static_assert(D > 0, "seriously?");
        return os << v.template cast<unsigned>();
    }

    template <typename N, size_t D>
    inline std::string to_string(vec<N,D> const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }
#endif
}
