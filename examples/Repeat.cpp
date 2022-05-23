#include "decorators/Repeat.hpp"

#include <iostream>

[[decorate(repeat(5))]]
void foo()
{
    std::cout << "Hello!\n";
}

int main()
{
    foo();
    /// Output:
    //   Hello!
    //   Hello!
    //   Hello!
    //   Hello!
    //   Hello!
}
