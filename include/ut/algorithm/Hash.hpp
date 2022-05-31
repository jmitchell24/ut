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
#include <type_traits>

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

namespace ut
{
    template <typename ResultT, ResultT OffsetBasis, ResultT Prime>
    struct basic_hasher
    {
        static_assert(std::is_unsigned<ResultT>::value, "need unsigned integer");

        using size_type     = std::size_t;
        using result_type   = ResultT;
        using hasher_type   = basic_hasher<ResultT, OffsetBasis, Prime>;

        M_DECL basic_hasher() noexcept : m_state {OffsetBasis} {}

        M_DECL_PURE result_type digest() const noexcept { return m_state; }

        M_DECL void reset() { m_state = OffsetBasis; }

        template <typename T> M_DECL void putValue(T const& t)
        {
            putBytes(&t, sizeof(t));
        }

        template <typename... Ts> M_DECL void putArgs(Ts&&... ts)
        {
            (putValue(ts), ...);
        }

        template <typename It> M_DECL void putRange(It first, It last)
        {
            for (; first != last; ++first)
                putValue(*first);
        }

        template <typename T> M_DECL void putRange(T&& t)
        {
            for (auto&& x: t)
                putValue(x);
        }

        M_DECL void putBytes(void const* data, size_type size) noexcept
        {
            assert(data != nullptr);

            auto cdata  = static_cast<std::uint8_t const*>(data);
            auto acc    = m_state;

            for (size_type i = 0; i < size; ++i)
            {
                auto next = (size_type)cdata[i];
                acc = (acc ^ next) * Prime;
            }

            m_state = acc;
        }

        template <typename... Ts> M_DECL static result_type args(Ts&&... ts)
        { hasher_type h; h.template putArgs(ts...); return h.digest(); }

        template <typename T> M_DECL static result_type range(T&& t)
        { hasher_type h; h.template putRange(t); return h.digest(); }

        template <typename It> M_DECL static result_type range(It&& first, It&& last)
        { hasher_type h; h.template putRange(first, last); return h.digest(); }

        template <typename T> M_DECL static result_type value(T&& t)
        { hasher_type h; h.putValue(t); return h.digest(); }

        M_DECL static result_type bytes(void* data, size_t size)
        { hasher_type h; h.putBytes(data, size); return h.digest(); }

    private:
        result_type m_state;
    };

    using hash32_t = std::uint32_t;
    using hash64_t = std::uint64_t;
    using hash_t = hash64_t;

    using hasher32  = basic_hasher<hash32_t, 2166136261u, 16777619u>;
    using hasher64  = basic_hasher<hash64_t, 14695981039346656037u, 1099511628211u>;
    using hasher    = hasher64;
}

#undef M_DECL_PURE
#undef M_DECL
