#pragma once

namespace accumula
{

template <bool revered = false, class... Args>
struct LinkedAccumulatorImpl;

template <bool reversed, class _Current, class... _Nexts>
struct LinkedAccumulatorImpl<reversed, _Current, _Nexts...>
{
    using Next = LinkedAccumulatorImpl<reversed, _Nexts...>;
    using Current = _Current;

private:
    Current _current;
    Next _next;

public:
    template <class Args>
    LinkedAccumulatorImpl(const Args &args)
        : _current(args)
        , _next(args)
    {
    }
    Current &current()
    {
        return _current;
    }
    Next &nexts()
    {
        return _next;
    }

    const Current &current() const noexcept
    {
        return _current;
    }
    const Next &nexts() const noexcept
    {
        return _next;
    }

public:
    template <class T>
    inline void add(const T &value)
    {
        if constexpr(!reversed)
        {
            _current.add(value);
            _next.add(value);
        }
        //else
        //{
        //    _current.remove(value);
        //    _next.remove(value);
        //}
    }
    template <class T>
    inline void remove(const T &value)
    {
        if constexpr(!reversed)
        {
            _current.remove(value);
            _next.remove(value);
        }
        else
        {
            _current.add(value);
            _next.add(value);
        }
    }
};

template <bool reversed, class _Current>
struct LinkedAccumulatorImpl<reversed, _Current>: _Current
{
    using Current = _Current;

public:
    using Current::Current;
    Current &current()
    {
        return *this;
    }
    const Current &current() const noexcept
    {
        return *this;
    }
    void nexts() const noexcept
    {
        return;
    }

public:
    template <class T>
    inline void add(const T &value)
    {
        if constexpr(!reversed)
        {
            Current::add(value);
        }
        //else
        //{
        //    Current::remove(value);
        //}
    }
    template <class T>
    inline void remove(const T &value)
    {
        if constexpr(!reversed)
        {
            Current::remove(value);
        }
        else
        {
            Current::add(value);
        }
    }
};

template <bool reversed>
struct LinkedAccumulatorImpl<reversed>
{
public:
    template <class Args>
    LinkedAccumulatorImpl(const Args &args)
    {
    }

    void current() const noexcept
    {
        return;
    }
    void nexts() const noexcept
    {
        return;
    }

public:
    template <class T>
    inline void add(const T &)
    {
    }
    template <class T>
    inline void remove(const T &)
    {
    }
};

template <class... Args>
using LinkedAccumulator =
    LinkedAccumulatorImpl<false, Args...>;

template <class... Args>
using ReversedLinkedAccumulator =
    LinkedAccumulatorImpl<true, Args...>;


}// namespace accumula
