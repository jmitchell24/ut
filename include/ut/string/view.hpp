#pragma once

//---- Enable implicit conversion to c string (char const*)
#define UT_VIEW_CSTR_CONVERSION

//
// ut
//
#include "ut/check.hpp"

//
// std
//

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <string_view>
#include <cstring>
#include <climits>

#define SUPPORTS_STD_FORMAT          ( __cplusplus >= 202002L )
#define M_DECL_PURE                 [[nodiscard]] inline constexpr
#define M_DECL                      inline constexpr

#if SUPPORTS_STD_FORMAT
#include <format>
#endif

namespace ut
{
    struct basic_strview_cstr_tag {};

#define ENABLE_IF_NULL_TERMINATED  template <bool N_ = NullTerminated, typename = std::enable_if_t<N_ == true>>
#define ENABLE_IF_NOT_NULL_TERMINATED template <bool N_ = NullTerminated, typename = std::enable_if_t<N_ == false>>

    template <typename Char, typename Traits = std::char_traits<Char>, bool NullTerminated = false>
    class basic_strview
    {
    public:


        #pragma region Types

        /// \brief The type of the characters in the string.
        using char_type = Char;

        /// \brief The traits of the characters in the string.
        using traits_type = Traits;

        /// \brief The type of pointer to the characters in the string.
        using pointer_type = char_type const*;

        /// \brief The type of unique pointer to the characters in the string.
        using unique_pointer_type = std::unique_ptr<char_type[]>;

        /// \brief The type of shared pointer to the characters in the string.
        using shared_pointer_type = std::shared_ptr<char_type[]>;

        /// \brief The type of string, given the char_type and traits_type.
        using string_type = std::basic_string<char_type,traits_type>;

        /// \brief The type of ostream, given the char_type and traits_type.
        using ostream_type = std::basic_ostream <char_type,traits_type>;

        /// \brief The type of string_view, given the char_type and traits_type.
        using string_view_type = std::basic_string_view<char_type,traits_type>;

        /// \brief The type of strview, given the char_type and traits_type, not null-terminated.
        using strview_nstr_type = basic_strview<char_type,traits_type, false>;

        /// \brief The type of strview, given the char_type and traits_type, null-terminated.
        using strview_cstr_type = basic_strview<char_type,traits_type, true>;

        /// \brief The type of strview, given the char_type and traits_type
        using strview_type = basic_strview<char_type,traits_type, NullTerminated>;

        /// \brief A struct that contains a unique pointer to the characters in the string and a view of the string.
        struct unique_copy { unique_pointer_type ptr; strview_cstr_type view; };

        /// \brief A struct that contains a shared pointer to the characters in the string and a view of the string.
        struct shared_copy { shared_pointer_type ptr; strview_cstr_type view; };

        /// \brief A struct that contains indices representing the beginning and end of a strview.
        struct indices
        {
            size_t begin=0, end=0;

            indices()=default;

            indices(size_t begin, size_t end)
                : begin{begin}, end{end}
            { check(end >= begin); }

            inline size_t size() const
            { return end - begin; }

            inline strview_nstr_type view(strview_type const& s) const
            { return s.with(*this); }

            friend inline ostream_type& operator<<(ostream_type& os, indices const& i)
            { return os << '[' << i.begin << ", " << i.end << ']'; }
        };

        #pragma endregion Types

        /// \brief A static constant that indicates if the string is null-terminated.
        inline constexpr static bool NULL_TERMINATED = NullTerminated;

        #pragma region Constructors

        M_DECL basic_strview(strview_type&&) noexcept =default;
        M_DECL basic_strview(strview_type const&)=default;
        M_DECL basic_strview& operator=(strview_type&&) noexcept =default;
        M_DECL basic_strview& operator=(strview_type const&)=default;

        /// \brief Construct with default string ("")
        M_DECL basic_strview()
            : m_begin{""}, m_end{m_begin}
        {}

        /// \brief Construct with indices to an enclosing string.
        M_DECL basic_strview(strview_type const& s, indices const& i)
            : m_begin{s.m_begin + i.begin }, m_end{s.m_begin + i.end}
        {}

        /// \brief Construct with an STL basic_string
        M_DECL basic_strview(string_type const& s)
            : m_begin{s.data()}, m_end{s.data()+s.size()}
        {
            check(m_begin != nullptr);
            check(m_end   != nullptr);
            check(m_begin <= m_end);
        }

        /// \brief Construct with an STL basic_string_view
        M_DECL basic_strview(string_view_type const& s)
            : m_begin{s.data()}, m_end{s.data()+s.size()}
        {
            check(m_begin != nullptr);
            check(m_end   != nullptr);
            check(m_begin <= m_end);
        }


        /// \brief Construct with a begin and end pointer
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL basic_strview(pointer_type begin, pointer_type end)
            : m_begin{begin}, m_end{end}
        {
            check(m_begin != nullptr);
            check(m_end   != nullptr);
            check(m_begin <= m_end);
        }


        /// \brief Construct with a begin pointer and length
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL_PURE explicit basic_strview(pointer_type begin, size_t sz)
            : m_begin{begin}, m_end{m_begin+sz}
        { check(m_begin != nullptr); }


        /// \brief Construct with a begin pointer and length. Will dereference to check null-terminated.
        M_DECL_PURE basic_strview(pointer_type begin, size_t sz, basic_strview_cstr_tag)
                : m_begin{begin}, m_end{m_begin+sz}
        {
            check(m_begin != nullptr);
            check(m_end   != nullptr);
            check(m_begin <= m_end);
            check(*m_end == '\0');
        }


        /// \brief Construct with a null-terminated stringview
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL basic_strview(strview_cstr_type const& s)
            : m_begin{s.begin()}, m_end{s.end()}
        {
            check(m_begin != nullptr);
            check(m_end   != nullptr);
            check(m_begin <= m_end);
        }


        /// \brief Construct with a null-terminated string
        M_DECL basic_strview(pointer_type p)
            : m_begin{p}, m_end{p+traits_type::length(p)}
        {
            check(m_begin != nullptr);
            check(m_end   != nullptr);
            check(m_begin <= m_end);
        }

        #pragma endregion Constructors


        /// Returns a pointer to the first char of the string. Equivalent to \a begin()
        /// \return     Pointer to the beginning of the string.
        M_DECL_PURE pointer_type data() const { return m_begin; }


        /// Returns a pointer to the first char of the string.
        /// \return     Pointer to the beginning of the string.
        M_DECL_PURE pointer_type begin() const { return m_begin; }


        /// Returns a pointer to the \b past-the-end character of the string.
        /// \return     Pointer to the past-the-end of the string.
        M_DECL_PURE pointer_type end() const { return m_end; }


        /// Returns indices for the entire range of this view.
        /// \return     A struct containing the indices.
        M_DECL_PURE indices asIndices() const
        { return indices(m_begin, m_end - m_begin); }


        /// Returns indices of this view within an enclosing string.
        /// \param s    The enclosing string.
        /// \return     A struct containing the indices.
        M_DECL_PURE indices asIndices(strview_type const& s) const
        {
            check(isEnclosed(s));
            return indices( m_begin - s.m_begin, m_end - s.m_begin );
        }

        /// Returns the first char of the string.
        /// \return     The first char of the string.
        M_DECL_PURE char_type first() const { check(m_begin != m_end); return *m_begin; }


        /// Returns the last char of the string.
        /// \return     The last char of the string.
        M_DECL_PURE char_type last() const { check(m_begin != m_end); return *(m_end-1); }


        /// Returns the length of the string, in number of chars.
        /// \return     The number of chars in the string.
        M_DECL_PURE size_t size() const { return m_end - m_begin; }


        /// Returns whether the string is empty
        /// \return     \a true if the string length is 0, \a false otherwise.
        M_DECL_PURE bool empty() const { return m_begin == m_end; }


        /// Returns \a true if the specified view is within the bounds of this view (inclusive).
        /// \param s    The (presumed) enclosed string.
        /// \return     \a true if specified view is within the bounds of this view (inclusive).
        M_DECL_PURE bool encloses(strview_type const& s) const
        { return m_begin <= s.m_begin && m_end >= s.m_end; }


        /// Returns \a true if this view is within the bounds of the specified view (inclusive).
        /// \param s    The (presumed) enclosing string.
        /// \return     \a true if this view is within the bounds of the specified view (inclusive).
        M_DECL_PURE bool isEnclosed(strview_type const& s) const
        { return s.encloses(*this); }


        /// Returns the character at a given position.
        /// \param i    The index of the character to access.
        /// \return     The character at the specified position.
        /// \throw      std::out_of_range if \a i is out of range.
        M_DECL_PURE char_type at(size_t i) const
        {
            if (i < size())
                return m_begin[i];
            throw std::out_of_range{"ut::stringview::at()"};
        }


        /// Copys the string to a buffer using strncpy() from the C standard library. 
        /// \param dest The destination buffer.
        /// \param n    The maximum number of characters to copy.
        /// \return     A pointer to the destination buffer.
        M_DECL char_type* strncpy(char_type* dest, size_t n) const
        {
            return ::strncpy(dest, m_begin, std::min<size_t>(n, m_end-m_begin));
        }


        /// Returns a stl shared_ptr copy of the string.
        /// \return     A shared_ptr copy of the string. 
        M_DECL_PURE shared_copy shared_ptr_copy() const
        {
            size_t sz = size();
            auto ptr = new char_type[sz+1];
            ::strncpy(ptr, m_begin, sz);
            *(ptr+sz) = 0;
            return {shared_pointer_type(ptr), make_cstrview(ptr, sz)};
        }


        /// Returns a stl unique_ptr copy of the string.
        /// \return     A unique_ptr copy of the string. 
        M_DECL_PURE unique_copy unique_ptr_copy() const
        {
            size_t sz = size();
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


        #if SUPPORTS_STD_FORMAT

        template <typename... T>
        M_DECL_PURE string_type vformat(T&&... t) const
        { return std::vformat(view(), std::make_format_args(t...)); }

        #endif

        /// Returns an equivalent sub-view for \p copy, assuming that this sub-view is of \p orig.
        /// \p orig and \p copy are assumed to be equivalent.
        /// \param orig     The view from which this sub-view is derived.
        /// \param copy     The view from which the new sub-view will be derived.
        /// \return         A new sub-view derived from \p copy and equivalent to this sub-view.
        M_DECL_PURE strview_nstr_type mirror(strview_type const& orig, strview_type const& copy) const
        {
            check(orig == copy);
            check(m_begin >= orig.m_begin);
            check(m_begin <= orig.m_end);
            check(m_end   <= orig.m_end);
            check(m_end   >= orig.m_begin);

            pointer_type begin = copy.m_begin + size_type(m_begin - orig.m_begin);
            pointer_type end   = begin + size();
            return copy.with(begin, end);
        }


        /// Returns a view of the substring [begin, end], where begin >= begin() and end <= end().
        /// \param begin    Pointer to the first char of the substring.
        /// \param end      Pointer to the char following the last char of the view.
        /// \return         View of the substring [begin, end].
        M_DECL_PURE strview_nstr_type with(pointer_type begin, pointer_type end) const
        {
            check(begin >= m_begin);
            check(begin <= m_end);
            check(end   <= m_end);
            check(end   >= m_begin);

            return strview_nstr_type{begin, end};
        }


        /// Returns a view of the substring specified by indices.
        /// \param indices  Indices object that specifies substring
        /// \return         View of the substring defined by indices.
        M_DECL_PURE strview_nstr_type with(indices const& i) const
        { return with(m_begin+i.begin, m_begin+i.end); }


        /// Returns a view of substring [begin, end], where begin >= begin() and end <= end().
        /// \param begin    Pointer to the first char of the substring.
        /// \param end      Pointer to the char following the last char of the view.
        /// \return         View of the substring [begin, end].
        M_DECL_PURE strview_nstr_type withClamp(pointer_type begin, pointer_type end) const
        {
            if (begin < m_begin) begin = m_begin;
            if (end   > m_end  ) end   = m_end;
            return strview_nstr_type{begin, end};
        }


        /// Returns a view of substring [begin(), end], where end <= end().
        /// \param end  Pointer to the char following the last char of the view.
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
        M_DECL_PURE strview_nstr_type take(size_t size) const
        { return takeBegin(size); }


        /// Returns a view of substring [begin()+begin_idx, begin()+end_idx].
        /// \param beg_idx  index of the first char of the substring.
        /// \param end_idx  Pointer to the char following the last char of the view.
        /// \return         View of the substring [begin, end].
        M_DECL_PURE strview_nstr_type withIndices(size_t beg_idx, size_t end_idx) const
        { return with(m_begin+beg_idx, m_begin+end_idx); }


        M_DECL_PURE strview_nstr_type withIndices(indices const& i) const
        { return withIndices(i.begin, i.end); }


        /// Returns a view of substring [begin(), begin()+size].
        /// \param size     Requested view size
        /// \return         View of the substring [begin(), begin()+size]
        M_DECL_PURE strview_nstr_type takeBegin(size_t size) const
        { return with(m_begin, m_begin+size); }


        /// Returns a view of substring [end()-size, end()].
        /// \param size     Requested view size.
        /// \return         View of the substring [end()-size, end()].
        M_DECL_PURE strview_nstr_type takeEnd(size_t size) const
        { return with(m_end-size, m_end); }


        /// Returns a view of substring [end()-size, end()]. Equivalent to \a skipBegin()
        /// \param size     Requested view size.
        /// \return         View of the substring [end()-size, end()].
        M_DECL_PURE strview_nstr_type skip(size_t size) const
        { return skipBegin(size); }


        /// Returns a view of the substring [ \a begin()+size, \a end() ].
        /// \param size     Requested view size.
        /// \return         View of the substring [ \a begin()+size, \a end() ].
        M_DECL_PURE strview_nstr_type skipBegin(size_t size) const
        { return with(m_begin+size, m_end); }


        /// Returns a view of the substring [ \a begin(), \a end()-size ].
        /// \param size     Requested view size.
        /// \return         View of the substring [ \a begin(), \a end()-size ].
        M_DECL_PURE strview_nstr_type skipEnd(size_t size) const
        { return with(m_begin, m_end-size); }

        /// Returns a pointer to the first occurrence of \a s in this view.
        /// \param s        String to search for.
        /// \param n        Position to start searching from. Defaults to 0.
        /// \return         Pointer to the first occurrence of \a s, or \a end() if not found.
        M_DECL_PURE pointer_type find(strview_type const& s, size_t n=0) const
        {
            size_t sz = s.size();
            if (sz > this->size())
                return m_end;

            for (pointer_type i = m_begin+n; i < m_end-sz+1; ++i)
                if (equals(with(i,i+sz), s))
                    return i;
            return m_end;
        }

        /// Returns a pointer to the first occurrence of \a c in this view.
        /// \param c        Character to search for.
        /// \param n        Position to start searching from. Defaults to 0.
        /// \return         Pointer to the first occurrence of \a c, or \a end() if not found.
        M_DECL_PURE pointer_type find(char_type c, size_t n=0) const
        {
            for (pointer_type i = m_begin+n; i < m_end; ++i)
                if (traits_type::eq(*i, c))
                    return i;
            return m_end;
        }
 
        /// Returns a pointer to the first occurrence of \a s in this view.
        /// \param s        String to search for.
        /// \param n        Position to start searching from. Defaults to 0.
        /// \return         Pointer to the first occurrence of \a s, or \a end() if not found.
        M_DECL_PURE pointer_type findFirst(strview_type const& s, size_t n=0) const
        { return find(s, n); }

        /// Returns a pointer to the first occurrence of \a c in this view.
        /// \param c        Character to search for.
        /// \param n        Position to start searching from. Defaults to 0.
        /// \return         Pointer to the first occurrence of \a c, or \a end() if not found.
        M_DECL_PURE pointer_type findFirst(char_type c, size_t n=0) const
        { return find(c, n); }

        /// Returns a pointer to the first occurence of a character not equal to \a c in this view.
        /// \param c        Character to search for.
        /// \param n        Position to start searching from. Defaults to 0.
        /// \return         Pointer to the first occurrence of a character not equal to \a c, or \a end() if not found.
        M_DECL_PURE pointer_type findFirstNot(char_type c, size_t n=0) const
        {
            for (pointer_type i = m_begin+n; i < m_end; ++i)
                if (!traits_type::eq(*i, c))
                    return i;
            return m_end;
        }

        /// Returns a pointer to the last occurrence of \a s in this view.
        /// \param s        String to search for.
        /// \param n        Position to start searching from. Defaults to 0.
        /// \return         Pointer to the last occurrence of \a s, or \a end() if not found.
        M_DECL_PURE pointer_type findLast(strview_type const& s) const
        {
            size_t sz = s.size();
            if (sz > size())
                return m_end;
            for(pointer_type i = m_end-s.size(); i > m_begin-1; --i)
                if (equals(with(i,i+sz), s))
                    return i;
            return m_end;
        }

        /// Returns a pointer to the last occurrence of \a c in this view.
        /// \param c        Character to search for.
        /// \param n        Position to start searching from. Defaults to 0.
        /// \return         Pointer to the last occurrence of \a c, or \a end() if not found.
        M_DECL_PURE pointer_type findLast(char_type c) const
        {
            if (m_begin == m_end)
                return m_begin;
            for (pointer_type i = m_end-1; i > m_begin-1; --i)
                if (traits_type::eq(*i,c))
                    return i;
            return m_end;
        }

        /// Returns a pointer to the last occurrence of a character not equal to \a c in this view.
        /// \param c        Character to search for.
        /// \return         Pointer to the last occurrence of a character not equal to \a c, or \a end() if not found.
        M_DECL_PURE pointer_type findLastNot(char_type c) const
        {
            for (pointer_type i = m_end-1; i != m_begin-1; --i)
                if (!traits_type::eq(*i,c))
                    return i;
            return m_end;
        }

        /// Returns true if this view begins with the character \a c.
        /// \param c        Character to search for.
        /// \return         True if this view begins with the character \a c, false otherwise.
        M_DECL_PURE bool beginsWith(char_type c) const
        {
            if (m_begin == m_end)
                return false;
            return *m_begin == c;
        }

        /// Returns true if this view begins with the string \a s.
        /// \param s        String to search for.
        /// \return         True if this view begins with the string \a s, false otherwise.
        M_DECL_PURE bool beginsWith(strview_type const& s) const
        {
            if (s.size() > size())
                return false;
            return equals(takeBegin(s.size()), s);
        }

        /// Returns true if this view ends with the character \a c.
        /// \param c        Character to search for.
        /// \return         True if this view ends with the character \a c, false otherwise.
        M_DECL_PURE bool endsWith(char_type c) const
        {
            if (m_begin == m_end)
                return false;
            return *(m_end-1) == c;
        }

        /// Returns true if this view ends with the string \a s.
        /// \param s        String to search for.
        /// \return         True if this view ends with the string \a s, false otherwise.
        M_DECL_PURE bool endsWith(strview_type const& s) const
        {
            return equals(with(m_end - s.size(), m_end), s);
        }

        ///
        M_DECL_PURE bool isPrint() const
        {
            for (auto&& it: *this)
                if (!isprint(it))
                    return false;
            return true;
        }

        #pragma region Trim

        /// Removes leading whitespace from this view.
        /// \return        A new view with leading whitespace removed.
        M_DECL_PURE strview_nstr_type ltrim() const { return with(trimBegin(), m_end); }

        /// Removes trailing whitespace from this view.
        /// \return       A new view with trailing whitespace removed.
        M_DECL_PURE strview_nstr_type rtrim() const { return with(m_begin, trimEnd()); }

        /// Removes leading and trailing whitespace from this view.
        /// \return       A new view with leading and trailing whitespace removed.
        M_DECL_PURE strview_nstr_type trim() const { return with(trimBegin(), trimEnd()); }

        /// Checks if this view is trimmed. A view is considered trimmed if it is empty or if the first and last characters are not whitespace.
        /// \return       True if this view is trimmed, false otherwise.       
        M_DECL_PURE bool isTrim() const { return empty() || ( !std::isspace(first()) && !std::isspace(last()) ); }

        /// Checks if leading whitespace has been removed from this view.
        /// \return       True if leading whitespace has been removed, false otherwise.
        M_DECL_PURE bool isLtrim() const { return empty() || ( !std::isspace(first()) ); }

        /// Checks if trailing whitespace has been removed from this view.
        /// \return       True if trailing whitespace has been removed, false otherwise.
        M_DECL_PURE bool isRtrim() const { return empty() || ( !std::isspace(last()) ); }

        ///
        M_DECL_PURE size_t ltrimLength() const { return trimBegin() - m_begin; }

        ///
        M_DECL_PURE size_t rtrimLength() const { return m_end - trimEnd(); }


        M_DECL_PURE std::string trimMargin(strview_cstr_type margin_prefix = "|") const
        {
            if (empty())
                return "";

            std::string result;

            auto line_begin = m_begin;

            while (line_begin < m_end)
            {
                // Find end of current line
                auto line_end = line_begin;
                while (line_end < m_end && *line_end != '\n')
                    ++line_end;

                // Skip leading whitespace
                auto content_begin = line_begin;
                while (content_begin < line_end && isspace(*content_begin) && *content_begin != '\n')
                    ++content_begin;

                // Check if line starts with margin prefix after whitespace
                if (withBegin(content_begin).beginsWith(margin_prefix))
                {

                    // Copy content after margin prefix
                    auto after_prefix = content_begin + margin_prefix.size();
                    result += with(after_prefix, line_end).str();
                }
                else
                {
                    // Copy entire line (no margin prefix found)
                    result += with(line_begin, line_end).str();
                }

                if (*line_end != '\n')
                    break; // End of string

                // Add newline if there was one in original
                result += '\n';
                line_begin = line_end + 1;
            }

            return result;
        }

        #pragma endregion Trim

        #pragma region Split

M_DECL_PURE std::vector<strview_nstr_type> rsplit(std::string const& sep = {}, int max_split = -1)
        {

            // TODO: implement this correctly (below is broken)
            nopath_impl;
            return { };

#define RSPLIT_CHECK { if (max_split > 0 && v.size() >= (size_t)(max_split)) { v.insert(v.begin(), with(m_begin, word_end)); return v; } }

            std::vector<strview_nstr_type> v;

            if (max_split == 0)
                return v;

            // Default separator (whitespace)
            if (sep.empty())
            {
                auto word_end = m_end;

                while (word_end != m_begin)
                {
                    // Skip trailing whitespace
                    while (word_end != m_begin && std::isspace(*(word_end - 1)))
                    {
                        if (--word_end == m_begin)
                            return v;
                    }

                    auto word_beg = word_end;

                    // Find beginning of current token
                    while (word_beg != m_begin && !std::isspace(*(word_beg - 1)))
                    {
                        if (--word_beg == m_begin)
                        {
                            v.insert(v.begin(), with(word_beg, word_end));
                            return v;
                        }
                    }

                    RSPLIT_CHECK
                    v.insert(v.begin(), with(word_beg, word_end));

                    word_end = word_beg;
                }
            }
            else
            {
                auto word_end = m_end;
                auto sep_beg = m_begin;
                auto sep_sz = sep.size();

                while (word_end >= sep_beg + sep_sz)
                {
                    auto word_beg = word_end;

                    while (with(word_beg - sep_sz, word_beg) != sep)
                    {
                        if (--word_beg < sep_beg)
                        {
                            RSPLIT_CHECK
                            v.insert(v.begin(), with(m_begin, m_end));
                            return v;
                        }
                    }

                    RSPLIT_CHECK
                    v.insert(v.begin(), with(word_beg + sep_sz, word_end));

                    word_end = word_beg;
                }

                RSPLIT_CHECK
                v.insert(v.begin(), with(m_begin, word_end));
            }

            return v;
#undef RSPLIT_CHECK
        }

        M_DECL_PURE std::vector<strview_nstr_type> split(std::string const& sep = {}, int max_split = -1) const
        {
#define SPLIT_CHECK { if (max_split > 0 && v.size() >= (size_t)(max_split)) { v.push_back(with(word_beg, m_end)); return v; } }

            std::vector<strview_nstr_type> v;

            if (max_split == 0)
                return v;

            // Default separator (whitespace)
            if (sep.empty())
            {
                auto word_beg = m_begin;

                while (word_beg != m_end)
                {
                    // Skip leading whitespace
                    while (std::isspace(*word_beg))
                    {
                        if (++word_beg == m_end)
                            return v;
                    }

                    auto word_end = word_beg;

                    // Find end of current token
                    while (not std::isspace(*word_end))
                    {
                        if (++word_end == m_end)
                        {
                            v.push_back(with(word_beg, word_end));
                            return v;
                        }
                    }

                    SPLIT_CHECK
                    v.push_back(with(word_beg, word_end));

                    word_beg = word_end;
                }
            }
            else
            {
                auto word_beg = m_begin;
                auto sep_end = m_end-sep.size();
                auto sep_sz = sep.size();

                while (word_beg <= sep_end)
                {
                    auto word_end = word_beg;

                    while (with(word_end, word_end+sep_sz) != sep)
                    {
                        if (++word_end > sep_end)
                        {
                            SPLIT_CHECK
                            v.push_back(with(word_beg, m_end));
                            return v;
                        }
                    }

                    SPLIT_CHECK
                    v.push_back(with(word_beg, word_end));

                    word_beg = word_end + sep_sz;
                }

                SPLIT_CHECK
                v.push_back(with(word_beg, m_end));
            }

            return v;
#undef SPLIT_CHECK
        }

        #pragma endregion Split


        M_DECL_PURE bool contains(strview_type const& s) const
        { return find(s) != m_end; }

        M_DECL_PURE bool contains(char_type c) const
        { return find(c) != nullptr; }

        M_DECL_PURE char_type operator[] (size_t i) const
        { return *(m_begin+i); }

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
        M_DECL_PURE static strview_cstr_type make_cstrview(pointer_type begin, size_t sz)
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

template <typename Char, typename Traits, bool NullTerminated>
struct std::hash<ut::basic_strview<Char,Traits,NullTerminated>>
{
    size_t operator()(ut::basic_strview<Char,Traits,NullTerminated> const& k) const
    {
        using string_view_type = typename ut::basic_strview<Char,Traits,NullTerminated>::string_view_type;
        return hash<string_view_type>{}(k.view());
    }
};

#undef M_DECL_PURE
#undef M_DECL
#undef SUPPORTS_STD_FORMAT