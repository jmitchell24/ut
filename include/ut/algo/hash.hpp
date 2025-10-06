//
// Created by james on 31/05/22.
//

//
//  an implementation of the fnv hash algorithm (FNV-1a):
//  http://www.isthe.com/chongo/tech/comp/fnv/index.html
//

//
//  adapted from the specific implementation in this SO post:
//  https://stackoverflow.com/a/34597785
//

#pragma once

#include <cstdint>
#include <cassert>
#include <sstream>
#include <type_traits>

#define M_DECL_PURE         [[nodiscard]] inline constexpr
#define M_DECL              inline constexpr

#define TM_DECL_PURE(t_)    template<typename t_> M_DECL_PURE
#define TM_DECL(t_)         template<typename t_> M_DECL

namespace ut
{
    /// \brief A utility struct for computing the FNV1 hash
    /// @link  https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    template <typename N, N O, N P>
    struct basic_digest
    {
        static_assert(std::is_unsigned<N>::value, "need unsigned integer");

        using size_type     = std::size_t;
        using value_type    = N;
        using digest_type   = basic_digest<N, O, P>;
        using digest_param  = digest_type const&;

        N static constexpr OFFSET_BASIS     = O;
        N static constexpr PRIME            = P;

        M_DECL basic_digest() : m_state {OFFSET_BASIS} {}

        M_DECL explicit basic_digest(value_type state) : m_state{state} {}

        M_DECL explicit operator value_type() const { return value(); }

        M_DECL basic_digest(basic_digest const&)=default;
        M_DECL basic_digest(basic_digest&&) noexcept =default;

        M_DECL basic_digest& operator=(basic_digest const&)=default;
        M_DECL basic_digest& operator=(basic_digest&&) noexcept =default;

        M_DECL_PURE value_type value() const { return m_state; }

        M_DECL void reset() { m_state = OFFSET_BASIS; }

        //
        // addition operators
        //

        M_DECL_PURE digest_type operator+ (digest_param d) const { return withDigest(d); }
        M_DECL digest_type& operator+= (digest_param d) { putDigest(d); return *this; }

        //
        // comparison operators
        //

        M_DECL_PURE bool operator== (digest_param d) const { return m_state == d.m_state; }
        M_DECL_PURE bool operator!= (digest_param d) const { return !(*this == d); }
        M_DECL_PURE bool operator<  (digest_param d) const { return m_state < d.m_state; }
        M_DECL_PURE bool operator>  (digest_param d) const { return *this < d; }
        M_DECL_PURE bool operator<= (digest_param d) const { return !(*this < d); }
        M_DECL_PURE bool operator>= (digest_param d) const { return !(d < *this); }

        //
        // copy with hash
        //

        TM_DECL_PURE(...Ts)  digest_type withArgs (Ts&&... ts)                  const { digest_type tmp=*this; tmp.putArgs(ts...);         return tmp; }
        TM_DECL_PURE(T)      digest_type withRange(T&& t)                       const { digest_type tmp=*this; tmp.putRange(t);            return tmp; }
        TM_DECL_PURE(It)     digest_type withRange(It&& first, It&& last)       const { digest_type tmp=*this; tmp.putRange(first, last);  return tmp; }
        TM_DECL_PURE(T)      digest_type withValue(T&& t)                       const { digest_type tmp=*this; tmp.putValue(t);            return tmp; }
        M_DECL_PURE          digest_type withDigest (digest_type d)             const { digest_type tmp=*this; tmp.putDigest(d);           return tmp; }
        M_DECL_PURE          digest_type withBytes(void* data, size_type size)  const { digest_type tmp=*this; tmp.putBytes(data, size);   return tmp; }

        //
        // incremental hash
        //

        TM_DECL(...Ts)  void putArgs (Ts&&... ts)           { (putValue(ts), ...); }
        TM_DECL(T)      void putRange(T&& t)                { for (auto&& x: t) putValue(x); }
        TM_DECL(It)     void putRange(It first, It last)    { for (; first != last; ++first) putValue(*first); }
        TM_DECL(T)      void putValue(T const& t)           { putBytes(&t, sizeof(t)); }

        M_DECL void putDigest(digest_type x)
        {
            putValue(x.m_state);
        }

        M_DECL void putBytes(void const* data, size_type size) noexcept
        {
            assert(data != nullptr);
            assert(m_state != 0);

            auto cdata  = static_cast<std::uint8_t const*>(data);
            auto acc    = m_state;

            for (size_type i = 0; i < size; ++i)
            {
                auto next = (size_type) *(cdata+i);
                acc = (acc ^ next) * PRIME;
            }

            m_state = acc;
        }

        //
        // single-put convenience methods
        //

        TM_DECL(...Ts)  static digest_type args  (Ts&&... ts)                  { digest_type tmp; tmp.putArgs(ts...);         return tmp; }
        TM_DECL(T)      static digest_type range (T&& t)                       { digest_type tmp; tmp.putRange(t);            return tmp; }
        TM_DECL(It)     static digest_type range (It&& first, It&& last)       { digest_type tmp; tmp.putRange(first, last);  return tmp; }
        TM_DECL(T)      static digest_type value (T&& t)                       { digest_type tmp; tmp.putValue(t);            return tmp; }
        M_DECL          static digest_type digest(digest_type d)               { digest_type tmp; tmp.putDigest(d);                    return tmp; }
        M_DECL          static digest_type bytes (void* data, size_type size)  { digest_type tmp; tmp.putBytes(data, size);            return tmp; }

        TM_DECL(...Ts)  static digest_type nextArgs  (digest_param d, Ts&&... ts)                { digest_type tmp(d); tmp.putArgs(ts...);         return tmp; }
        TM_DECL(T)      static digest_type nextRange (digest_param d, T&& t)                     { digest_type tmp(d); tmp.putRange(t);            return tmp; }
        TM_DECL(It)     static digest_type nextRange (digest_param d, It&& first, It&& last)     { digest_type tmp(d); tmp.putRange(first, last);  return tmp; }
        TM_DECL(T)      static digest_type nextValue (digest_param d, T&& t)                     { digest_type tmp(d); tmp.putValue(t);            return tmp; }
        M_DECL          static digest_type nextDigest(digest_param d, digest_type y)             { digest_type tmp(d); tmp.putDigest(y);                    return tmp; }
        M_DECL          static digest_type nextBytes (digest_param d, void* data, size_type size){ digest_type tmp(d); tmp.putBytes(data, size);            return tmp; }



    private:
        value_type m_state;
    };

    using digest32_t = std::uint32_t;
    using digest64_t = std::uint64_t;
    using digest_t   = digest64_t;

    using digest32  = basic_digest<std::uint32_t, 2166136261u, 16777619u>;
    using digest64  = basic_digest<std::uint64_t, 14695981039346656037u, 1099511628211u>;
    using digest    = digest64;

    template <typename N, N O, N P>
    constexpr std::ostream& operator<<(std::ostream& os, basic_digest<N,O,P> const& d)
    {
        static_assert(sizeof(N) <= 8, "your int is too big");
        static_assert(std::is_integral_v<N>, "your int isn't an int");

        char buf[100];

        if constexpr(sizeof(N)==1)
            snprintf(buf, 100, "%02x", d.value());
        if constexpr(sizeof(N)==2)
            snprintf(buf, 100, "%04x", d.value());
        if constexpr(sizeof(N)==4)
            snprintf(buf, 100, "%08x", d.value());
        if constexpr(sizeof(N)==8)
            snprintf(buf, 100, "%016lx", d.value());

        os << buf;
        return os;
    }

    template <typename N, N O, N P>
    std::string to_string(basic_digest<N,O,P> const& d)
    {
        std::ostringstream ss;
        ss << d;
        return ss.str();
    }
}

#undef M_DECL_PURE
#undef M_DECL

#undef TM_DECL_PURE
#undef TM_DECL
