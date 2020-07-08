#pragma once
#include <accumula/concepts/basic.h>
#include <array>

#include <boost/mp11/mpl.hpp>

#include "linked_accumulator.h"

static_assert(false); // not finished.
namespace aa
{

using namespace aa;
namespace mp = boost::mp11;


//! reentrant: false
template <class ValueType,
          size_t size,
          class... _Accumulators>
struct FixedWindowStorageImpl
    : LinkedAccumulator<_Accumulators...>
{
public:
    using Accumulators =
        LinkedAccumulator<_Accumulators...>;
    using base = Accumulators;
    using Value = ValueType;

    // property getters
public:
    [[nodiscard]] inline const Value &newest() const
    {
        return _value;
    }

    //////////////////////////////////////////////////////////////////////////
    // internal:
protected:
    AA_FORCE_INLINE void add(const Value &new_value)
    {
        // hook add
        Accumulators::add(new_value);
        // hook erase
        Accumulators::remove(_value);
        _value = new_value;
    }
    AA_FORCE_INLINE void remove(const Value &value)
    {
        throw std::runtime_error("not implemented");
    }

private:
    Value _value;
};

}// namespace aa
