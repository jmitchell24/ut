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

namespace ut
{
    template <typename N, size_t D> struct VecND
    {
        typedef N               Num;
        typedef Num&            NumRef;
        typedef Num             NumParam;

        typedef VecND<Num,D>    Vec;
        typedef Vec&            VecRef;    
        typedef Vec const&      VecParam;

        typedef N               Components[D];

        typedef std::size_t     size_type;
        typedef Num*            iterator;
        typedef Num const*      const_iterator;

        size_type static constexpr Size = D;

        Components components;

        inline VecND()
        {
            std::fill(std::begin(components), std::end(components), (Num)0);
        }

        constexpr VecND(Vec&&)=default;
        constexpr VecND(Vec const&)=default;

        template <typename P>
        inline VecND(VecND<P,D> const& v)
        {
            for (size_t i = 0; i < Size; ++i)
                components[i] = (Num)v.components[i];
        }

        inline explicit VecND(NumParam n)
        {
            std::fill(begin(), end(), n);
        }

//        inline VecND(std::initializer_list<N>&& il)
//            : VecND(make_vector<N,D>(il.begin(), il.end()))
//        {}

        inline explicit VecND(Components const& components)
        {
            std::copy(std::begin(components), std::end(components), begin());
        }

        inline Num sum       () const { return std::accumulate(begin(), end(), (Num)0, std::plus<Num>()); }
        inline Num sumSquared() const { return std::inner_product(begin(), end(), begin(), (Num)0); }
        inline Num length    () const { return std::sqrt(sumSquared()); }
        inline Num dot     (VecParam v) const { return std::inner_product(begin(), end(), v.begin(), (Num)0); }
        inline Num angle   (VecParam v) const { return (*this==v) ? (Num)0 : std::acos(dot(v) / (length()*v.length())); }
        inline Num distance(VecParam v) const { return ((*this) - v).length(); }

        inline Vec round() const
        {
            Components components;
            for (size_type i = 0; i < Size; ++i)
                components[i] = std::round(this->components[i]);
            return Vec(components);
        }

        inline Vec floor() const
        {
            Components components;
            for (size_type i = 0; i < Size; ++i)
                components[i] = std::floor(this->components[i]);
            return Vec(components);
        }

        inline Vec ceil() const
        {
            Components components;
            for (size_type i = 0; i < Size; ++i)
                components[i] = std::ceil(this->components[i]);
            return Vec(components);
        }

        inline Vec reverse() const
        {
            Components copy;
            std::reverse_copy(begin(), end(), std::begin(copy));
            return Vec(copy);
        }

        inline Vec normal() const { return *this / length(); }
        inline Vec project(VecParam base)   const { return base * ( (*this*base) / base.sumSquared() ); }
        inline Vec reflect(VecParam normal) const { return *this + (normal * -(normal * *this) * (Num)2); }

        inline bool isNan() const
        {
            bool nan = false;
            for (size_type i = 0; i < Size; ++i)
                nan |= std::isnan(components[i]);
            return nan;
        }

        inline bool isInf() const
        {
            bool inf = false;
            for (size_type i = 0; i < Size; ++i)
                inf |= std::isinf(components[i]);
            return inf;
        }

        inline Vec operator- () const { return transform(std::negate<Num>()); }

        inline Vec operator+ (VecParam v) const { return transform(v,std::plus<Num>()); }
        inline Vec operator- (VecParam v) const { return transform(v,std::minus<Num>()); }
        inline Vec operator* (VecParam v) const { return transform(v,std::multiplies<Num>()); }
        inline Vec operator/ (VecParam v) const { return transform(v,std::divides<Num>()); }

        inline Vec operator+ (NumParam n) const { return transform([n](NumParam x){ return x + n; }); }
        inline Vec operator- (NumParam n) const { return transform([n](NumParam x){ return x - n; }); }
        inline Vec operator* (NumParam n) const { return transform([n](NumParam x){ return x * n; }); }
        inline Vec operator/ (NumParam n) const { return transform([n](NumParam x){ return x / n; }); }

        inline VecRef operator=  (Vec&&)=default;
        inline VecRef operator=  (VecParam v) { std::copy(v.begin(), v.end(), begin()); return *this; }
        inline VecRef operator+= (VecParam v) { return (*this = *this + v); }
        inline VecRef operator-= (VecParam v) { return (*this = *this - v); }
        inline VecRef operator*= (VecParam v) { return (*this = *this * v); }
        inline VecRef operator/= (VecParam v) { return (*this = *this / v); }

        inline VecRef operator=  (NumParam n) { std::fill(begin(), end(), n); return *this; }
        inline VecRef operator+= (NumParam n) { return (*this = *this + n); }
        inline VecRef operator-= (NumParam n) { return (*this = *this - n); }
        inline VecRef operator*= (NumParam n) { return (*this = *this * n); }
        inline VecRef operator/= (NumParam n) { return (*this = *this / n); }

        inline bool operator== (VecParam v) const { return std::equal(begin(), end(), v.begin()); }
        inline bool operator!= (VecParam v) const { return !(*this == v); }
        inline bool operator<  (VecParam v) const { return std::lexicographical_compare(begin(), end(), v.begin(), v.end()); }
        inline bool operator>  (VecParam v) const { return *this < v; }
        inline bool operator<= (VecParam v) const { return !(*this < v); }
        inline bool operator>= (VecParam v) const { return !(v < *this); }

        inline NumParam operator[] (size_type const i) const { assert(i < Size); return components[i]; }
        inline Num    & operator[] (size_type const i)       { assert(i < Size); return components[i]; }

        inline iterator       begin()       { return std::begin(components); }
        inline const_iterator begin() const { return std::begin(components); }

        inline iterator       end()       { return std::end(components); }
        inline const_iterator end() const { return std::end(components); }

        static_assert(D > 1, "Dim must be greater than one");

    private:
        template <typename P> inline Vec transform(P predicate) const
        {
            Vec copy = *this;
            std::transform(copy.begin(), copy.end(), copy.begin(), predicate);
            return copy;
        }

        template <typename P> inline Vec transform(VecParam v, P predicate) const
        {
            Vec copy = *this;
            std::transform(v.begin(), v.end(), copy.begin(), copy.end(), predicate);
            return copy;
        }
    };

    template <typename N, size_t D, typename It>
    inline VecND<N,D> make_vector(It first, It last)
    {
        VecND<N,D> v;
        auto const e = v.end();
        auto       c = v.begin();
        for (auto i = first; i != last && c != e; ++i, ++c)
            *c = (N)*i;
        return v;
    }
}
