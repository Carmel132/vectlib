#pragma once
#include <array>
#include <cmath>
#include <cstddef>
namespace vect::core
{

    template <typename Derived>
    class VecExpr
    {
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
    };

    template <typename T, size_t N>
    class Vector : public VecExpr<Vector<T, N>>
    {
    public:
        std::array<T, N> data_{};

        constexpr Vector(std::initializer_list<T> list)
        {
            std::copy(list.begin(), list.end(), data_.begin());
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
    };
}