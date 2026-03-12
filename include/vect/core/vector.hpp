#pragma once
#include "vect/detail/simd_packet.hpp"
#include <array>
#include <immintrin.h>
#include <vect/core/vec_expr.hpp>

#include <iostream>
// TODO: remove immintrin include and define loadPacket in different file
namespace vect::core
{

    template <typename T, size_t N>
    class Vector : public VecExpr<Vector<T, N>>
    {
    public:
        alignas(sizeof(T) * (N == 3 ? 4 : N))
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
            size_t idx = 0;

            if constexpr (detail::SimdTraits<T, N>::available)
            {
                for (; idx <= N - 4; idx += 4)
                {
                    auto packet = derived.loadPacket(idx);
                    _mm_store_ps(&data_[idx], packet.reg);
                }

                std::cout << "Hallo!";
            }

            for (; idx < N; ++idx)
            {
                data_[idx] = derived[idx];
            }

            return *this;
        }

        auto loadPacket(size_t idx) const
        {
            return _mm_load_ps(&data_[idx]);
        }

    private:
        template <typename Expr, size_t... Ints>
        Vector(const VecExpr<Expr> &expr, std::index_sequence<Ints...>) : data_{expr.self()[Ints]...} {}
    };
}