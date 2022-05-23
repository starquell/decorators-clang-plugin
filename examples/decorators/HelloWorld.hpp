#pragma once

#include "Common.hpp"
#include <utility>
#include <iostream>

template <typename Func>
constexpr auto hello_world(Func&& f)
{
    return [f = std::forward<Func>(f)] (auto&&... args)
    {
        std::cout << "Hello, world!" << std::endl;
        return_if_needed(f(std::forward<decltype(args)>(args)...));
    };
}
