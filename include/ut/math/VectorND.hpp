// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#define ENABLE_IF_INTEGRAL template <typename N_ = N, typename = std::enable_if_t<std::is_integral_v<N_>>>
#define ENABLE_IF_FLOAT    template <typename N_ = N, typename = std::enable_if_t<std::is_floating_point_v<N_>>>

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define FOR_SIZE(i_)            for (size_t i_ = 0; i_ < SIZE; ++i)

#include <array>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <type_traits>
#include <cmath>
#include <cstdint>
#include <execution>

#include <cstddef>
#include <sstream>

namespace ut
{
    using real_t = float;
    using size_t = std::size_t;

    template <typename N, size_t D>
    struct vec;

    template<typename N, size_t D>
    constexpr bool less(vec<N,D> const& a, vec<N,D> const& b);

    template<typename N, size_t D>
    constexpr bool same(vec<N,D> const& a, vec<N,D> const& b);

    template <typename N, size_t D> struct vec
    {
        static_assert(D > 1, "D must be greater than one");

        using scalar_type       = N;
        using vector_type       = vec<N,D>;
        using vector_ref        = vector_type&;
        using scalar_ref        = scalar_type&;
        using vector_param      = vector_type const&;
        using scalar_param      = scalar_type;
        using elements_type     = scalar_type[D];

        size_t static constexpr SIZE = D;

        elements_type elements;

        //
        // ctor
        //

        M_DECL vec()
            : elements{}
        { for (auto& e: elements) e = (scalar_type)0; }

        M_DECL explicit vec(scalar_param s)
            : elements{}
        { for (auto& e: elements) e = s; }

        M_DECL vec(elements_type const& e)
            : elements{}
        { for (size_t i = 0; i < SIZE; ++i) this->elements[i] = e[i]; }

        M_DECL vec(vector_type const&)=default;
        M_DECL vec(vector_type&&) noexcept =default;

        M_DECL vec& operator=(vector_type const&)=default;
        M_DECL vec& operator=(vector_type&&) noexcept =default;

        template <typename T>
        M_DECL_PURE vec<T,SIZE> cast() const
        {
            T tmp[SIZE];
            FOR_SIZE(i) tmp[i] = static_cast<T>(elements[i]);
            return vec<T,SIZE>{tmp};
        }

        //
        // mutators
        //

        M_DECL void set(scalar_param s)
        { FOR_SIZE(i) this->elements[i] = s; }

        M_DECL void set(elements_type const& e)
        { FOR_SIZE(i) this->elements[i] = e[i];  }

        M_DECL void set(vector_param p)
        { FOR_SIZE(i) this->elements[i] = p.elements[i];  }

        M_DECL void add(scalar_param s) { transform([s](scalar_param x){ return x + s; }); }
        M_DECL void sub(scalar_param s) { transform([s](scalar_param x){ return x - s; }); }
        M_DECL void mul(scalar_param s) { transform([s](scalar_param x){ return x * s; }); }
        M_DECL void div(scalar_param s) { transform([s](scalar_param x){ return x / s; }); }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(scalar_param s) { transform([s](scalar_param x){ return x % s; }); }

        M_DECL void add(vector_param p) { transform(std::plus      <scalar_type>()); }
        M_DECL void sub(vector_param p) { transform(std::minus     <scalar_type>()); }
        M_DECL void mul(vector_param p) { transform(std::multiplies<scalar_type>()); }
        M_DECL void div(vector_param p) { transform(std::divides   <scalar_type>()); }

        ENABLE_IF_INTEGRAL
        M_DECL void mod(vector_param p) { transform(std::modulus   <scalar_type>()); }

        //
        // utilities
        //

        M_DECL_PURE scalar_type sum       () const { return std::accumulate(begin(), end(), (scalar_type)0, std::plus<scalar_type>()); }
        M_DECL_PURE scalar_type sumSquared() const { return std::inner_product(begin(), end(), begin(), (scalar_type)0); }
        M_DECL_PURE scalar_type length    () const { return std::sqrt(sumSquared()); }

        ENABLE_IF_FLOAT M_DECL_PURE scalar_type dot     (vector_param v) const { return std::inner_product(begin(), end(), v.begin(), (scalar_type)0); }
        ENABLE_IF_FLOAT M_DECL_PURE scalar_type angle   (vector_param v) const { return (*this == v) ? (scalar_type)0 : std::acos(dot(v) / (length() * v.length())); }
        ENABLE_IF_FLOAT M_DECL_PURE scalar_type distance(vector_param v) const { return ((*this) - v).length(); }

        M_DECL_PURE vector_type round() const
        {
            elements_type tmp;
            FOR_SIZE(i) tmp[i] = std::round(this->elements[i]);
            return {tmp};
        }

        M_DECL_PURE vector_type floor() const
        {
            elements_type tmp;
            FOR_SIZE(i) tmp[i] = std::floor(this->elements[i]);
            return {tmp};
        }

        M_DECL_PURE vector_type ceil() const
        {
            elements_type tmp;
            FOR_SIZE(i) tmp[i] = std::ceil(this->elements[i]);
            return {tmp};
        }

        M_DECL_PURE vector_type neg() const
        { return transform(std::negate<scalar_type>()); }

        M_DECL_PURE vector_type reverse() const
        {
            elements_type temp;
            std::reverse_copy(begin(), end(), std::begin(temp));
            return vector_type(temp);
        }

        ENABLE_IF_FLOAT M_DECL_PURE vector_type normal() const { return *this / length(); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type project(vector_param base)   const { return base * ((*this * base) / base.sumSquared() ); }
        ENABLE_IF_FLOAT M_DECL_PURE vector_type reflect(vector_param normal) const { return *this + (normal * -(normal * *this) * (scalar_type)2); }

        M_DECL_PURE bool isNan() const
        { return std::accumulate(begin(), end(), false, [](auto a, auto b){ return a|std::isnan(b); }); }

        M_DECL_PURE bool isInf() const
        { return std::accumulate(begin(), end(), false, [](auto a, auto b){ return a|std::isinf(b); }); }


        M_DECL_PURE vector_type operator - () const { return neg(); }

        M_DECL_PURE vector_type operator+ (scalar_param s) const { return transform([s](scalar_param x){ return x + s; }); }
        M_DECL_PURE vector_type operator- (scalar_param s) const { return transform([s](scalar_param x){ return x - s; }); }
        M_DECL_PURE vector_type operator* (scalar_param s) const { return transform([s](scalar_param x){ return x * s; }); }
        M_DECL_PURE vector_type operator/ (scalar_param s) const { return transform([s](scalar_param x){ return x / s; }); }

        ENABLE_IF_INTEGRAL
        M_DECL_PURE vector_type operator% (scalar_param s) const { return transform([s](scalar_param x){ return x % s; }); }

        M_DECL_PURE vector_type operator+ (vector_param v) const { return transform(v, std::plus<scalar_type>()); }
        M_DECL_PURE vector_type operator- (vector_param v) const { return transform(v, std::minus<scalar_type>()); }
        M_DECL_PURE vector_type operator* (vector_param v) const { return transform(v, std::multiplies<scalar_type>()); }
        M_DECL_PURE vector_type operator/ (vector_param v) const { return transform(v, std::divides<scalar_type>()); }

        ENABLE_IF_INTEGRAL
        M_DECL_PURE vector_type operator% (vector_param v) const { return transform(v, std::modulus<scalar_type>()); }

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
        M_DECL_PURE bool operator!= (vector_param v) const { return !(*this == v);  }
        M_DECL_PURE bool operator<  (vector_param v) const { return less(*this, v); }
        M_DECL_PURE bool operator>  (vector_param v) const { return *this < v;      }
        M_DECL_PURE bool operator<= (vector_param v) const { return !(*this < v);   }
        M_DECL_PURE bool operator>= (vector_param v) const { return !(v < *this);   }

        //
        // container utilities
        //

        M_DECL_PURE scalar_type  operator[] (size_t i) const { assert(i < SIZE); return elements[i]; }
        M_DECL      scalar_type& operator[] (size_t i)       { assert(i < SIZE); return elements[i]; }

        M_DECL_PURE scalar_type const* begin() const { return &elements[0]; }
        M_DECL      scalar_type*       begin()       { return &elements[0]; }

        M_DECL_PURE scalar_type const* end() const { return &elements[SIZE-1]; }
        M_DECL      scalar_type*       end()       { return &elements[SIZE-1]; }

        M_DECL_PURE scalar_type const* data() const { return elements; }
        M_DECL      scalar_type*       data()       { return elements; }

        M_DECL_PURE size_t size() const { return SIZE; }

    private:
        template <typename P>
        M_DECL void transform(P predicate)
        { std::transform(begin(), end(), begin(), predicate); }

        template <typename P>
        M_DECL_PURE vector_type transform(P predicate) const
        {
            vector_type copy = *this;
            std::transform(begin(), end(), copy.begin(), predicate);
            return copy;
        }

        template <typename P>
        M_DECL_PURE vector_type transform(vector_param v, P predicate) const
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
    constexpr inline vec<N,sizeof...(T)> make_vec(T&&... t)
    {
        return vec<N,sizeof...(T)>( {t...} );
    }

    template<typename N, size_t D>
    constexpr bool less(vec<N,D> const& a, vec<N,D> const& b)
    {
        // https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
        auto first1 = a.begin(), last1 = a.end();
        auto first2 = b.begin(), last2 = b.end();
        for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 )
        {
            if (*first1 < *first2) return true;
            if (*first2 < *first1) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template<typename N, size_t D>
    constexpr bool same(vec<N,D> const& a, vec<N,D> const& b)
    {
        for (size_t i = 0; i < D; ++i)
            if (a.elements[i] != b.elements[i])
                return false;
        return true;
    }

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
}

#undef M_DECL_PURE
#undef M_DECL
#undef FOR_SIZE
