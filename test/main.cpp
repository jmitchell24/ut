#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
#include <ut/random.hpp>
using namespace ut;

#include <iostream>
using namespace std;

#define PRINT(x_) cout << #x_ << ": " << (x_) << endl

float rotate(float f)
{
    return fmod(f < 0.0f ? 360.0f + f : f, 360.0f);
}

int main()
{
    float f = 50;

    cout << rotate(f - 360);



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
