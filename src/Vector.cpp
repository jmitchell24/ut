#include <ut/math/Vector2D.hpp>
#include <ut/math/Vector3D.hpp>
#include <ut/math/Vector4D.hpp>

#include <ut/Region.hpp>

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

    template struct regionx<float       , false>;
    template struct regionx<double      , false>;
    template struct regionx<int         , false>;
    template struct regionx<unsigned    , false>;
    template struct regionx<std::uint8_t, false>;

    template struct regionx<int         , true>;
    template struct regionx<unsigned    , true>;
    template struct regionx<std::uint8_t, true>;

}
