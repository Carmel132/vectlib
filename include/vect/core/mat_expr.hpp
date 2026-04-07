#pragma once
#include <cmath>
#include <type_traits>
namespace vect::core
{

    template <typename Derived, typename T, size_t R, size_t C>
    class MatExpr
    {
    protected:
        MatExpr() = default;

    public:
        const Derived &self() const { return static_cast<const Derived &>(*this); }

        auto operator()(size_t r, size_t c) const { return self().at(r, c); }

        auto row(size_t r) const
        {
            return self().getRow(r);
        }

        static constexpr size_t rows = R;
        static constexpr size_t columns = C;
        using valueType = T;
    };

    template <size_t R, size_t C, typename T>
    template <typename E>
    concept IsMatExpr = std::is_base_of_v<MatExpr<std::decay_t<E>, T, R, C>, std::decay_t<E>>;
}