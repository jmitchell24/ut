//
// Created by james on 16/05/25.
//

#pragma once

//
// ut
//
#include "ut/string.hpp"
#include "ut/term/spinners.hpp"

//
// std
//
#include <sstream>
#include <string>
#include <functional>
#include <mutex>

namespace ut
{
    class SpinnerRunner
    {
    public:
        struct Affix
        {
            template <typename T>
            inline Affix& operator<< (T&& t)
            { std::scoped_lock sl(mut); oss << std::forward<T>(t); return *this;}

            inline std::string str() const
            { std::scoped_lock sl(mut); return oss.str(); }

            inline Affix& reset()
            { std::scoped_lock sl(mut); oss.str(""); oss.clear(); return *this; }



            std::ostringstream oss;
            mutable std::mutex mut;
        };

        Affix prefix;
        Affix suffix;

        using task_param = std::function<void(SpinnerRunner&)> const&;

        void spin(Spinner const& spinner, task_param task);
    };

    inline static void spin(SpinnerRunner::task_param task)
    {
        SpinnerRunner sr;
        sr.spin(spinners::bouncingBar, task);
    }
}
