#pragma once

//---- Enable implicit conversion to c string (char const*)
#define UT_VIEW_CSTR_CONVERSION

#include <memory>
#include <string>
#include <vector>
#include <streambuf>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <type_traits>
#include <string_view>
#include <cwctype>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <ostream>
#include <istream>

#include "ut/color.hpp"

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

namespace ut
{
    struct basic_strview_cstr_tag {};

#define ENABLE_IF_NULL_TERMINATED  template <bool N_ = NullTerminated, typename = std::enable_if_t<N_ == true>>
#define ENABLE_IF_NOT_NULL_TERMINATED template <bool N_ = NullTerminated, typename = std::enable_if_t<N_ == false>>

    template <typename Char, typename Traits = std::char_traits<Char>, bool NullTerminated = false>
    class basic_strview
    {
    public:
        using char_type             = Char;
        using traits_type           = Traits;
        using pointer_type          = char_type const*;
        using unique_pointer_type   = std::unique_ptr<char_type[]>;
        using shared_pointer_type   = std::shared_ptr<char_type[]>;
        using size_type             = std::size_t;
        using string_type           = std::basic_string     <char_type,traits_type>;
        using ostream_type          = std::basic_ostream    <char_type,traits_type>;
        using string_view_type      = std::basic_string_view<char_type,traits_type>;
        using strview_nstr_type     = basic_strview         <char_type,traits_type, false>;
        using strview_cstr_type     = basic_strview         <char_type,traits_type, true>;
        using strview_type          = basic_strview         <char_type,traits_type, NullTerminated>;

        struct unique_copy { unique_pointer_type ptr; strview_cstr_type view; };
        struct shared_copy { shared_pointer_type ptr; strview_cstr_type view; };

        inline constexpr static bool NULL_TERMINATED = NullTerminated;

        M_DECL basic_strview(strview_type&&) noexcept =default;
        M_DECL basic_strview(strview_type const&)=default;
        M_DECL basic_strview& operator=(strview_type&&) noexcept =default;
        M_DECL basic_strview& operator=(strview_type const&)=default;

        /// Construct with default string ("")
        M_DECL basic_strview()
            : m_begin{""}, m_end{m_begin}
        {}

        /// Construct with an STL basic_string
        M_DECL basic_strview(string_type const& s)
            : m_begin{s.data()}, m_end{s.data()+s.size()}
        {
            assert(m_begin != nullptr);
            assert(m_end   != nullptr);
            assert(m_begin <= m_end);
        }

        /// Construct with an STL basic_string_view
        M_DECL basic_strview(string_view_type const& s)
            : m_begin{s.data()}, m_end{s.data()+s.size()}
        {
            assert(m_begin != nullptr);
            assert(m_end   != nullptr);
            assert(m_begin <= m_end);
        }

        /// Construct with a begin and end pointer
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL basic_strview(pointer_type begin, pointer_type end)
            : m_begin{begin}, m_end{end}
        {
            assert(m_begin != nullptr);
            assert(m_end   != nullptr);
            assert(m_begin <= m_end);
        }

        /// Construct with a begin pointer and length
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL_PURE explicit basic_strview(pointer_type begin, size_type sz)
            : m_begin{begin}, m_end{m_begin+sz}
        { assert(m_begin != nullptr); }

        /// Construct with a begin pointer and length. Will dereference to check null-terminated.
        M_DECL_PURE basic_strview(pointer_type begin, size_type sz, basic_strview_cstr_tag)
                : m_begin{begin}, m_end{m_begin+sz}
        {
            assert(m_begin != nullptr);
            assert(m_end   != nullptr);
            assert(m_begin <= m_end);
            assert(*m_end == '\0');
        }

        /// Construct with a null-terminated stringview
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL basic_strview(strview_cstr_type const& s)
            : m_begin{s.begin()}, m_end{s.end()}
        {
            assert(m_begin != nullptr);
            assert(m_end   != nullptr);
            assert(m_begin <= m_end);
        }

        /// Construct with a null-terminated string
        ENABLE_IF_NULL_TERMINATED
        M_DECL basic_strview(pointer_type p)
            : m_begin{p}, m_end{p+traits_type::length(p)}
        {
            assert(m_begin != nullptr);
            assert(m_end   != nullptr);
            assert(m_begin <= m_end);
        }

        /// Returns a pointer to the first char of the string. Equivalent to \a begin()
        /// \return     Pointer to the beginning of the string.
        M_DECL_PURE pointer_type data() const { return m_begin; }

        /// Returns a pointer to the first char of the string.
        /// \return     Pointer to the beginning of the string.
        M_DECL_PURE pointer_type begin() const { return m_begin; }

        /// Returns a pointer to the \b past-the-end character of the string.
        /// \return     Pointer to the past-the-end of the string.
        M_DECL_PURE pointer_type end() const { return m_end; }

        /// Returns the first char of the string.
        /// \return     The first char of the string.
        M_DECL_PURE char_type first() const { assert(m_begin != m_end); return *m_begin; }

        /// Returns the last char of the string.
        /// \return     The last char of the string.
        M_DECL_PURE char_type last() const { assert(m_begin != m_end); return *(m_end-1); }

        /// Returns the length of the string, in number of chars.
        /// \return     The number of chars in the string.
        M_DECL_PURE size_type size() const { return m_end - m_begin; }

        /// Returns whether the string is empty
        /// \return     \a true if the string length is 0, \a false otherwise.
        M_DECL_PURE bool empty() const { return m_begin == m_end; }

        M_DECL_PURE char_type at(size_type i) const
        {
            if (i < size())
                return m_begin[i];
            throw std::out_of_range{"ut::stringview::at()"};
        }

        M_DECL char_type* strncpy(char_type* dest, size_t n) const
        {
            return ::strncpy(dest, m_begin, std::min<size_t>(n, m_end-m_begin));
        }

        M_DECL_PURE shared_copy shared_ptr_copy() const
        {
            size_type sz = size();
            auto ptr = new char_type[sz+1];
            ::strncpy(ptr, m_begin, sz);
            *(ptr+sz) = 0;
            return {shared_pointer_type(ptr), make_cstrview(ptr, sz)};
        }

        M_DECL_PURE unique_copy unique_ptr_copy() const
        {
            size_type sz = size();
            auto ptr = new char_type[sz+1];
            ::strncpy(ptr, m_begin, sz);
            *(ptr+sz) = 0;
            return {unique_pointer_type(ptr), make_cstrview(ptr, sz)};
        }

        /// Returns a c-string (if NullTerminated == true)
        /// \return     Pointer to null-terminated char array
        ENABLE_IF_NULL_TERMINATED
        M_DECL_PURE pointer_type c_str() const { return m_begin; }

        /// Returns an STL basic_string_view
        M_DECL_PURE string_view_type view() const { return string_view_type(m_begin, m_end-m_begin); }

        /// Returns an STL basic_string
        M_DECL_PURE string_type str() const { return string_type(m_begin, m_end); }


        /// Returns an equivalent sub-view for \p copy, assuming that this sub-view is of \p orig.
        /// \p orig and \p copy are assumed to be equivalent.
        /// \param orig     The view from which this sub-view is derived.
        /// \param copy     The view from which the new sub-view will be derived.
        /// \return         A new sub-view derived from \p copy and equivalent to this sub-view.
        M_DECL_PURE strview_nstr_type mirror(strview_type const& orig, strview_type const& copy) const
        {
            assert(orig == copy);
            assert(m_begin >= orig.m_begin);
            assert(m_begin <= orig.m_end);
            assert(m_end   <= orig.m_end);
            assert(m_end   >= orig.m_begin);

            pointer_type begin = copy.m_begin + size_type(m_begin - orig.m_begin);
            pointer_type end   = begin + size();
            return copy.with(begin, end);
        }


        /// Returns a view of the substring [begin, end], where begin >= begin() and end <= end().
        /// \param begin    Pointer to the first char of the substring.
        /// \param end      Pointer to the char following following the last char of the view.
        /// \return         View of the substring [begin, end].
        M_DECL_PURE strview_nstr_type with(pointer_type begin, pointer_type end) const
        {
            assert(begin >= m_begin);
            assert(begin <= m_end);
            assert(end   <= m_end);
            assert(end   >= m_begin);

            return strview_nstr_type{begin, end};
        }


        /// Returns a view of substring [begin, end], where begin >= begin() and end <= end().
        /// \param begin    Pointer to the first char of the substring.
        /// \param end      Pointer to the char following following the last char of the view.
        /// \return         View of the substring [begin, end].
        M_DECL_PURE strview_nstr_type withClamp(pointer_type begin, pointer_type end) const
        {
            if (begin < m_begin) begin = m_begin;
            if (end   > m_end  ) end   = m_end;
            return strview_nstr_type{begin, end};
        }


        /// Returns a view of substring [begin(), end], where end <= end().
        /// \param end  Pointer to the char following following the last char of the view.
        /// \return     View of the substring [begin(), end]
        M_DECL_PURE strview_nstr_type withEnd(pointer_type end) const
        { return with(m_begin, end); }


        /// Returns a view of substring [begin, end()] where begin >= begin().
        /// \param begin    Pointer the first char of the substring.
        /// \return         View of the substring [begin, end()]
        M_DECL_PURE strview_nstr_type withBegin(pointer_type begin) const
        { return with(begin, m_end); }


        /// Returns a view of substring [begin(), begin()+size]. Equivalent to takeBegin().
        /// \param size     Requested view size
        /// \return         View of the substring [begin(), begin()+size]
        M_DECL_PURE strview_nstr_type take(size_type size) const
        { return takeBegin(size); }


        /// Returns a view of substring [begin()+begin_idx, begin()+end_idx].
        /// \param beg_idx  index of the first char of the substring.
        /// \param end_idx  Pointer to the char following following the last char of the view.
        /// \return         View of the substring [begin, end].
        M_DECL_PURE strview_nstr_type withIndices(size_t beg_idx, size_t end_idx) const
        { return with(m_begin+beg_idx, m_begin+end_idx); }


        /// Returns a view of substring [begin(), begin()+size].
        /// \param size     Requested view size
        /// \return         View of the substring [begin(), begin()+size]
        M_DECL_PURE strview_nstr_type takeBegin(size_type size) const
        { return with(m_begin, m_begin+size); }


        /// Returns a view of substring [end()-size, end()].
        /// \param size     Requested view size.
        /// \return         View of the substring [end()-size, end()].
        M_DECL_PURE strview_nstr_type takeEnd(size_type size) const
        { return with(m_end-size, m_end); }


        /// Returns a view of substring [end()-size, end()]. Equivalent to \a skipBegin()
        /// \param size     Requested view size.
        /// \return         View of the substring [end()-size, end()].
        M_DECL_PURE strview_nstr_type skip(size_type size) const
        { return skipBegin(size); }


        /// Returns a view of the substring [ \a begin()+size, \a end() ].
        /// \param size     Requested view size.
        /// \return         View of the substring [ \a begin()+size, \a end() ].
        M_DECL_PURE strview_nstr_type skipBegin(size_type size) const
        { return with(m_begin+size, m_end); }


        /// Returns a view of the substring [ \a begin(), \a end()-size ].
        /// \param size     Requested view size.
        /// \return         View of the substring [ \a begin(), \a end()-size ].
        M_DECL_PURE strview_nstr_type skipEnd(size_type size) const
        { return with(m_begin, m_end-size); }


        M_DECL_PURE pointer_type find(strview_type const& s) const
        {
            size_type sz = s.size();
            if (sz > this->size())
                return m_end;

            for (pointer_type i = m_begin; i != m_end-sz+1; ++i)
                if (equals(with(i,i+sz), s))
                    return i;
            return m_end;
        }

        M_DECL_PURE pointer_type find(char_type c) const
        { return traits_type::find(m_begin, size(), c); }

        M_DECL_PURE pointer_type findFirst(strview_type const& s) const
        { return find(s); }

        M_DECL_PURE pointer_type findFirst(char_type c) const
        { return find(c); }

        M_DECL_PURE pointer_type findFirstNot(char_type c) const
        {
            for (pointer_type i = m_begin; i != m_end; ++i)
                if (!traits_type::eq(*i, c))
                    return i;
            return m_end;
        }

        M_DECL_PURE pointer_type findLast(strview_type const& s) const
        {
            size_type sz = s.size();
            if (sz > size())
                return m_end;
            for(pointer_type i = m_end-s.size(); i != m_begin-1; --i)
                if (equals(with(i,i+sz), s))
                    return i;
            return m_end;
        }

        M_DECL_PURE pointer_type findLast(char_type c) const
        {
            if (m_begin == m_end)
                return m_begin;
            for (pointer_type i = m_end-1; i != m_begin-1; --i)
                if (traits_type::eq(*i,c))
                    return i;
            return m_end;
        }

        M_DECL_PURE pointer_type findLastNot(char_type c) const
        {
            for (pointer_type i = m_end-1; i != m_begin-1; --i)
                if (!traits_type::eq(*i,c))
                    return i;
            return m_end;
        }

        M_DECL_PURE bool beginsWith(char_type c) const
        {
            if (m_begin == m_end)
                return false;
            return *m_begin == c;
        }

        M_DECL_PURE bool beginsWith(strview_type const& s) const
        {
            if (s.size() > size())
                return false;
            return equals(takeBegin(s.size()), s);
        }

        M_DECL_PURE bool endsWith(char_type c) const
        {
            if (m_begin == m_end)
                return false;
            return *(m_end-1) == c;
        }

        M_DECL_PURE bool endsWith(strview_type const& s) const
        {
            return equals(with(m_end - s.size(), m_end), s);
        }

        M_DECL_PURE strview_nstr_type trimLeft  () const { return with(trimBegin(), m_end); }
        M_DECL_PURE strview_nstr_type trimRight () const { return with(m_begin, trimEnd()); }
        M_DECL_PURE strview_nstr_type trim      () const { return with(trimBegin(), trimEnd()); }

        M_DECL_PURE strview_nstr_type trimLeft  (size_t n) const { return with(m_begin+n, m_end  ); }
        M_DECL_PURE strview_nstr_type trimRight (size_t n) const { return with(m_begin  , m_end-n); }
        M_DECL_PURE strview_nstr_type trim      (size_t n) const { return with(m_begin+n, m_end-n); }

        M_DECL_PURE bool trimmed     () const { return empty() || ( !std::isspace(first()) && !std::isspace(last()) ); }
        M_DECL_PURE bool trimmedLeft () const { return empty() || ( !std::isspace(first()) ); }
        M_DECL_PURE bool trimmedRight() const { return empty() || ( !std::isspace(last()) ); }

        M_DECL_PURE bool contains(strview_type const& s) const
        { return find(s) != m_end; }

        M_DECL_PURE bool contains(char_type c) const
        { return find(c) != nullptr; }

        M_DECL_PURE char_type operator[] (size_type i) const { return *(m_begin+i); }

#if defined(UT_VIEW_CSTR_CONVERSION)
        ENABLE_IF_NULL_TERMINATED
        M_DECL_PURE operator pointer_type() const { return m_begin; }
#endif

        M_DECL_PURE bool operator<  (strview_type const& s) const { return compare(*this, s) <  0; }
        M_DECL_PURE bool operator>  (strview_type const& s) const { return compare(*this, s) >  0; }
        M_DECL_PURE bool operator== (strview_type const& s) const { return equals (*this, s) == true; }
        M_DECL_PURE bool operator!= (strview_type const& s) const { return equals (*this, s) == false; }

        M_DECL_PURE static bool equals(strview_nstr_type const& a, strview_nstr_type const& b)
        {
            auto sz_a=a.size(), sz_b=b.size();
            return sz_a == sz_b && traits_type::compare(a.begin(), b.begin(), sz_a) == 0;
        }

        M_DECL_PURE static bool equals(strview_nstr_type const& a, char_type b)
        {
            return a.size() == 1 && traits_type::compare(a.begin(), &b, 1) == 0;
        }

        M_DECL_PURE static int compare(strview_nstr_type const& a, strview_nstr_type const& b)
        {
            return traits_type::compare(a.begin(), b.begin(), std::min(a.size(), b.size()));
        }

        M_DECL_PURE static int compare(strview_nstr_type const& a, char_type b)
        {
            return traits_type::compare(a.begin(), &b, 1);
        }

        friend inline ostream_type& operator<<(ostream_type& os, strview_type const& s)
        { return os.write(s.begin(), s.end() - s.begin()); }

        /// make a cstrview from ptr+sz, dereference m_end to verify null-termination (assert).
        M_DECL_PURE static strview_cstr_type make_cstrview(pointer_type begin, size_type sz)
        { return strview_cstr_type(begin, sz, basic_strview_cstr_tag{}); }

    private:
        char_type const* m_begin;
        char_type const* m_end;

        M_DECL_PURE pointer_type trimBegin() const
        {
            pointer_type i = m_begin;
            while (i != m_end && std::isspace(*i)) ++i;
            return i;
        }

        M_DECL_PURE pointer_type trimEnd() const
        {
            pointer_type i = m_end-1;
            pointer_type end = m_begin-1;
            while (i!=end && std::isspace(*i)) --i;
            return (i==end) ? m_end : i+1;
        }
    };

#undef ENABLE_IF_NULL_TERMINATED
#undef ENABLE_IF_NOT_NULL_TERMINATED

    using strview  = basic_strview<char, std::char_traits<char>, false>;
    using cstrview = basic_strview<char, std::char_traits<char>, true>;

    using strparam  = strview;
    using cstrparam = cstrview;


    M_DECL_PURE cstrview operator "" _sv(char const* str, size_t sz) noexcept
    { return cstrview::make_cstrview(str, sz); }
}

namespace std
{
    template<class Key> struct hash;

    template <typename Char, typename Traits, bool NullTerminated>
    struct hash<ut::basic_strview<Char,Traits,NullTerminated>>
    {
        size_t operator()(ut::basic_strview<Char,Traits,NullTerminated> const& k) const
        {
            using string_view_type = typename ut::basic_strview<Char,Traits,NullTerminated>::string_view_type;
            return hash<string_view_type>{}(k.view());
        }
    };
}

#undef M_DECL_PURE
#undef M_DECL