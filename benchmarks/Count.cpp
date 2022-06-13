#include "../examples/decorators/Repeat.hpp"

#include <chrono>
#include <benchmark/benchmark.h>

void little_work() 
{
    int res = 0;
    for (auto i = 0; i < 10; ++i) {
        res += i * 3;
    }
}

[[decorate(repeat(3))]]
void decorated_with_plugin()
{
    little_work();
}

void undecorated()
{
    little_work();
}

constinit auto decorated_without_plugin = repeat(3)(undecorated);

static void BM_PluginDecorator(benchmark::State& state)
{
    for (auto _ : state) {
        decorated_with_plugin();
    }
}

static void BM_DecoratorWithoutPlugin(benchmark::State& state) 
{
    for (auto _ : state) {
        decorated_without_plugin();
    }
}

BENCHMARK(BM_PluginDecorator);
BENCHMARK(BM_DecoratorWithoutPlugin);
BENCHMARK_MAIN();