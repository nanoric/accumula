#pragma once
#include <list>
#include <memory_resource>

#include <boost/mp11/mpl.hpp>

#include <aaqt/core/base/macros.h>
#include <aaqt/core/base/strategy.h>
#include <aaqt/core/base/types.h>
#include "concepts.h"
#include "linkedaccumulator.h"

namespace aa
{
namespace accumulator
{
// Storage
template <class T>
concept Storage = requires(T x,
                           SysTime<> tp,
                           typename T::Value v)
{
    x.update(tp, v);
};

}// namespace accumulator
}// namespace aa
