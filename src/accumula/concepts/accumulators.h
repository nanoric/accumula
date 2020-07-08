#pragma once
#include <aaqt/core/base/types.h>
#include <aaqt/core/base/macros.h>
#include "concepts.h"
namespace aa
{
using namespace aa;


template <class ValueType, class AddOperator, class EraseOperator>
struct Accumulate
{
public:
    using Value = ValueType;

public:
    AA_FORCE_INLINE Value sum() const { return _sum; }

public:
    AA_FORCE_INLINE void add(const Value &value)
    {
        _sum = AddOperator{}(_sum, value);
    }
    AA_FORCE_INLINE void remove(const Value &value)
    {
        _sum = EraseOperator{}(_sum, value);
    }

private:
    Value _sum = {};
};


template <class ValueType>
using Sum = Accumulate<ValueType, std::plus<ValueType>, std::minus<ValueType>>;

#if HAS_CONCEPTS()
static_assert(Accumulator<Sum<Price>>, "false negative");
static_assert(IsAccumulator<Sum<Price>>::value, "false negative");
#endif

}// namespace aa
