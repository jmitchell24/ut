#pragma once

#include <thread>
#include <chrono>
#include <cassert>

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline


namespace ut
{
namespace chrono_wrapper
{
    using cnt_t                 = float;
    using time_t                = decltype(std::chrono::high_resolution_clock::now());

    using sec_t                 = std::chrono::duration<cnt_t>;
    using sec_ns_t              = std::chrono::duration<cnt_t, std::nano>;
    using sec_us_t              = std::chrono::duration<cnt_t, std::micro>;
    using sec_ms_t              = std::chrono::duration<cnt_t, std::milli>;

    M_DECL static time_t getnow()
    { return std::chrono::high_resolution_clock::now(); }

    struct duration
    {
        sec_t value;

        M_DECL constexpr static duration seconds       (cnt_t cnt) { return {sec_t     {cnt}   }; }
        M_DECL constexpr static duration milliseconds  (cnt_t cnt) { return {sec_ms_t  {cnt}   }; }
        M_DECL constexpr static duration microseconds  (cnt_t cnt) { return {sec_us_t  {cnt}   }; }
        M_DECL constexpr static duration nanoseconds   (cnt_t cnt) { return {sec_ns_t  {cnt}   }; }

        M_DECL_PURE constexpr cnt_t seconds       () const { return value.count(); }
        M_DECL_PURE constexpr cnt_t milliseconds  () const { return sec_ms_t{value}.count(); }
        M_DECL_PURE constexpr cnt_t microseconds  () const { return sec_us_t{value}.count(); }
        M_DECL_PURE constexpr cnt_t nanoseconds   () const { return sec_ns_t{value}.count(); }

        M_DECL_PURE constexpr duration operator + (duration const& d) const { return duration{value + d.value}; }
        M_DECL_PURE constexpr duration operator - (duration const& d) const { return duration{value - d.value}; }

        M_DECL_PURE constexpr bool operator <  (duration const& d) const { return bool{value <  d.value}; }
        M_DECL_PURE constexpr bool operator >  (duration const& d) const { return bool{value >  d.value}; }
        M_DECL_PURE constexpr bool operator <= (duration const& d) const { return bool{value <= d.value}; }
        M_DECL_PURE constexpr bool operator >= (duration const& d) const { return bool{value >= d.value}; }

        M_DECL constexpr duration& operator += (duration const& d) { value += d.value; return *this; }
        M_DECL constexpr duration& operator -= (duration const& d) { value -= d.value; return *this; }
    };

    /// convenience class for tracking time for profiling and real-time applications (e.g. games)
    class timer
    {
    public:
        struct scope_guard
        {
            scope_guard(scope_guard const&)=delete;
            scope_guard(scope_guard&&)=delete;
            scope_guard& operator= (scope_guard const&)=delete;
            scope_guard& operator= (scope_guard&&)=delete;
            inline explicit scope_guard(duration& dur): m_dur{dur}, m_now{getnow()} {}
            inline ~scope_guard() { m_dur.value = getnow() - m_now; }
        private:
            duration&       m_dur;
            time_t          m_now;
        };

        M_DECL void reset()
        { m_now = getnow(); }

        M_DECL duration next()
        {
            auto now = getnow();
            auto next = peek();
            m_now = now;
            return next;
        }

        M_DECL bool nextIf(duration max_duration)
        {
            if (peek() > max_duration)
            {
                next();
                return true;
            }
            return false;
        }

        M_DECL bool nextIf(duration max_duration, duration& next)
        {
            if (peek() > max_duration)
            {
                next = this->next();
                return true;
            }
            return false;
        }

        M_DECL_PURE duration peek() const
        { return {getnow() - m_now}; }

        //
        // Scope
        //

        M_DECL static scope_guard scope(duration& duration)
        { return scope_guard{duration}; }

        //
        // Sleep
        //

        M_DECL static void sleep(duration const& duration)
        { std::this_thread::sleep_for(duration.value); }

        //
        // Unix Time
        //

        M_DECL_PURE static size_t epoch()
        { return static_cast<size_t>(std::time(nullptr)); }

    private:
        time_t m_now{getnow()};


    };
}

    using timer         = chrono_wrapper::timer;
    using duration      = chrono_wrapper::duration;



    inline constexpr duration operator ""_nanoseconds (long double cnt) { return duration::nanoseconds (cnt); }
    inline constexpr duration operator ""_microseconds(long double cnt) { return duration::microseconds(cnt); }
    inline constexpr duration operator ""_milliseconds(long double cnt) { return duration::milliseconds(cnt); }
    inline constexpr duration operator ""_seconds     (long double cnt) { return duration::seconds     (cnt); }

    inline constexpr duration operator ""_nanoseconds (unsigned long long int cnt) { return duration::nanoseconds (cnt); }
    inline constexpr duration operator ""_microseconds(unsigned long long int cnt) { return duration::microseconds(cnt); }
    inline constexpr duration operator ""_milliseconds(unsigned long long int cnt) { return duration::milliseconds(cnt); }
    inline constexpr duration operator ""_seconds     (unsigned long long int cnt) { return duration::seconds     (cnt); }


}

#undef M_DECL_PURE
#undef M_DECL

