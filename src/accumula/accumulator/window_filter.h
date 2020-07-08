#pragma once
#include <accumula/concepts/basic.h>
#include <deque>
#include <memory_resource>

#include <boost/mp11/mpl.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/preprocessor.hpp>

#include "accumula/concepts/subtraction.h"
#include "linked_accumulator.h"

namespace accumula
{

namespace mp = boost::mp11;

namespace parameter
{
BOOST_PARAMETER_NAME(window_size)
}// namespace parameter


//! reentrant: false
template <class ValueType,
          class WindowType,
          class... _Accumulators>
requires Subtraction<ValueType,
                     WindowType>//
    struct WindowFilter: LinkedAccumulator<_Accumulators...>
{

public:
    using Accumulators =
        LinkedAccumulator<_Accumulators...>;
    using base = Accumulators;
    using Value = ValueType;
    using Window = WindowType;
    using Values = std::pmr::deque<Value>;

    // arguments
public:
    Window window;

    // helper variables
private:
    std::pmr::unsynchronized_pool_resource memory;
    Values _datas = Values(&memory);

    // constructors
public:
    template <class Args>
    constexpr WindowFilter(const Args &args)
        : base(args)
        , window(args[parameter::_window_size])
    {
    }

    // property getters
public:
    [[nodiscard]] inline const Value &front() const
    {
        return _datas.front();
    }
    [[nodiscard]] inline const Value &back() const
    {
        return _datas.back();
    }
    [[nodiscard]] inline size_t size() const
    {
        return _datas.size();
    }

    [[nodiscard]] constexpr decltype(auto)
    current_window_size() const
    {
        if(_datas.size() == 0)
            return Window{};
        return back() - front();
    }
    inline void puah_back(const Value &value)
    {

        _datas.emplace_back(value);

        // hook add
        Accumulators::add(value);

        auto oldest_time = value - window;

        // delete all old data
        for(auto i = _datas.begin(), e = _datas.end();
            i != e;)
        {
            const auto &value = *i;
            if(value < oldest_time)
            {
                // hook remove
                Accumulators::remove(value);

                i = _datas.erase(i);
                continue;
            }
            else
            {
                break;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // internal:
public:
    inline void add(const Value &value)
    {
        return puah_back(value);
    }
    inline void remove(const Value &value)
    {
        throw std::runtime_error("not implemented");
    }
};


}// namespace accumula
