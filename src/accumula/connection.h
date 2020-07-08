#pragma once
#include <boost/mp11/mpl.hpp>

#include <aaqt/core/base/types.h>
#include <aaqt/core/base/macros.h>
namespace aa
{
using namespace aa;
namespace mp = boost::mp11;

template <class _Getter, class _Storage>
struct Connection: _Storage
{
    using Getter = _Getter;
    using Storage = _Storage;

public:
    template <class T>
    constexpr AA_FORCE_INLINE void add(const T &tick)
    {
        Storage::add(Getter{}(tick));
    }
    template <class T>
    constexpr AA_FORCE_INLINE void remove(const T &tick)
    {
        Storage::remove(Getter{}(tick));
    }
};

template <class... _Connections>
struct LinkedConnection;

template <class C, class... _Connections>
struct LinkedConnection<C, _Connections...>: C,
                                             LinkedConnection<_Connections...>
{
    using Current = C;
    using Rest = LinkedConnection<_Connections...>;
    template <class T>
    /*requires ESymbol<T>*/
    constexpr AA_FORCE_INLINE void add(const T &tick)
    {
        Current::add(tick);
        Rest::add(tick);
    }
    template <class T>
    constexpr AA_FORCE_INLINE void remove(const T &tick)
    {
        Current::remove(tick);
        Rest::remove(tick);
    }
};

template <class C>
struct LinkedConnection<C>: C
{
    using Current = C;
    template <class T>
    constexpr AA_FORCE_INLINE void add(const T &tick)
    {
        Current::add(tick);
    }
    template <class T>
    constexpr AA_FORCE_INLINE void remove(const T &tick)
    {
        Current::remove(tick);
    }
};

template <>
struct LinkedConnection<>
{
    template <class T>
    constexpr AA_FORCE_INLINE void add(const T &tick)
    {}
    template <class T>
    constexpr AA_FORCE_INLINE void remove(const T &tick)
    {}
};

template <>
struct LinkedConnection<void>: LinkedConnection<>
{};


}// namespace aa
