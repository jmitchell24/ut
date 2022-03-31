#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#define UT_STL_INTEROP
#include <ut/ut.hpp>
using namespace ut;

#include <iostream>
using namespace std;

int main()
{
    rectf r{0,0,800,600};
    cout << r << endl;

    r = r.topLeft(0,0, fraction(.5), fraction(.5));
    cout << r << endl;
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
