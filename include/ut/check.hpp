//
// Created by james on 9/28/23.
//


#pragma once

#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <cstdlib>


#ifdef NDEBUG
#define ut_check_msg(...)          (void)0;
#define ut_constexpr_check(x_)     (void)0;
#define ut_nopath(...)             (void)0;
#define ut_nopath_case(x_)         (void)0;
#else
#define ut_check_msg(x_, ...)  do { ::ut::internal::check_expression(x_, __LINE__, #x_, __func__, __FILE__, __VA_ARGS__); }while(0)
#define ut_nopath_msg(...)     do { ::ut::internal::check_impossible(__LINE__, __func__, __FILE__, __VA_ARGS__); }while(0)
#define ut_nopath_case(x_)     do { static_assert(std::is_enum_v<x_>); ::ut::internal::check_impossible(__LINE__, __func__, __FILE__, "BAD ENUM CASE: %s", #x_); }while(0)
#endif

// constexpr-compatible check: no-op at compile time, full check at runtime
#if __cplusplus >= 202002L // C++20 constexpr support
#include <type_traits>
#define ut_constexpr_check(x_) \
    do { if constexpr (!std::is_constant_evaluated()) \
        { ut_check_msg(x_, "SILENT CHECK"); } \
        else \
        { if ( (x_) == false ) throw std::runtime_error("SILENT CHECK"); } \
    } while(0)
#else
#define ut_constexpr_check(x_)     ut_check_msg(x_, "SILENT CHECK")
#endif

#undef HAS_IS_CONSTANT_EVALUATED

#define ut_check(x_)           ut_check_msg(x_, "SILENT CHECK")
#define ut_check_null(x_)      ut_check_msg((x_) != nullptr, "NULL POINTER")

#define ut_nopath_impl         ut_nopath_msg("UNIMPLEMENTED")
#define ut_nopath_break        ut_nopath_msg("BREAKPOINT")
#define ut_nopath              ut_nopath_msg("IMPOSSIBLE");

#ifndef UT_CHECK_PREFIXED_ONLY
#define check_msg          ut_check_msg
#define constexpr_check    ut_constexpr_check
#define nopath             ut_nopath
#define nopath_case        ut_nopath_case
#define check              ut_check
#define check_null         ut_check_null
#define nopath_msg         ut_nopath_msg
#define nopath_impl        ut_nopath_impl
#define nopath_break       ut_nopath_break
#define nopath             ut_nopath
#endif

namespace ut::internal
{
    [[noreturn]] static inline void check_impl(int line, char const* msg, char const* expr, char const* func, char const* file)
    {
        bool m=msg != nullptr, e=expr != nullptr;

        if (m && e)
        {
            fprintf(stderr, "\nASSERTION : %s"
                            "\nEXPR      : %s"
                            "\nFILE      : %s:%d"
                            "\nFUNC      : %s",
                    msg, expr, file, line, func);

        }
        else if (!m && !e)
        {
            fprintf(stderr, "\nASSERTION"
                            "\nFILE      : %s:%d"
                            "\nFUNC      : %s",
                    file, line, func);
        }
        else
        {
            fprintf(stderr, "\nASSERTION : %s"
                            "\nFILE      : %s:%d"
                            "\nFUNC      : %s",
                    m ? msg : expr, file, line, func);
        }

        fflush(stderr);
        abort();
    }

    static inline void check_expression(bool b, int line, char const* expr, char const* func, char const* file, char const* fmt, ...)
    {
        if (!b)
        {
            size_t constexpr MAX_LEN = 512;
            static char buffer[MAX_LEN];

            va_list args;
            va_start(args, fmt);
            bool has_msg = vsnprintf(buffer, MAX_LEN, fmt, args) > 0;
            va_end(args);

            check_impl(line, has_msg ? buffer : nullptr, expr, func, file);
        }
    }

    static inline void check_impossible(int line, char const* func, char const* file, char const* fmt, ...)
    {
        size_t constexpr MAX_LEN = 512;
        static char buffer[MAX_LEN];

        va_list args;
        va_start(args, fmt);
        bool has_msg = vsnprintf(buffer, MAX_LEN, fmt, args) > 0;
        va_end(args);

        check_impl(line, has_msg ? buffer : nullptr, nullptr, func, file);
    }

}
