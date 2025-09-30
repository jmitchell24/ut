#pragma once

//
// Preprocessor Options
//

//---- Use std::string_view if enabled, otherwise use ut::cstrview
//#define UT_PRINTER_USE_STL_STRINGVIEW

//---- Specify string buffer size
//#define UT_PRINTER_BUFFER_SIZE 256

#if !defined(UT_PRINTER_BUFFER_SIZE)
#define UT_PRINTER_BUFFER_SIZE 256
#endif

//---- Specify number of string buffers
//#define UT_PRINTER_BUFFER_COUNT 8

#if !defined(UT_PRINTER_BUFFER_COUNT)
#define UT_PRINTER_BUFFER_COUNT 8
#endif

#include <array>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <charconv>

#if defined(UT_PRINTER_USE_STL_STRINGVIEW)
#include <string_view>
#else
#include "ut/string/view.hpp"
#endif

#if defined(UT_PRINTER_USE_FMT)
#include <fmt/core.h>
#include <fmt/ostream.h>
#endif

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline
#define M_ARR                   ( m_buffer[m_index%BUFFER_COUNT].arr )
#define M_SZ                    ( m_buffer[m_index%BUFFER_COUNT].sz )

/// Default instance of format_ring_buffer.
/// Stores previously formatted strings in a ring buffer so that subsequent
/// format results remain available through the returned view objects.
#define ut_printer ( ut::printer_t::instance() )

namespace ut
{
    template <size_t BufferSize, size_t BufferCount>
    class basic_printer
    {
        static_assert(BufferSize > 1, "invalid BufferSize");
        static_assert(BufferCount > 0, "invalid BufferCount");

    public:
        int static constexpr BUFFER_SIZE  = BufferSize;
        int static constexpr BUFFER_COUNT = BufferCount;

        using string_type = std::string;

#if defined(UT_PRINTER_USE_STL_STRINGVIEW)
        using view_type = std::string_view;
#else
        using view_type = cstrview;
#endif

        struct buf { std::array<char, BUFFER_SIZE> arr; size_t sz; };
        using buffer_container_type = std::array<buf, BUFFER_COUNT>;

        basic_printer()
        {
            for (auto&& it : m_buffer)
            {
                it.sz = 0;
                it.arr[0] = '\0';
            }
        }

        static basic_printer& instance() noexcept;

        M_DECL_PURE char const* data() const { return M_ARR.data(); }
        M_DECL_PURE int         sz  () const { return M_SZ; }

#if defined(UT_PRINTER_USE_STL_STRINGVIEW)
        M_DECL_PURE view_type lastView() const
        { return {M_ARR.data(), M_SZ}; }

        M_DECL view_type copy(std::string_view const& s)
        {
            nextCopy(s.data(), s.size());
            return view();
        }
#else

        M_DECL_PURE view_type lastView() const
        { return cstrview::make_cstrview(M_ARR.data(), M_SZ); }

        M_DECL view_type copy(ut::strparam s)
        {
            nextCopy(s.data(), s.size());
            return lastView();
        }
#endif

#if defined(UT_PRINTER_USE_FMT)
        template <typename... T>
        M_DECL view_type fmt(fmt::string_view format_str, T&&... args)
        {
            incrementIndex();
            M_SZ = ::fmt::vformat_to_n(M_ARR.data(), BUFFER_SIZE-1,
                format_str, fmt::make_format_args(std::forward<T>(args)...)).size;
            return lastView();
        }
#endif

        M_DECL view_type operator() (char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            next(fmt, args);
            va_end(args);

            return lastView();
        }

        //
        // integer format
        //

        M_DECL view_type intView(short     x, int base = 10) { nextInt(x, base); return lastView(); }
        M_DECL view_type intView(int       x, int base = 10) { nextInt(x, base); return lastView(); }
        M_DECL view_type intView(long      x, int base = 10) { nextInt(x, base); return lastView(); }
        M_DECL view_type intView(long long x, int base = 10) { nextInt(x, base); return lastView(); }

        M_DECL view_type intView(unsigned short     x, int base = 10) { nextInt(x, base); return lastView(); }
        M_DECL view_type intView(unsigned int       x, int base = 10) { nextInt(x, base); return lastView(); }
        M_DECL view_type intView(unsigned long      x, int base = 10) { nextInt(x, base); return lastView(); }
        M_DECL view_type intView(unsigned long long x, int base = 10) { nextInt(x, base); return lastView(); }

        //
        // float format
        //

        M_DECL view_type floatView(float        x, int precision) { nextFloat(x, precision); return lastView(); }
        M_DECL view_type floatView(double       x, int precision) { nextFloat(x, precision); return lastView(); }
        M_DECL view_type floatView(long double  x, int precision) { nextFloat(x, precision); return lastView(); }

        M_DECL view_type floatView(float        x) { nextFloat(x); return lastView(); }
        M_DECL view_type floatView(double       x) { nextFloat(x); return lastView(); }
        M_DECL view_type floatView(long double  x) { nextFloat(x); return lastView(); }

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

        M_DECL void nextCopy(char const* src, size_t n)
        {
            incrementIndex();

            if (n < BUFFER_SIZE-1)
            {
                M_SZ        = n;
                M_ARR[M_SZ] = '\0';
                ::memcpy(M_ARR.data(), src, M_SZ * sizeof(char));
            }
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
    basic_printer<BufferSize, BufferCount>& basic_printer<BufferSize, BufferCount>::instance() noexcept
    {
        static thread_local basic_printer<BufferSize, BufferCount> x;
        return x;
    }

    using printer_t = basic_printer<UT_PRINTER_BUFFER_SIZE, UT_PRINTER_BUFFER_COUNT>;
}

#undef M_DECL_PURE
#undef M_DECL
#undef M_ARR
#undef M_SZ