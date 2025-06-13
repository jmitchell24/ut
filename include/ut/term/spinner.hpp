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
        using task_param = std::function<void(SpinnerRunner&)> const&;

        inline std::string prefix() const
        { std::scoped_lock sl(m_mutex); return m_prefix; }

        inline std::string suffix() const
        { std::scoped_lock sl(m_mutex); return m_suffix; }

        inline Spinner spinner() const
        { std::scoped_lock sl(m_mutex); return m_spinner; }

        inline void prefix(strparam s)
        { std::scoped_lock sl(m_mutex); m_prefix = s.str(); }

        inline void suffix(strparam s)
        { std::scoped_lock sl(m_mutex); m_suffix = s.str(); }

        inline void spinner(Spinner const& spinner)
        { std::scoped_lock sl(m_mutex); m_spinner = spinner; m_frame=0; }

        void spin(Spinner const& spinner, task_param task);

    private:
        int m_frame=0;
        std::string m_prefix;
        std::string m_suffix;
        Spinner m_spinner;

        mutable std::mutex m_mutex;
    };

    inline static void spin(SpinnerRunner::task_param task)
    {
        SpinnerRunner sr;
        sr.spin(spinners::bouncingBar, task);
    }
}
