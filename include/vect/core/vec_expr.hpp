#pragma once
#include <cmath>
#include <cstddef>
#include <utility>
#include "vect/core/vec_expr_iterator.hpp"
#include "vect/core/vector_iterator.hpp"
namespace vect::core
{
    template <typename Derived>
    class VecExpr
    {
    protected:
        VecExpr() = default;

    public:
        [[nodiscard]] auto self() const -> const Derived &
        {
            return static_cast<const Derived &>(*this);
        }

        auto operator[](size_t idx) const
        {
            return self()[idx];
        }

        [[nodiscard]] auto size() const -> size_t
        {
            return self().size();
        }
        friend Derived;

        template <size_t... Indices>
        constexpr auto swizzle() const;

        [[nodiscard]] constexpr auto loadPacket(size_t idx) const
        {
            return static_cast<const Derived *>(this)->loadPacket(idx);
        }

        [[nodiscard]] constexpr auto loadPacketUnaligned(size_t idx) const
        {
            return static_cast<const Derived *>(this)->loadPacketUnaligned(idx);
        }

        auto begin() const {return ExprIterator<Derived>(self(), 0);}
        auto end() const {return ExprIterator<Derived>(self(), Derived::dim);}


        // For structured bindings if ur into that sorta thing
        template <size_t I>
        auto get() {
            static_assert(I < Derived::dim, "Index out of bounds");
            return self()[I];
        }

    };

    template <typename T>
    concept IsVecExpr = std::derived_from<std::decay_t<T>, VecExpr<std::decay_t<T>>>;

    template <typename T>
    concept IsVec3Expr = IsVecExpr<T> && T::dim == 3;
}

namespace std {
    template <vect::core::IsVecExpr E>
    struct tuple_size<E> : std::integral_constant<size_t, E::dim> {};

    template <size_t I, vect::core::IsVecExpr E>
    struct tuple_element<I, E> {
        using type = typename E::valueType;
    };

}