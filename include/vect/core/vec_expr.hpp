#pragma once 
#include <cmath>
#include <cstddef>
#include <utility>
namespace vect::core {
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
    };

    template <typename T>
    concept IsVecExpr = std::derived_from<std::decay_t<T>, VecExpr<std::decay_t<T>>>;

    template <typename T>
    concept IsVec3Expr = IsVecExpr<T> && T::dim == 3;
}