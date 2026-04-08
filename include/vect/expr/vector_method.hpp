#pragma once
#include "vect/core/scalar.hpp"
#include "vect/core/vec_expr.hpp"
#include "vect/core/vector.hpp"
#include "vect/expr/binary_op.hpp"
#include "vect/expr/capture_strategy.hpp"
#include "vect/expr/unary_op.hpp"
#include "vect/expr/where_op.hpp"

#include "vect/detail/simd_reduction.hpp"
#include "vect/detail/vector_method.hpp"
#include "vect/expr/mat_method.hpp"
#include <algorithm>
namespace vect::expr {
/**
 * @brief Computes the dot product of two vector expressions.
 */
template <core::IsVecExpr L, core::IsVecExpr R>
constexpr auto dot(const L &l, const R &r) {
  static_assert(
      L::dim == R::dim,
      "Dot product is only defined for vectors of the same dimension");

  using T = typename L::valueType;
  using Traits = detail::SimdTraits<T, L::dim>;

  constexpr size_t imax = (L::dim / Traits::width) * Traits::width;
  constexpr size_t tailSize = L::dim - imax;

  T total = 0;

  if constexpr (imax > 0) {
    auto acc = detail::broadcast<T, Traits>(0);

    for (size_t i = 0; i < imax; i += Traits::width) {
      acc = acc + (l.loadPacket(i) * r.loadPacket(i));
    }

    total = detail::sum(acc);
  }

  if constexpr (tailSize > 0) {
    total +=
        detail::dot_tail_impl(l, r, imax, std::make_index_sequence<tailSize>{});
  }

  return total;
}

template <typename L, typename R>
class CrossProduct : public core::VecExpr<CrossProduct<L, R>> {
  core::capture_t<L> l_;
  core::capture_t<R> r_;

public:
  using valueType =
      std::common_type_t<typename L::valueType, typename R::valueType>;
  static constexpr size_t dim = 3;
  constexpr CrossProduct(const L &l, const R &r) : l_{l}, r_{r} {
    if constexpr (L::dim != 3) {
      static_assert(L::dim == 3,
                    "Cross product is only defined for vectors of dimension 3");
    }
  }

  constexpr auto operator[](size_t idx) const -> valueType {
    size_t i = (idx + 1) % 3;
    size_t j = (idx + 2) % 3;
    return l_[i] * r_[j] - l_[j] * r_[i]; // neat lil trick
  }

  [[nodiscard]] constexpr auto size() const -> size_t { return 3; }
};

/// @brief Computes the cross product of two 3D vector expressions
/// @note The function is only defined for 3D vectors
template <core::IsVec3Expr L, core::IsVec3Expr R>
constexpr auto cross(const L &l, const R &r) {
  return CrossProduct<L, R>(l, r);
}

template <core::IsVecExpr V> constexpr auto sum(const V &v) {
  return detail::sum_fold_impl(v, std::make_index_sequence<V::dim>{});
}

template <core::IsVecExpr V> constexpr auto normSquared(const V &v) {
  return dot(v, v);
}

template <core::IsVecExpr V> constexpr auto norm(const V &v) {
  return std::sqrt(normSquared(v));
}

template <core::IsVecExpr V> constexpr auto normalize(const V &v) {
  return v / norm(v);
}

template <core::IsVecExpr A, core::IsVecExpr B, core::Scalar S>
constexpr auto lerp(const A &a, const B &b, S t) {
  return (b - a) * t + a;
}

template <core::IsVecExpr A, core::IsVecExpr B>
constexpr auto project(const A &a, const B &b) {
  return (dot(a, b) / dot(b, b)) * b;
}

template <core::IsVecExpr A, core::IsVecExpr B>
constexpr auto reject(const A &a, const B &b) {
  return a - project(a, b);
}

template <core::IsVecExpr A, core::IsVecExpr B>
constexpr auto reflect(const A &i, const B &n) {
  return i - n * (2.0 * dot(i, n));
}

template <core::IsVecExpr V, typename F> constexpr auto map(V &&v, F &&f) {
  using ExprType = std::decay_t<V>;
  using FuncType = std::decay_t<F>;

  return UnaryOp<ExprType, FuncType>(std::forward<V>(v), std::forward<F>(f));
}

template <core::IsVecExpr V> bool all(const V &expr) {
  const V &derived = expr.self();
  using valueType = V::valueType;
  using Traits = detail::SimdTraits<valueType, V::dim>;

  size_t i = 0;
  if constexpr (Traits::available) {
    for (; i <= V::dim - Traits::width; i += Traits::width) {
      auto packet = derived.loadPacketUnaligned(i);

      if constexpr (detail::isMask_v<decltype(packet)>) {
        if (!detail::simdAllFull(packet))
          return false;
      } else {
        auto mask = detail::simdIsNonzero(packet);
        if (!detail::simdAllFull(mask))
          return false;
      }
    }
  }

  for (; i < V::dim; ++i) {
    if (!static_cast<bool>(derived[i]))
      return false;
  }

  return true;
}

template <core::IsVecExpr V> bool any(const V &expr) {
  const V &derived = expr.self();
  using valueType = V::valueType;
  using Traits = detail::SimdTraits<valueType, V::dim>;

  size_t i = 0;
  if constexpr (Traits::available) {
    for (; i <= V::dim - Traits::width; i += Traits::width) {
      auto packet = derived.loadPacketUnaligned(i);
      if constexpr (detail::isMask_v<decltype(packet)>) {
        if (detail::simdAnyFull(packet))
          return true;
      } else {
        auto mask = detail::simdIsNonzero(packet);
        if (detail::simdAnyFull(mask))
          return true;
      }
    }
  }

  for (; i < V::dim; ++i) {
    if (static_cast<bool>(derived[i]))
      return true;
  }

  return false;
}

template <typename M, typename T, typename F>
auto where(const M &mask, const T &ifTrue, const F &ifFalse) {
  return WhereOp<M, T, F>(mask, ifTrue, ifFalse);
}

} // namespace vect::expr