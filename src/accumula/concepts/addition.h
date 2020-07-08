#pragma once

namespace accumula
{
template <class A, class B>
concept Addition = requires (A a, B b)
{
    a + b;
};

template <class A, class B>
concept InplaceAddition = requires (A a, B b)
{
    a += b;
};
}
