#pragma once
#include <accumula/concepts/basic.h>
#include <accumula/concepts/subtraction.h>
#include <accumula/macros.h>
#include <accumula/parameters/parameters.h>
#include <deque>
#include <memory_resource>

#include <boost/mp11/mpl.hpp>

#include "linked_accumulator.h"

namespace accumula
{
namespace mp = boost::mp11;

template <class _Value>
using WindowFilterDefaultContainer =
    std::pmr::deque<_Value>;


//! reentrant: false
template <class _Value,
          class _Window,
          class _Container =
              WindowFilterDefaultContainer<_Value>,
          class... _Accumulators>
requires Subtraction<_Value,
                     _Window>//
    struct BasicWindowFilter
    : LinkedAccumulator<_Accumulators...>
{
public:
    using Accumulators =
        LinkedAccumulator<_Accumulators...>;
    using base = Accumulators;
    using Value = _Value;
    using Window = _Window;
    using Container = _Container;
    using Values = Container;

    // arguments
public:
    Window window;

    // helper variables
private:
    Values _datas;
    bool _filled = false;
#if ACCUMULA_IS_DEBUG()
    int _debug = 0;
#endif

    // constructors
public:
    template <class Args>
    constexpr BasicWindowFilter(const Args &args)
        : base(args)
        , _datas(args[parameter::_allocator
                      | std::pmr::new_delete_resource()])
    {
    }

    Accumulators &accumulators()
    {
        return *this;
    }

    // property getters
public:
    [[nodiscard]] inline auto begin() const
    {
        return _datas.begin();
    }
    [[nodiscard]] inline auto end() const
    {
        return _datas.end();
    }
    [[nodiscard]] inline auto rbegin() const
    {
        return _datas.rbegin();
    }
    [[nodiscard]] inline auto rend() const
    {
        return _datas.rend();
    }
    [[nodiscard]] inline const Value &front() const
    {
        return _datas.front();
    }
    [[nodiscard]] inline const Value &back() const
    {
        return _datas.back();
    }
    [[nodiscard]] inline bool empty() const noexcept
    {
        return _datas.empty();
    }
    [[nodiscard]] inline bool filled() const noexcept
    {
        return this->_filled;
    }
    [[nodiscard]] inline size_t size() const
    {
        return _datas.size();
    }

    //! real-time window size.(returns back() - fron())
    [[nodiscard]] constexpr decltype(auto)
    current_window_size() const
    {
        if(_datas.size() == 0)
            return Window{};
        return back() - front();
    }

public:
    inline void clear()
    {
        this->_datas.clear();
        this->_filled = false;
    }

    inline void push_back(const Value &value)
    {
        auto &stored_value = _datas.emplace_back(value);
#if ACCUMULA_IS_DEBUG()
        if(_debug == 0)
            int a = 1;
#endif

        // hook add
        Accumulators::add(stored_value);

        auto oldest_time = stored_value - window;

        // delete all old data
        for(auto i = _datas.begin(); i != _datas.end();)
        {
            const auto &value = *i;
            if(value < oldest_time)
            {
                // hook remove
                Accumulators::remove(value);

                i = _datas.erase(i);
                _filled = true;
                continue;
            }
            else
            {
                break;
            }
        }
#if ACCUMULA_IS_DEBUG()
        if(_debug == 0)
            int a = 1;
#endif
    }

    // works only under DEBUG mode.
    void debug(int value)
    {
#if ACCUMULA_IS_DEBUG()
        _debug = value;
#endif
    }

    //////////////////////////////////////////////////////////////////////////
    // internal:
public:
    inline void add(const Value &value)
    {
        return push_back(value);
    }
    inline void remove(const Value &value)
    {
    }
};

template <class _Value,
          class _Window,
          class... _Accumulators>
using WindowFilter =
    BasicWindowFilter<_Value,
                      _Window,
                      WindowFilterDefaultContainer<_Value>,
                      _Accumulators...>;


}// namespace accumula
