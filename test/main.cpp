#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



//#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
#include <ut/container/vector_array.hpp>
using namespace ut;

#include <iomanip>
#include <iostream>
using namespace std;

#define DEBUG_PRINT(x_) cout << #x_ << endl; (x_);





int main()
{
    int a=0, b=0;
    for (size_t i = 0; i < 1000000; ++i)
    {
        if (RNG.nextb())
            ++a;
        else
            ++b;
    }

    float fa = float(a) / float(a+b) * 100;
    float fb = float(b) / float(a+b) * 100;

    cout << fa << " / " << fb << endl;


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
