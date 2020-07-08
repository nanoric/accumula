#pragma once
#include <accumula/concepts/basic.h>
namespace aa
{

template <class... Args>
struct LinkedAccumulator;

template <class C, class... Args>
//#if HAS_CONCEPTS()
//requires Accumulator<C>
//#endif
struct LinkedAccumulator<C, Args...>
    : C, LinkedAccumulator<Args...>
{
    using base = LinkedAccumulator<Args...>;
    using Current = C;

public:
    template <class T>
    inline void add(const T &value)
    {
        Current::add(value);
        base::add(value);
    }
    template <class T>
    inline void remove(const T &value)
    {
        Current::remove(value);
        base::remove(value);
    }
};

template <class C>
//#if HAS_CONCEPTS()
//requires Accumulator<C>
//#endif
struct LinkedAccumulator<C>: C
{
    using Current = C;

public:
    template <class T>
    inline void add(const T &value)
    {
        Current::add(value);
        ;
    }
    template <class T>
    inline void remove(const T &value)
    {
        Current::remove(value);
        ;
    }
};

template <>
struct LinkedAccumulator<>
{
public:
    template <class T>
    inline void add(const T &)
    {
    }
    template <class T>
    inline void remove(const T &)
    {
    }
};
}// namespace aa
