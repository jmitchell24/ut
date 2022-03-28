#define UT_STL_INTEROP
#include <ut/ut.hpp>
using namespace ut;

#include <iostream>
using namespace std;

#include <cfloat>

int main()
{
    auto v1 = make_vec<int>(1,2,3,4,5);
    auto v2 = v1.reverse();
    auto v3 = v1 + v2;
    auto v4 = make_vec<float>(
            INFINITY,
            INFINITY,
            INFINITY,
            INFINITY,
            NAN
            );

    auto r1 = rect(1,1,500,400);

    cout << "v1: " << v1 << endl;
    cout << "v2: " << v2 << endl;
    cout << "v3: " << v3 << endl;
    cout << "v4: " << v4 << endl;
    cout << "v4.isinf: " << v4.isInf() << endl;
    cout << "v4.isnan: " << v4.isNan() << endl;
    cout << "r1: " << r1 << endl;
    return 0;
}

//inline vector_type cross  (vector_param v)      const { return vector_type((y * v.z) - (v.y * z), (z * v.x) - (v.z * x), (x * v.y) - (v.x * y)); }