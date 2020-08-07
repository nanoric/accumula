#pragma once
#include <accumula/concepts/basic.h>
#include <deque>
#include <memory_resource>

#include <boost/mp11/mpl.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/preprocessor.hpp>

#include "accumula/concepts/addition.h"
#include "accumula/concepts/subtraction.h"

namespace accumula
{

namespace parameter
{
BOOST_PARAMETER_NAME(init_value)
}// namespace parameter

/*! Accumulate using AddOp or RemoveOp
 * reentrant: false
 */
template <class _SumType,
          class AddOp = std::plus<_SumType>,
          class RemoveOp = std::minus<_SumType>>
struct Accumulate
{
public:
    using SumType = _SumType;

private:
    SumType _sum;

    // constructors
public:
    template <class Args>
    constexpr Accumulate(const Args &args)
        : _sum(args[parameter::_init_value | SumType{}])
    {
    }

    // property getters
public:
    inline constexpr SumType &sum() const
    {
        return _sum;
    }

    //////////////////////////////////////////////////////////////////////////
    // internal:
public:
    template <class Value>
    inline void add(const Value &value)
    {
        this->_sum = AddOp{}(this->_sum, value);
    }
    template <class Value>
    inline void remove(const Value &value)
    {
        this->_sum = RemoveOp{}(this->_sum, value);
    }
};

/*! Calculate Sum using +=
 * reentrant: false
 */
template <class _SumType>
struct Sum
{
public:
    using SumType = _SumType;

private:
    SumType _sum;

    // constructors
public:
    template <class Args>
    constexpr Sum(const Args &args)
        : _sum(args[parameter::_init_value | SumType{}])
    {
    }

    // property getters
public:
    inline constexpr SumType &sum() const
    {
        return _sum;
    }

    //////////////////////////////////////////////////////////////////////////
    // internal:
public:
    template <class Value>
    inline void add(const Value &value)
    {
        this->_sum += value;
    }
    template <class Value>
    inline void remove(const Value &value)
    {
        this->_sum -= value;
    }
};

template <class _Getter, class _SumType = uint64_t>
struct SumG
{
public:
    using SumType = _SumType;

private:
    SumType _sum;

    // constructors
public:
    template <class Args>
    constexpr SumG(const Args &args)
        : _sum(args[parameter::_init_value | SumType{}])
    {
    }

    template <class T>
    requires std::is_same_v<T, SumG>//
        SumG &get()
    {
        return *this;
    }


    // property getters
public:
    inline constexpr const SumType &sum() const
    {
        return _sum;
    }

    //////////////////////////////////////////////////////////////////////////
    // internal:
public:
    template <class Value>
    inline void add(const Value &value)
    {
        this->_sum += _Getter{}(value);
    }
    template <class Value>
    inline void remove(const Value &value)
    {
        this->_sum -= _Getter{}(value);
    }
};

}// namespace accumula
