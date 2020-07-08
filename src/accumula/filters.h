#pragma once

#include <aaqt/core/base/types.h>
#include <aaqt/core/base/macros.h>
#include "concepts.h"

namespace aa
{
using namespace aa;


struct UseAll
{
    template <class T>
    constexpr AA_FORCE_INLINE bool operator()(const T &) const
    {
        return true;
    }
};


struct IgnoreCanceledTick
{
    template <class T>
    constexpr AA_FORCE_INLINE bool operator()(const T &) const
    {
        return true;
    }
    AA_FORCE_INLINE bool operator()(const HQTrade &tick) const
    {
        return tick.type != TradeType::Cancel;
    }
};


template <class T>
struct Ignore
{
    template <class S>
    constexpr AA_FORCE_INLINE bool operator()(const S &) const
    {
        return true;
    }
    constexpr AA_FORCE_INLINE bool operator()(const T &) const { return false; }
};

struct IgnoreSnapshot: Ignore<Snapshot>
{};
struct IgnoreHQTrade: Ignore<HQTrade>
{};
struct IgnoreHQOrder: Ignore<HQOrder>
{};

template <class T>
struct UseOnly
{
    template <class S>
    constexpr AA_FORCE_INLINE bool operator()(const S &) const
    {
        return false;
    }
    constexpr AA_FORCE_INLINE bool operator()(const T &) const { return true; }
};


template <class... Filters>
struct LinkedFilter;

template <class CurrentFilter, class... Filters>
struct LinkedFilter<CurrentFilter, Filters...>
{
    template <class T>
    constexpr AA_FORCE_INLINE bool operator()(const T &tick) const
    {
        return CurrentFilter{}(tick) && LinkedFilter<Filters...>{}(tick);
    }
};
template <class CurrentFilter>
struct LinkedFilter<CurrentFilter>
{
    template <class T>
    constexpr AA_FORCE_INLINE bool operator()(const T &tick) const
    {
        return CurrentFilter{}(tick);
    }
};

template <>
struct LinkedFilter<>
{
    template <class T>
    constexpr AA_FORCE_INLINE bool operator()(const T &) const
    {
        return true;
    }
};

template <>
struct LinkedFilter<void>: LinkedFilter<>
{};

#if HAS_CONCEPTS()
static_assert(Filter<IgnoreCanceledTick>);
static_assert(IsFilter<IgnoreCanceledTick>::value);
#endif

}// namespace aa
