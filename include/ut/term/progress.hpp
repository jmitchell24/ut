//
// Created by james on 07/07/25.
//

//
// ut
//
#include "ut/string.hpp"

//
// std
//
#include <atomic>
#include <string>
#include <functional>
#include <mutex>
#include <memory>

#include "escapes.hpp"

namespace ut
{
    struct ProgressBar
    {
        char const* start;
        char const* off;
        char const* on;
        char const* tip;
        char const* end;
    };

namespace bars
{
    ProgressBar static const regular = { "[", ".", "o", TERM_FG_BRIGHT_RED "0", "]" };
    ProgressBar static const line = { "[", "-", "=", ">", "]" };
}

    class ProgressRunner
    {
    public:
        static constexpr size_t INTERVAL = 50;

        using task_param = std::function<void(ProgressRunner&)> const&;
        using task_param_multi = std::function<void(ProgressRunner&, size_t)>;

        using runnerlist_type = std::vector<std::unique_ptr<ProgressRunner>>;

        ProgressRunner()
             : m_bar(bars::regular)
        { }

        ProgressRunner(ProgressBar const& bar)
            : m_bar(bar)
        { }

        ProgressRunner(ProgressRunner const&)=delete;
        ProgressRunner& operator=(ProgressRunner const&)=delete;

        ProgressRunner(ProgressRunner&&)=delete;
        ProgressRunner& operator=(ProgressRunner&&)=delete;

        inline bool abortRequested() const
        { return m_abort_flag; }

        inline std::string prefix() const
        { std::scoped_lock sl(m_mutex); return m_prefix; }

        inline std::string suffix() const
        { std::scoped_lock sl(m_mutex); return m_suffix; }

        inline ProgressBar bar() const
        { std::scoped_lock sl(m_mutex); return m_bar; }

        inline void prefix(strparam s)
        { std::scoped_lock sl(m_mutex); m_prefix = s.str(); }

        inline void suffix(strparam s)
        { std::scoped_lock sl(m_mutex); m_suffix = s.str(); }

        inline void suffixPercent()
        { suffix(std::format("{}%", progressValue())); }

        inline void bar(ProgressBar const& bar)
        { std::scoped_lock sl(m_mutex); m_bar = bar; }

        /// \brief Set progress state as a percent value (integer between 1 and 100)
        /// \param  value   Percent value.
        inline void valuePercent(int value)
        {
            if (value < 1) value = 1;
            if (value > 100) value = 100;
            progressValue(value);
        }

        /// \brief Set progress state as a fractional value (number between 0 and 1)
        /// \param  value   Fractional value.
        inline void valueFraction(double value)
        {
            if (value < 0) value = 0.0;
            if (value > 1) value = 1.0;
            progressValue(static_cast<int>(value * 100.0));
        }

        /// \brief Set progress state as a value within a specified range
        /// \param  min     Minimum value of the range (inclusive).
        /// \param  max     Maximum value of the range (inclusive).
        /// \param  value   Value within the range.
        inline void valueRange(double min, double max, double value)
        {
            double n = (value - min) / (max - min);
            progressValue(static_cast<int>(1.0 + n * 99.0));
        }

        /// \brief Set progress state to the maximum of the range, to indicate completion of a task.
        inline void valueComplete()
        { progressValue(100); }

        /// \brief
        void progress(task_param task);

        /// \brief
        static void progressParallel(runnerlist_type& runners, task_param_multi);

    private:
        ProgressBar m_bar;
        std::string m_prefix;
        std::string m_suffix;
        int m_progress_value=1;

        std::atomic_bool m_abort_flag=false;
        mutable std::mutex m_mutex;

        void print();

        inline int progressValue() const
        { std::scoped_lock sl(m_mutex); return m_progress_value; }

        inline void progressValue(int value)
        { std::scoped_lock sl(m_mutex); m_progress_value = value; }
    };


    inline static void progress(ProgressRunner::task_param task)
    {
        ProgressRunner pr;
        pr.progress(task);
    }

    inline static void progress(ProgressBar const& p, ProgressRunner::task_param task)
    {
        ProgressRunner pr(p);
        pr.progress(task);
    }

    inline static void progressParallel(size_t n, ProgressRunner::task_param_multi task)
    {
        if (n < 1)
            return;

        ProgressRunner::runnerlist_type runners;
        for (size_t i = 0; i < n; ++i)
            runners.emplace_back(new ProgressRunner());
        ProgressRunner::progressParallel(runners, task);
    }

    inline static void progressParallel(size_t n, ProgressBar const& p, ProgressRunner::task_param_multi task)
    {
        if (n < 1)
            return;

        ProgressRunner::runnerlist_type runners;
        for (size_t i = 0; i < n; ++i)
            runners.emplace_back(new ProgressRunner(p));
        ProgressRunner::progressParallel(runners, task);
    }
}
