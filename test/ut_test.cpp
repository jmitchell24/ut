//#define CATCH_CONFIG_MAIN
//#include <catch2/catch.hpp>
//#include <modern/lib.hpp>

// TEST_CASE( "Quick check", "[main]" ) {
    
//     REQUIRE( 2+2 == 5 );
    
// }

#include <cstdio>
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_map>
using namespace std;

#include <ut/StringView.hpp>
#include <ut/Algorithms.hpp>
using namespace ut;

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/color.h>
using namespace fmt;

template<typename T>
typename T::size_type GeneralizedLevenshteinDistance(const T &source,
                                                    const T &target,
                                                    typename T::size_type insert_cost = 1,
                                                    typename T::size_type delete_cost = 1,
                                                    typename T::size_type replace_cost = 1) {
    if (source.size() > target.size()) {
        return GeneralizedLevenshteinDistance(target, source, delete_cost, insert_cost, replace_cost);
    }

    using TSizeType = typename T::size_type;
    const TSizeType min_size = source.size(), max_size = target.size();
    std::vector<TSizeType> lev_dist(min_size + 1);

    lev_dist[0] = 0;
    for (TSizeType i = 1; i <= min_size; ++i) {
        lev_dist[i] = lev_dist[i - 1] + delete_cost;
    }

    for (TSizeType j = 1; j <= max_size; ++j) {
        TSizeType previous_diagonal = lev_dist[0], previous_diagonal_save;
        lev_dist[0] += insert_cost;

        for (TSizeType i = 1; i <= min_size; ++i) {
            previous_diagonal_save = lev_dist[i];
            if (source[i - 1] == target[j - 1]) {
                lev_dist[i] = previous_diagonal;
            } else {
                lev_dist[i] = std::min(std::min(lev_dist[i - 1] + delete_cost, lev_dist[i] + insert_cost), previous_diagonal + replace_cost);
            }
            previous_diagonal = previous_diagonal_save;
        }
    }

    return lev_dist[min_size];
}

using distance_type = unsigned;
distance_type myLevenshtein2(stringview source, stringview target,
                             distance_type d_insert  =1,
                             distance_type d_delete  =1,
                             distance_type d_replace =1)
{
    if (source.size() > target.size())
    {
        swap(source, target);
        swap(d_insert, d_delete);
    }

    distance_type sz_source = distance_type(source.size());
    distance_type sz_target = distance_type(target.size());

    vector<distance_type> v(sz_source + 1);

    v[0] = 0;
    for (distance_type i = 1; i <= sz_source; ++i)
    {
        v[i] = v[i - 1] + d_delete;
    }

    for (distance_type y = 1; y <= sz_target; ++y)
    {
        distance_type cell_xy = v[0];

        v[0] += d_insert;
        for (distance_type x = 1; x <= sz_source; ++x)
        {
            //       x-1       x
            //     ┌─────────┬────────┐
            // y-1 │ cell_xy │ cell_y │
            //     ├─────────┼────────┤
            // y   │ cell_x  │ cell   │
            //     └─────────┴────────┘

            distance_type& cell = v[x];             // cell to be computed
            distance_type cell_y = cell;            // computed cell from the previous row

            if (source[x-1] == target[y-1])
            {
                cell = cell_xy;                     // no added distance if chars are equal
            }
            else
            {
                distance_type cell_x = v[x-1];      // computed cell from the previous column

                cell = min(min(
                    cell_x  + d_delete,
                    cell_y  + d_insert),
                    cell_xy + d_replace
                );                                  // add distance
            }

            cell_xy = cell_y;                       // cell_y becomes cell_yx in the next column
        }
    }

    return v[sz_source];
}

int myLevenshtein(stringview const& a, stringview const b)
{
    vector<int> v;
    int w = a.size()+2;
    int h = b.size()+2;

    auto xy = [w](int x, int y){ return y * w + x; };

    auto print = [&]()
    {
        fmt::print("    ");
        for (int x = 2; x < w; ++x)
        {
            //printf("  %c", v[xy(x,0)]);
            fmt::print(bg(color::dark_slate_gray), "{:>3}", char(v[xy(x,0)]));
        }

        fmt::print("\n");

        for (int y = 1; y < h; ++y)
        {
            if (y == 1)
                fmt::print(" ");
            else
                fmt::print(bg(color::dark_slate_gray), "{}", char(v[xy(0,y)]));

            for (int x = 1; x < w; ++x)
            {
                fmt::print(fg(color::dark_cyan), "{:>3}", v[y * w + x]);
            }
            fmt::print("\n");
        }
    };

    v.resize(w*h);

    for (int x = 2; x < w; ++x)
    {
        v[xy(x,0)] = a[x-2];
        v[xy(x,1)] = x-1;
    }

    for (int y = 2; y < h; ++y)
    {
        v[xy(0,y)] = b[y-2];
        v[xy(1,y)] = y-1;
    }

    for (int y = 2; y < h; ++y)
    {
        for (int x = 2; x < w; ++x)
        {
            auto& cell = v[xy(x,y)];
            auto& cell_xy = v[xy(x-1,y-1)];

            if (a[x-2] == b[y-2])
            {
                cell = cell_xy;
            }
            else
            {
                auto& cell_x  = v[xy(x-1,y  )];
                auto& cell_y  = v[xy(x  ,y-1)];

                cell = min(min(cell_x, cell_y), cell_xy) + 1;
            }
        }
    }

    print();
    return v.back();
}

template <>
struct fmt::formatter<ut::stringview> : fmt::formatter<fmt::string_view> {
    template <typename FormatContext>
    auto format(ut::stringview const& sv, FormatContext& ctx)
    {
        fmt::string_view fsv{sv.begin(), sv.size()};
        return formatter<fmt::string_view>::format(fsv, ctx);
    }
};

void rstr(string& s)
{
    s.resize(rand() % 20 + 1);

    for (int i = 0; i < s.size(); ++i)
    {
        char c = 0;
        switch (rand() % 3)
        {
            case 0: c = 'a' + rand() % 26; break;
            case 1: c = 'A' + rand() % 26; break;
            case 2: c = '0' + rand() % 10; break;
            default:assert(false);
        }
        s[i] = c;
    }
}

#include "ut/Time.hpp"

int main() 
{
    auto seed = clock();
    auto test_size = 1000000;

    srand(seed);
    vector<string> r_strs(test_size);
    for (auto& s : r_strs)
        rstr(s);

    print("test_size: {}\n", test_size);


    size_t hash = 0;

    Timer timer;
    for (int i = 1; i < test_size; ++i)
    {
        hash += string_metric::editDistance(r_strs[i-1], r_strs[i], 3,2,1);
    }
    print("yours: {:.2f}s, check:{:x}\n", timer.seconds(), hash);

    hash = 0;

    timer.reset();
    for (int i = 1; i < test_size; ++i)
    {
        hash += myLevenshtein2(r_strs[i-1], r_strs[i], 3,2,1);
    }
    print("mine : {:.2f}s, check:{:x}\n", timer.seconds(), hash);

//    for (;;)
//    {
//        string a=rstr(),b=rstr();
//        string _q;
//        getline(cin,_q);
//        if (_q == "q")
//            break;

//        print("\n");

//        auto source = stringview(a).trim();
//        auto target = stringview(b).trim();

//        int ed = myLevenshtein(source,target);

//        print("\nedit distance : {}\n", ed);
//        print("\nedit distance2: {}\n", myLevenshtein2(source,target, 4,2,3));
//        print("\nedit distance3: {}\n", GeneralizedLevenshteinDistance(source,target, 4,2,3));
//        print("\n");
//    }





//    {
//        int ed = myLevenshtein(stringview("replace").trim(), stringview("delete").trim());

//        printf("\nedit distance: %d\n\n", ed);
//    }

//    for (;;)
//    {
//        string a,b;
//        print("str1: ");
//        getline(cin, a);

//        print("str2: ");
//        getline(cin, b);

//        if (a == "q" || b == "q")
//            break;

//        print("\n");

//        auto source = stringview(a).trim();
//        auto target = stringview(b).trim();

//        int ed = myLevenshtein(source,target);

//        print("\nedit distance : {}\n", ed);
//        print("\nedit distance2: {}\n", myLevenshtein2(source,target));
//        print("\nedit distance3: {}\n", GeneralizedLevenshteinDistance(source,target));
//        print("\n");
//    }



    return EXIT_SUCCESS;
}
