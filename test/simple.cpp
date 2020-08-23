#include <accumula/accumulator/minmax.h>
#include <accumula/accumulator/sum.h>
#include <accumula/accumulator/window_filter.h>
#include <fmt/format.h>
#include <memory_resource>

using namespace std;
using namespace accumula;

struct TimePoint
{
    double key;
    TimePoint operator-(const TimePoint &rhs) const noexcept
    {
        return TimePoint{this->key - rhs.key};
    }
    TimePoint &operator+=(const TimePoint &rhs)
    {
        this->key += rhs.key;
        return *this;
    }
    TimePoint &operator-=(const TimePoint &rhs)
    {
        this->key -= rhs.key;
        return *this;
    }
    bool operator<(const TimePoint &rhs) const noexcept
    {
        return this->key < rhs.key;
    }
};

struct Value: TimePoint
{
    int value;
    Value()
    {
    }
    Value(double key, int v)
        : TimePoint{key}
        , value(v)
    {
    }
};

void construct_only()
{
    std::pmr::unsynchronized_pool_resource res;
    auto param = parameter::_resource = &res;
    {

        accumula::WindowFilter<Value,
                               TimePoint,
                               Accumulate<Value>>
            acc{param};
    }
    {

        accumula::WindowFilter<Value,
                               TimePoint,
                               Accumulate<Value>>
            acc{param};
    }
    {

        accumula::
            WindowFilter<Value, TimePoint, MinMax<Value>>
            acc{param};
    }
}
int main(int argc, char **argv)
{
    construct_only();

    std::pmr::unsynchronized_pool_resource res;
    auto param = parameter::_resource = &res;
    accumula::WindowFilter<Value, TimePoint, Sum<Value>> a{
        param};

    accumula::WindowFilter<Value, TimePoint, Sum<Value>>
        sum2{param};

    a.window = TimePoint{1.0};
    a.add(Value{1.0, 1});
    a.add(Value{2.0, 2});
    a.add(Value{3.0, 3});
    a.add(Value{4.0, 4});

    fmt::print("{}:{}\n", a.back().key, a.back().value);
    fmt::print("{}:{}\n", a.front().key, a.front().value);
    fmt::print("{}\n", a.window.key);
}
