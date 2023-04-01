#ifndef STRING_HPP
#define STRING_HPP

//
// Preprocessor Options
//

//---- Use std::string_view if enabled, otherwise use ut::cstrview
//#define UT_FMT_USE_STL_STRINGVIEW

//---- Specify string buffer size
//#define UT_FMT_BUFFER_SIZE 256

#if !defined(UT_FMT_BUFFER_SIZE)
#define UT_FMT_BUFFER_SIZE 256
#endif

//---- Specify number of string buffers
//#define UT_FMT_BUFFER_COUNT 8

#if !defined(UT_FMT_BUFFER_COUNT)
#define UT_FMT_BUFFER_COUNT 8
#endif

#include <array>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <charconv>

#if defined(UT_FMT_USE_STL_STRINGVIEW)
#include <string_view>
#else
#include "ut/string/view.hpp"
#endif

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline
#define M_ARR                   ( m_buffer[m_index%BUFFER_COUNT].arr )
#define M_SZ                    ( m_buffer[m_index%BUFFER_COUNT].sz )

namespace ut
{
    template <size_t BufferSize, size_t BufferCount>
    class format_ring_buffer
    {
        static_assert(BufferSize > 1, "invalid BufferSize");
        static_assert(BufferCount > 0, "invalid BufferCount");

    public:
        int static constexpr BUFFER_SIZE  = BufferSize;
        int static constexpr BUFFER_COUNT = BufferCount;

        using string_type = std::string;

#if defined(UT_FMT_USE_STL_STRINGVIEW)
        using view_type = std::string_view;
#else
        using view_type = cstrview;
#endif

        struct buf { std::array<char, BUFFER_SIZE> arr; size_t sz; };
        using buffer_container_type = std::array<buf, BUFFER_COUNT>;

        format_ring_buffer()
        {
            for (auto&& it : m_buffer)
            {
                it.sz = 0;
                it.arr[0] = '\0';
            }
        }

        static format_ring_buffer& instance() noexcept;

        M_DECL_PURE char const* data() const { return M_ARR.data(); }
        M_DECL_PURE int         sz  () const { return M_SZ; }

#if defined(UT_FMT_USE_STL_STRINGVIEW)
        M_DECL_PURE view_type view() const
        { return {M_ARR.data(), M_SZ}; }
#else

        M_DECL_PURE view_type view() const
        { return cstrview::make_cstrview(M_ARR.data(), M_SZ); }
#endif

        M_DECL_PURE string_type string() const
        { return string_type{M_ARR.data(), M_ARR.data() + M_SZ}; }

        M_DECL string_type string(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            next(fmt, args);
            va_end(args);

            return string();
        }

        M_DECL string_type stringv(char const* fmt, va_list args)
        {
            next(fmt, args);
            return string();
        }

        M_DECL view_type view(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            next(fmt, args);
            va_end(args);

            return view();
        }

        M_DECL view_type viewv(char const* fmt, va_list args)
        {
            next(fmt, args);
            return view();
        }

        M_DECL view_type operator() (char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            next(fmt, args);
            va_end(args);

            return view();
        }

        //
        // integer format
        //

        M_DECL view_type intView(short     x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(int       x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(long      x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(long long x, int base = 10) { nextInt(x, base); return view(); }

        M_DECL view_type intView(unsigned short     x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(unsigned int       x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(unsigned long      x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(unsigned long long x, int base = 10) { nextInt(x, base); return view(); }

        M_DECL string_type intString(short     x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(int       x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(long      x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(long long x, int base = 10) { nextInt(x, base); return string(); }

        M_DECL string_type intString(unsigned short     x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(unsigned int       x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(unsigned long      x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(unsigned long long x, int base = 10) { nextInt(x, base); return string(); }

        //
        // float format
        //

        M_DECL view_type floatView(float        x, int precision) { nextFloat(x, precision); return view(); }
        M_DECL view_type floatView(double       x, int precision) { nextFloat(x, precision); return view(); }
        M_DECL view_type floatView(long double  x, int precision) { nextFloat(x, precision); return view(); }

        M_DECL view_type floatView(float        x) { nextFloat(x); return view(); }
        M_DECL view_type floatView(double       x) { nextFloat(x); return view(); }
        M_DECL view_type floatView(long double  x) { nextFloat(x); return view(); }

        M_DECL string_type floatString(float        x, int precision) { nextFloat(x, precision); return string(); }
        M_DECL string_type floatString(double       x, int precision) { nextFloat(x, precision); return string(); }
        M_DECL string_type floatString(long double  x, int precision) { nextFloat(x, precision); return string(); }

        M_DECL string_type floatString(float        x) { nextFloat(x); return string(); }
        M_DECL string_type floatString(double       x) { nextFloat(x); return string(); }
        M_DECL string_type floatString(long double  x) { nextFloat(x); return string(); }

    private:
        buffer_container_type   m_buffer    { };
        size_t                  m_index     {0};

        M_DECL void reset()
        {
            M_SZ = 0;
            M_ARR[0] = '\0';
        }

        M_DECL void incrementIndex()
        {
            ++m_index;
            m_index = (m_index % BUFFER_COUNT + BUFFER_COUNT) % BUFFER_COUNT;
        }

        M_DECL void next(char const* fmt, va_list args)
        {
            incrementIndex();

            if (int res = vsnprintf(M_ARR.data(), M_ARR.size(), fmt, args); res >= 0 && res < BUFFER_SIZE)
                M_SZ = res;
            else
                reset();
        }

        template <typename T>
        M_DECL void nextInt(T x, int base)
        {
            incrementIndex();

            char* first = M_ARR.data();
            char* last  = M_ARR.data() + M_ARR.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x, base); ec == std::errc())
                M_SZ = std::distance(first, ptr);
            else
                reset();
        }

        template <typename T>
        M_DECL void nextFloat(T x, int precision)
        {
            incrementIndex();

            char* first = M_ARR.data();
            char* last  = M_ARR.data() + M_ARR.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x, std::chars_format::fixed, precision); ec == std::errc())
                M_SZ = std::distance(first, ptr);
            else
                reset();
        }

        template <typename T>
        M_DECL void nextFloat(T x)
        {
            incrementIndex();

            char* first = M_ARR.data();
            char* last  = M_ARR.data() + M_ARR.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x); ec == std::errc())
                M_SZ = std::distance(first, ptr);
            else
                reset();
        }


    };

    template <size_t BufferSize, size_t BufferCount>
    format_ring_buffer<BufferSize, BufferCount>& format_ring_buffer<BufferSize, BufferCount>::instance() noexcept
    {
        static thread_local format_ring_buffer<BufferSize, BufferCount> x;
        return x;
    }

    template<size_t Size>
    using format_buffer = format_ring_buffer<Size, 1>;

    using  fbuf_t = format_ring_buffer<UT_FMT_BUFFER_SIZE, 1>;
    using rfbuf_t = format_ring_buffer<UT_FMT_BUFFER_SIZE, UT_FMT_BUFFER_COUNT>;

    /// Default instance of format_ring_buffer.
    /// Stores previously formatted strings in a ring buffer so that subsequent
    /// format results remain available through the returned view objects.
    [[maybe_unused]] static rfbuf_t& FRB = rfbuf_t::instance();
}

#undef M_DECL_PURE
#undef M_DECL
#undef M_ARR
#undef M_SZ

#endif // STRING_HPP
