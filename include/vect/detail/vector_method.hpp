#pragma once
#include <utility>

namespace vect::detail {
    template <typename L, typename R, size_t... Is>
    constexpr auto dot_fold_impl(const L& l, const R& r, std::index_sequence<Is...>) {
        return ((l[Is] * r[Is]) + ...);
    }

    template <typename V, size_t... Is>
    constexpr auto sum_fold_impl(const V& v, std::index_sequence<Is...>) {
        return (v[Is] + ...);
    }
}