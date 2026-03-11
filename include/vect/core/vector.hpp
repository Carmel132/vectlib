#pragma once
#include <array>
#include <vect/core/vec_expr.hpp>
namespace vect::core
{

    
    template <typename T, size_t N>
    class Vector : public VecExpr<Vector<T, N>>
    {
    public:
        std::array<T, N> data_{};
        using valueType = T;

        /// @brief Direct `VecExpr` initializer
        /// @param expr the `VecExp` to initialize a vector from
        template <typename Expr>
        Vector(const VecExpr<Expr> &expr) : Vector(expr, std::make_index_sequence<N>{}){

        }

        constexpr Vector(std::initializer_list<T> list)
        {
            size_t i = 0;
            for (const auto& val : list) {
                if (i >= N) break;
                data_[i++] = val;
            }
        }

        constexpr auto operator[](size_t idx) -> T & { return data_[idx]; }
        constexpr auto operator[](size_t idx) const -> const T & { return data_[idx]; }

        [[nodiscard]] constexpr auto size() const -> size_t { return N; }

        template <typename Expr>
        auto operator=(const VecExpr<Expr> &expr) -> Vector &
        {
            const Expr &e = expr.self();
            for (size_t idx{}; idx < N; ++idx)
            {
                data_[idx] = e[idx];
            }

            return *this;
        }

        private:
        template <typename Expr, size_t... Ints>
        Vector(const VecExpr<Expr> &expr, std::index_sequence<Ints...>) : data_{expr.self()[Ints]...} {}
    };
}