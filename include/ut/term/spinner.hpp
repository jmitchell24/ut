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
#include <atomic>
#include <sstream>
#include <string>
#include <functional>
#include <mutex>
#include <memory>

namespace ut
{
    /// \brief
    class SpinnerRunner
    {
    public:
        using task_param = std::function<void(SpinnerRunner&)> const&;
        using task_param_multi = std::function<void(SpinnerRunner&, size_t)> const&;

        using runnerlist_type = std::vector<std::unique_ptr<SpinnerRunner>>;

        SpinnerRunner()
            : m_spinner(spinners::bouncingBar)
        { }

        SpinnerRunner(Spinner const& spinner)
            : m_spinner(spinner)
        { }

        SpinnerRunner(SpinnerRunner const&)=delete;
        SpinnerRunner& operator=(SpinnerRunner const&)=delete;

        SpinnerRunner(SpinnerRunner&&)=delete;
        SpinnerRunner& operator=(SpinnerRunner&&)=delete;

        inline bool abortRequested() const
        { std::scoped_lock sl(m_mutex); return m_abort_flag; }

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


        /// \brief Executes task on calling thread, prints spinner on new thread.
        /// \param spinner  The spinner to use.
        /// \param task     The task to execute while spinning.
        void spin(task_param task);

        /// \brief Executes task on new threads, for each specified runner, and prints to the main thread.
        static void spinParallel(runnerlist_type& runners, task_param_multi task);

    private:
        Spinner m_spinner;
        std::string m_prefix;
        std::string m_suffix;

        std::atomic_int m_frame=0;
        std::atomic_bool m_abort_flag=false;
        mutable std::mutex m_mutex;

        void advanceFrame();
    };



    inline static void spin(SpinnerRunner::task_param task)
    {
        SpinnerRunner sr;
        sr.spin(task);
    }

    inline static void spin(Spinner const& s, SpinnerRunner::task_param task)
    {
        SpinnerRunner sr(s);
        sr.spin(task);
    }

    inline static void spinParallel(size_t n, SpinnerRunner::task_param_multi task)
    {
        if (n < 1)
            return;

        SpinnerRunner::runnerlist_type runners;
        for (size_t i = 0; i < n; ++i)
            runners.emplace_back(new SpinnerRunner());
        SpinnerRunner::spinParallel(runners, task);
    }

    inline static void spinParallel(size_t n, Spinner const& s, SpinnerRunner::task_param_multi task)
    {
        if (n < 1)
            return;

        SpinnerRunner::runnerlist_type runners;
        for (size_t i = 0; i < n; ++i)
            runners.emplace_back(new SpinnerRunner(s));
        SpinnerRunner::spinParallel(runners, task);
    }
}
