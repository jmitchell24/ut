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
    class stack_array
    {
    public:
        static_assert(std::is_trivial_v<T>);

        stack_array()=default;


        M_DECL ~stack_array() { clear(); }

        M_DECL_PURE size_t size      () const { return m_end-M_DATA_CONST; }
        M_DECL_PURE size_t capacity  () const { return N; }
        M_DECL_PURE bool   empty     () const { return m_end==M_DATA_CONST; }

        M_DECL_PURE T const* data       ()          const   { return M_DATA_CONST; }
        M_DECL_PURE T const* begin      ()          const   { return M_DATA_CONST; }
        M_DECL_PURE T const* end        ()          const   { return m_end; }
        M_DECL_PURE T const& top        ()          const   { assert(!empty()); return *m_top; }
        M_DECL_PURE T const& peek       (size_t i)  const   { assert(i < size()); return *(m_top-i); }
        M_DECL_PURE T const& operator[] (size_t i)  const   { assert(i < size()); return *(M_DATA_CONST+i); }

        M_DECL T*   data        ()          { return M_DATA; }
        M_DECL T*   begin       ()          { return M_DATA; }
        M_DECL T*   end         ()          { return m_end; }
        M_DECL T&   top         ()          { assert(!empty()); return *(m_top); }
        M_DECL T&   peek        (size_t i)  { assert(i < size()); return *(m_top-i); }
        M_DECL T&   operator[]  (size_t i)  { assert(i < size()); return *(M_DATA+i); }


        template <typename... Args>
        M_DECL void emplace(Args&&... args)
        {
            assert(size() < N);
            m_top = m_end++;
            ::new (m_top) T(std::forward<Args>(args)...);
        }

        M_DECL void push(T const& t) { emplace(t); }
        M_DECL void push(T&& t) { emplace(std::move(t)); }

        M_DECL void pop()
        {
            assert(!empty());
            std::destroy_at(std::launder(m_top));
            m_end = m_top--;
        }

        M_DECL void pop(size_t n)
        {
            assert(n <= size());
            for (T* end = m_top-n; m_top != end; --m_top)
                std::destroy_at(std::launder(m_top));
            m_end = m_top+1;
        }

        M_DECL void clear()
        {
            for (T* end = M_DATA-1; m_top != end; --m_top)
                std::destroy_at(std::launder(m_top));
            m_end = m_top+1;
        }



    private:
        stack_array(stack_array const&)=delete;
        stack_array(stack_array&&)=default;
        stack_array& operator= (stack_array const&)=delete;
        stack_array& operator= (stack_array&&)=delete;

        // using store_type = std::aligned_storage<sizeof(T), alignof(T)>;
        using store_type = std::byte[sizeof(T)];

        store_type  m_data[N];
        T*          m_top       = M_DATA-1;
        T*          m_end       = M_DATA;


    };
}

#undef M_DATA
#undef M_DATA_CONST
#undef M_DECL_PURE
#undef M_DECL
