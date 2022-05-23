#include "decorators/Cached.hpp"

#include <iostream>
#include <thread>
#include <chrono>


[[decorate(cached)]]
int long_computation(int a)
{
    std::this_thread::sleep_for(std::chrono::seconds{1});
    return a * 10;
}

int main() 
{
    /// First 5 calls of long_computation will take 1s for each
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Result of computation: " << long_computation(i) << std::endl;
    }
    /// This loop executes immedeately, because `cached` decorator cached result of long_computation for values 0, 1, 2, 3, 4
    for (int i = 5; i > 0; --i)
    {
        std::cout << "Result of computation: " << long_computation(i) << std::endl;
    }
}
