#pragma once

#include <random>
#include <limits>

#include "math/vectorND.hpp"

#define M_DECL inline

namespace ut
{
    class rng
    {
    public:
        using seed_type  = std::size_t;
        using disti_type = std::uniform_int_distribution <int>;
        using distu_type = std::uniform_int_distribution <unsigned>;
        using distf_type = std::uniform_real_distribution<float>;
        using distd_type = std::uniform_real_distribution<double>;

        using nli_type = std::numeric_limits<int>;
        using nlu_type = std::numeric_limits<unsigned>;
        using nlf_type = std::numeric_limits<float>;
        using nld_type = std::numeric_limits<double>;

        M_DECL rng() : m_dev(), m_rng(m_dev()) { }

        M_DECL static rng& instance() { static rng x; return x; }

        M_DECL float    nextf(float     min, float      max) { return distf_type(min,max)(m_rng); }
        M_DECL double   nextd(double    min, double     max) { return distd_type(min,max)(m_rng); }
        M_DECL int      nexti(int       min, int        max) { return disti_type(min,max)(m_rng); }
        M_DECL unsigned nextu(unsigned  min, unsigned   max) { return distu_type(min,max)(m_rng); }

        M_DECL float    nextf() { return nextf(0.0f, 1.0f); }
        M_DECL double   nextd() { return nextd(0.0 , 1.0 ); }
        M_DECL int      nexti() { return nexti(nli_type::min(), nli_type::max()); }
        M_DECL unsigned nextu() { return nextu(nlu_type::min(), nlu_type::max()); }
        M_DECL bool     nextb() { return nexti() > 0; }

        M_DECL int      nexti(int      bound) { return bound < 1 ? 0 : nexti(0, bound-1); }
        M_DECL unsigned nextu(unsigned bound) { return bound < 1 ? 0 : nextu(0, bound-1); }

        template <typename T>
        M_DECL T* choosePtr(T* data, size_t size)
        { return data + nextu(size); }

        template <typename T>
        M_DECL T const* choosePtr(T const* data, size_t size)
        { return data + nextu(size); }

        template <typename T, size_t I>
        M_DECL T const& choose(std::array<T, I> const& arr)
        { return *(arr.data() + nextu(arr.size())); }

    private:
        std::random_device  m_dev;
        std::mt19937        m_rng;
    };

    static rng& RNG = rng::instance();
}

#undef M_DECL
