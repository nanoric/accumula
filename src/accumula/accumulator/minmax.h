#pragma once
#include <accumula/macros.h>
#include <accumula/parameters/parameters.h>
#include <map>
#include <memory_resource>

#include <boost/mp11/mpl.hpp>


namespace accumula
{


/*! Accumulate using AddOp or RemoveOp
 * reentrant: false
 */
template <class _ValueType = int,
          class CompOp = std::less<_ValueType>>
struct MinMax
{
public:
    using Value = _ValueType;
    using Values = std::pmr::map<Value, uint32_t, CompOp>;

private:
    Values _values;
#if ACCUMULA_IS_DEBUG()
    int n = 0;
#endif

    // constructors
public:
    template <class Args>
    constexpr MinMax(const Args &args)
        : _values(args[parameter::_allocator
                       | std::pmr::new_delete_resource()])
    {
    }

    // property getters
public:
    inline constexpr bool empty() const
    {
        return _values.empty();
    }
    inline constexpr bool has_minmax() const
    {
        return _values.size() > 0;
    }
    inline constexpr const Value &min() const
    {
        return _values.begin()->first;
    }
    inline constexpr const Value &max() const
    {
        return _values.rbegin()->first;
    }

    //////////////////////////////////////////////////////////////////////////
    // internal:
public:
    inline static auto comp()
    {
        return CompOp{};
    }
    inline static bool equal(const Value &value1,
                             const Value &value2)
    {
        return !comp()(value1, value2)
               && !comp()(value2, value1);
    }
    inline void add(const Value &value)
    {
        auto it = _values.lower_bound(value);
        if(it != _values.end() && equal(it->first, value))
        {
            ++it->second;
        }
        else
        {
            auto it2 = this->_values.emplace_hint(
                it,
                std::make_pair(value, 1));
        }
    }
    inline void remove(const Value &value)
    {
        auto it = _values.find(value);
        BOOST_ASSERT(it
                     != _values.end());// value must exist

        if(--it->second == 0)
        {
            _values.erase(it);
        }
    }
};

}// namespace accumula
