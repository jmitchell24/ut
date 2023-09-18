#pragma once

#include <cstddef>
#include <cassert>
#include <memory>
#include <new>

#define M_DATA                  reinterpret_cast<T*>(m_data)
#define M_DATA_CONST            reinterpret_cast<T const*>(m_data)
#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline

namespace ut
{
    template <typename T, size_t N>
    class vector_array
    {
    public:
        static_assert(N > 0);
        static_assert(std::is_trivial_v<T>);

        vector_array()=default;

        M_DECL ~vector_array() { clear(); }

        M_DECL_PURE size_t size      () const { return m_size; }
        M_DECL_PURE size_t capacity  () const { return N; }
        M_DECL_PURE bool   empty     () const { return m_size==0; }

        M_DECL_PURE T const* data       ()          const   { return M_DATA_CONST; }
        M_DECL_PURE T const* begin      ()          const   { return M_DATA_CONST; }
        M_DECL_PURE T const* end        ()          const   { return M_DATA+m_size; }
        M_DECL_PURE T const& top        ()          const   { assert(!empty()); return M_DATA+m_size-1; }
        M_DECL_PURE T const& peek       (size_t i)  const   { assert(i < size()); return *(M_DATA+m_size-i-1); }
        M_DECL_PURE T const& operator[] (size_t i)  const   { assert(i < size()); return *(M_DATA_CONST+i); }

        M_DECL T*   data        ()          { return M_DATA; }
        M_DECL T*   begin       ()          { return M_DATA; }
        M_DECL T*   end         ()          { return M_DATA+m_size; }
        M_DECL T&   top         ()          { assert(!empty()); return *(M_DATA+m_size-1); }
        M_DECL T&   peek        (size_t i)  { assert(i < size()); return *(M_DATA+m_size-i-1); }
        M_DECL T&   operator[]  (size_t i)  { assert(i < size()); return *(M_DATA+i); }


        template <typename... Args>
        M_DECL void emplace(Args&&... args)
        {
            assert(size() < N);
            ::new (M_DATA+m_size) T(std::forward<Args>(args)...);
            ++m_size;
        }

        M_DECL void push(T const& t) { emplace(t); }
        M_DECL void push(T&& t) { emplace(std::move(t)); }

        M_DECL void pop()
        {
            assert(!empty());
            --m_size;
            std::destroy_at(std::launder(M_DATA+m_size));
        }

        M_DECL void pop(size_t n)
        {
            assert(n <= size());

            for (size_t i = 0; i < n; ++i)
            {
                --m_size;
                std::destroy_at(std::launder(M_DATA+m_size));
            }
        }

        M_DECL void clear()
        {
            for (size_t i = 0; i < m_size; ++i)
                std::destroy_at(std::launder(M_DATA+i));
            m_size=0;
        }



    private:
        using store_type = std::aligned_storage<sizeof(T), alignof(T)>;

        store_type  m_data[N];
        size_t      m_size=0;

        vector_array(vector_array const&)=delete;
        vector_array& operator= (vector_array const&)=delete;
    };
}

#undef M_DATA
#undef M_DATA_CONST
#undef M_DECL_PURE
#undef M_DECL
