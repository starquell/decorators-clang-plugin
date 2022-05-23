#include "decorators/All.hpp"

class HasDeletedFunction {

    /// warning: 'decorate' attribute does not apply to deleted functions
    [[decorate(count_calls)]]
    void foo() = delete;
};

class HasPureFunction {

    /// warning: 'decorate' attribute does not apply to pure functions
    [[decorate(count_calls)]]
    virtual void foo() = 0;
};


template <typename T>
class HasDeductionGuide
{
    HasDeductionGuide(T&& t);
};


///  warning: 'decorate' attribute does not apply to deduction guides
template <typename T> 
[[decorate(count_calls)]]
HasDeductionGuide(T&& v) -> HasDeductionGuide<T>;

int main() {}