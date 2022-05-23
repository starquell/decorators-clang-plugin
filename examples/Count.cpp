#include "decorators/Count.hpp"

#include <iostream>
#include <thread>

[[decorate(count_calls)]]
void foobar()
{
    std::this_thread::sleep_for(std::chrono::milliseconds{500});
    std::cout << "Hello!\n";
}

int main()
{
    for (auto i = 0; i < 5; ++i) {
        foobar();    
    }
}

//// Output: 
// this function called 1 times
// Hello!
// this function called 2 times
// Hello!
// this function called 3 times
// Hello!
// this function called 4 times
// Hello!
// this function called 5 times
// Hello!