#include <ut/math.hpp>

namespace ut
{
    template struct vec<float       , 2>;
    template struct vec<double      , 2>;
    template struct vec<int         , 2>;
    template struct vec<unsigned    , 2>;
    template struct vec<std::uint8_t, 2>;

    template struct vec<float       , 3>;
    template struct vec<double      , 3>;
    template struct vec<int         , 3>;
    template struct vec<unsigned    , 3>;
    template struct vec<std::uint8_t, 3>;

    template struct vec<float       , 4>;
    template struct vec<double      , 4>;
    template struct vec<int         , 4>;
    template struct vec<unsigned    , 4>;
    template struct vec<std::uint8_t, 4>;

    template struct mat<float       , 4, 4>;
    template struct mat<double      , 4, 4>;

    template struct rectx<float       , false>;
    template struct rectx<double      , false>;
    template struct rectx<int         , false>;
    template struct rectx<unsigned    , false>;

    template struct rectx<int         , true>;
    template struct rectx<unsigned    , true>;

    template struct psizex<float       >;
    template struct psizex<double      >;
    template struct psizex<int         >;
    template struct psizex<unsigned    >;
    //template struct psizex<std::uint8_t>;

}
