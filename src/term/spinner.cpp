//
// Created by james on 16/05/25.
//

//
// ut
//
#include "ut/term/spinner.hpp"

#include <atomic>

#include "ut/term/escapes.hpp"
#include "ut/time.hpp"
using namespace ut;

//
// std
//
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
using namespace std;

void SpinnerRunner::spin(Spinner const& spinner, task_param task)
{
    atomic_bool done = false;
    int i = 0;

    thread spin_thread([&]
    {
        cout << endl << TERM_CURSOR_HIDE << flush;

        while (!done)
        {
            cout
                << TERM_CURSOR_COLUMN(0) TERM_CLEAR_LINE
                << prefix.str()
                << spinner.frames[i]
                << TERM_RESET " "
                << suffix.str()
                << TERM_RESET
                << flush;

            i = (i + 1) % spinner.frames.size();

            timer::sleep(duration::milliseconds(spinner.interval));
        }
        // Clear the spinner character when done
        cout << TERM_CURSOR_SHOW TERM_CURSOR_COLUMN(0) TERM_CLEAR_LINE << flush;
    });

    task(*this);

    done = true;

    if (spin_thread.joinable())
        spin_thread.join();
}