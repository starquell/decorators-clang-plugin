#pragma once

#include "Common.hpp"

#include <utility>

consteval auto repeat(int n)
{
    return [n] (auto&& func) constexpr
    {
        return [n, func = std::forward<decltype(func)>(func)] (auto&&... args) mutable
        {
            for (int i = 0; i < n; ++i) {
                func(std::forward<decltype(args)>(args)...);
            }
        };
    };
}