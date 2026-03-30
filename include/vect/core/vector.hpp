#pragma once
#include "vect/detail/simd_resolver.hpp"
#include "vect/detail/simd_traits.hpp"
#include <array>
#include <vect/core/vec_expr.hpp>

#include <bit>
namespace vect::core
{

    template <typename T, size_t N>
    class alignas(detail::SimdTraits<T, N>::alignment) Vector : public VecExpr<Vector<T, N>>
    {
    public:
        static constexpr size_t alignment = detail::SimdTraits<T, N>::alignment;

        alignas(alignment)
            std::array<T, N> data_{};
        using valueType = T;
        static constexpr size_t dim = N;

        /// @brief Direct `VecExpr` initializer
        /// @param expr the `VecExp` to initialize a vector from
        template <typename Expr>
        Vector(const VecExpr<Expr> &expr) : Vector(expr, std::make_index_sequence<N>{})
        {
        }

        constexpr Vector(std::initializer_list<T> list)
        {
            size_t i = 0;
            for (const auto &val : list)
            {
                if (i >= N)
                    break;
                data_[i++] = val;
            }
        }

        constexpr auto operator[](size_t idx) -> T & { return data_[idx]; }
        constexpr auto operator[](size_t idx) const -> const T & { return data_[idx]; }

        [[nodiscard]] constexpr auto size() const -> size_t { return N; }

        template <typename V>
        Vector<T, N> &operator=(const VecExpr<V> &expr)
        {
            const V &derived = static_cast<const V &>(expr);
            using R = detail::SimdResolver<V>;
            using Traits = detail::SimdTraits<typename R::valueType, R::dim>;
            size_t idx = 0;

            if constexpr (Traits::available)
            {
                for (; idx <= N - Traits::width; idx += Traits::width)
                {
                    if constexpr (R::isAligned)
                    {
                        auto packet = derived.loadPacket(idx);
                        packet.store(&data_[idx]);
                    }
                    else
                    {
                        auto packet = derived.loadPacketUnaligned(idx);
                        packet.storeUnaligned(&data_[idx]);
                    }
                }
            }

            for (; idx < N; ++idx)
            {
                data_[idx] = derived[idx];
            }

            return *this;
        }

        [[nodiscard]] auto loadPacket(size_t idx) const
        {
            using Packet = typename detail::SimdTraits<T, N>::packetType;
            return Packet::load(&data_[idx]);
        }

        [[nodiscard]] auto loadPacketUnaligned(size_t idx) const
        {
            using Packet = typename detail::SimdTraits<T, N>::packetType;
            return Packet::loadUnaligned(&data_[idx]);
        }

    private:
        template <typename Expr, size_t... Ints>
        Vector(const VecExpr<Expr> &expr, std::index_sequence<Ints...>) : data_{expr.self()[Ints]...} {}
    };
}