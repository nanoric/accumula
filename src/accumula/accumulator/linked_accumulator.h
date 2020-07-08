#pragma once

#include <accumula/concepts/basic.h>

namespace accumula
{


template <class... Args>
struct LinkedAccumulator;

template <class C, class... Args>
struct LinkedAccumulator<C, Args...>
    : C, LinkedAccumulator<Args...>
{
    using nexts = LinkedAccumulator<Args...>;
    using Current = C;

public:
    template <class Args>
    LinkedAccumulator(const Args &args)
        : Current(args)
        , nexts(args)
    {
    }

public:
    template <class T>
    inline void add(const T &value)
    {
        Current::add(value);
        nexts::add(value);
    }
    template <class T>
    inline void remove(const T &value)
    {
        Current::remove(value);
        nexts::remove(value);
    }
};

template <class C>
//#if HAS_CONCEPTS()
//requires Accumulator<C>
//#endif
struct LinkedAccumulator<C>: C
{
    using Current = C;
    using Current::Current;


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
    template <class Args>
    LinkedAccumulator(const Args &args)
    {
    }

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

}// namespace accumula
