#ifndef STRINGVIEW_HPP
#define STRINGVIEW_HPP

#include <string>
#include <vector>
#include <streambuf>
#include <algorithm>
#include <stdexcept>
#include <utility>

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <istream>

namespace ut
{
    template <typename Char, typename Traits = std::char_traits<Char>>
    struct basic_stringviewbuf;

    template <typename Char, typename Traits = std::char_traits<Char>>
    struct basic_svstream;

    template <typename Char, typename Traits = std::char_traits<Char>>
    class basic_stringview
    {
    public:
        using char_type             = Char;
        using traits_type           = Traits;
        using pointer_type          = char_type const*;
        using size_type             = size_t;
        using split_container_type  = std::vector           <basic_stringview<char_type>>;
        using string_type           = std::basic_string     <char_type,traits_type>;
        using ostream_type          = std::basic_ostream    <char_type,traits_type>;
        using string_view_type      = basic_stringview      <char_type,traits_type>;
        using svstreambuf_type      = basic_stringviewbuf     <char_type,traits_type>;
        using svstream_type         = basic_svstream        <char_type,traits_type>;

        basic_stringview()
            : m_begin{getDefaultString()}, m_end{getDefaultString()}
        {}

        basic_stringview(string_view_type&&)=default;
        basic_stringview(string_view_type const&)=default;
        basic_stringview& operator=(string_view_type&&)=default;
        basic_stringview& operator=(string_view_type const&)=default;

        basic_stringview(pointer_type begin, pointer_type end)
            : m_begin{begin}, m_end{end}
        {
            assert(begin != nullptr);
            assert(end   != nullptr);
            assert(begin <= end);
        }

        basic_stringview(string_type const& s)
            : basic_stringview{s.data(), s.data()+s.size()}
        {}

        basic_stringview(pointer_type p)
            : basic_stringview{p, p+strlen(p)}
        {}

        inline pointer_type begin() const { return m_begin; }
        inline pointer_type end  () const { return m_end; }

        inline size_type size() const { return m_end - m_begin; }

        inline bool empty() const { return m_begin == m_end; }

        inline char_type at(size_type i) const
        {
            if (i < m_begin || i >= m_end)
                throw std::out_of_range{"util::stringview::at()"};
            return m_begin[i];
        }

        inline string_type str() const { return string_type(m_begin, m_end); }

        inline svstream_type stream() const { return svstream_type(*this); }

        inline string_view_type sub(pointer_type begin, pointer_type end) const
        {
            assert(begin >= m_begin);
            assert(begin <= m_end);
            assert(end   <= m_end);
            assert(end   >= m_begin);

            return string_view_type{begin, end};
        }

        inline string_view_type take(size_type size) const 
        { return takeBegin(size); }

        inline string_view_type takeBegin(size_type size) const 
        { return sub(m_begin, m_begin+size); }

        inline string_view_type takeEnd(size_type size) const 
        { return sub(m_end-size, m_end); }

        inline bool same(string_view_type const& s) const { return same(*this, s); }
        inline bool same(char_type c)               const { return same(*this, c); }

        inline int compare(string_view_type const& s) const { return compare(*this, s); }
        inline int compare(char_type c)               const { return compare(*this, c); }

        inline pointer_type find(string_view_type const& s) const
        {
            size_type sz = s.size();
            if (sz > this->size())
                return m_end;

            for (pointer_type i = m_begin; i != m_end-s.size()+1; ++i)
                if (sub(i,sz).same(s))
                    return i;
            return m_end;
        }

        inline pointer_type find(char_type c) const
        { return traits_type::find(m_begin, size(), c); }

        inline pointer_type findFirst(string_view_type const& s) const
        { return find(s); }

        inline pointer_type findFirst(char_type c) const
        { return find(c); }

        inline pointer_type findFirstNot(char_type c) const
        {
            for (pointer_type i = m_begin; i != m_end; ++i)
                if (!traits_type::eq(*i, c))
                    return i;
            return m_end;
        }

        inline pointer_type findLast(string_view_type const& s) const
        {
            size_type sz = s.size();
            if (sz > size())
                return m_end;
            for(pointer_type i = m_end-s.size(); i != m_begin-1; --i)
                if (sub(i,sz).same(s))
                    return i;
            return m_end;
        }

        inline pointer_type findLast(char_type c) const
        {
            if (m_begin == m_end)
                return m_begin;
            for (pointer_type i = m_end-1; i != m_begin-1; --i)
                if (traits_type::eq(*i,c))
                    return i;
            return m_end;
        }

        inline pointer_type findLastNot(char_type c) const
        {
            for (pointer_type i = m_end-1; i != m_begin-1; --i)
                if (!traits_type::eq(*i != c))
                    return i;
            return end;
        }

        inline bool beginsWith(char_type c) const 
        {
            if (m_begin == m_end)
                return false;
            return *m_begin == c;
        }

        inline bool beginsWith(string_view_type const& s) const 
        {
            if (m_begin == m_end)
                return false;
            return takeBegin(s.size()).same(s);
        }

        inline bool endsWith(char_type c) const 
        {
            if (m_begin == m_end)
                return false;
            return *(m_end-1) == c;
        }

        inline bool endsWith(string_view_type const& s) const 
        {
            return sub(m_end - s.size(), m_end).same(s);
        }

        inline pointer_type trimBegin() const
        {
            pointer_type i = m_begin;
            while (i != m_end && std::isspace(*i)) ++i;
            return i;
        }

        inline pointer_type trimEnd() const
        {
            pointer_type i = m_end-1;
            pointer_type end = m_begin-1;
            while (i!=end && std::isspace(*i)) --i;
            return (i==end) ? m_end : i+1;
        }

        inline string_view_type trimLeft()  const { return sub(trimBegin(), m_end); }
        inline string_view_type trimRight() const { return sub(m_begin, trimEnd()); }
        inline string_view_type trim()      const { return sub(trimBegin(), trimEnd()); }

        template <typename Callable>
        inline void splitCallback(char_type delim, Callable&& callback) const
        {

            pointer_type token_begin = m_begin;

            if (m_begin == m_end)
                return;

            for (pointer_type i = token_begin; i != m_end; ++i)
            {
                if (traits_type::eq(*i, delim))
                {
                    callback(sub(token_begin, i));
                    token_begin = i+1;
                }
            }

            callback(sub(token_begin, m_end));
        }

        inline split_container_type splitContainer(char_type delim) const
        {
            split_container_type tokens;
            splitCallback(delim, [&tokens](string_view_type&& s){ tokens.emplace_back(s); });
            return tokens;
        }

        template <typename... Params>
        inline size_type split(char_type delim, Params&... params)
        {
            static_assert (sizeof...(Params) > 0, "Non-zero number of out parameters are required");

            if (m_begin == m_end)
                return 0;

            return split_char_impl<1>(delim, m_begin, params...);
        }

        inline bool contains(string_view_type const& s) const
        { return find(s) != m_end; }

        inline bool contains(char_type c) const
        { return find(c) != m_end; }

        inline char_type operator[] (size_type i) const { return *(m_begin+i); }

        inline bool operator<  (string_view_type const& s) const { return compare(s) < 0; }
        inline bool operator>  (string_view_type const& s) const { return compare(s) > 0; }
        inline bool operator== (string_view_type const& s) const { return  same(s); }
        inline bool operator!= (string_view_type const& s) const { return !same(s); }

        static bool same(string_view_type const& a, string_view_type const& b)
        {
            if (a.size() != b.size())
                return false;

            pointer_type ia=a.m_begin, ib=b.m_begin;
            for (; ia != a.m_end; ++ia,++ib)
                if (!traits_type::eq(*ia,*ib))
                    return false;
            return true;
        }

        inline static bool same(string_view_type const& a, char_type b)
        { return a.size() == 1 ? traits_type::eq(*a.begin, b) : false; }

        inline static int compare(string_view_type const& a, string_view_type const& b)
        { return traits_type::compare(a.m_begin, b.m_begin, std::min(a.size(), b.size())); }

        inline static int compare(string_view_type const& a, char_type b)
        { return traits_type::compare(a.begin, &b, 1); }

        friend ostream_type& operator<<(ostream_type& os, string_view_type const& s)
        {
            os.write(s.begin(), s.end() - s.begin());
            return os;
        }

    private:
        char_type const* m_begin;
        char_type const* m_end;

        static char_type const* getDefaultString()
        {
            static char_type buf[1];
            return buf;
        }

        template <size_type I, typename T>
        inline size_type split_char_impl(char_type delim, pointer_type token_begin, T& out)
        {
            static_assert (std::is_same<T, string_view_type>::value, "out parameter must be a string view");

            for (pointer_type i = token_begin; i != m_end; ++i)
            {
                if (traits_type::eq(*i, delim))
                {
                    out = sub(token_begin, i);
                    return I;
                }
            }

            out = sub(token_begin, m_end);
            return I;
        }

        template <size_type I, typename T, typename... TT>
        inline size_type split_char_impl(char_type delim, pointer_type token_begin, T& out, TT&... tt)
        {
            static_assert (std::is_same<T, string_view_type>::value, "out parameter must be a string view");

            for (pointer_type i = token_begin; i != m_end; ++i)
            {
                if (traits_type::eq(*i, delim))
                {
                    out = sub(token_begin, i);
                    return split_char_impl<I+1>(delim, i+1, tt...);
                }
            }

            out = sub(token_begin, m_end);
            return I;
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

        basic_stringviewbuf(string_view_type const& s) :
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

        basic_svstream(string_view_type const& s)
            : m_buf(s), std::basic_istream<char_type, traits_type>(&m_buf)
        {}

        inline string_view_type all() const
        { return m_buf.all(); }

        inline string_view_type remainder() const
        { return m_buf.remainder(); }

    protected:
        streambuf_type m_buf;
    };

    template <typename C> inline C const* begin(basic_stringview<C> const& s) { return s.begin(); }
    template <typename C> inline C const* end  (basic_stringview<C> const& s) { return s.end(); }

    using stringview    = basic_stringview<char>;
    using stringviewbuf = basic_stringviewbuf<char>;
    using svstream      = basic_svstream<char>;

    inline stringview operator "" _sv(char const* str, size_t sz) noexcept
    {
        return stringview{ str, str+sz };
    }
}


#endif // STRINGVIEW_HPP
