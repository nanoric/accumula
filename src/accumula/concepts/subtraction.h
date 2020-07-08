#pragma once

namespace accumula
{
template <class A, class B>
concept Subtraction = requires (A a, B b)
{
    a - b;
};
template <class A, class B>
concept InplaceSubtraction = requires (A a, B b)
{
    a - b;
};
}
