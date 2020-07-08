#pragma once

#include <aaqt/core/base/types.h>
#include "concepts.h"


namespace aa
{
using namespace aa;
struct GetOriginal
{
    template <class T = HQOrder>
    AA_FORCE_INLINE Volume operator()(const T &tick) const
    {
        return tick;
    }
};
struct GetVolume
{
    template <class T = HQOrder>
    AA_FORCE_INLINE Volume operator()(const T &tick) const
    {
        return tick.volume;
    }
    inline Volume operator()(const Snapshot &tick) const
    {
        throw std::logic_error("snapshot tick should be ignored!");
    }
};

struct GetPrice
{
    template <class T = HQTrade>
    AA_FORCE_INLINE Price operator()(const T &tick) const
    {
        return tick.price;
    }

    AA_FORCE_INLINE Price operator()(const Snapshot &tick) const
    {
        return tick.last_price;
    }
};

}// namespace aa