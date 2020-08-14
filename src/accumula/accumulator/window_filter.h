#pragma once
#include <accumula/concepts/basic.h>
#include <accumula/concepts/subtraction.h>
#include <accumula/parameters/parameters.h>
#include <deque>
#include <memory_resource>

#include <boost/mp11/mpl.hpp>

#include "linked_accumulator.h"

namespace accumula
{
namespace mp = boost::mp11;

struct GetValue
{
    template <class T>
    inline T &operator()(const T &val) const noexcept
    {
        return val;
    }
};

struct GetRefWrapper
{
    template <class T>
    inline decltype(auto) operator()(
        const T &val) const noexcept
    {
        return std::ref(val);
    }
};

//! reentrant: false
template <class _Getter,
          class _Value,
          class _Window,
          class... _Accumulators>
requires Subtraction<_Value,
                     _Window>//
    struct WindowFilterG
    : LinkedAccumulator<_Accumulators...>
{
public:
    using Accumulators =
        LinkedAccumulator<_Accumulators...>;
    using base = Accumulators;
    using Getter = _Getter;
    using Value = _Value;
    using Window = _Window;
    using Values = std::pmr::deque<Value>;

    // arguments
public:
    Window window;
    Getter getter;

    // helper variables
private:
    Values _datas;
    bool _filled = false;

    // constructors
public:
    template <class Args>
    constexpr WindowFilterG(const Args &args)
        : base(args)
        , _datas(args[parameter::_resource
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
                _filled = true;
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
    }
};

template <class _Value,
          class _Window,
          class... _Accumulators>
using WindowFilter = WindowFilterG<GetValue,
                                   _Value,
                                   _Window,
                                   _Accumulators...>;


}// namespace accumula
