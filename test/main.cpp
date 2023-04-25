#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
using namespace ut;

#include <iostream>
using namespace std;

int main()
{


    datetime dt;

    while (true)
    {
        dt.reset();
        cout << PRINTER("%d %d %d\n", dt.hours(), dt.minutes(), dt.seconds());
        timer::sleep(1_seconds);
    }

    return EXIT_SUCCESS;
}

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

//    const int result = context.run();
//    if (context.shouldExit())
//        return result;
//    return result;
//}
