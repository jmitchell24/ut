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
    struct basic_stringviewbuf;

    template <typename Char, typename Traits = std::char_traits<Char>>
    struct basic_svstream;

    template <bool SkipEmpty, typename Char, typename Traits = std::char_traits<Char>>
    struct basic_svsplit;

#define ENABLE_IF_NULL_TERMINATED  template <bool N_ = NullTerminated, typename = std::enable_if_t<N_ == true>>
#define ENABLE_IF_NOT_NULL_TERMINATED template <bool N_ = NullTerminated, typename = std::enable_if_t<N_ == false>>

    template <typename Char, typename Traits = std::char_traits<Char>, bool NullTerminated = false>
    class basic_stringview
    {
    public:
        using char_type             = Char;
        using traits_type           = Traits;
        using pointer_type          = char_type const*;
        using unique_pointer_type   = std::unique_ptr<char_type>;
        using shared_pointer_type   = std::shared_ptr<char_type>;
        using size_type             = size_t;
        using string_type           = std::basic_string     <char_type,traits_type>;
        using ostream_type          = std::basic_ostream    <char_type,traits_type>;
        using string_view_type      = std::basic_string_view<char_type,traits_type>;
        using stringview_nstr_type  = basic_stringview      <char_type,traits_type, false>;
        using stringview_cstr_type  = basic_stringview      <char_type,traits_type, true>;
        using stringview_type       = basic_stringview      <char_type,traits_type, NullTerminated>;
        using svstreambuf_type      = basic_stringviewbuf   <char_type,traits_type>;
        using svstream_type         = basic_svstream        <char_type,traits_type>;

        template <bool SkipEmpty>
        using svsplit_type = basic_svsplit<SkipEmpty, char_type,traits_type>;

        inline constexpr static bool NULL_TERMINATED = NullTerminated;

        M_DECL basic_stringview(stringview_type&&) noexcept =default;
        M_DECL basic_stringview(stringview_type const&)=default;
        M_DECL basic_stringview& operator=(stringview_type&&) noexcept =default;
        M_DECL basic_stringview& operator=(stringview_type const&)=default;

        /// Construct with default string ("")
        inline basic_stringview()
                : m_begin{getDefaultString()}, m_end{getDefaultString()}
        {}

        /// Construct with an STL basic_string
        M_DECL basic_stringview(string_type const& s)
                : m_begin{s.data()}, m_end{s.data()+s.size()}
        {}

        /// Construct with an STL basic_string_view
        M_DECL basic_stringview(string_view_type const& s)
                : m_begin{s.data()}, m_end{s.data()+s.size()}
        {}

        /// Construct with a begin and end pointer
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL basic_stringview(pointer_type begin, pointer_type end)
                : m_begin{begin}, m_end{end}
        {
            assert(begin != nullptr);
            assert(end   != nullptr);
            assert(begin <= end);
        }

        /// Construct with a null-terminated stringview
        ENABLE_IF_NOT_NULL_TERMINATED
        M_DECL basic_stringview(stringview_cstr_type const& s)
                : m_begin{s.begin()}, m_end{s.end()}
        {}

        /// Construct with a null-terminated string
        ENABLE_IF_NULL_TERMINATED
        M_DECL basic_stringview(pointer_type p)
                : m_begin{p}, m_end{p+strlen(p)}
        {}

        M_DECL_PURE pointer_type begin() const { return m_begin; }
        M_DECL_PURE pointer_type end  () const { return m_end; }

        M_DECL_PURE char_type first() const { assert(m_begin != m_end); return *m_begin; }
        M_DECL_PURE char_type last () const { assert(m_begin != m_end); return *(m_end-1); }

        M_DECL_PURE size_type size() const { return m_end - m_begin; }

        M_DECL_PURE bool empty() const { return m_begin == m_end; }

        M_DECL_PURE char_type at(size_type i) const
        {
            if (i < m_begin || i >= m_end)
                throw std::out_of_range{"ut::stringview::at()"};
            return m_begin[i];
        }

        M_DECL void strncpy(char_type* dest, size_t n) const
        {
            return ::strncpy(dest, m_begin, min(n, m_end-m_begin));
        }

        M_DECL_PURE shared_pointer_type shared_ptr_copy() const
        {
            size_type sz = size();
            shared_pointer_type ptr = shared_pointer_type(new char_type[sz+1]);
            ::strncpy(ptr.get(), m_begin, sz);
            *(ptr.get()+sz) = 0;
            return ptr;
        }

        M_DECL_PURE unique_pointer_type unique_ptr_copy() const
        {
            size_type sz = size();
            unique_pointer_type ptr = unique_pointer_type(new char_type[sz+1]);
            ::strncpy(ptr.get(), m_begin, sz);
            *(ptr.get()+sz) = 0;
            return ptr;
        }

        /// Return c-string (if NullTerminated == true)
        M_DECL_PURE pointer_type c_str() const
        {
            static_assert (NullTerminated, "stringview must be null-terminated to call c_str()");
            return m_begin;
        }

        /// Convert to STL basic_string_view
        M_DECL_PURE string_view_type view() const { return string_view_type(m_begin, m_end-m_begin); }

        /// Convert to STL basic_string
        M_DECL_PURE string_type str() const { return string_type(m_begin, m_end); }

        /// Convert to STL basic_streambuf
        M_DECL_PURE svstreambuf_type buf() const { return svstreambuf_type(*this); }

        /// Convert to STL basic_istream
        M_DECL_PURE svstream_type stream() const { return svstream_type(*this); }

        M_DECL_PURE stringview_nstr_type sub(pointer_type begin, pointer_type end) const
        {
            assert(begin >= m_begin);
            assert(begin <= m_end);
            assert(end   <= m_end);
            assert(end   >= m_begin);

            return stringview_type{begin, end};
        }

        /// get sub-string from \a m_begin ptr to specified \p end ptr
        M_DECL_PURE stringview_nstr_type subBegin(pointer_type end) const
        { return sub(m_begin, end); }

        /// get sub-string from specified begin ptr to \a m_end ptr
        M_DECL_PURE stringview_nstr_type subEnd(pointer_type begin) const
        { return sub(begin, m_end); }

        /// get sub-string from \a m_begin to \a m_begin + \p size
        M_DECL_PURE stringview_nstr_type take(size_type size) const
        { return takeBegin(size); }

        M_DECL_PURE stringview_nstr_type takeBegin(size_type size) const
        { return sub(m_begin, m_begin+size); }

        M_DECL_PURE stringview_nstr_type takeEnd(size_type size) const
        { return sub(m_end-size, m_end); }

        M_DECL_PURE bool same(stringview_type const& s) const { return same(*this, s); }
        M_DECL_PURE bool same(char_type c)              const { return same(*this, c); }

        M_DECL_PURE int compare(stringview_type const& s) const { return compare(*this, s); }
        M_DECL_PURE int compare(char_type c)              const { return compare(*this, c); }

        M_DECL_PURE pointer_type find(stringview_type const& s) const
        {
            size_type sz = s.size();
            if (sz > this->size())
                return m_end;

            for (pointer_type i = m_begin; i != m_end-sz+1; ++i)
                if (sub(i,sz).same(s))
                    return i;
            return m_end;
        }

        M_DECL_PURE pointer_type find(char_type c) const
        { return traits_type::find(m_begin, size(), c); }

        M_DECL_PURE pointer_type findFirst(stringview_type const& s) const
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

        M_DECL_PURE pointer_type findLast(stringview_type const& s) const
        {
            size_type sz = s.size();
            if (sz > size())
                return m_end;
            for(pointer_type i = m_end-s.size(); i != m_begin-1; --i)
                if (sub(i,sz).same(s))
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
                if (!traits_type::eq(*i != c))
                    return i;
            return end;
        }

        M_DECL_PURE bool beginsWith(char_type c) const
        {
            if (m_begin == m_end)
                return false;
            return *m_begin == c;
        }

        M_DECL_PURE bool beginsWith(stringview_type const& s) const
        {
            if (m_begin == m_end)
                return false;
            return takeBegin(s.size()).same(s);
        }

        M_DECL_PURE bool endsWith(char_type c) const
        {
            if (m_begin == m_end)
                return false;
            return *(m_end-1) == c;
        }

        M_DECL_PURE bool endsWith(stringview_type const& s) const
        {
            return sub(m_end - s.size(), m_end).same(s);
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

        M_DECL_PURE stringview_nstr_type trimLeft()  const { return sub(trimBegin(), m_end); }
        M_DECL_PURE stringview_nstr_type trimRight() const { return sub(m_begin, trimEnd()); }
        M_DECL_PURE stringview_nstr_type trim()      const { return sub(trimBegin(), trimEnd()); }

        M_DECL_PURE bool trimmed     () const { return empty() || ( !std::isspace(first()) && !std::isspace(last()) ); }
        M_DECL_PURE bool trimmedLeft () const { return empty() || ( !std::isspace(first()) ); }
        M_DECL_PURE bool trimmedRight() const { return empty() || ( !std::isspace(last()) ); }

        M_DECL_PURE bool contains(stringview_type const& s) const
        { return find(s) != m_end; }

        M_DECL_PURE bool contains(char_type c) const
        { return find(c) != m_end; }

        M_DECL_PURE char_type operator[] (size_type i) const { return *(m_begin+i); }

        M_DECL_PURE bool operator<  (stringview_type const& s) const { return compare(s) < 0; }
        M_DECL_PURE bool operator>  (stringview_type const& s) const { return compare(s) > 0; }
        M_DECL_PURE bool operator== (stringview_type const& s) const { return  same(s); }
        M_DECL_PURE bool operator!= (stringview_type const& s) const { return !same(s); }

        M_DECL_PURE static bool same(stringview_type const& a, stringview_type const& b)
        {
            if (a.size() != b.size())
                return false;

            pointer_type ia=a.m_begin, ib=b.m_begin;
            for (; ia != a.m_end; ++ia,++ib)
                if (!traits_type::eq(*ia,*ib))
                    return false;
            return true;
        }

        M_DECL_PURE static bool same(stringview_type const& a, char_type b)
        { return a.size() == 1 && traits_type::eq(*a.begin, b); }

        M_DECL_PURE static int compare(stringview_type const& a, stringview_type const& b)
        { return traits_type::compare(a.m_begin, b.m_begin, std::min(a.size(), b.size())); }

        M_DECL_PURE static int compare(stringview_type const& a, char_type b)
        { return traits_type::compare(a.begin, &b, 1); }

        M_DECL_PURE static stringview_cstr_type explicit_construct_cstr(pointer_type begin, size_type sz)
        { return stringview_cstr_type(begin, sz); }

        friend inline ostream_type& operator<<(ostream_type& os, stringview_type const& s)
        { return os.write(s.begin(), s.end() - s.begin()); }

    private:
        char_type const* m_begin;
        char_type const* m_end;

        M_DECL_PURE explicit basic_stringview(pointer_type begin, size_type sz)
                : m_begin{begin}, m_end{m_begin+sz}
        {}

        static char_type const* getDefaultString()
        {
            static char_type buf[1];
            return buf;
        }
    };

#undef ENABLE_IF_NULL_TERMINATED
#undef ENABLE_IF_NOT_NULL_TERMINATED

    template <bool SkipEmpty, typename Char, typename Traits>
    struct basic_svsplit
    {
        using char_type         = Char;
        using traits_type       = Traits;
        using stringview_type   = basic_stringview<Char, Traits>;
        using size_type         = typename stringview_type::size_type;
        using pointer_type      = typename stringview_type::pointer_type;
        using container_type    = std::vector<basic_stringview<char_type>>;

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
            static_assert (is_callback_v<Callable>, "Callable must accept 1 parameter of stringview_type, return value must cast to bool");

            if (sv.empty())
                return;

            pointer_type token_begin = sv.begin();
            for (pointer_type i = token_begin; i != sv.end(); ++i)
            {
                if (static_cast<bool>(unary_delim(*i)))
                {
                    if constexpr(SkipEmpty)
                    {
                        stringview_type tok = sv.sub(token_begin, i);
                        if (!tok.empty())
                        {
                            if (!callback(tok))
                                return;
                        }
                    }
                    else
                    {
                        if (!callback(sv.sub(token_begin, i)))
                            return;
                    }

                    token_begin = i+1;
                }
            }

            if constexpr(SkipEmpty)
            {
                stringview_type tok = sv.subEnd(token_begin);
                if (!tok.empty())
                    callback(tok);
            }
            else
            {
                callback(sv.subEnd(token_begin));
            }
        }


        template <typename Callable>
        inline static void splitWS(stringview_type const& sv, Callable&& callback)
        {
            static_assert (SkipEmpty, "SkipEmpty should be true when delim is white space ('  x  x  ' should only split once).");
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
            static_assert (SkipEmpty, "SkipEmpty should be true when delim is white space ('  x  x  ' should only split once).");
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
            static_assert (is_tieable_v<T>, "out parameter must be stringview or std::ignore");

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
            static_assert (is_tieable_v<T>, "out parameter must be stringview or std::ignore");

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
    class basic_stringviewbuf : public std::basic_streambuf<Char, Traits>
    {
    public:
        using char_type         = Char;
        using traits_type       = Traits;
        using int_type          = typename traits_type::int_type;
        using streambuf_type    = std::basic_streambuf<char_type, traits_type>;
        using string_view_type  = basic_stringview<char_type, traits_type>;

        explicit basic_stringviewbuf(string_view_type const& s) :
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
    struct basic_svstream : public std::basic_istream<Char, Traits>
    {
        using char_type         = Char;
        using traits_type       = Traits;
        using string_view_type  = basic_stringview<char_type, traits_type>;
        using streambuf_type    = basic_stringviewbuf<char_type, traits_type>;

        explicit basic_svstream(string_view_type const& s)
                : m_buf(s), std::basic_istream<char_type, traits_type>(&m_buf)
        {}

        inline string_view_type all() const
        { return m_buf.all(); }

        inline string_view_type remainder() const
        { return m_buf.remainder(); }

    protected:
        streambuf_type m_buf;
    };

    template <typename Char, typename Traits, bool NullTerminated> inline Char const* begin(basic_stringview<Char,Traits,NullTerminated> const& s) { return s.begin(); }
    template <typename Char, typename Traits, bool NullTerminated> inline Char const* end  (basic_stringview<Char,Traits,NullTerminated> const& s) { return s.end(); }

    using stringview = basic_stringview<char, std::char_traits<char>, false>;
    using cstrview = basic_stringview<char, std::char_traits<char>, true>;

    //
    // *new* aliases (experiment)
    //

    using strview = basic_stringview<char, std::char_traits<char>, false>;
    //using cstrview = basic_stringview<char, std::char_traits<char>, true>;

    using strparam = strview const&;
    using cstrparam = cstrview const&;

    //
    //
    //

    template <bool SkipEmpty = true>
    using svsplit = basic_svsplit<SkipEmpty, char, std::char_traits<char>>;

    using stringviewbuf = basic_stringviewbuf<char>;
    using svstream      = basic_svstream<char>;

    M_DECL_PURE cstrview operator "" _sv(char const* str, size_t sz) noexcept
    {
        return cstrview::explicit_construct_cstr(str, sz);
    }

    template <char Delim>
    inline bool delim(char c)
    { return c == Delim; }
}

namespace std
{
    template<class Key> struct hash;

    template <typename Char, typename Traits, bool NullTerminated>
    struct hash<ut::basic_stringview<Char,Traits,NullTerminated>>
    {
        size_t operator()(ut::basic_stringview<Char,Traits,NullTerminated> const& k) const
        {
            using string_view_type = typename ut::basic_stringview<Char,Traits,NullTerminated>::string_view_type;
            return hash<string_view_type>{}(k.view());
        }
    };
}


#undef M_DECL_PURE
#undef M_DECL

#endif // STRINGVIEW_HPP
