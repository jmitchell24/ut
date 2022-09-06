#pragma once

#include "ut/string/view.hpp"

namespace ut
{
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

    using strview_buf       = basic_strview_buf<char>;
    using strview_stream    = basic_strview_stream<char>;
}

//namespace std
//{
//    template<class Key> struct hash;
//
//    template <typename Char, typename Traits, bool NullTerminated>
//    struct hash<ut::basic_strview<Char,Traits,NullTerminated>>
//    {
//        size_t operator()(ut::basic_strview<Char,Traits,NullTerminated> const& k) const
//        {
//            using string_view_type = typename ut::basic_strview<Char,Traits,NullTerminated>::string_view_type;
//            return hash<string_view_type>{}(k.view());
//        }
//    };
//}