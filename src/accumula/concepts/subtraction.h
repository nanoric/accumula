#pragma

namespace accumula
{
template <class A, class B>
concept Subtraction = requires (A a, B b)
{
    a - b;
};
}
