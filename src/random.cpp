//
// Created by james on 06/05/25.
//

#include "ut/random.hpp"
using namespace ut;

rng& rng::instance() { static rng x; return x; }