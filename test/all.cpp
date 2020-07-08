#include <accumula/accumulator/window_storage.h>
#include <fmt/format.h>

using namespace std;
using namespace accumula;

struct Key
{
    double key;
    Key operator-(const Key &rhs) const noexcept
    {
        return Key{this->key - rhs.key};
    }
    bool operator<(const Key &rhs) const noexcept
    {
        return this->key < rhs.key;
    }
};

struct Value: Key
{
    int value;
    Value(double key, int v)
        : Key{key}
        , value(v)
    {
    }
};

int main(int argc, char **argv)
{
    accumula::WindowStorage<Value, Key> a(
        accumula::parameter::_window_size = Key{2});
    a.add(Value{1.0, 1});
    a.add(Value{2.0, 2});
    a.add(Value{3.0, 3});
    a.add(Value{4.0, 4});
    fmt::print("{}:{}\n", a.back().key, a.back().value);
    fmt::print("{}:{}\n", a.front().key, a.front().value);
    fmt::print("{}\n", a.window.key);
}
