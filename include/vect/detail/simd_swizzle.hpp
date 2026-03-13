#pragma once
#include "vect/detail/simd_traits.hpp"
#include "vect/expr/swizzle.hpp"

namespace vect::detail {
    template <typename V, size_t... Indices>
    struct SimdTraits<expr::Swizzle<V, Indices...>> {
        using T = typename V::valueType;
        static constexpr size_t N = sizeof...(Indices);

    };
}