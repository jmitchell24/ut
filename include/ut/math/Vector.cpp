#include "Vector2D.hpp"
#include "Vector3D.hpp"
#include "Vector4D.hpp"

namespace ut
{
    template struct VecND<float        , 4>;
    template struct VecND<double       , 4>;
    template struct VecND<int          , 4>;
    template struct VecND<unsigned     , 4>;
    template struct VecND<unsigned char, 4>;

    template struct VecND<float        , 3>;
    template struct VecND<double       , 3>;
    template struct VecND<int          , 3>;
    template struct VecND<unsigned     , 3>;
    template struct VecND<unsigned char, 3>;

    template struct VecND<float        , 2>;
    template struct VecND<double       , 2>;
    template struct VecND<int          , 2>;
    template struct VecND<unsigned     , 2>;
    template struct VecND<unsigned char, 2>;
}
