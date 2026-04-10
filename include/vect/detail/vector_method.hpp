#pragma once
#include <cstddef>
#include <utility>

namespace vect::detail {
template <typename L, typename R, size_t... Is>
constexpr auto dot_fold_impl(const L &l, const R &r,
                             std::index_sequence<Is...>) {
  return ((l[Is] * r[Is]) + ...);
}

template <typename L, typename R, size_t Offset, size_t... Is>
constexpr auto dot_tail_impl(const L &l, const R &r,
                             std::index_sequence<Is...>) {
  return ((l[Offset + Is] * r[Offset + Is]) + ...);
}

template <typename V, size_t... Is>
constexpr auto sum_fold_impl(const V &v, std::index_sequence<Is...>) {
  return (v[Is] + ...);
}

template <typename L, typename R, size_t... Is>
constexpr auto all_equal_fold_impl(const L &l, const R &r,
                                   std::index_sequence<Is...>) {
  return ((l[Is] == r[Is]) && ...);
}

// TODO: Delete this upon successful test
// template <typename V>
// inline bool all(const V& v) {return static_cast<bool>(v);}

// template <typename V>
// inline bool any(const V& v) {return static_cast<bool>(v);}
} // namespace vect::detail