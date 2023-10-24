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
        struct transpose_ctor_tag {};

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
                    m11, m21, m31, m41,
                    m12, m22, m32, m42,
                    m13, m23, m33, m43,
                    m14, m24, m34, m44;
            };

            pack_type pack;
        };

        //
        // ctor
        //

        M_DECL mat() : mat{
            one_ , zero_, zero_, zero_,
            zero_, one_ , zero_, zero_,
            zero_, zero_, one_ , zero_,
            zero_, zero_, zero_, one_
        } {}

        M_DECL explicit mat(
                     scalar_type m11, scalar_type m12, scalar_type m13, scalar_type m14,
                     scalar_type m21, scalar_type m22, scalar_type m23, scalar_type m24,
                     scalar_type m31, scalar_type m32, scalar_type m33, scalar_type m34,
                     scalar_type m41, scalar_type m42, scalar_type m43, scalar_type m44) :
            m11{m11}, m21{m21}, m31{m31}, m41{m41},
            m12{m12}, m22{m22}, m32{m32}, m42{m42},
            m13{m13}, m23{m23}, m33{m33}, m43{m43},
            m14{m14}, m24{m24}, m34{m34}, m44{m44}
        {}

        M_DECL explicit mat(
            scalar_type m11, scalar_type m12, scalar_type m13, scalar_type m14,
            scalar_type m21, scalar_type m22, scalar_type m23, scalar_type m24,
            scalar_type m31, scalar_type m32, scalar_type m33, scalar_type m34,
            scalar_type m41, scalar_type m42, scalar_type m43, scalar_type m44,
            transpose_ctor_tag) :
            m11{m11}, m21{m12}, m31{m13}, m41{m14},
            m12{m21}, m22{m22}, m32{m23}, m42{m24},
            m13{m31}, m23{m32}, m33{m33}, m43{m34},
            m14{m41}, m24{m42}, m34{m43}, m44{m44}
        {}

        M_DECL explicit mat(scalar_param s) { EXPAND_M(pack, s, M_SET_SCALAR) }
        M_DECL explicit mat(pack_type const& p) { EXPAND_M(pack, p, M_SET_ARRAY) }

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

        M_DECL void set(scalar_param s)     { EXPAND_M(pack, s     , M_SET_SCALAR) }
        M_DECL void set(matrix_param m)     { EXPAND_M(pack, m.pack, M_SET_ARRAY ) }
        M_DECL void set(pack_type const& p) { EXPAND_M(pack, p     , M_SET_ARRAY ) }

        M_DECL void add(matrix_param m) { EXPAND_M(pack, m.pack, M_SET_ARRAY_ADD) }
        M_DECL void sub(matrix_param m) { EXPAND_M(pack, m.pack, M_SET_ARRAY_SUB) }

        M_DECL void mul(matrix_param m)
        {
            matrix_type tmp{noinit_tag{}};
            mul(tmp.pack, pack, m.pack);
            set(tmp);
        }

        M_DECL void transpose()
        {
            matrix_type tmp{noinit_tag{}};

            tmp.m11=m11; tmp.m21=m12; tmp.m31=m13; tmp.m41=m14;
            tmp.m12=m21; tmp.m22=m22; tmp.m32=m23; tmp.m42=m24;
            tmp.m13=m31; tmp.m23=m32; tmp.m33=m33; tmp.m43=m34;
            tmp.m14=m41; tmp.m24=m42; tmp.m34=m43; tmp.m44=m44;

            set(tmp);
        }

        M_DECL void invert()
        {
            matrix_type tmp{noinit_tag{}};
            inv(tmp.pack, pack);
            set(tmp);
        }

        //
        // utilities
        //

        M_DECL_PURE scalar_type trace() const
        {
            return m11 + m22 + m33 + m44;
        }

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
                p.x * pack[0] + p.y * pack[4],
                p.x * pack[1] + p.y * pack[5]
            };
        }

        M_DECL_PURE vector3_type transform(vector3_type const& p) const
        {
            return
            {
                p.x * pack[0] + p.y * pack[4] + p.z * pack[8 ],
                p.x * pack[1] + p.y * pack[5] + p.z * pack[9 ],
                p.x * pack[2] + p.y * pack[6] + p.z * pack[10]
            };
        }

        M_DECL_PURE vector4_type transform(vector4_type const& p) const
        {
            return
            {
                p.x * pack[0] + p.y * pack[4] + p.z * pack[8 ] + p.w * pack[12],
                p.x * pack[1] + p.y * pack[5] + p.z * pack[9 ] + p.w * pack[13],
                p.x * pack[2] + p.y * pack[6] + p.z * pack[10] + p.w * pack[14],
                p.x * pack[3] + p.y * pack[7] + p.z * pack[11] + p.w * pack[15]
            };
        }

        M_DECL_PURE vector2_type transformPoint(vector2_type const& p) const
        {
            return
            {
                p.x * pack[0] + p.y * pack[4] + pack[12],
                p.x * pack[1] + p.y * pack[5] + pack[13]
            };
        }

        M_DECL_PURE vector3_type transformPoint(vector3_type const& p) const
        {
            return
            {
                p.x * pack[0] + p.y * pack[4] + p.z * pack[8 ] + pack[12],
                p.x * pack[1] + p.y * pack[5] + p.z * pack[9 ] + pack[13],
                p.x * pack[2] + p.y * pack[6] + p.z * pack[10] + pack[14]
            };
        }

        M_DECL_PURE vector4_type transformPoint(vector4_type const& p) const
        {
            return
            {
                p.x * pack[0] + p.y * pack[4] + p.z * pack[8 ] + pack[12],
                p.x * pack[1] + p.y * pack[5] + p.z * pack[9 ] + pack[13],
                p.x * pack[2] + p.y * pack[6] + p.z * pack[10] + pack[14],
                p.x * pack[3] + p.y * pack[7] + p.z * pack[11] + pack[15]
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

        M_DECL static void mul(pack_type& tmp, pack_type const& a, pack_type const& b)
        {
            tmp[0 ] = a[0 ] * b[0] + a[1 ] * b[4] + a[2 ] * b[8 ] + a[3 ] * b[12];
            tmp[1 ] = a[0 ] * b[1] + a[1 ] * b[5] + a[2 ] * b[9 ] + a[3 ] * b[13];
            tmp[2 ] = a[0 ] * b[2] + a[1 ] * b[6] + a[2 ] * b[10] + a[3 ] * b[14];
            tmp[3 ] = a[0 ] * b[3] + a[1 ] * b[7] + a[2 ] * b[11] + a[3 ] * b[15];
            tmp[4 ] = a[4 ] * b[0] + a[5 ] * b[4] + a[6 ] * b[8 ] + a[7 ] * b[12];
            tmp[5 ] = a[4 ] * b[1] + a[5 ] * b[5] + a[6 ] * b[9 ] + a[7 ] * b[13];
            tmp[6 ] = a[4 ] * b[2] + a[5 ] * b[6] + a[6 ] * b[10] + a[7 ] * b[14];
            tmp[7 ] = a[4 ] * b[3] + a[5 ] * b[7] + a[6 ] * b[11] + a[7 ] * b[15];
            tmp[8 ] = a[8 ] * b[0] + a[9 ] * b[4] + a[10] * b[8 ] + a[11] * b[12];
            tmp[9 ] = a[8 ] * b[1] + a[9 ] * b[5] + a[10] * b[9 ] + a[11] * b[13];
            tmp[10] = a[8 ] * b[2] + a[9 ] * b[6] + a[10] * b[10] + a[11] * b[14];
            tmp[11] = a[8 ] * b[3] + a[9 ] * b[7] + a[10] * b[11] + a[11] * b[15];
            tmp[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8 ] + a[15] * b[12];
            tmp[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9 ] + a[15] * b[13];
            tmp[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
            tmp[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
        }

        M_DECL static void inv(pack_type& tmp, pack_type const& mat)
        {
            // Cache the matrix values (speed optimization)
            float a00 = mat[0 ], a01 = mat[1 ], a02 = mat[2 ], a03 = mat[3 ];
            float a10 = mat[4 ], a11 = mat[5 ], a12 = mat[6 ], a13 = mat[7 ];
            float a20 = mat[8 ], a21 = mat[9 ], a22 = mat[10], a23 = mat[11];
            float a30 = mat[12], a31 = mat[13], a32 = mat[14], a33 = mat[15];

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

            tmp[0 ] = ( a11*b11 - a12*b10 + a13*b09)*invDet;
            tmp[1 ] = (-a01*b11 + a02*b10 - a03*b09)*invDet;
            tmp[2 ] = ( a31*b05 - a32*b04 + a33*b03)*invDet;
            tmp[3 ] = (-a21*b05 + a22*b04 - a23*b03)*invDet;
            tmp[4 ] = (-a10*b11 + a12*b08 - a13*b07)*invDet;
            tmp[5 ] = ( a00*b11 - a02*b08 + a03*b07)*invDet;
            tmp[6 ] = (-a30*b05 + a32*b02 - a33*b01)*invDet;
            tmp[7 ] = ( a20*b05 - a22*b02 + a23*b01)*invDet;
            tmp[8 ] = ( a10*b10 - a11*b08 + a13*b06)*invDet;
            tmp[9 ] = (-a00*b10 + a01*b08 - a03*b06)*invDet;
            tmp[10] = ( a30*b04 - a31*b02 + a33*b00)*invDet;
            tmp[11] = (-a20*b04 + a21*b02 - a23*b00)*invDet;
            tmp[12] = (-a10*b09 + a11*b07 - a12*b06)*invDet;
            tmp[13] = ( a00*b09 - a01*b07 + a02*b06)*invDet;
            tmp[14] = (-a30*b03 + a31*b01 - a32*b00)*invDet;
            tmp[15] = ( a20*b03 - a21*b01 + a22*b00)*invDet;
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
        static_assert(offsetof(mat_t, m11) == offsetof(mat_t, pack) + sizeof(N)*0 , "wrong 'm11' layout in pack");
        static_assert(offsetof(mat_t, m21) == offsetof(mat_t, pack) + sizeof(N)*1 , "wrong 'm21' layout in pack");
        static_assert(offsetof(mat_t, m31) == offsetof(mat_t, pack) + sizeof(N)*2 , "wrong 'm31' layout in pack");
        static_assert(offsetof(mat_t, m41) == offsetof(mat_t, pack) + sizeof(N)*3 , "wrong 'm41' layout in pack");
        static_assert(offsetof(mat_t, m12) == offsetof(mat_t, pack) + sizeof(N)*4 , "wrong 'm12' layout in pack");
        static_assert(offsetof(mat_t, m22) == offsetof(mat_t, pack) + sizeof(N)*5 , "wrong 'm22' layout in pack");
        static_assert(offsetof(mat_t, m32) == offsetof(mat_t, pack) + sizeof(N)*6 , "wrong 'm32' layout in pack");
        static_assert(offsetof(mat_t, m42) == offsetof(mat_t, pack) + sizeof(N)*7 , "wrong 'm42' layout in pack");
        static_assert(offsetof(mat_t, m13) == offsetof(mat_t, pack) + sizeof(N)*8 , "wrong 'm13' layout in pack");
        static_assert(offsetof(mat_t, m23) == offsetof(mat_t, pack) + sizeof(N)*9 , "wrong 'm23' layout in pack");
        static_assert(offsetof(mat_t, m33) == offsetof(mat_t, pack) + sizeof(N)*10, "wrong 'm33' layout in pack");
        static_assert(offsetof(mat_t, m43) == offsetof(mat_t, pack) + sizeof(N)*11, "wrong 'm43' layout in pack");
        static_assert(offsetof(mat_t, m14) == offsetof(mat_t, pack) + sizeof(N)*12, "wrong 'm14' layout in pack");
        static_assert(offsetof(mat_t, m24) == offsetof(mat_t, pack) + sizeof(N)*13, "wrong 'm24' layout in pack");
        static_assert(offsetof(mat_t, m34) == offsetof(mat_t, pack) + sizeof(N)*14, "wrong 'm34' layout in pack");
        static_assert(offsetof(mat_t, m44) == offsetof(mat_t, pack) + sizeof(N)*15, "wrong 'm44' layout in pack");
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
