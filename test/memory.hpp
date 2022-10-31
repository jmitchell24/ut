#pragma once

#include <cstddef>
#include <cassert>
#include <memory>
#include <new>

#define M_DATA                  reinterpret_cast<T*>(m_store)
#define M_DATA_CONST            reinterpret_cast<T const*>(m_store)
#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline

namespace ut
{
    template <typename T, size_t N>
    class stack_array
    {
    public:
        stack_array()=default;

        inline ~stack_array() { clear(); }

        inline T const* data() const    { return M_DATA_CONST; }
        inline T*       data()          { return M_DATA; }

        inline size_t size      () const { return m_count; }
        inline size_t capacity  () const { return N; }
        inline bool   empty     () const { return m_count==0; }

        inline T const* begin   () const { return M_DATA_CONST; }
        inline T const* end     () const { return M_DATA_CONST+m_count; }

        inline T* begin () { return M_DATA; }
        inline T* end   () { return M_DATA+m_count; }

        inline T&       top()          { assert(m_count > 0); return *(M_DATA+m_count-1); }
        inline T const& top() const    { assert(m_count > 0); return *(M_DATA_CONST+m_count-1); }

        inline T&       peek (size_t i)         { assert(i < m_count); return *(M_DATA+m_count-1-i); }
        inline T const& peek (size_t i) const   { assert(i < m_count); return *(M_DATA_CONST+m_count-1-i); }

        inline T&       operator[] (size_t i)       { assert(i < m_count); return *(M_DATA+i); }
        inline T const& operator[] (size_t i) const { assert(i < m_count); return *(M_DATA_CONST+i); }

        template <typename... Args>
        inline void emplace(Args&&... args)
        {
            assert(m_count < N);
            ::new (M_DATA+m_count) T(std::forward<Args>(args)...);
            ++m_count;
        }

        inline void push(T const& t) { emplace(t); }
        inline void push(T&& t) { emplace(std::move(t)); }

        inline void pop()
        {
            assert(m_count > 0);
            --m_count;
            std::destroy_at(std::launder(m_store+m_count));
        }

        inline void pop(size_t n)
        {
            assert(n <= m_count);

            T* end = M_DATA + m_count;
            for (T* it = end-n; it != end; ++it)
                std::destroy_at(std::launder(it));
            m_count -= n;
        }

        inline void clear()
        {
            T* end = M_DATA + m_count;
            for (T* it = M_DATA; it != end; ++it)
                std::destroy_at(std::launder(it));
            m_count=0;
        }



    private:
        using store_type = std::aligned_storage<sizeof(T), alignof(T)>;

        store_type  m_store[N];
        size_t      m_count=0;

        stack_array(stack_array const&)=delete;
        stack_array& operator= (stack_array const&)=delete;
    };
}

#undef M_DATA
#undef M_DATA_CONST
#undef M_DECL_PURE
#undef M_DECL
