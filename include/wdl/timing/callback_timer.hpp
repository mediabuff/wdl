// callback_timer.hpp
//
// Class Implementations: wdl::timing::callback_timer
//
// Simple performance benchmarking utilities.

#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <functional>

namespace wdl::timing
{
    // wdl::timing::callback_timer
    //
    // RAII-based timer class that accepts a callback function invoked on expiration

    struct callback_timer
    {
        using f_callback = std::function<void(double)>;

        f_callback callback;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint;

    public:
        callback_timer(f_callback cb)
        {
            callback        = cb;
            start_timepoint = std::chrono::high_resolution_clock::now();
        }

        ~callback_timer()
        {
            auto ms = stop();
            callback(ms);
        }

    private:
        double stop()
        {
            auto end_timepoint = std::chrono::high_resolution_clock::now();

            auto start =
                std::chrono::time_point_cast<std::chrono::microseconds>(start_timepoint)
                .time_since_epoch()
                .count();

            auto end =
                std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint)
                .time_since_epoch()
                .count();

            auto duration = end - start;
            return duration * 0.001;
        }
    };
}