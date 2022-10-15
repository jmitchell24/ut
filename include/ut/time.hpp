#pragma once

#include <thread>
#include <chrono>
#include <cassert>

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline


namespace ut
{
    /// convenience class for tracking time for profiling and real-time applications (e.g. games)
    class timer
    {
    public:
        using count_type                = float;
        using time_point_type           = decltype(std::chrono::high_resolution_clock::now());
        using nanoseconds_type          = std::chrono::duration<count_type, std::nano>;
        using microseconds_type         = std::chrono::duration<count_type, std::micro>;
        using milliseconds_type         = std::chrono::duration<count_type, std::milli>;
        using seconds_type              = std::chrono::duration<count_type>;

        struct duration
        {
            seconds_type value;

            M_DECL constexpr static duration seconds       (count_type cnt) { return {seconds_type       {cnt}   }; }
            M_DECL constexpr static duration milliseconds  (count_type cnt) { return {milliseconds_type  {cnt}   }; }
            M_DECL constexpr static duration microseconds  (count_type cnt) { return {microseconds_type  {cnt}   }; }
            M_DECL constexpr static duration nanoseconds   (count_type cnt) { return {nanoseconds_type   {cnt}   }; }

            M_DECL_PURE constexpr count_type seconds       () const { return value.count(); }
            M_DECL_PURE constexpr count_type milliseconds  () const { return milliseconds_type{value}.count(); }
            M_DECL_PURE constexpr count_type microseconds  () const { return microseconds_type{value}.count(); }
            M_DECL_PURE constexpr count_type nanoseconds   () const { return nanoseconds_type {value}.count(); }

            M_DECL_PURE constexpr duration operator + (duration const& d) const { return duration{value + d.value}; }
            M_DECL_PURE constexpr duration operator - (duration const& d) const { return duration{value - d.value}; }

            M_DECL_PURE constexpr bool operator <  (duration const& d) const { return bool{value <  d.value}; }
            M_DECL_PURE constexpr bool operator >  (duration const& d) const { return bool{value >  d.value}; }
            M_DECL_PURE constexpr bool operator <= (duration const& d) const { return bool{value <= d.value}; }
            M_DECL_PURE constexpr bool operator >= (duration const& d) const { return bool{value >= d.value}; }

            M_DECL constexpr duration& operator += (duration const& d) { value += d.value; return *this; }
            M_DECL constexpr duration& operator -= (duration const& d) { value -= d.value; return *this; }
        };

        struct scope_guard
        {
            scope_guard(scope_guard const&)=delete;
            scope_guard(scope_guard&&)=delete;
            scope_guard& operator= (scope_guard const&)=delete;
            scope_guard& operator= (scope_guard&&)=delete;
            inline explicit scope_guard(duration& dur): m_dur{dur}, m_now{getNow()} {}
            inline ~scope_guard() { m_dur.value = getNow() - m_now; }
        private:
            duration&       m_dur;
            time_point_type m_now;
        };

        M_DECL void reset()
        { m_now = getNow(); }

        M_DECL duration next()
        {
            auto now = getNow();
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
        { return {getNow() - m_now}; }

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

    private:
        time_point_type m_now{getNow()};

        M_DECL static time_point_type getNow()
        { return std::chrono::high_resolution_clock::now(); }
    };

    inline constexpr timer::duration operator ""_nanoseconds (long double cnt) { return timer::duration::nanoseconds (cnt); }
    inline constexpr timer::duration operator ""_microseconds(long double cnt) { return timer::duration::microseconds(cnt); }
    inline constexpr timer::duration operator ""_milliseconds(long double cnt) { return timer::duration::milliseconds(cnt); }
    inline constexpr timer::duration operator ""_seconds     (long double cnt) { return timer::duration::seconds     (cnt); }

    inline constexpr timer::duration operator ""_nanoseconds (unsigned long long int cnt) { return timer::duration::nanoseconds (cnt); }
    inline constexpr timer::duration operator ""_microseconds(unsigned long long int cnt) { return timer::duration::microseconds(cnt); }
    inline constexpr timer::duration operator ""_milliseconds(unsigned long long int cnt) { return timer::duration::milliseconds(cnt); }
    inline constexpr timer::duration operator ""_seconds     (unsigned long long int cnt) { return timer::duration::seconds     (cnt); }


}

#undef M_DECL_PURE
#undef M_DECL

