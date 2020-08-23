#pragma once

namespace accumula
{
template <class T>
struct Ref
{
    T *r;
    Ref(T &val)
        : r(&val)
    {
    }
    Ref(const Ref &rhs)
        : r(rhs.r)
    {
    }
    operator T &() const
    {
        return *r;
    }
    operator T &()
    {
        return *r;
    }

#define OP(op)                                             \
    template <class T>                                     \
    auto operator op(const T &rhs) const                   \
    {                                                      \
        return this->get() op rhs;                         \
    }
    OP(+)
    OP(-)
    OP(*)
    OP(/)
    OP(<)
    OP(>)
    OP(<=)
    OP(>=)

    auto &get()
    {
        return *r;
    }
    auto &get() const
    {
        return *r;
    }
    auto operator->() const
    {
        return &this->get();
    }
};
};// namespace accumula
