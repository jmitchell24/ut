#pragma once

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline

#include <vector>

namespace ut
{
    template <typename T, typename Compare, typename Allocator, bool IsMulti>
    class ordered_vector
    {
    public:
        using ordered_vector_type = ordered_vector<T,Compare,Allocator,IsMulti>;


        using vector_type                       = std::vector<T,Allocator>;
        using allocator_type                    = typename vector_type::allocator_type;
        using size_type                         = typename vector_type::size_type;
        using difference_type                   = typename vector_type::difference_type;
        using reference                         = typename vector_type::reference;
        using const_reference                   = typename vector_type::const_reference;
        using value_type                        = typename vector_type::value_type;
        using value_param                       = value_type const&;
        using key_type                          = T;
        using key_param                         = T const&;
        using key_compare_type                  = Compare;
        using value_compare_type                = Compare;

        using iterator                          = typename vector_type::iterator;
        using const_iterator                    = typename vector_type::const_iterator;
        using reverse_iterator                  = typename vector_type::reverse_iterator;
        using const_reverse_iterator            = typename vector_type::const_reverse_iterator;

        using iterator_iterator_pair_type       = std::pair<iterator, iterator>;
        using citerator_citerator_pair_type     = std::pair<const_iterator, const_iterator>;
        using iterator_bool_pair_type           = std::pair<iterator, bool>;


        M_DECL explicit ordered_vector(Compare const& c = Compare(), Allocator const& a = Allocator())
            : m_comp{c}, m_vec{a}
        {}

        M_DECL ordered_vector(std::initializer_list<value_type> const& il, Compare const& c = Compare(), Allocator const& a = Allocator())
            : m_comp{c}, m_vec{a}
        { for (auto&& it : il) insert(it); }


        template <typename It>
        M_DECL ordered_vector(It const& begin, It const& end, Compare const& c = Compare(), Allocator const& a = Allocator())
            : m_comp{c}, m_vec{begin,end,a}
        { stable_sort(); }

        ordered_vector(ordered_vector_type const&)=default;
        ordered_vector(ordered_vector_type&&)=default;
        ordered_vector& operator=(ordered_vector const&)=default;
        ordered_vector& operator=(ordered_vector&&)=default;
        ~ordered_vector()=default;

        M_DECL void reserve(size_type n)	{ m_vec.reserve(n); }
        M_DECL void shrink_to_fit() { m_vec.shrink_to_fit(); }
        
        M_DECL iterator         begin   ()  { return m_vec.begin(); }
        M_DECL iterator         end     ()  { return m_vec.end(); }
        M_DECL reverse_iterator rbegin  ()  { return m_vec.rbegin(); }
        M_DECL reverse_iterator rend    ()  { return m_vec.rend(); }
        
        M_DECL_PURE const_iterator         begin   () const { return m_vec.begin(); }
        M_DECL_PURE const_iterator         end     () const { return m_vec.end(); }
        M_DECL_PURE const_reverse_iterator rbegin  () const { return m_vec.rbegin(); }
        M_DECL_PURE const_reverse_iterator rend    () const { return m_vec.rend(); }
        
        M_DECL_PURE size_type size    () const { return m_vec.size(); }
        M_DECL_PURE size_type max_size() const { return m_vec.max_size(); }

        M_DECL_PURE bool empty() const              { return m_vec.empty(); }
        M_DECL_PURE Allocator get_allocator() const { return m_vec.get_allocator(); }

        M_DECL_PURE const_reference     at(size_type i)         const { return m_vec.at(i); }
        M_DECL_PURE value_type const*   data()                  const { return m_vec.data(); }
        M_DECL_PURE const_reference     front()                 const { return m_vec.front(); }
        M_DECL_PURE const_reference     back()                  const { return m_vec.back(); }
        M_DECL_PURE const_reference     operator[](size_type i) const { return m_vec[i]; }
        
        M_DECL reference            at(size_type i)         { return m_vec.at(i); }
        M_DECL value_type const*    data()                  { return m_vec.data(); }
        M_DECL reference            front()                 { return m_vec.front(); }
        M_DECL reference            back()                  { return m_vec.back(); }
        M_DECL reference            operator[](size_type i) { return m_vec[i]; }

    private:
        key_compare_type m_comp;
        vector_type      m_vec;

        M_DECL void sort()
        {
            std::sort(m_vec.begin(), m_vec.end(), m_comp);
            if(!IsMulti)
                m_vec.erase(moveDuplicates(),m_vec.end());
        }

        M_DECL void qsort()
        {
            std::qsort(m_vec.begin(), m_vec.end(), m_comp);
            if (!IsMulti)
                m_vec.erase(moveDuplicates(), m_vec.end());
        }

        M_DECL void stable_sort()
        {
            std::stable_sort(m_vec.begin(), m_vec.end(), m_comp);
            if(!IsMulti)
                erase(moveDuplicates(),end());
        }

        M_DECL iterator moveDuplicates()
        {
            iterator begin = m_vec.begin();
            iterator out   = m_vec.end();
            iterator end   = m_vec.end();

            bool bCopy_ = false;

            for (iterator previous; (previous = begin) != end && ++begin != end;)
            {
                if(m_key_compare(*previous, *begin))
                {
                    if(bCopy_)
                    {
                        *out = *begin;
                        ++out;
                    }
                }
                else
                {
                    if(!bCopy_)
                    {
                        out=begin;
                        bCopy_=true;
                    }
                }
            }
            return out;
        }

        M_DECL iterator insertImpl(iterator const p, value_param x) { return m_vec.insert(p,x);}

        M_DECL bool keyCompareLeq(key_param a, key_param b) { return !m_comp(a,b); }
        M_DECL bool keyCompareGeq(key_param a, key_param b) { return !m_comp(a,b); }
        M_DECL bool keyCompareGt (key_param a, key_param b) { return  m_comp(a,b); }
    };
}

#undef M_DECL_PURE
#undef M_DECL
