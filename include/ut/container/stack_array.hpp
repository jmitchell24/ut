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

        inline size_t size      () const { return m_end-M_DATA_CONST; }
        inline size_t capacity  () const { return N; }
        inline bool   empty     () const { return m_end==M_DATA_CONST; }

        inline T const* begin   () const { return M_DATA_CONST; }
        inline T const* end     () const { return m_end; }

        inline T* begin () { return M_DATA; }
        inline T* end   () { return m_end; }

        inline T&       top()          { assert(!empty()); return *m_top; }
        inline T const& top() const    { assert(!empty()); return *m_top; }

        inline T&       peek (size_t i)         { assert(!empty()); return *(m_end-i-1); }
        inline T const& peek (size_t i) const   { assert(!empty()); return *(m_end-i-1); }

        inline T&       operator[] (size_t i)       { assert(!empty()); assert(i < size()); return *(M_DATA+i); }
        inline T const& operator[] (size_t i) const { assert(!empty()); assert(i < size()); return *(M_DATA_CONST+i); }

        template <typename... Args>
        inline void emplace(Args&&... args)
        {
            assert(size() < N);
            m_top = m_end++;
            ::new (m_top) T(std::forward<Args>(args)...);
        }

        inline void push(T const& t) { emplace(t); }
        inline void push(T&& t) { emplace(std::move(t)); }

        inline void pop()
        {
            assert(!empty());
            std::destroy_at(std::launder(m_top));
            m_end = m_top--;
        }

        inline void pop(size_t n)
        {
            assert(n <= size());
            for (T* end = m_top-n; m_top != end; --m_top)
                std::destroy_at(std::launder(m_top));
            m_end = m_top+1;
        }

        inline void clear()
        {
            for (T* end = M_DATA-1; m_top != end; --m_top)
                std::destroy_at(std::launder(m_top));
            m_top = M_DATA-1;
            m_end = M_DATA;
        }



    private:
        using store_type = std::aligned_storage<sizeof(T), alignof(T)>;

        store_type  m_store[N];
        T*          m_top       = M_DATA-1;
        T*          m_end       = M_DATA;

        stack_array(stack_array const&)=delete;
        stack_array& operator= (stack_array const&)=delete;
    };
}

#undef M_DATA
#undef M_DATA_CONST
#undef M_DECL_PURE
#undef M_DECL
