#pragma once

#include "Common.hpp"
#include <iostream>

template <typename Func>
constexpr auto count_calls(Func&& func)
{
    return [func = std::forward<Func>(func), calls = 0] (auto&&... args) mutable {
        std::cout << "this function called " << ++calls << " times\n";
        return_if_needed(func(std::forward<decltype(args)>(args)...));
    };
}