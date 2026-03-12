#pragma once

#include "vect/core/vector.hpp"
#include "vect/core/vec_expr.hpp"
#include "vect/expr/capture_strategy.hpp"

#include <array>

namespace vect::expr
{
    template <typename V, size_t... Indices>
    class Swizzle : public core::VecExpr<Swizzle<V, Indices...>> {
        core::capture_t<V> v_;
        static constexpr std::array<size_t, sizeof...(Indices)> indexMap_ = {Indices...};
    public:
        using valueType = typename V::valueType;
        static constexpr size_t dim = sizeof...(Indices);

        Swizzle(const V& v) : v_{v} {}

        constexpr auto operator[](size_t idx) const {
            return v_[indexMap_[idx]];
        }

        [[nodiscard]] constexpr auto size() const -> size_t { return dim; }
    };

}

namespace vect::core {
 template <typename D>
    template <size_t... Indices>
    constexpr auto core::VecExpr<D>::swizzle() const {
        return expr::Swizzle<D, Indices...>(static_cast<const D&>(*this));
    }
}