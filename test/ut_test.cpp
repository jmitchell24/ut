#include <ut/ut.hpp>
using namespace ut;

#include <cstdio>
#include <cstdlib>

int main()
{
    for (auto e : rectf{0,0,50,50})
        printf("%.2f ", e);
    printf("\n");
    return 0;
}

//inline vector_type cross  (vector_param v)      const { return vector_type((y * v.z) - (v.y * z), (z * v.x) - (v.z * x), (x * v.y) - (v.x * y)); }