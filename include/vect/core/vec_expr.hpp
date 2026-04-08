#pragma once
#include <cstddef>
#include <utility>
#include "vect/core/vec_expr_iterator.hpp"
#include "vect/detail/simd_traits.hpp"
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

        template <typename Op, typename T>
        [[nodiscard]] auto reduce(Op op, T init) const {
            static constexpr size_t N = Derived::dim;

            using Traits = detail::SimdTraits<T, N>;

            size_t idx = 0;

            T acc = init;

            if constexpr (Traits::available && N >= Traits::width) {
                auto packetAcc = detail::broadcast(init);

                for (; idx <= N - Traits::width; idx += Traits::width) {
                    packetAcc = op(packetAcc, self().loadPacket(idx));
                }
            }

            for (; idx < N; ++idx) {
                acc = op(acc, self()[idx]);
            }

            return acc;
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