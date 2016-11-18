#pragma once

template<typename... Types>
class Visitor;

template<typename T>
class Visitor<T>
{
public:
    virtual void visit(const T & visitable) = 0;
};

template<typename T, typename... Types>
class Visitor<T, Types...> : public Visitor<Types...>
{
public:
    using Visitor<Types...>::visit;

    virtual void visit(const T & visitable) = 0;
};

template<typename Derived, typename T, typename Base>
class VisitableWithBase : public Base
{
public:
    template <typename... Types>
    VisitableWithBase(Types... args) : Base(args...) {}

    virtual void accept(T & visitor) const
    {
        visitor.visit(static_cast<const Derived&>(*this));
        Base::accept(visitor);
    }
};