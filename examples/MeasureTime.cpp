#include "decorators/MeasureTime.hpp"

[[decorate(measure_time)]]
void print_hello_world()
{
    std::cout << "Hello, world!" << std::endl;
}

int main() 
{
    print_hello_world();
}