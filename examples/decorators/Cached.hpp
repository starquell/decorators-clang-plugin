#pragma once

#include "Common.hpp"

#include <type_traits>
#include <map>
#include <utility>

template <typename Func>
constexpr auto cached(Func&& func)
{
    return [func = std::forward<Func>(func)] (auto&&... args) 
    {
        using Cache = std::map<std::tuple<std::remove_cvref_t<decltype(args)>...>, 
                               std::invoke_result_t<Func, decltype(args)...>>;

        static auto cache = Cache{};
        if (auto it = cache.find(std::make_tuple(args...)); it != cache.end()) {
            return it->second;
        }
        else {
            auto res = func(std::forward<decltype(args)>(args)...);
            cache[std::make_tuple(args...)] = res;
            return res;
        }
    };
}