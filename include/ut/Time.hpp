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

        M_DECL timer()
            : m_now{getNow()}, m_previous{}
        { }

        M_DECL void reset()
        {
            m_now = getNow();
        }

        M_DECL void next()
        {
            time_point_type now = getNow();
            m_previous = now - m_now;
            m_now = now;
        }

        M_DECL_PURE count_type previousSeconds       () const { return m_previous.count(); }
        M_DECL_PURE count_type previousMilliseconds  () const { return milliseconds_type{m_previous}.count(); }
        M_DECL_PURE count_type previousMicroseconds  () const { return microseconds_type{m_previous}.count(); }
        M_DECL_PURE count_type previousNanoseconds   () const { return nanoseconds_type {m_previous}.count(); }

        M_DECL count_type nextSeconds       () { return (next(), previousSeconds()); }
        M_DECL count_type nextMilliseconds  () { return (next(), previousMilliseconds()); }
        M_DECL count_type nextMicroseconds  () { return (next(), previousMicroseconds()); }
        M_DECL count_type nextNanoseconds   () { return (next(), previousNanoseconds()); }

        //
        // return elapsed time between now and member timepoint
        //

        M_DECL_PURE count_type seconds       () const { return seconds_type     {getNow() - m_now}.count(); }
        M_DECL_PURE count_type milliseconds  () const { return milliseconds_type{getNow() - m_now}.count(); }
        M_DECL_PURE count_type microseconds  () const { return microseconds_type{getNow() - m_now}.count(); }
        M_DECL_PURE count_type nanoseconds   () const { return nanoseconds_type {getNow() - m_now}.count(); }

        //
        // return elapsed time between 't' timepoint and member timepoint
        //

        M_DECL count_type seconds       (timer const& t) const { return seconds_type     {t.m_now - m_now}.count(); }
        M_DECL count_type milliseconds  (timer const& t) const { return milliseconds_type{t.m_now - m_now}.count(); }
        M_DECL count_type microseconds  (timer const& t) const { return microseconds_type{t.m_now - m_now}.count(); }
        M_DECL count_type nanoseconds   (timer const& t) const { return nanoseconds_type {t.m_now - m_now}.count(); }

        //
        // return elapsed time between now and member timepoint, then reset
        //

        M_DECL count_type resetSeconds()
        {
            count_type cnt = seconds();
            return (reset(), cnt);
        }

        M_DECL count_type resetMilliseconds()
        {
            count_type cnt = milliseconds();
            return (reset(), cnt);
        }

        M_DECL count_type resetMicroseconds()
        {
            count_type cnt = microseconds();
            return (reset(), cnt);
        }

        M_DECL count_type resetNanoseconds()
        {
            count_type cnt = nanoseconds();
            return (reset(), cnt);
        }

        //
        // invoke 'callback' and reset if elapsed time exceeds 'interval'
        //

        template <typename Callable>
        M_DECL void callbackSeconds(count_type interval, Callable&& callback)
        {
            count_type elapsed = seconds();
            if (elapsed > interval)
                reset(), callback(elapsed);
        }

        template <typename Callable>
        M_DECL void callbackMilliseconds(count_type interval, Callable&& callback)
        {
            count_type elapsed = milliseconds();
            if (elapsed > interval)
                reset(), callback(elapsed);
        }

        template <typename Callable>
        M_DECL void callbackMicroseconds(count_type interval, Callable&& callback)
        {
            count_type elapsed = microseconds();
            if (elapsed > interval)
                reset(), callback(elapsed);
        }

        template <typename Callable>
        M_DECL void callbackNanoseconds(count_type interval, Callable&& callback)
        {
            count_type elapsed = nanoseconds();
            if (elapsed > interval)
                reset(), callback(elapsed);
        }

        //
        // Sleep
        //

        M_DECL static void sleepSeconds(count_type interval)
        { std::this_thread::sleep_for(seconds_type{interval}); }

        M_DECL static void sleepMilliseconds(count_type interval)
        { std::this_thread::sleep_for(milliseconds_type{interval}); }

        M_DECL static void sleepMicroseconds(count_type interval)
        { std::this_thread::sleep_for(microseconds_type{interval}); }

        M_DECL static void sleepNanoseconds(count_type interval)
        { std::this_thread::sleep_for(nanoseconds_type{interval}); }

    private:
        time_point_type     m_now;
        seconds_type        m_previous;

        M_DECL static time_point_type getNow()
        {
            return std::chrono::high_resolution_clock::now();
        }
    };
}

#undef M_DECL_PURE
#undef M_DECL

