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

            M_DECL_PURE count_type seconds       () const { return value.count(); }
            M_DECL_PURE count_type milliseconds  () const { return milliseconds_type{value}.count(); }
            M_DECL_PURE count_type microseconds  () const { return microseconds_type{value}.count(); }
            M_DECL_PURE count_type nanoseconds   () const { return nanoseconds_type {value}.count(); }

            M_DECL static duration seconds       (count_type cnt) { return { seconds_type       {cnt}   }; }
            M_DECL static duration milliseconds  (count_type cnt) { return { milliseconds_type  {cnt}   }; }
            M_DECL static duration microseconds  (count_type cnt) { return { microseconds_type  {cnt}   }; }
            M_DECL static duration nanoseconds   (count_type cnt) { return { nanoseconds_type   {cnt}   }; }
        };

        struct scope_guard
        {
            scope_guard(scope_guard const&)=delete;
            scope_guard(scope_guard&&)=delete;
            scope_guard& operator= (scope_guard const&)=delete;
            scope_guard& operator= (scope_guard&&)=delete;
            inline explicit scope_guard(timer& timer): m_timer{timer} { m_timer.reset(); }
            inline ~scope_guard() { m_timer.next(); }
        private:
            timer& m_timer;
        };

        M_DECL timer()
            : m_now{getNow()}, m_previous{}
        { }

        M_DECL void reset()
        {
            m_now = getNow();
        }

        M_DECL duration next()
        {
            time_point_type now = getNow();
            m_previous.value = now - m_now;
            m_now = now;
            return m_previous;
        }

        M_DECL scope_guard scoped() { return scope_guard{*this}; }

        M_DECL_PURE duration   previous              () const { return m_previous; }
        M_DECL_PURE count_type previousSeconds       () const { return m_previous.seconds(); }
        M_DECL_PURE count_type previousMilliseconds  () const { return m_previous.milliseconds(); }
        M_DECL_PURE count_type previousMicroseconds  () const { return m_previous.microseconds(); }
        M_DECL_PURE count_type previousNanoseconds   () const { return m_previous.nanoseconds(); }

        M_DECL count_type nextSeconds       () { return next().seconds(); }
        M_DECL count_type nextMilliseconds  () { return next().milliseconds(); }
        M_DECL count_type nextMicroseconds  () { return next().microseconds(); }
        M_DECL count_type nextNanoseconds   () { return next().nanoseconds(); }

        //
        // return elapsed time between now and member timepoint
        //

        M_DECL_PURE count_type seconds       () const { return seconds_type     {getNow() - m_now}.count(); }
        M_DECL_PURE count_type milliseconds  () const { return milliseconds_type{getNow() - m_now}.count(); }
        M_DECL_PURE count_type microseconds  () const { return microseconds_type{getNow() - m_now}.count(); }
        M_DECL_PURE count_type nanoseconds   () const { return nanoseconds_type {getNow() - m_now}.count(); }

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
        time_point_type m_now;
        duration        m_previous;

        M_DECL static time_point_type getNow()
        {
            return std::chrono::high_resolution_clock::now();
        }
    };
}

#undef M_DECL_PURE
#undef M_DECL

