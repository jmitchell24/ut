#pragma once

#include <thread>
#include <chrono>
#include <cassert>

namespace ut
{
    class Timer
    {
    public:
        using count_type                = double;
        using time_point_type           = decltype(std::chrono::high_resolution_clock::now());
        using nanoseconds_type          = std::chrono::duration<count_type, std::nano>;
        using microseconds_type         = std::chrono::duration<count_type, std::micro>;
        using milliseconds_type         = std::chrono::duration<count_type, std::milli>;
        using seconds_type              = std::chrono::duration<count_type>;

        Timer()
            : m_now{getNow()}, m_previous{}
        { }

        inline void reset()
        {
            m_now = getNow();
        }

        inline void next()
        {
            time_point_type now = getNow();
            m_previous = now - m_now;
            m_now = now;
        }

        inline count_type previousSeconds       () const { return m_previous.count(); }
        inline count_type previousMilliseconds  () const { return milliseconds_type{m_previous}.count(); }
        inline count_type previousMicroseconds  () const { return microseconds_type{m_previous}.count(); }
        inline count_type previousNanoseconds   () const { return nanoseconds_type {m_previous}.count(); }

        inline count_type nextSeconds       () { return (next(), previousSeconds()); }
        inline count_type nextMilliseconds  () { return (next(), previousMilliseconds()); }
        inline count_type nextMicroseconds  () { return (next(), previousMicroseconds()); }
        inline count_type nextNanoseconds   () { return (next(), previousNanoseconds()); }

        ///
        /// return elapsed time between now and member timepoint
        ///

        inline count_type seconds       () const { return seconds_type     {getNow() - m_now}.count(); }
        inline count_type milliseconds  () const { return milliseconds_type{getNow() - m_now}.count(); }
        inline count_type microseconds  () const { return microseconds_type{getNow() - m_now}.count(); }
        inline count_type nanoseconds   () const { return nanoseconds_type {getNow() - m_now}.count(); }

        ///
        /// return elapsed time between 't' timepoint and member timepoint
        ///

        inline count_type seconds       (Timer const& t) const { return seconds_type     {t.m_now - m_now}.count(); }
        inline count_type milliseconds  (Timer const& t) const { return milliseconds_type{t.m_now - m_now}.count(); }
        inline count_type microseconds  (Timer const& t) const { return microseconds_type{t.m_now - m_now}.count(); }
        inline count_type nanoseconds   (Timer const& t) const { return nanoseconds_type {t.m_now - m_now}.count(); }

        ///
        /// return elapsed time between now and member timepoint, then reset
        ///

        inline count_type resetSeconds()
        {
            count_type cnt = seconds();
            return (reset(), cnt);
        }

        inline count_type resetMilliseconds()
        {
            count_type cnt = milliseconds();
            return (reset(), cnt);
        }

        inline count_type resetMicroseconds()
        {
            count_type cnt = microseconds();
            return (reset(), cnt);
        }

        inline count_type resetNanoseconds()
        {
            count_type cnt = nanoseconds();
            return (reset(), cnt);
        }

        ///
        /// invoke 'callback' and reset if elapsed time exceeds 'interval'
        ///

        template <typename Callable>
        inline void callbackSeconds(count_type interval, Callable&& callback)
        {
            count_type elapsed = seconds();
            if (elapsed > interval)
                reset(), callback(elapsed);
        }

        template <typename Callable>
        inline void callbackMilliseconds(count_type interval, Callable&& callback)
        {
            count_type elapsed = milliseconds();
            if (elapsed > interval)
                reset(), callback(elapsed);
        }

        template <typename Callable>
        inline void callbackMicroseconds(count_type interval, Callable&& callback)
        {
            count_type elapsed = microseconds();
            if (elapsed > interval)
                reset(), callback(elapsed);
        }

        template <typename Callable>
        inline void callbackNanoseconds(count_type interval, Callable&& callback)
        {
            count_type elapsed = nanoseconds();
            if (elapsed > interval)
                reset(), callback(elapsed);
        }

        ///
        /// Sleep
        ///

        inline static void sleepSeconds(count_type interval)
        { std::this_thread::sleep_for(seconds_type{interval}); }

        inline static void sleepMilliseconds(count_type interval)
        { std::this_thread::sleep_for(milliseconds_type{interval}); }

        inline static void sleepMicroseconds(count_type interval)
        { std::this_thread::sleep_for(microseconds_type{interval}); }

        inline static void sleepNanoseconds(count_type interval)
        { std::this_thread::sleep_for(nanoseconds_type{interval}); }

    private:
        time_point_type     m_now;
        seconds_type        m_previous;

        inline static time_point_type getNow()
        {
            return std::chrono::high_resolution_clock::now();
        }
    };
}
