#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



//#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
#include <ut/container/vector_array.hpp>
#include <ut/check.hpp>
#include <ut/math/matrix4D.hpp>
using namespace ut;

#include <iomanip>
#include <iostream>
#include <ranges>
using namespace std;

#define DEBUG_PRINT(x_) cout << #x_ << endl; (x_);


//struct Plane
//{
//    ut::vec2 scale  { 1, 1 };
//    ut::vec2 offset { 0, 0 };

//    inline ut::vec2 toReal(ut::vec2 const& p) const
//    {
//        return (p / scale) - offset;
//    }

//    inline ut::vec2 toPlane(ut::vec2 const& p) const
//    {
//        return (p + offset) * scale;
//    }
//};

//double convertRange(double x, double inMin, double inMax, double outMin, double outMax) {
//    // Calculate the linear scaling factor
//    double k = (outMax - outMin) / (inMax - inMin);

//    // Apply the linear mapping
//    double result = outMin + ( (x - inMin) * k );

//    return result;
//}

//ut::vec2 transform(ut::rect const& a, ut::rect const& b, ut::vec2 p)
//{
//    auto scale = (b.max - b.min) / (a.max - a.min);
//    return b.min + (p - a.min) * scale;
//}


int main()
{

    constexpr rect r{1,1,1,1};

    auto test = "test yaml"_sv;

    cout << (test.contains('.') ? "yes" : "no") << endl;

    test = "test.yaml"_sv;
    cout << (test.contains('.') ? "yes" : "no") << endl;

    test = "test"_sv;
    cout << (test.contains(".yaml") ? "yes" : "no") << endl;

    test = "test.yaml"_sv;
    cout << (test.contains(".yaml") ? "yes" : "no") << endl;


    cout << "eq: " << ( r==rect() ? "yes" : "no" ) << endl;



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
