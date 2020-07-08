#pragma once
#include <map>
#include <memory_resource>
#include <optional>
#include <set>

#include <boost/operators.hpp>

#include <aaqt/core/base/macros.h>
#include "linkedaccumulator.h"

namespace aa
{
namespace internal
{

template <class Predicate>
struct ReversePredicate
{
    template <class T, class S>
    AA_FORCE_INLINE bool operator()(const T &a, const S &b)
    {
        return !Predicate{}(a, b);
    }
};

template <class Pred, class Iterator, class Func, class ValueType>
AA_FORCE_INLINE void move_edge_to_right(const ValueType &value,
                                        Iterator &edge,
                                        Iterator end,
                                        Func &&process)
{
    Pred pred;
    if(edge == end)
        return;
    // execute at the last equal one
    auto it = ++Iterator(edge);
    for(it; it != end; ++it)
    {
        AA_IF_UNLIKELY(!pred(*it, value))
        {
            break;
        }
        process(--Iterator(it));
    }
    edge = it;
}
template <class Pred, class Iterator, class Func, class ValueType>
AA_FORCE_INLINE void move_edge_to_left(const ValueType &value,
                                       Iterator &edge,
                                       Iterator beg,
                                       Func &&process)
{
    auto it = std::make_reverse_iterator(Iterator(edge));
    auto lend = std::make_reverse_iterator(beg);
    move_edge_to_right<Pred>(value, it, lend, std::move(process));
    edge = it.base();
}

template <class Predicate,
          class ValueType,
          class Iterator,
          // class Limit,
          class Add,
          class Erase>
void move_edge(const ValueType &value,
               std::optional<ValueType> &limit,
               Iterator &edge,
               Iterator begin,
               Iterator end,
               Add &&add,
               Erase &&erase)
{
    Predicate pred;
    if(!limit || pred(value, *limit))
    {
        move_edge_to_left<Predicate>(value, edge, begin, std::move(add));
    }
    // >=
    else if(!pred(value, *limit))
    {
        // ==
    }
    else
    {
        // value > limit
        move_edge_to_right<ReversePredicate<Predicate>>(value,
                                                        edge,
                                                        end,
                                                        std::move(add));
    }
    limit = value;
}

}// namespace internal

template <class ValueType = double,
          class _Predicate = std::less<ValueType>,
          class... _Accumulators>
struct RangeAccumulator: LinkedAccumulator<_Accumulators...>
{
public:
    using Accumulators = LinkedAccumulator<_Accumulators...>;
    using base = Accumulators;
    using Value = ValueType;
    using CValueRef = const Value &;
    using Predicate = _Predicate;
    using Container = std::pmr::multiset<ValueType>;
    using Iterator = typename Container::iterator;

public:
    AA_FORCE_INLINE void set_lower_limit(CValueRef value)
    {
        internal::move_edge<Predicate>(
            value,
            lower_limit,
            lower_edge,
            datas.begin(),
            datas.end(),
            [this](const auto &value) {
                Accumulators::add(value);
            },
            [this](const auto &value) {
                Accumulators::remove(value);
            });
    }
    AA_FORCE_INLINE void set_upper_limit(CValueRef value)
    {
        using Reversed = internal::ReversePredicate<Predicate>;
        internal::move_edge<Reversed>(
            value,
            upper_limit,
            upper_edge,
            datas.rbegin(),
            datas.rend(),
            [this](const auto &value) {
                Accumulators::add(value);
            },
            [this](const auto &value) {
                Accumulators::remove(value);
            });
    }

    AA_FORCE_INLINE void add(CValueRef value)
    {
        Predicate pred;
        AA_IF_UNLIKELY(!lower_limit || !upper_limit)
        {
            datas.emplace_hint(lower_edge, value);// log
        }
        else AA_IF_UNLIKELY(pred(value, *lower_limit))// value < lower_limit
        {
            datas.emplace_hint(lower_edge, value);// const
        }
        else AA_IF_UNLIKELY(!pred(*lower_limit, value))
        // value == lower_limit, insertion changes lower_edge
        {
            // todo: test if emplace_hint will insert at lower_bound if key
            // exists.
            lower_edge = datas.emplace_hint(lower_edge, value);// const
        }
        else AA_IF_UNLIKELY(pred(*upper_limit, value))// value > upper_limit
        {
            datas.emplace_hint(datas.end(), value);// log
        }
        else
        // inside edges :
        {
            datas.emplace_hint(upper_bound(), value);// log
            Accumulators::add(value);
        }
    }
    AA_FORCE_INLINE void erase(CValueRef value)
    {
        Predicate pred;
        AA_IF_UNLIKELY(!lower_limit || !upper_limit)
        {
            datas.erase(value);// log
        }
        else AA_IF_UNLIKELY(pred(value, *lower_limit))// value < lower_limit
        {
            datas.erase(value);// log
        }
        else AA_IF_UNLIKELY(!pred(*lower_limit, value))// value == lower_limit
        {
            lower_edge = datas.erase(lower_edge);// const
            Accumulators::erase(value);
        }
        else AA_IF_UNLIKELY(pred(*upper_limit, value))// value > upper_limit
        {
            datas.erase(value);// log
        }
        else AA_IF_UNLIKELY(!pred(value, *upper_limit))// value == upper_limit
        {
            upper_edge = datas.erase(upper_edge);// const
            Accumulators::erase(value);
        }
        else
        {
            if(!pred(value, *lower_edge)
               && !pred(*lower_edge, value))// value == *lower_edge
            {
                lower_edge = datas.erase(lower_edge);// const
            }
            else if(!*pred(value, *upper_edge)// value == *upper_edge
                    && !pred(*upper_edge, value))
            {
                upper_edge = datas.erase(upper_edge);// const
            }
            else
            {
                datas.erase(value);// log
            }
            Accumulators::erase(value);
        }
    }

private:
    Container datas;
    Iterator lower_edge = datas.end();
    std::reverse_iterator<Iterator> upper_edge = datas.rend();
    AA_FORCE_INLINE auto upper_bound() const
    {
        return upper_edge.base();
    }
    std::optional<Value> upper_limit;
    std::optional<Value> lower_limit;
};

#if HAS_CONCEPTS()
static_assert(Accumulator<RangeAccumulator<double, std::less<double>, void>>);
#endif
}// namespace aa