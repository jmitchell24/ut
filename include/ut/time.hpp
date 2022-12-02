#pragma once

#include <thread>
#include <chrono>
#include <cassert>
#include <string>

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline


namespace ut
{
namespace chrono_wrapper
{
    using cnt_t                 = float;
    using now_t                 = decltype(std::chrono::high_resolution_clock::now());
    using epoch_t               = std::size_t;

    using sec_t                 = std::chrono::duration<cnt_t>;
    using sec_ns_t              = std::chrono::duration<cnt_t, std::nano>;
    using sec_us_t              = std::chrono::duration<cnt_t, std::micro>;
    using sec_ms_t              = std::chrono::duration<cnt_t, std::milli>;

    M_DECL static now_t getnow()
    { return std::chrono::high_resolution_clock::now(); }

    template <bool IsUTC>
    struct datetime
    {
        enum Month_ { JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };
        using Month = int;

        enum Weekday_ { MON, TUE, WED, THU, FRI, SAT, SUN };
        using Weekday = int;

        constexpr static char const* FMT_ISO8601 = "%Y-%m-%dT%H:%M:%SZ";
        constexpr static bool IS_UTC = IsUTC;

        ::tm tm;

        datetime()
            : tm{nowtm()}
        { }

        datetime(datetime const&)=default;

#define TM_GET(_mem, _min, _max) (  \
        assert( (_mem) >= (_min) ), \
        assert( (_mem) >= (_max) ), \
        (_mem) )                    \

        M_DECL_PURE int     seconds     () const { return TM_GET(tm.tm_sec  , 0, 61 ); }
        M_DECL_PURE int     minutes     () const { return TM_GET(tm.tm_min  , 0, 59 ); }
        M_DECL_PURE int     hours       () const { return TM_GET(tm.tm_hour , 0, 23 ); }
        M_DECL_PURE int     dayOfMonth  () const { return TM_GET(tm.tm_mday , 1, 31 ); }
        M_DECL_PURE Weekday dayOfWeek   () const { return TM_GET(tm.tm_wday , 0, 6  ); }
        M_DECL_PURE int     dayOfYear   () const { return TM_GET(tm.tm_yday , 0, 365); }
        M_DECL_PURE Month   month       () const { return TM_GET(tm.tm_mon  , 0, 11 ); }
        M_DECL_PURE int     year        () const { return tm.tm_year + 1900; }

#undef TM_GET

        /**
            %a     The abbreviated name of the day of the week according to
                   the current locale.  (Calculated from tm_wday.)  (The
                   specific names used in the current locale can be obtained
                   by calling nl_langinfo(3) with ABDAY_{1–7} as an
                   argument.)

            %A     The full name of the day of the week according to the
                   current locale.  (Calculated from tm_wday.)  (The specific
                   names used in the current locale can be obtained by
                   calling nl_langinfo(3) with DAY_{1–7} as an argument.)

            %b     The abbreviated month name according to the current
                   locale.  (Calculated from tm_mon.)  (The specific names
                   used in the current locale can be obtained by calling
                   nl_langinfo(3) with ABMON_{1–12} as an argument.)

            %B     The full month name according to the current locale.
                   (Calculated from tm_mon.)  (The specific names used in the
                   current locale can be obtained by calling nl_langinfo(3)
                   with MON_{1–12} as an argument.)

            %c     The preferred date and time representation for the current
                   locale.  (The specific format used in the current locale
                   can be obtained by calling nl_langinfo(3) with D_T_FMT as
                   an argument for the %c conversion specification, and with
                   ERA_D_T_FMT for the %Ec conversion specification.)  (In
                   the POSIX locale this is equivalent to %a %b %e %H:%M:%S
                   %Y.)

            %C     The century number (year/100) as a 2-digit integer. (SU)
                   (The %EC conversion specification corresponds to the name
                   of the era.)  (Calculated from tm_year.)

            %d     The day of the month as a decimal number (range 01 to 31).
                   (Calculated from tm_mday.)

            %D     Equivalent to %m/%d/%y.  (Yecch—for Americans only.
                   Americans should note that in other countries %d/%m/%y is
                   rather common.  This means that in international context
                   this format is ambiguous and should not be used.) (SU)

            %e     Like %d, the day of the month as a decimal number, but a
                   leading zero is replaced by a space. (SU) (Calculated from
                   tm_mday.)

            %E     Modifier: use alternative ("era-based") format, see below.
                   (SU)

            %F     Equivalent to %Y-%m-%d (the ISO 8601 date format). (C99)

            %G     The ISO 8601 week-based year (see NOTES) with century as a
                   decimal number.  The 4-digit year corresponding to the ISO
                   week number (see %V).  This has the same format and value
                   as %Y, except that if the ISO week number belongs to the
                   previous or next year, that year is used instead. (TZ)
                   (Calculated from tm_year, tm_yday, and tm_wday.)

            %g     Like %G, but without century, that is, with a 2-digit year
                   (00–99). (TZ) (Calculated from tm_year, tm_yday, and
                   tm_wday.)

            %h     Equivalent to %b.  (SU)

            %H     The hour as a decimal number using a 24-hour clock (range
                   00 to 23).  (Calculated from tm_hour.)

            %I     The hour as a decimal number using a 12-hour clock (range
                   01 to 12).  (Calculated from tm_hour.)

            %j     The day of the year as a decimal number (range 001 to
                   366).  (Calculated from tm_yday.)

            %k     The hour (24-hour clock) as a decimal number (range 0 to
                   23); single digits are preceded by a blank.  (See also
                   %H.)  (Calculated from tm_hour.)  (TZ)

            %l     The hour (12-hour clock) as a decimal number (range 1 to
                   12); single digits are preceded by a blank.  (See also
                   %I.)  (Calculated from tm_hour.)  (TZ)

            %m     The month as a decimal number (range 01 to 12).
                   (Calculated from tm_mon.)

            %M     The minute as a decimal number (range 00 to 59).
                   (Calculated from tm_min.)

            %n     A newline character. (SU)

            %O     Modifier: use alternative numeric symbols, see below. (SU)

            %p     Either "AM" or "PM" according to the given time value, or
                   the corresponding strings for the current locale.  Noon is
                   treated as "PM" and midnight as "AM".  (Calculated from
                   tm_hour.)  (The specific string representations used for
                   "AM" and "PM" in the current locale can be obtained by
                   calling nl_langinfo(3) with AM_STR and PM_STR,
                   respectively.)

            %P     Like %p but in lowercase: "am" or "pm" or a corresponding
                   string for the current locale.  (Calculated from tm_hour.)
                   (GNU)

            %r     The time in a.m. or p.m. notation.  (SU) (The specific
                   format used in the current locale can be obtained by
                   calling nl_langinfo(3) with T_FMT_AMPM as an argument.)
                   (In the POSIX locale this is equivalent to %I:%M:%S %p.)

            %R     The time in 24-hour notation (%H:%M).  (SU) For a version
                   including the seconds, see %T below.

            %s     The number of seconds since the Epoch, 1970-01-01 00:00:00
                   +0000 (UTC). (TZ) (Calculated from mktime(tm).)

            %S     The second as a decimal number (range 00 to 60).  (The
                   range is up to 60 to allow for occasional leap seconds.)
                   (Calculated from tm_sec.)

            %t     A tab character. (SU)

            %T     The time in 24-hour notation (%H:%M:%S).  (SU)

            %u     The day of the week as a decimal, range 1 to 7, Monday
                   being 1.  See also %w.  (Calculated from tm_wday.)  (SU)

            %U     The week number of the current year as a decimal number,
                   range 00 to 53, starting with the first Sunday as the
                   first day of week 01.  See also %V and %W.  (Calculated
                   from tm_yday and tm_wday.)

            %V     The ISO 8601 week number (see NOTES) of the current year
                   as a decimal number, range 01 to 53, where week 1 is the
                   first week that has at least 4 days in the new year.  See
                   also %U and %W.  (Calculated from tm_year, tm_yday, and
                   tm_wday.)  (SU)

            %w     The day of the week as a decimal, range 0 to 6, Sunday
                   being 0.  See also %u.  (Calculated from tm_wday.)

            %W     The week number of the current year as a decimal number,
                   range 00 to 53, starting with the first Monday as the
                   first day of week 01.  (Calculated from tm_yday and
                   tm_wday.)

            %x     The preferred date representation for the current locale
                   without the time.  (The specific format used in the
                   current locale can be obtained by calling nl_langinfo(3)
                   with D_FMT as an argument for the %x conversion
                   specification, and with ERA_D_FMT for the %Ex conversion
                   specification.)  (In the POSIX locale this is equivalent
                   to %m/%d/%y.)

            %X     The preferred time representation for the current locale
                   without the date.  (The specific format used in the
                   current locale can be obtained by calling nl_langinfo(3)
                   with T_FMT as an argument for the %X conversion
                   specification, and with ERA_T_FMT for the %EX conversion
                   specification.)  (In the POSIX locale this is equivalent
                   to %H:%M:%S.)

            %y     The year as a decimal number without a century (range 00
                   to 99).  (The %Ey conversion specification corresponds to
                   the year since the beginning of the era denoted by the %EC
                   conversion specification.)  (Calculated from tm_year)

            %Y     The year as a decimal number including the century.  (The
                   %EY conversion specification corresponds to the full
                   alternative year representation.)  (Calculated from
                   tm_year)

            %z     The +hhmm or -hhmm numeric timezone (that is, the hour and
                   minute offset from UTC). (SU)

            %Z     The timezone name or abbreviation.

            %+     The date and time in date(1) format. (TZ) (Not supported
                   in glibc2.)

            %%     A literal '%' character.
          */
        M_DECL_PURE std::string str(char const* fmt = FMT_ISO8601, size_t max_size = 256) const
        {
            std::string s(max_size, '\0');
            if (int res = strftime(s.data(), s.size(), fmt, &tm); res != 0)
            {
                s.resize(res);
                return s;
            }

            return std::string{};
        }

        M_DECL static datetime ofEpoch(epoch_t epoch)
        {
            return datetime{epoch};
        }

    private:
        datetime(epoch_t epoch)
            : tm{nowtm(epoch)}
        {}

        static ::tm nowtm()
        {
            std::time_t t;
            std::time(&t);
            return nowtm(t);
        }

        static ::tm nowtm(std::time_t t)
        {
            if constexpr (IsUTC)
                return *gmtime(&t);
            else
                return *localtime(&t);
        }
    };

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
            now_t           m_now;
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

        M_DECL_PURE static epoch_t epoch()
        { return static_cast<epoch_t>(std::time(nullptr)); }

    private:
        now_t m_now{getnow()};


    };
}

    using epoch_t           = chrono_wrapper::epoch_t;
    using timer             = chrono_wrapper::timer;
    using duration          = chrono_wrapper::duration;
    using utc_datetime      = chrono_wrapper::datetime<true>;
    using local_datetime    = chrono_wrapper::datetime<false>;
    using datetime          = utc_datetime;


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

