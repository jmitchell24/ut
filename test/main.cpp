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

void gulpTest()
{
    vector<b8> out_bytes;

    auto sz = 16777216;



    for (size_t i = 0; i < sz; ++i)
    {
        out_bytes.push_back(RNG.nextu() % 255);
    }

    if (FILE* file = fopen("/tmp/gulp.bin", "wb"))
    {
        fwrite(out_bytes.data(), sizeof(b8), out_bytes.size(), file);
        fclose(file);
    }
    else
    {
        fprintf(stderr, "failed to write gulp test file");
        return;
    }

    if (FILE* file = fopen("/tmp/gulp.bin", "rb"))
    {
        auto in_bytes = gulp::file_to_vector(file);

        check(out_bytes.size() == in_bytes.size(), "size doesn't match");
        for (size_t i = 0; i < out_bytes.size(); ++i)
        {
            check(out_bytes[i] == in_bytes[i], "byte doesn't match");
        }

        fclose(file);
    }
    else
    {
        fprintf(stderr, "failed to read gulp test file");
        return;
    }
}


int main()
{

    gulpTest();

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
