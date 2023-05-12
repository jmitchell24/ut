#pragma once

#include <random>

/*

#include <random>
#include <iostream>

int main()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6); // distribution in range [1, 6]

    std::cout << dist6(rng) << std::endl;
}


 */

#define M_DECL inline

namespace ut
{
    class rangen
    {
    public:
        using seed_type  = std::size_t;
        using disti_type = std::uniform_int_distribution <int>;
        using distu_type = std::uniform_int_distribution <unsigned>;
        using distf_type = std::uniform_real_distribution<float>;
        using distd_type = std::uniform_real_distribution<double>;

        M_DECL rangen() : m_dev(), m_rng(m_dev()) { }

        M_DECL float    nextf(float     min, float      max) { return distf_type(min,max)(m_rng); }
        M_DECL double   nextd(double    min, double     max) { return distd_type(min,max)(m_rng); }
        M_DECL int      nexti(int       min, int        max) { return disti_type(min,max)(m_rng); }
        M_DECL unsigned nextu(unsigned  min, unsigned   max) { return distu_type(min,max)(m_rng); }

        M_DECL unsigned nexti(int max) { return nexti(0, max); }
        M_DECL int      nextu(int max) { return nextu(0, max); }

    private:
        std::random_device  m_dev;
        std::mt19937        m_rng;
    };


}

#undef M_DECL
