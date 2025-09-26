#pragma once

#include "ut/string/printer.hpp"
#include "ut/string/metric.hpp"
#include "ut/string/builder.hpp"
#include "ut/string/view.hpp"
#include "ut/string/view_istream.hpp"

namespace ut
{

namespace strio
{
    struct print_wrap
    {
        strparam    str;
        size_t      limit;
        char const* prefix;
        char const* suffix;
    };

    static std::ostream& operator<< (std::ostream& os, print_wrap const& pw)
    {
        auto limit = pw.limit;
        auto s = pw.str;
        auto prefix = pw.prefix;
        auto suffix = pw.suffix;


        if (limit > 0 && !s.empty())
        {
            while (s.size() > limit)
            {
                os << prefix << s.take(limit) << suffix;
                s = s.skipBegin(limit);

            }
        }
        return os << prefix << s << suffix;
    }

    static print_wrap wrap(strparam s, size_t limit, char const* prefix="", char const* suffix="\n")
    {
        check_null(prefix); check_null(suffix);
        return { .str = s, .limit=limit, .prefix=prefix, .suffix=suffix };
    }
}

}
