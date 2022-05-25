#pragma once

#include "Common.hpp"
#include <iostream>
#include <utility>
#include <chrono>

template <typename Func>
constexpr auto measure_time(Func&& func)
{
    return [func = std::forward<Func>(func)] ()
    {
        const auto start = std::chrono::system_clock::now();
        func();
        const auto finish = std::chrono::system_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
        std::cout << "function executed in " << duration.count() << " ns\n";
    };
}