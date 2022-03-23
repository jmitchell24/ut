#include <ut/ut.hpp>
using namespace ut;

#include <fmt/core.h>
using namespace fmt;

int main()
{
    auto r = Rect { 0,0,100,100 };

    r = r.fit(23,32);

    print("{} {} {} {}", r.x, r.y, r.w, r.h);
    return 0;
}
