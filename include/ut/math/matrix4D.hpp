// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "matrixND.hpp"
#include "vector2D.hpp"
#include "vector3D.hpp"
#include "vector4D.hpp"

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

#define EXPAND_M(w_, r_, M) \
    M(w_, r_, 0 ) \
    M(w_, r_, 1 ) \
    M(w_, r_, 2 ) \
    M(w_, r_, 3 ) \
    M(w_, r_, 4 ) \
    M(w_, r_, 5 ) \
    M(w_, r_, 6 ) \
    M(w_, r_, 7 ) \
    M(w_, r_, 8 ) \
    M(w_, r_, 9 ) \
    M(w_, r_, 10) \
    M(w_, r_, 11) \
    M(w_, r_, 12) \
    M(w_, r_, 13) \
    M(w_, r_, 14) \
    M(w_, r_, 15)

#define M_SET_ZERO(w_, r_, i_)              w_[i_] = scalar_type{0};
#define M_SET_SCALAR(w_, r_, i_)            w_[i_] = r_;
#define M_SET_ARRAY(w_, r_, i_)             w_[i_] = r_[i_];
#define M_SET_ARRAY_CAST(w_, r_, i_)        w_[i_] = static_cast<T>(r_[i_]);

#define M_SET_ARRAY_ROUND(w_, r_, i_)       w_[i_] = std::round(r_[i_]);
#define M_SET_ARRAY_FLOOR(w_, r_, i_)       w_[i_] = std::floor(r_[i_]);
#define M_SET_ARRAY_CEIL(w_, r_, i_)        w_[i_] = std::ceil(r_[i_]);

#define M_SET_ARRAY_ADD(w_, r_, i_)         w_[i_] += r_[i_];
#define M_SET_ARRAY_SUB(w_, r_, i_)         w_[i_] -= r_[i_];

#define M_SET_ARRAY_IS_NAN(w_, r_, i_)      std::isnan(r_[i_]) ||
#define M_SET_ARRAY_IS_INF(w_, r_, i_)      std::isinf(r_[i_]) ||

#define zero_                               scalar_type{0}
#define one_                                scalar_type{1}
#define two_                                scalar_type{2}

namespace ut
{
    template <typename N> struct mat<N,4,4>
    {
        using scalar_type       = N;
        using vector2_type      = vec2x<N>;
        using vector3_type      = vec3x<N>;
        using vector4_type      = vec4x<N>;
        using matrix_type       = mat<N, 4,4>;

        using scalar_param      = scalar_type;
        using vector2_param     = vec2x<N> const&;
        using vector3_param     = vec3x<N> const&;
        using vector4_param     = vec4x<N> const&;
        using matrix_param      = matrix_type const&;
        using pack_type         = N[16];

        size_t static constexpr PACK_SIZE   = 16;
        size_t static constexpr ROWS        = 4;
        size_t static constexpr COLS        = 4;

        union
        {
            struct
            {
                scalar_type
                    m0, m4, m8 , m12,
                    m1, m5, m9 , m13,
                    m2, m6, m10, m14,
                    m3, m7, m11, m15;
            };

            pack_type pack;
        };

        //
        // ctor
        //

        M_DECL mat()
        { EXPAND_M(pack, _, M_SET_ZERO) }

        M_DECL explicit mat(scalar_param s)
        { EXPAND_M(pack, s, M_SET_SCALAR) }

        M_DECL explicit mat(pack_type const& p)
        { EXPAND_M(pack, p, M_SET_ARRAY) }

        M_DECL explicit mat(
            scalar_type m0, scalar_type m4, scalar_type m8 , scalar_type m12,
            scalar_type m1, scalar_type m5, scalar_type m9 , scalar_type m13,
            scalar_type m2, scalar_type m6, scalar_type m10, scalar_type m14,
            scalar_type m3, scalar_type m7, scalar_type m11, scalar_type m15) :
            m0{m0}, m4{m4}, m8 {m8 }, m12{m12},
            m1{m1}, m5{m5}, m9 {m9 }, m13{m13},
            m2{m2}, m6{m6}, m10{m10}, m14{m14},
            m3{m3}, m7{m7}, m11{m11}, m15{m15}
        {}

        M_DECL mat(matrix_type const&)=default;
        M_DECL mat(matrix_type&&) noexcept =default;

        M_DECL mat& operator=(matrix_type const&)=default;
        M_DECL mat& operator=(matrix_type&&) noexcept =default;

        template <typename T>
        M_DECL_PURE mat<T,COLS,ROWS> cast() const
        {
            matrix_type tmp{noinit_tag{}};
            EXPAND_M(tmp.pack, pack, M_SET_ARRAY_CAST)
            return tmp;
        }

        //
        // factories
        //

        M_DECL static matrix_type identity()
        {
            return matrix_type {
                one_ , zero_, zero_, zero_,
                zero_, one_ , zero_, zero_,
                zero_, zero_, one_ , zero_,
                zero_, zero_, zero_, one_
            };
        }

        M_DECL static matrix_type translate(scalar_type x, scalar_type y, scalar_type z)
        {
            return matrix_type {
                one_ , zero_, zero_, x,
                zero_, one_ , zero_, y,
                zero_, zero_, one_ , z,
                zero_, zero_, zero_, one_
            };
        }

        M_DECL static matrix_type scale(scalar_type x, scalar_type y, scalar_type z)
        {
            return matrix_type {
                x    , zero_, zero_, zero_,
                zero_, y    , zero_, zero_,
                zero_, zero_, z    , zero_,
                zero_, zero_, zero_, one_
            };
        }

        M_DECL static matrix_type scaleTranslate(
            scalar_type sx, scalar_type sy, scalar_type sz,
            scalar_type tx, scalar_type ty, scalar_type tz)
        {
            return matrix_type {
                sx   , zero_, zero_, tx,
                zero_, sy   , zero_, ty,
                zero_, zero_, sz   , tz,
                zero_, zero_, zero_, one_
            };
        }

        //
        // mutators
        //

        M_DECL void set(
            scalar_type m0, scalar_type m4, scalar_type m8 , scalar_type m12,
            scalar_type m1, scalar_type m5, scalar_type m9 , scalar_type m13,
            scalar_type m2, scalar_type m6, scalar_type m10, scalar_type m14,
            scalar_type m3, scalar_type m7, scalar_type m11, scalar_type m15)
        {
            this->m0=m0; this->m4=m4; this->m8 =m8 ; this->m12=m12;
            this->m1=m1; this->m5=m5; this->m9 =m9 ; this->m13=m13;
            this->m2=m2; this->m6=m6; this->m10=m10; this->m14=m14;
            this->m3=m3; this->m7=m7; this->m11=m11; this->m15=m15;
        }

        M_DECL void set(scalar_param s)     { EXPAND_M(pack, s     , M_SET_SCALAR) }
        M_DECL void set(matrix_param m)     { EXPAND_M(pack, m.pack, M_SET_ARRAY ) }
        M_DECL void set(pack_type const& p) { EXPAND_M(pack, p     , M_SET_ARRAY ) }

        M_DECL void add(matrix_param m) { EXPAND_M(pack, m.pack, M_SET_ARRAY_ADD) }
        M_DECL void sub(matrix_param m) { EXPAND_M(pack, m.pack, M_SET_ARRAY_SUB) }

        M_DECL void mul(matrix_param m)
        {
            matrix_type tmp{noinit_tag{}};
            mul(tmp, *this, m);
            set(tmp);
        }

        M_DECL void transpose()
        {
            matrix_type tmp{noinit_tag{}};

            tmp.m0  = m0; tmp.m1  = m4; tmp.m2  =  m8; tmp.m3  = m12;
            tmp.m4  = m1; tmp.m5  = m5; tmp.m6  =  m9; tmp.m7  = m13;
            tmp.m8  = m2; tmp.m9  = m6; tmp.m10 = m10; tmp.m11 = m14;
            tmp.m12 = m3; tmp.m13 = m7; tmp.m14 = m11; tmp.m15 = m15;

            set(tmp);
        }

        //
        // utilities
        //

        M_DECL_PURE matrix_type round() const
        {
            matrix_type tmp{noinit_tag{}};
            EXPAND_M(tmp.pack, pack, M_SET_ARRAY_ROUND);
            return tmp;
        }

        M_DECL_PURE matrix_type floor() const
        {
            matrix_type tmp{noinit_tag{}};
            EXPAND_M(tmp, pack, M_SET_ARRAY_FLOOR);
            return tmp;
        }

        M_DECL_PURE matrix_type ceil() const
        {
            matrix_type tmp{noinit_tag{}};
            EXPAND_M(tmp, pack, M_SET_ARRAY_CEIL);
            return tmp;
        }

        M_DECL_PURE bool isNan() const
        {
            return EXPAND_M(_, pack, M_SET_ARRAY_IS_NAN) false;
        }

        M_DECL_PURE bool isInf() const
        {
            return EXPAND_M(_, pack, M_SET_ARRAY_IS_INF) false;
        }

        M_DECL_PURE vector2_type transform(vector2_type const& p) const
        {
            return
            {
                p.x * m0 + p.y * m4,
                p.x * m1 + p.y * m5
            };
        }

        M_DECL_PURE vector3_type transform(vector3_type const& p) const
        {
            return
            {
                p.x * m0 + p.y * m4 + p.z * m8 ,
                p.x * m1 + p.y * m5 + p.z * m9 ,
                p.x * m2 + p.y * m6 + p.z * m10
            };
        }

        M_DECL_PURE vector4_type transform(vector4_type const& p) const
        {
            return
            {
                p.x * m0 + p.y * m4 + p.z * m8  + p.w * m12,
                p.x * m1 + p.y * m5 + p.z * m9  + p.w * m13,
                p.x * m2 + p.y * m6 + p.z * m10 + p.w * m14,
                p.x * m3 + p.y * m7 + p.z * m11 + p.w * m15
            };
        }

        M_DECL_PURE vector2_type transformPoint(vector2_type const& p) const
        {
            return
            {
                p.x * m0 + p.y * m4 + m12,
                p.x * m1 + p.y * m5 + m13
            };
        }

        M_DECL_PURE vector3_type transformPoint(vector3_type const& p) const
        {
            return
            {
                p.x * m0 + p.y * m4 + p.z * m8  + m12,
                p.x * m1 + p.y * m5 + p.z * m9  + m13,
                p.x * m2 + p.y * m6 + p.z * m10 + m14
            };
        }

        M_DECL_PURE vector4_type transformPoint(vector4_type const& p) const
        {
            return
            {
                p.x * m0 + p.y * m4 + p.z * m8  + m12,
                p.x * m1 + p.y * m5 + p.z * m9  + m13,
                p.x * m2 + p.y * m6 + p.z * m10 + m14,
                p.x * m3 + p.y * m7 + p.z * m11 + m15
            };
        }

        M_DECL_PURE matrix_type& operator+= (matrix_param m) { add(m); return *this; }
        M_DECL_PURE matrix_type& operator-= (matrix_param m) { sub(m); return *this; }
        M_DECL_PURE matrix_type& operator*= (matrix_param m) { mul(m); return *this; }

        M_DECL_PURE matrix_type operator+ (matrix_param m) const { auto tmp=*this; tmp.add(m); return tmp; }
        M_DECL_PURE matrix_type operator- (matrix_param m) const { auto tmp=*this; tmp.sub(m); return tmp; }
        M_DECL_PURE matrix_type operator* (matrix_param m) const { auto tmp=*this; tmp.mul(m); return tmp; }

        M_DECL_PURE bool operator== (matrix_param v) const { return same(*this, v); }
        M_DECL_PURE bool operator!= (matrix_param v) const { return !(*this == v);  }
        M_DECL_PURE bool operator<  (matrix_param v) const { return less(*this, v); }
        M_DECL_PURE bool operator>  (matrix_param v) const { return *this < v;      }
        M_DECL_PURE bool operator<= (matrix_param v) const { return !(*this < v);   }
        M_DECL_PURE bool operator>= (matrix_param v) const { return !(v < *this);   }

        //
        // static utilities
        //

        M_DECL static void mul(matrix_type& tmp, matrix_param a, matrix_param b)
        {
            tmp.m0  = a.m0  * b.m0 + a.m1  * b.m4 + a.m2  * b.m8  + a.m3  * b.m12;
            tmp.m1  = a.m0  * b.m1 + a.m1  * b.m5 + a.m2  * b.m9  + a.m3  * b.m13;
            tmp.m2  = a.m0  * b.m2 + a.m1  * b.m6 + a.m2  * b.m10 + a.m3  * b.m14;
            tmp.m3  = a.m0  * b.m3 + a.m1  * b.m7 + a.m2  * b.m11 + a.m3  * b.m15;
            tmp.m4  = a.m4  * b.m0 + a.m5  * b.m4 + a.m6  * b.m8  + a.m7  * b.m12;
            tmp.m5  = a.m4  * b.m1 + a.m5  * b.m5 + a.m6  * b.m9  + a.m7  * b.m13;
            tmp.m6  = a.m4  * b.m2 + a.m5  * b.m6 + a.m6  * b.m10 + a.m7  * b.m14;
            tmp.m7  = a.m4  * b.m3 + a.m5  * b.m7 + a.m6  * b.m11 + a.m7  * b.m15;
            tmp.m8  = a.m8  * b.m0 + a.m9  * b.m4 + a.m10 * b.m8  + a.m11 * b.m12;
            tmp.m9  = a.m8  * b.m1 + a.m9  * b.m5 + a.m10 * b.m9  + a.m11 * b.m13;
            tmp.m10 = a.m8  * b.m2 + a.m9  * b.m6 + a.m10 * b.m10 + a.m11 * b.m14;
            tmp.m11 = a.m8  * b.m3 + a.m9  * b.m7 + a.m10 * b.m11 + a.m11 * b.m15;
            tmp.m12 = a.m12 * b.m0 + a.m13 * b.m4 + a.m14 * b.m8  + a.m15 * b.m12;
            tmp.m13 = a.m12 * b.m1 + a.m13 * b.m5 + a.m14 * b.m9  + a.m15 * b.m13;
            tmp.m14 = a.m12 * b.m2 + a.m13 * b.m6 + a.m14 * b.m10 + a.m15 * b.m14;
            tmp.m15 = a.m12 * b.m3 + a.m13 * b.m7 + a.m14 * b.m11 + a.m15 * b.m15;
        }

        M_DECL static void inv(matrix_type& tmp, matrix_param mat)
        {
            // Cache the matrix values (speed optimization)
            float a00 = mat.m0 , a01 = mat.m1 , a02 = mat.m2 , a03 = mat.m3;
            float a10 = mat.m4 , a11 = mat.m5 , a12 = mat.m6 , a13 = mat.m7;
            float a20 = mat.m8 , a21 = mat.m9 , a22 = mat.m10, a23 = mat.m11;
            float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

            float b00 = a00*a11 - a01*a10;
            float b01 = a00*a12 - a02*a10;
            float b02 = a00*a13 - a03*a10;
            float b03 = a01*a12 - a02*a11;
            float b04 = a01*a13 - a03*a11;
            float b05 = a02*a13 - a03*a12;
            float b06 = a20*a31 - a21*a30;
            float b07 = a20*a32 - a22*a30;
            float b08 = a20*a33 - a23*a30;
            float b09 = a21*a32 - a22*a31;
            float b10 = a21*a33 - a23*a31;
            float b11 = a22*a33 - a23*a32;

            // Calculate the invert determinant (inlined to avoid double-caching)
            float invDet = one_ / (b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

            tmp.m0  = ( a11*b11 - a12*b10 + a13*b09)*invDet;
            tmp.m1  = (-a01*b11 + a02*b10 - a03*b09)*invDet;
            tmp.m2  = ( a31*b05 - a32*b04 + a33*b03)*invDet;
            tmp.m3  = (-a21*b05 + a22*b04 - a23*b03)*invDet;
            tmp.m4  = (-a10*b11 + a12*b08 - a13*b07)*invDet;
            tmp.m5  = ( a00*b11 - a02*b08 + a03*b07)*invDet;
            tmp.m6  = (-a30*b05 + a32*b02 - a33*b01)*invDet;
            tmp.m7  = ( a20*b05 - a22*b02 + a23*b01)*invDet;
            tmp.m8  = ( a10*b10 - a11*b08 + a13*b06)*invDet;
            tmp.m9  = (-a00*b10 + a01*b08 - a03*b06)*invDet;
            tmp.m10 = ( a30*b04 - a31*b02 + a33*b00)*invDet;
            tmp.m11 = (-a20*b04 + a21*b02 - a23*b00)*invDet;
            tmp.m12 = (-a10*b09 + a11*b07 - a12*b06)*invDet;
            tmp.m13 = ( a00*b09 - a01*b07 + a02*b06)*invDet;
            tmp.m14 = (-a30*b03 + a31*b01 - a32*b00)*invDet;
            tmp.m15 = ( a20*b03 - a21*b01 + a22*b00)*invDet;
        }

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
        struct noinit_tag {};
        M_DECL explicit mat(noinit_tag) {}

        //
        // pack layout check
        //

        M_DECL mat(std::nullptr_t);
    };

    template <typename N> constexpr mat<N,4,4>::mat(std::nullptr_t)
    {
        using mat_t = mat<N,4,4>;
        static_assert(offsetof(mat_t, m0 ) == offsetof(mat_t, pack) + sizeof(N)*0 , "wrong 'm0'  layout in pack");
        static_assert(offsetof(mat_t, m4 ) == offsetof(mat_t, pack) + sizeof(N)*1 , "wrong 'm4'  layout in pack");
        static_assert(offsetof(mat_t, m8 ) == offsetof(mat_t, pack) + sizeof(N)*2 , "wrong 'm8'  layout in pack");
        static_assert(offsetof(mat_t, m12) == offsetof(mat_t, pack) + sizeof(N)*3 , "wrong 'm12' layout in pack");
        static_assert(offsetof(mat_t, m1 ) == offsetof(mat_t, pack) + sizeof(N)*4 , "wrong 'm1'  layout in pack");
        static_assert(offsetof(mat_t, m5 ) == offsetof(mat_t, pack) + sizeof(N)*5 , "wrong 'm5'  layout in pack");
        static_assert(offsetof(mat_t, m9 ) == offsetof(mat_t, pack) + sizeof(N)*6 , "wrong 'm9'  layout in pack");
        static_assert(offsetof(mat_t, m13) == offsetof(mat_t, pack) + sizeof(N)*7 , "wrong 'm13' layout in pack");
        static_assert(offsetof(mat_t, m2 ) == offsetof(mat_t, pack) + sizeof(N)*8 , "wrong 'm2'  layout in pack");
        static_assert(offsetof(mat_t, m6 ) == offsetof(mat_t, pack) + sizeof(N)*9 , "wrong 'm6'  layout in pack");
        static_assert(offsetof(mat_t, m10) == offsetof(mat_t, pack) + sizeof(N)*10, "wrong 'm10' layout in pack");
        static_assert(offsetof(mat_t, m14) == offsetof(mat_t, pack) + sizeof(N)*11, "wrong 'm14' layout in pack");
        static_assert(offsetof(mat_t, m3 ) == offsetof(mat_t, pack) + sizeof(N)*12, "wrong 'm3'  layout in pack");
        static_assert(offsetof(mat_t, m7 ) == offsetof(mat_t, pack) + sizeof(N)*13, "wrong 'm7'  layout in pack");
        static_assert(offsetof(mat_t, m11) == offsetof(mat_t, pack) + sizeof(N)*14, "wrong 'm11' layout in pack");
        static_assert(offsetof(mat_t, m15) == offsetof(mat_t, pack) + sizeof(N)*15, "wrong 'm15' layout in pack");
    }

    template <typename N> using mat4x = mat<N,4,4>;

    typedef mat4x<real_t>       mat4;
    typedef mat4x<float>        mat4f;
    typedef mat4x<double>       mat4d;
    typedef mat4x<int>          mat4i;
    typedef mat4x<unsigned>     mat4u;
    typedef mat4x<std::uint8_t> mat4b;
}

#undef M_DECL_PURE
#undef M_DECL
#undef EXPAND_M
#undef M_SET_ZERO
#undef M_SET_SCALAR
#undef M_SET_ARRAY
#undef M_SET_ARRAY_CAST
#undef M_SET_ARRAY_ROUND
#undef M_SET_ARRAY_FLOOR
#undef M_SET_ARRAY_CEIL
#undef M_SET_ARRAY_ADD
#undef M_SET_ARRAY_SUB
#undef M_SET_ARRAY_IS_NAN
#undef M_SET_ARRAY_IS_INF
#undef zero_
#undef one_
#undef two_
