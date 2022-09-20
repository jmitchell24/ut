#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#define UT_FMT_USE_CSTRVIEW
#include <ut/ut.hpp>
using namespace ut;

#include <fmt/core.h>
#include <fmt/color.h>
using namespace fmt;

#include <type_traits>
#include <iostream>
#include <iomanip>
using namespace std;

//
// forward declarations (to catch compile errors
//

namespace ut
{
    template struct basic_strview<char, std::char_traits<char>, false>;
    template struct basic_strview<char, std::char_traits<char>, true>;

    template struct basic_strview_split<true, char, std::char_traits<char>>;
    template struct basic_strview_split<false, char, std::char_traits<char>>;

    template struct basic_strview_buf<char>;
    template struct basic_strview_stream<char>;
}

template <typename T, float LF> struct hash_table
{
    struct entry
    {
        std::string key;
        bool empty=true;
        union { char dummy; T value; };
    };

    static constexpr float LOAD_FACTOR = LF;
    static_assert(LOAD_FACTOR > 0 && LOAD_FACTOR < 1, "invalid LOAD_FACTOR range");

    using entries_type = std::vector<entry>;

    entries_type buckets;
    size_t count = 0;

    inline size_t bucketIndex(strparam key)
    { return hasher::range(key.begin(), key.end()) % buckets.size(); }

    bool set(strparam key, T&& value)
    {
        if (count+1 > buckets.size() * 0.75)
        {
            buckets.reserve(buckets.capacity() * 1.5);
        }

        entry* e = findEntry(key);

        bool is_new = e->empty;
        if (is_new) ++count;

        e->key = key.str();
        e->operator=(value);
        return is_new;
    }

    entry* findEntry(strparam key)
    {
        size_t index = bucketIndex(key);

        for (;;)
        {
            auto entry = &buckets[index];
            if (entry->key == key.str() || entry->empty)
                return entry;
            index = (index + 1) % buckets.size();
        }
    }

    static hash64_t hash(strparam s)
    {
        return hasher::range(s.begin(), s.end());
    }
};

//template <typename Char, typename Traits, bool NullTerminated>
//inline std::basic_ostream<Char, Traits>& operator << (std::basic_ostream<Char, Traits>& os, ut::basic_strview<Char, Traits, NullTerminated> const& s)
//{ return os.write(s.begin(), (std::streamsize)s.size()); }

int main(int argc, char** argv)
{
    string string1 = "string1";
    string string2 = "string1";

    auto view = strview(string1).takeEnd(1);
    auto mirror = view.mirror(string1, string2);

    cout << (size_t)view.begin() << endl;
    cout << (size_t)mirror.end() << endl;
    return EXIT_SUCCESS;
//    doctest::Context context;
//    context.applyCommandLine(argc, argv);
//    context.setOption("no-breaks", true);
//    context.setOption("--version", true);
//    context.setOption("--count", true);
//    context.setOption("--list-test-cases", true);
//    context.setOption("--list-test-suites", true);
//    context.setOption("--success", false);
//    context.setOption("--exit", true);
//
//    const int result = context.run();
//    if (context.shouldExit())
//        return result;
//    return result;
}

//int main()
//{
//    using vec10f = vec<float,10>;
//
//    constexpr vec10f v1{{1,2,3,4,5,6,7,8,9,10}};
//    constexpr vec10f v2{};
//    constexpr vec10f v3{1};
//
//
//
//    cout << v1.length() << endl;
//    cout << v2 << endl;
//    cout << v3 << endl;
//    cout << (v2 < v3) << endl;
//
//    for (auto& x : v3)
//        cout << x << endl;
//
//}



