#pragma once

//#include <concepts>  needs gcc 10 which is under dev.
#include <boost/mp11/mpl.hpp>

namespace accumula
{
namespace mp = boost::mp11;


// Accumulator
template <class T>
concept Accumulator = requires(T x, typename T::Value v)
{
    x.add(v);
    x.remove(v);
};
static_assert(!Accumulator<int>, "false positive");


template <class T>
struct IsAccumulator: std::false_type
{
};

template <class T>
requires Accumulator<T>//
    struct IsAccumulator<T>: std::true_type
{
};


static_assert(!IsAccumulator<int>::value, "false positive");


template <auto>
struct require_constant;

template <class Func, class... Args>
concept is_constexpr_c = requires
{
    typename require_constant<Func{}(
        *std::add_pointer_t<Args>{}...)>;
};

template <class...>
struct is_constexpr: std::false_type
{
};

template <class Func, class... Args>
requires is_constexpr_c<Func, Args...>
    //
    struct is_constexpr<Func, Args...>: std::true_type
{
};

template <class Func, class... Args>
constexpr bool is_constexpr_v =
    is_constexpr<Func, Args...>::value;

}// namespace accumula
