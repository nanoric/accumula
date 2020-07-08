#pragma once

#include <list>
#include "concepts.h"

namespace aa
{

template <class... _Storages>
struct LinkedStorage;

template <class S, class... _Storages>
/*requires Storage<S>*/
struct LinkedStorage<S, _Storages...>
    : S, LinkedStorage<_Storages...>
{
    using Current = S;
    using Rest = LinkedStorage<_Storages...>;
    AA_FORCE_INLINE void update(
        SysTime<> datetime,
        typename Current::Value value)
    {
        Current::update(datetime, value);
        Rest::update(datetime, value);
    }
};

template <class S>
/*requires Storage<S> */
struct LinkedStorage<S>: S
{
    using Current = S;
    AA_FORCE_INLINE void update(
        SysTime<> datetime,
        typename Current::Value value)
    {
        Current::update(datetime, value);
        ;
    }
};

template <>
struct LinkedStorage<>
{
    template <class T>
    AA_FORCE_INLINE void update(SysTime<> datetime,
                                T &value)
    {
    }
};

}// namespace aa
