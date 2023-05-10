#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

namespace ut
{
    template<typename Key,
             bool     AllowDuplicates = true,
             typename Compare         = std::less<Key>,
             typename Allocator       = std::allocator<Key>>
    class sorted_vector
    {
    public:
        typedef sorted_vector<Key,AllowDuplicates,Compare,Allocator> sorted_vector_type;

        typedef std::vector<Key,Allocator>                      vector_type;
        typedef typename vector_type::allocator_type            allocator_type;
        typedef typename vector_type::size_type                 size_type;
        typedef typename vector_type::difference_type           difference_type;
        typedef typename vector_type::reference                 reference;
        typedef typename vector_type::const_reference           const_reference;
        typedef typename vector_type::value_type                value_type;
        typedef value_type const&                               value_param;
        typedef Key                                             key_type;
        typedef Key const&                                      key_param;
        typedef Compare                                         key_compare_type;
        typedef Compare                                         value_compare_type;

        typedef typename vector_type::iterator                  iterator;
        typedef typename vector_type::const_iterator            const_iterator;
        typedef typename vector_type::reverse_iterator          reverse_iterator;
        typedef typename vector_type::const_reverse_iterator    const_reverse_iterator;

        typedef std::pair<iterator, iterator>                   iterator_iterator_pair_type;
        typedef std::pair<const_iterator, const_iterator>       citerator_citerator_pair_type;
        typedef std::pair<iterator, bool>                       iterator_bool_pair_type;

        explicit sorted_vector(Compare const& pred = Compare(), Allocator const& al = Allocator())
            : m_key_compare(pred), m_vec(al)
        {}

        sorted_vector(std::initializer_list<value_type> const& il, Compare const& pred = Compare(), Allocator const& al = Allocator())
            : m_key_compare(pred), m_vec(al)
        {
            for (value_param x : il)
                insert(x);
        }

        template<class It>
        sorted_vector(It const begin, It const end, Compare const& pred = Compare(), Allocator const& al = Allocator())
            : m_key_compare(pred),m_vec(begin,end,al)
        { stable_sort(); }

        sorted_vector(sorted_vector_type&& x)
            : m_key_compare(std::forward<key_compare_type>(x.m_key_compare)), m_vec(std::forward<vector_type>(x.m_vec))
        {}

        sorted_vector(sorted_vector_type const& x)
            : m_key_compare(x.m_key_compare), m_vec(x.m_vec)
        {}

        ~sorted_vector()
        {}

        sorted_vector_type& operator= (std::initializer_list<value_type> const& il)
        {
            m_vec.clear();
            return *new (this) vector_type(il);
        }

        sorted_vector_type& operator= (sorted_vector_type&& x)
        {
            m_key_compare = std::forward<key_compare_type>(x.m_key_compare);
            m_vec = std::forward<vector_type>(x.m_vec);
            return *this;
        }

        sorted_vector_type& operator= (sorted_vector_type const& x)
        {
            m_key_compare = x.m_key_compare;
            m_vec = x.m_vec;
            return *this;
        }

        sorted_vector_type& operator= (vector_type const& x)
        {
            m_vec.operator=(x);
            sort();
            return *this;
        }

        inline void reserve(size_type const n)	{ m_vec.reserve(n); }
        inline void shrink_to_fit() { m_vec.shrink_to_fit(); }

        inline iterator                 begin()       { return m_vec.begin(); }
        inline const_iterator           begin() const { return m_vec.begin(); }
        inline const_iterator          cbegin() const { return m_vec.cbegin(); }
        inline reverse_iterator        rbegin()       { return m_vec.rbegin(); }
        inline const_reverse_iterator crbegin() const { return m_vec.crbegin(); }

        inline iterator                 end()       { return m_vec.end(); }
        inline const_iterator           end() const { return m_vec.end(); }
        inline const_iterator          cend() const { return m_vec.cend(); }
        inline reverse_iterator        rend()       { return m_vec.rend(); }
        inline const_reverse_iterator crend() const { return m_vec.crend(); }

        inline size_type size    () const { return m_vec.size(); }
        inline size_type max_size() const { return m_vec.max_size(); }

        inline bool empty() const              { return m_vec.empty(); }
        inline Allocator get_allocator() const { return m_vec.get_allocator(); }

        inline const_reference at(size_type const p) const { return m_vec.at(p); }
        inline reference       at(size_type const p)       { return m_vec.at(p); }

        inline const_reference operator[](size_type const p) const { return m_vec.operator[](p); }
        inline reference       operator[](size_type const p)       { return m_vec.operator[](p); }

        inline value_type*       data()       { return m_vec.data(); }
        inline value_type const* data() const { return m_vec.data(); }

        inline reference       front()       { return m_vec.front(); }
        inline const_reference front() const { return m_vec.front(); }

        inline reference       back()       { return m_vec.back(); }
        inline const_reference back() const { return m_vec.back(); }

        inline void pop_back() { m_vec.pop_back(); }

        inline void assign(const_iterator first, const_iterator beyond) { m_vec.assign(first,beyond); }
        inline void assign(size_type const n, key_param x = Key())      { m_vec.assign(n,x); }

        iterator_bool_pair_type insert(value_param x)
        {
            if (!AllowDuplicates)
            {
                iterator const p = lower_bound(x);
                if(p == end() || m_key_compare(x,*p))
                    return iterator_bool_pair_type(insertImpl(p,x),true);
                else
                    return iterator_bool_pair_type(p,false);
            }
            else
            {
                iterator const p = upper_bound(x);
                return iterator_bool_pair_type(insertImpl(p,x),true);
            }
        }

        iterator insert(iterator it, value_param x)
        {
           if (it != end())
           {
               if(!AllowDuplicates)
               {
                    if(m_key_compare(*it,x))
                    {
                        if((it+1) == end() || keyCompareGt(*(it+1),x))
                            return insertImpl(it+1,x);
                        else if(keyCompareGeq(*(it+1),x))
                           return end();
                    }
               }
               else
               {
                   if(keyCompareLeq(*it,x) && ((it+1) == end() || keyCompareGeq(*(it+1),x)))
                       return insertImpl(it+1,x);
               }
           }
           return insert(x).first;
        }

        template<class It> void insert(It begin, It end)
        {
            size_type const n = std::distance(begin, end);
            reserve(size() + n);
            for(; begin != end; ++begin)
                insert(*begin);
        }

        inline iterator erase(iterator const p) { return m_vec.erase(p); }
        inline iterator erase(iterator const begin, iterator end) { return m_vec.erase(begin,end); }
        size_type erase(key_param key)
        {
            iterator_iterator_pair_type const begEnd = equal_range(key);
            erase(begEnd.first,begEnd.second);
            return std::distance(begEnd.first, begEnd.second);
        }

        inline void clear() { return m_vec.clear(); }

        inline bool equal(sorted_vector_type const& x) const { return size() == x.size() && std::equal(begin(), end(), x.begin()); }
        inline bool lexo (sorted_vector_type const& x) const { return std::lexicographical_compare(begin(), end(), x.begin(), x.end()); }

        void swap(sorted_vector_type& x)
        {
            m_vec.swap(x.m_vec);
            std::swap(m_key_compare,x.m_key_compare);
        }

        friend void swap(sorted_vector_type& x, sorted_vector_type& y)
        {
            x.swap(y);
        }

        inline key_compare_type key_comp() const { return m_key_compare; }
        inline value_compare_type value_comp() const { return key_comp(); }

        iterator find(key_param k)
        {
            iterator const p = lower_bound(k);
            return (p == end() || m_key_compare(k, *p)) ? end() : p;
        }

        const_iterator find(key_param k) const
        {
            const_iterator const p = lower_bound(k);
            return (p==end()||m_key_compare(k,*p))?end():p;
        }

        size_type count(key_param k) const
        {
            citerator_citerator_pair_type const p = equal_range(k);
            return std::distance(p.first, p.second);
        }

        inline iterator       lower_bound(key_param k)       { return std::lower_bound(begin(), end(), k, m_key_compare); }
        inline const_iterator lower_bound(key_param k) const { return std::lower_bound(begin(), end(), k, m_key_compare); }

        inline iterator       upper_bound(key_param k)       { return std::upper_bound(begin(), end(), k, m_key_compare); }
        inline const_iterator upper_bound(key_param k) const { return std::upper_bound(begin(), end(), k, m_key_compare); }

        inline iterator_iterator_pair_type   equal_range(key_param k)       { return std::equal_range(begin(), end(), k, m_key_compare); }
        inline citerator_citerator_pair_type equal_range(key_param k) const { return std::equal_range(begin(), end(), k, m_key_compare); }

        inline vector_type const& vec() const { return m_vec; }

        inline bool operator== (sorted_vector_type const& x) const { return equal(x); }
        inline bool operator!= (sorted_vector_type const& x) const { return !(*this == x); }
        inline bool operator<  (sorted_vector_type const& x) const { return lexo(x); }
        inline bool operator>  (sorted_vector_type const& x) const { return *this < x; }
        inline bool operator<= (sorted_vector_type const& x) const { return !(*this < x); }
        inline bool operator>= (sorted_vector_type const& x) const { return (!(x < *this)); }

    protected:
        void sort()
        {
            std::sort(m_vec.begin(), m_vec.end(), m_key_compare);
            if(!AllowDuplicates)
                m_vec.erase(moveDuplicates(),m_vec.end());
        }

        void qsort()
        {
            std::qsort(m_vec.begin(), m_vec.end(), m_key_compare);
            if (!AllowDuplicates)
                m_vec.erase(moveDuplicates(), m_vec.end());
        }

        void stable_sort()
        {
            std::stable_sort(m_vec.begin(), m_vec.end(), m_key_compare);
            if(!AllowDuplicates)
                erase(moveDuplicates(),end());
        }

        iterator moveDuplicates()
        {
            iterator begin = m_vec.begin();
            iterator out   = m_vec.end();
            iterator end   = m_vec.end();

            bool bCopy_ = false;

            for(iterator previous; (previous = begin) != end && ++begin != end;)
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

        inline iterator insertImpl(iterator const p, value_param x) { return m_vec.insert(p,x);}

        inline bool keyCompareLeq(key_param a, key_param b) { return !m_key_compare(a,b); }
        inline bool keyCompareGeq(key_param a, key_param b) { return !m_key_compare(a,b); }
        inline bool keyCompareGt (key_param a, key_param b) { return  m_key_compare(a,b); }

        key_compare_type m_key_compare;
        vector_type      m_vec;
    };
}
