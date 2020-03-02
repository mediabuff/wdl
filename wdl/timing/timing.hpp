// timing.h
// Class Implementations:
//  wdl::timing::scoped_timer
//  wdl::timing::callback_timer
//
// Simple performance benchmarking utilities.

#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <functional>

namespace wdl::timing
{
    // timer
    //
    // RAII-based timer class; prints duration to stdout

    struct scoped_timer
    {
        scoped_timer(const std::string& label = "")
            : label{label}
        {
            start_timepoint = std::chrono::high_resolution_clock::now();
        }

        ~scoped_timer()
        {
            stop();
        }

        void stop()
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
            double ms = duration * 0.001;

            // output result to console
            std::cout << "[WDL] (" << label << ") " << ms << "ms\n";
        }

    private:
        std::string label;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint;
    };

    // callback_timer
    //
    // RAII-based timer class that accepts a callback function invoked on expiration

    struct callback_timer
    {
        using f_callback = std::function<void(double)>;

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

    private:
        f_callback callback;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint;
    };
}