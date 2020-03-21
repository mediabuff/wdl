// scoped_timer.hpp
//
// Class Implementations: wdl::timing::scoped_timer
//
// Simple performance benchmarking utilities.

#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <functional>

namespace wdl::timing
{
    // wdl::timing::scoped_timer
    //
    // RAII-based timer class; prints duration to stdout

    class scoped_timer
    {
        std::string label;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint;

    public:
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
    };
}