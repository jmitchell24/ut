#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#define UT_STL_INTEROP
#include <ut/ut.hpp>
using namespace ut;

#include <iostream>
using namespace std;

int main()
{
    auto v = vector<color>
    {
#define COLOR(_n, _i) color(_i),
        UT_COLORS
#undef COLOR
    };

    sort(v.begin(), v.end());

    for (auto const& c : v)
        cout << c << endl;

    color::hsv x;


    return EXIT_SUCCESS;
}


//
//
//int main(int argc, char** argv)
//{
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
//}
