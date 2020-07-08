#pragma once
#include <list>
#include <memory_resource>

#include <boost/mp11/mpl.hpp>

#include <aaqt/core/base/macros.h>
#include <aaqt/core/base/strategy.h>
#include <aaqt/core/base/types.h>
#include "concepts.h"
#include "aaqt/core/base/timer.h"
#include "linkedaccumulator.h"

namespace aa
{

using namespace aa;
namespace mp = boost::mp11;

template <class _Clock>
struct Bar: TimePoint<_Clock>
{
    using Clock = _Clock;

public:
    Price open = 0;
    Price close = 0;
    Price high = std::numeric_limits<Price>::min();
    Price low = std::numeric_limits<Price>::max();
    Volume volume = 0;
    template <class T>
    void update(const T &value) noexcept
    {
        Price price = value.price;
        Volume volume = value.volume;
        this->close = price;
        this->high = std::max(this->high, price);
        this->low = std::min(this->low, price);
        this->volume += volume;
    }
    void next() noexcept
    {
        auto close = this->close;
        this->open = this->close = this->high = this->low =
            close;
        this->volume = 0;
    }
};

//! reentrant: false
template <class _BarType = Bar,
          class... _Accumulators>
struct BarGenerator: LinkedAccumulator<_Accumulators...>
{
public:
    using Accumulators =
        LinkedAccumulator<_Accumulators...>;
    using base = Accumulators;
    using BarType = _BarType;
    // arguments
public:
    BaseTimer _timer;

private:
    bool _started = false;
    bool _initialized = false;
    BarType _bar;

public:
    // constructors
public:
    constexpr BarGenerator(MicroSeconds duration)
    {
        _timer.set_repeat(true);
        _timer.set_interval(duration);
        _timer.set_handler([this]() {
            this->on_timer();
        });
    }
    inline void set_duration(MicroSeconds duration)
    {
        this->window = duration;
    }
    inline BaseTimer *timer() noexcept
    {
        return &_timer;
    }

    // property getters
protected:
    void on_timer()
    {
        AA_IF_LIKELY(_started)
        {
            AA_IF_UNLIKELY(!_initialized)
            {
                _initialized = true;
                _bar.next();
                return;// the first bar generated (partial)
                       // will be ignored.
            }

            // hook add
            Accumulators::add(_bar);
            _bar.next();
        }
    }

public:
    template <class T> // should use only trade tick.
    // todo: add concepts to ensure type of tick is only "public trade" ticks.
    AA_FORCE_INLINE void add(const T &tick)
    {
        _bar.update(tick);
        _started = true;
    }
    template <class T>
    AA_FORCE_INLINE void remove(const T &value)
    {
        throw std::runtime_error("not implemented");
    }
};
}// namespace aa
