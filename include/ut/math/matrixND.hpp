// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "real.hpp"

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

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define FOR_SIZE(i_)            for (size_t i_ = 0; i_ < PACK_SIZE; ++i)

#define zero_                   scalar_type{0}
#define one_                    scalar_type{1}
#define two_                    scalar_type{2}

namespace ut
{
    template <typename N, size_t Col, size_t Row>
    struct mat;

    template <typename N, size_t Col, size_t Row>
    constexpr bool less(mat<N,Col,Row> const& a, mat<N,Col,Row> const& b);

    template <typename N, size_t Col, size_t Row>
    constexpr bool same(mat<N,Col,Row> const& a, mat<N,Col,Row> const& b);

    template <typename N, size_t Col, size_t Row> struct mat
    {
        static_assert(Col > 1, "Col must be greater than one");
        static_assert(Row > 1, "Row must be greater than one");

        using scalar_type       = N;
        using matrix_type       = mat<N, Col, Row>;
        using matrix_ref        = matrix_type&;
        using scalar_ref        = scalar_type&;
        using matrix_param      = matrix_type const&;
        using scalar_param      = scalar_type;
        using pack_type         = N[Col*Row];

        size_t static constexpr PACK_SIZE   = Col*Row;
        size_t static constexpr ROWS        = Row;
        size_t static constexpr COLS        = Col;

        scalar_type pack[PACK_SIZE];

        //
        // ctor
        //

        M_DECL mat()
        { FOR_SIZE(i) pack[i] = zero_; }

        M_DECL explicit mat(scalar_param s)
        { FOR_SIZE(i) pack[i] = s; }

        M_DECL explicit mat(pack_type const& p)
        { FOR_SIZE(i) pack[i] = p[i]; }

        M_DECL mat(matrix_type const&)=default;
        M_DECL mat(matrix_type&&) noexcept =default;

        M_DECL mat& operator=(matrix_type const&)=default;
        M_DECL mat& operator=(matrix_type&&) noexcept =default;

        template <typename T>
        M_DECL_PURE mat<T,COLS,ROWS> cast() const
        {
            matrix_type tmp{noinit_tag{}};
            FOR_SIZE(i) tmp.pack[i] = static_cast<T>(pack[i]);
            return mat<T,COLS,ROWS>{tmp};
        }

        //
        // mutators
        //

        M_DECL void set(scalar_param s)
        { FOR_SIZE(i) this->pack[i] = s; }

        M_DECL void set(matrix_param p)
        { FOR_SIZE(i) this->pack[i] = p.pack[i];  }

        //
        // utilities
        //

        M_DECL_PURE matrix_type round() const
        {
            matrix_type tmp{noinit_tag{}};
            FOR_SIZE(i) tmp[i] = std::round(this->pack[i]);
            return tmp;
        }

        M_DECL_PURE matrix_type floor() const
        {
            matrix_type tmp{noinit_tag{}};
            FOR_SIZE(i) tmp[i] = std::floor(this->pack[i]);
            return tmp;
        }

        M_DECL_PURE matrix_type ceil() const
        {
            matrix_type tmp{noinit_tag{}};
            FOR_SIZE(i) tmp[i] = std::ceil(this->pack[i]);
            return tmp;
        }

        M_DECL_PURE bool isNan() const
        { return std::accumulate(begin(), end(), false, [](auto a, auto b){ return a|std::isnan(b); }); }

        M_DECL_PURE bool isInf() const
        { return std::accumulate(begin(), end(), false, [](auto a, auto b){ return a|std::isinf(b); }); }

        M_DECL_PURE bool operator== (matrix_param v) const { return same(*this, v); }
        M_DECL_PURE bool operator!= (matrix_param v) const { return !(*this == v);  }
        M_DECL_PURE bool operator<  (matrix_param v) const { return less(*this, v); }
        M_DECL_PURE bool operator>  (matrix_param v) const { return *this < v;      }
        M_DECL_PURE bool operator<= (matrix_param v) const { return !(*this < v);   }
        M_DECL_PURE bool operator>= (matrix_param v) const { return !(v < *this);   }

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
        struct noinit_tag{};
        explicit mat(noinit_tag) {}
    };

    template <size_t Col, size_t Row> using matn   = mat<real_t         , Col, Row>;
    template <size_t Col, size_t Row> using matnf  = mat<float          , Col, Row>;
    template <size_t Col, size_t Row> using matnd  = mat<double         , Col, Row>;
    template <size_t Col, size_t Row> using matni  = mat<int            , Col, Row>;
    template <size_t Col, size_t Row> using matnu  = mat<unsigned       , Col, Row>;
    template <size_t Col, size_t Row> using matnb  = mat<std::uint8_t   , Col, Row>;

    template<typename N, size_t Col, size_t Row>
    constexpr bool less(mat<N,Col,Row> const& a, mat<N,Col,Row> const& b)
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

    template<typename N, size_t Col, size_t Row>
    constexpr bool same(mat<N,Col,Row> const& a, mat<N,Col,Row> const& b)
    {
        for (size_t i = 0; i < (Col*Row); ++i)
            if (a.pack[i] != b.pack[i])
                return false;
        return true;
    }

    template<typename N, size_t Col, size_t Row>
    inline std::ostream& operator<<(std::ostream& os, mat<N,Col,Row> const& v)
    {
        for (size_t j = 0; j < Col; ++j)
        {
            size_t o = j*Row;

            os << '(' << v[o];
            for (size_t i = 1; i < Row; ++i)
                os << ", " << v[o+i];
            os << ')' << '\n';
        }

        return os;
    }

    template <size_t Col, size_t Row>
    inline std::ostream& operator<<(std::ostream& os, mat<char,Col,Row> const& v)
    {
        return os << v.template cast<int>();
    }

    template <size_t Col, size_t Row>
    inline std::ostream& operator<<(std::ostream& os, mat<unsigned char,Col,Row> const& v)
    {
        return os << v.template cast<unsigned>();
    }

    template <typename N, size_t Col, size_t Row>
    inline std::string to_string(mat<N,Col,Row> const& v)
    {
        std::ostringstream ss;
        ss << v;
        return ss.str();
    }
}

#undef zero_
#undef one_
#undef two_

#undef FOR_SIZE
#undef M_DECL_PURE
#undef M_DECL
