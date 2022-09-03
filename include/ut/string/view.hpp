#ifndef STRINGVIEW_HPP
#define STRINGVIEW_HPP

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
#include <istream>

#define M_DECL_PURE             [[nodiscard]] inline constexpr
#define M_DECL                  inline constexpr

namespace ut
{
    template <typename Char, typename Traits = std::char_traits<Char>>
    struct basic_strview_buf;

    template <typename Char, typename Traits = std::char_traits<Char>>
    struct basic_strview_stream;

    template <bool SkipEmpty, typename Char, typename Traits = std::char_traits<Char>>
    struct basic_strview_split;

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
        using buf_type              = basic_strview_buf     <char_type,traits_type>;
        using stream_type           = basic_strview_stream  <char_type,traits_type>;

        template <bool SkipEmpty>
        using split_type = basic_strview_split<SkipEmpty, char_type, traits_type>;

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
        {}

        /// Construct with an STL basic_string_view
        M_DECL basic_strview(string_view_type const& s)
            : m_begin{s.data()}, m_end{s.data()+s.size()}
        {}

        /// Construct with a begin and end pointer
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL basic_strview(pointer_type begin, pointer_type end)
            : m_begin{begin}, m_end{end}
        {
            assert(begin != nullptr);
            assert(end   != nullptr);
            assert(begin <= end);
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
            assert(*m_end == '\0');
        }

        /// Construct with a null-terminated stringview
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL basic_strview(strview_cstr_type const& s)
            : m_begin{s.begin()}, m_end{s.end()}
        {}

        /// Construct with a null-terminated string
        ENABLE_IF_NULL_TERMINATED
        M_DECL basic_strview(pointer_type p)
            : m_begin{p}, m_end{p+traits_type::length(p)}
        {}

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
        M_DECL_PURE char_type last () const { assert(m_begin != m_end); return *(m_end-1); }

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

        /// Returns an STL basic_strview_buf
        M_DECL_PURE buf_type buf() const { return buf_type(*this); }

        /// Returns an STL basic_strview_stream
        /// \return
        M_DECL_PURE stream_type stream() const { return stream_type(*this); }

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

        /// Returns a view of substring [begin(), begin()+size]
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
            if (m_begin == m_end)
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

        M_DECL_PURE strview_nstr_type trimLeft()  const { return with(trimBegin(), m_end); }
        M_DECL_PURE strview_nstr_type trimRight() const { return with(m_begin, trimEnd()); }
        M_DECL_PURE strview_nstr_type trim()      const { return with(trimBegin(), trimEnd()); }

        M_DECL_PURE bool trimmed     () const { return empty() || ( !std::isspace(first()) && !std::isspace(last()) ); }
        M_DECL_PURE bool trimmedLeft () const { return empty() || ( !std::isspace(first()) ); }
        M_DECL_PURE bool trimmedRight() const { return empty() || ( !std::isspace(last()) ); }

        M_DECL_PURE bool contains(strview_type const& s) const
        { return find(s) != m_end; }

        M_DECL_PURE bool contains(char_type c) const
        { return find(c) != m_end; }

        M_DECL_PURE char_type operator[] (size_type i) const { return *(m_begin+i); }

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
    };

#undef ENABLE_IF_NULL_TERMINATED
#undef ENABLE_IF_NOT_NULL_TERMINATED

    template <bool SkipEmpty, typename Char, typename Traits>
    struct basic_strview_split
    {
        using char_type         = Char;
        using traits_type       = Traits;
        using stringview_type   = basic_strview<Char, Traits>;
        using size_type         = typename stringview_type::size_type;
        using pointer_type      = typename stringview_type::pointer_type;
        using container_type    = std::vector<basic_strview<char_type>>;

        template <typename Unary>
        inline constexpr static bool is_unary_delim_v =
                std::is_invocable_r_v<bool, Unary, char_type>;

        template <typename Callable>
        inline constexpr static bool is_callback_v =
                std::is_invocable_r_v<bool, Callable, stringview_type>;

        template <typename T>
        inline constexpr static bool is_tieable_v =
                std::is_same_v<T, stringview_type> ||
                std::is_same_v<T, decltype(std::ignore)>;

        template <typename Callable, typename Unary>
        inline static void split(stringview_type const& sv, Callable&& callback, Unary&& unary_delim)
        {
            static_assert (is_unary_delim_v<Unary>, "Unary must accept 1 parameter of char_type, return value must cast to bool");
            static_assert (is_callback_v<Callable>, "Callable must accept 1 parameter of view_type, return value must cast to bool");

            if (sv.empty())
                return;

            pointer_type token_begin = sv.begin();
            for (pointer_type i = token_begin; i != sv.end(); ++i)
            {
                if (static_cast<bool>(unary_delim(*i)))
                {
                    if constexpr(SkipEmpty)
                    {
                        stringview_type tok = sv.with(token_begin, i);
                        if (!tok.empty())
                        {
                            if (!callback(tok))
                                return;
                        }
                    }
                    else
                    {
                        if (!callback(sv.with(token_begin, i)))
                            return;
                    }

                    token_begin = i+1;
                }
            }

            if constexpr(SkipEmpty)
            {
                stringview_type tok = sv.withEnd(token_begin);
                if (!tok.empty())
                    callback(tok);
            }
            else
            {
                callback(sv.withEnd(token_begin));
            }
        }


        template <typename Callable>
        inline static void splitWS(stringview_type const& sv, Callable&& callback)
        {
            //static_assert (SkipEmpty, "SkipEmpty should be true when delim is white space ('  x  x  ' should only split once).");
            return split(sv, callback, &std::iswspace);
        }

        template <typename Unary>
        inline static container_type container(stringview_type const& sv, Unary&& unary_delim)
        {
            static_assert (is_unary_delim_v<Unary>, "Unary must accept 1 parameter of char_type, return value must cast to bool");

            container_type tokens;
            split(
                    sv,
                    [&tokens](auto&& s){ tokens.emplace_back(s); return true; },
                    unary_delim
            );
            return tokens;
        }

        inline static container_type containerWS(stringview_type const& sv)
        {
            //static_assert (SkipEmpty, "SkipEmpty should be true when delim is white space ('  x  x  ' should only split once).");
            return container(sv, &std::iswspace);
        }

        template <typename Unary, typename... Params>
        inline static size_type tie(stringview_type const& sv, Unary&& unary_delim, Params&... params)
        {
            static_assert (is_unary_delim_v<Unary>, "Unary must accept 1 parameter of char_type, return value must cast to bool");
            static_assert (sizeof...(Params) > 0, "Non-zero number of out parameters are required");

            if (sv.empty())
                return 0;

            return tie_impl<1, Unary, Params...>(sv, sv.begin(), unary_delim, params...);
        }

        template <typename... Params>
        inline static size_type tieWS(stringview_type const& sv, Params&... params)
        {
            static_assert (SkipEmpty, "SkipEmpty should be true when delim is white space ('  x  x  ' should only split once).");
            return tie(sv, &std::iswspace, params...);
        }

    private:

        template <size_type I, typename U, typename T>
        inline static size_type tie_impl(stringview_type const& sv, pointer_type token_begin, U unary_delim, T& out)
        {
            static_assert (is_tieable_v<T>, "out parameter must be strview or std::ignore");

            for (pointer_type i = token_begin; i != sv.end(); ++i)
            {
                if (static_cast<bool>(unary_delim(*i)))
                {
                    if constexpr(SkipEmpty)
                    {
                        stringview_type tok = sv.sub(token_begin, i);
                        if (!tok.empty())
                        {
                            out = tok;
                            return I;
                        }

                        token_begin = i+1;
                    }
                    else
                    {
                        out = sv.sub(token_begin, i);
                        return I;
                    }
                }
            }

            if constexpr(SkipEmpty)
            {
                stringview_type tok = sv.subEnd(token_begin);
                if (!tok.empty())
                {
                    out = tok;
                    return I;
                }
                return 0;
            }
            else
            {
                out = sv.subEnd(token_begin);
                return I;
            }
        }

        template <size_type I, typename U, typename T, typename... TT>
        inline static size_type tie_impl(stringview_type const& sv, pointer_type token_begin, U unary_delim, T& out, TT&... tt)
        {
            static_assert (is_tieable_v<T>, "out parameter must be strview or std::ignore");

            for (pointer_type i = token_begin; i != sv.end(); ++i)
            {
                if (static_cast<bool>(unary_delim(*i)))
                {
                    if constexpr(SkipEmpty)
                    {
                        stringview_type tok = sv.sub(token_begin, i);
                        if (!tok.empty())
                        {
                            out = tok;
                            return tie_impl<I+1,U>(sv, i+1, unary_delim, tt...);
                        }

                        token_begin = i+1;
                    }
                    else
                    {
                        out = sv.sub(token_begin, i);
                        return tie_impl<I+1,U>(sv, i+1, unary_delim, tt...);
                    }
                }
            }

            if constexpr(SkipEmpty)
            {
                stringview_type tok = sv.subEnd(token_begin);
                if (!tok.empty())
                {
                    out = tok;
                    return I;
                }
                return 0;
            }
            else
            {
                out = sv.subEnd(token_begin);
                return I;
            }
        }
    };


    template <typename Char, typename Traits>
    class basic_strview_buf : public std::basic_streambuf<Char, Traits>
    {
    public:
        using char_type         = Char;
        using traits_type       = Traits;
        using int_type          = typename traits_type::int_type;
        using streambuf_type    = std::basic_streambuf<char_type, traits_type>;
        using string_view_type  = basic_strview<char_type, traits_type>;

        explicit basic_strview_buf(string_view_type const& s) :
                m_begin(s.begin()),
                m_cur  (m_begin),
                m_end  (s.end())
        { }

        inline string_view_type all() const
        { return string_view_type(m_begin, m_end); }

        inline string_view_type remainder() const
        { return string_view_type(m_cur, m_end); }

    private:
        char_type const* m_begin;
        char_type const* m_cur;
        char_type const* m_end;

        // Attempt to change cursor position
        // If new position is out of bounds, do nothing and return -1 (same behavior as default implementation)
        inline std::streampos seek(char_type const* cur)
        {
            if (cur < m_begin || cur > m_end)
                return std::streampos(std::streamoff(-1));
            m_cur = cur;
            return std::streampos(std::streamoff(m_cur-m_begin));
        }

        // Virtual Member Functions of std::basic_streambuf

        // Not Re-Implemented
        // void imbue (const locale& loc);

        // Not Re-Implemented
        // streambuf* setbuf (char* s, streamsize n);

        // Not Re-Implemented
        // int sync();

        // Not Re-Implemented
        // streamsize xsputn (const char* s, streamsize n);

        // Not Re-Implemented
        // int overflow (int c = EOF);

        // Seek by relative offset
        inline std::streampos seekoff(
                std::streamoff          off,
                std::ios_base::seekdir  way,
                std::ios_base::openmode which) override
        {
            assert(which == std::ios_base::in);

            char_type const* cur = nullptr;

            switch (way)
            {
                case std::ios_base::beg: cur = m_begin + off; break;
                case std::ios_base::end: cur = m_end   + off; break;
                case std::ios_base::cur: cur = m_cur   + off; break;
                default:break;
            }

            return seek(cur);
        }

        // Seek by absolute offset
        inline std::streampos seekpos(
                std::streampos sp,
                std::ios_base::openmode which) override
        {
            assert(which == std::ios_base::in);

            // This is why people don't like C++ and the STL
            std::streamoff off = sp - std::streampos(std::streamoff(0));
            char_type const* cur = m_begin + off;
            return seek(cur);
        }

        // s-how-many-c: Return size of stringview
        inline std::streamsize showmanyc() override
        {
            return m_end - m_cur;
        }

        // Copy chars to specified buffer
        inline std::streamsize xsgetn(char_type* c, std::streamsize count) override
        {
            count = std::min(count, m_cur - m_end);
            std::copy(m_cur, m_cur + count, c);
            return count;
        }

        // Return the char value at the current position
        inline int_type underflow() override
        {
            if (m_cur == m_end)
                return traits_type::eof();
            return traits_type::to_int_type(*m_cur);
        }

        // Return the char value at the current position, then increment current position
        inline int_type uflow() override
        {
            if (m_cur == m_end)
                return traits_type::eof();
            return traits_type::to_int_type(*m_cur++);
        }

        // Put char at current position, then decrement current position
        inline int_type pbackfail(int_type ch) override
        {
            // https://en.cppreference.com/w/cpp/io/basic_stringbuf/pbackfail
            // Mimic basic_stringbuf behavior but fail on attempt to write to buffer

            // Cannot decrement beyond beginning of buffer, fail
            if (m_cur == m_begin)
                return traits_type::eof();

            // ch != EOF means caller wants to write ch to buffer (which is read only), fail
            if (ch != traits_type::eof())
                return traits_type::eof();

            // decrement current position, return not_eof(ch) to indicate success
            --m_cur;
            return traits_type::not_eof(ch);
        }
    };

    template <typename Char, typename Traits>
    struct basic_strview_stream : public std::basic_istream<Char, Traits>
    {
        using char_type         = Char;
        using traits_type       = Traits;
        using string_view_type  = basic_strview<char_type, traits_type>;
        using streambuf_type    = basic_strview_buf<char_type, traits_type>;

        explicit basic_strview_stream(string_view_type const& s)
                : m_buf(s), std::basic_istream<char_type, traits_type>(&m_buf)
        {}

        inline string_view_type all() const
        { return m_buf.all(); }

        inline string_view_type remainder() const
        { return m_buf.remainder(); }

    protected:
        streambuf_type m_buf;
    };

    template <typename Char, typename Traits, bool NullTerminated> inline Char const* begin(basic_strview<Char,Traits,NullTerminated> const& s) { return s.begin(); }
    template <typename Char, typename Traits, bool NullTerminated> inline Char const* end  (basic_strview<Char,Traits,NullTerminated> const& s) { return s.end(); }

    using strview  = basic_strview<char, std::char_traits<char>, false>;
    using cstrview = basic_strview<char, std::char_traits<char>, true>;

    using strparam  = strview;
    using cstrparam = cstrview;

    template <bool SkipEmpty = true>
    using split = basic_strview_split<SkipEmpty, char, std::char_traits<char>>;

    using strview_buf       = basic_strview_buf<char>;
    using strview_stream    = basic_strview_stream<char>;

    M_DECL_PURE cstrview operator "" _sv(char const* str, size_t sz) noexcept
    {
        return cstrview::make_cstrview(str, sz);
    }

    template <char Delim>
    inline bool delim(char c)
    { return c == Delim; }
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

#endif // STRINGVIEW_HPP
