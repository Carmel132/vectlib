#pragma once
#include "vect/core/scalar.hpp"
#include "vect/detail/vector_method.hpp"
#include "vect/expr/binary_op.hpp"
#include "vect/expr/vector_method.hpp"

#include "vect/detail/simd_comparison.hpp"

#include <functional>
#include <iostream>

namespace vect::core {
template <IsVecExpr L, IsVecExpr R> auto operator+(const L &l, const R &r) {
  return expr::BinaryOp<L, R, std::plus<>>(l, r);
}

template <IsVecExpr L, IsVecExpr R> auto operator-(const L &l, const R &r) {
  return expr::BinaryOp<L, R, std::minus<>>(l, r);
}

template <IsVecExpr V> constexpr auto operator-(const V &v) {
  return expr::negate(v);
}

// Component-wise and scalar multiplication
template <IsVecExpr L, IsVecExpr R> auto operator*(const L &l, const R &r) {
  return expr::BinaryOp<L, R, std::multiplies<>>(l, r);
}

template <IsVecExpr V, Scalar S> auto operator*(const V &v, S s) {
  using ScalarExpr = VecScalar<S, V::dim>;
  return expr::BinaryOp<V, ScalarExpr, std::multiplies<>>(v, ScalarExpr(s));
}

template <Scalar S, IsVecExpr V> auto operator*(S s, const V &v) {
  return v * s;
}

template <IsVecExpr V, Scalar S> auto operator/(const V &v, S s) {
  using ScalarExpr = VecScalar<S, V::dim>;
  return expr::BinaryOp<V, ScalarExpr, std::divides<>>(v, ScalarExpr(s));
}

template <IsVecExpr V> std::ostream &operator<<(std::ostream &os, const V &v) {
  os << "[";
  for (size_t idx{}; idx < v.size(); ++idx) {
    os << v[idx] << (idx == v.size() - 1 ? "" : "  ");
  }
  return os << "]";
}

template <IsVecExpr L, IsVecExpr R> auto operator==(const L &l, const R &r) {
  return expr::BinaryOp<L, R, detail::ComparisonEqual>(l, r);
}

template <IsVecExpr L, Scalar R> auto operator==(const L &l, const R &r) {
  using ScalarExpr = VecScalar<R, L::dim>;
  return expr::BinaryOp<L, ScalarExpr, detail::ComparisonEqual>(l,
                                                                ScalarExpr(r));
}

template <Scalar L, IsVecExpr R> auto operator==(const L &l, const R &r) {
  using ScalarExpr = VecScalar<L, R::dim>;
  return expr::BinaryOp<ScalarExpr, R, detail::ComparisonEqual>(ScalarExpr(l),
                                                                r);
}

template <IsVecExpr L, IsVecExpr R> auto operator!=(const L &l, const R &r) {
  return expr::BinaryOp<L, R, detail::ComparisonNotEqual>(l, r);
}

template <IsVecExpr L, Scalar R> auto operator!=(const L &l, const R &r) {
  using ScalarExpr = VecScalar<R, L::dim>;
  return expr::BinaryOp<L, ScalarExpr, detail::ComparisonNotEqual>(
      l, ScalarExpr(r));
}

template <Scalar L, IsVecExpr R> auto operator!=(const L &l, const R &r) {
  using ScalarExpr = VecScalar<L, R::dim>;
  return expr::BinaryOp<ScalarExpr, R, detail::ComparisonNotEqual>(
      ScalarExpr(l), r);
}

template <IsVecExpr L, IsVecExpr R> auto operator>(const L &l, const R &r) {
  return expr::BinaryOp<L, R, detail::ComparisonGreater>(l, r);
}

template <IsVecExpr L, Scalar R> auto operator>(const L &l, const R &r) {
  using ScalarExpr = VecScalar<R, L::dim>;
  return expr::BinaryOp<L, ScalarExpr, detail::ComparisonGreater>(
      l, ScalarExpr(r));
}

template <Scalar L, IsVecExpr R> auto operator>(const L &l, const R &r) {
  using ScalarExpr = VecScalar<L, R::dim>;
  return expr::BinaryOp<ScalarExpr, R, detail::ComparisonGreater>(ScalarExpr(l),
                                                                  r);
}

template <IsVecExpr L, IsVecExpr R> auto operator<(const L &l, const R &r) {
  return expr::BinaryOp<L, R, detail::ComparisonLess>(l, r);
}

template <IsVecExpr L, Scalar R> auto operator<(const L &l, const R &r) {
  using ScalarExpr = VecScalar<R, L::dim>;
  return expr::BinaryOp<L, ScalarExpr, detail::ComparisonLess>(l,
                                                               ScalarExpr(r));
}

template <Scalar L, IsVecExpr R> auto operator<(const L &l, const R &r) {
  using ScalarExpr = VecScalar<L, R::dim>;
  return expr::BinaryOp<ScalarExpr, R, detail::ComparisonLess>(ScalarExpr(l),
                                                               r);
}

template <IsVecExpr L, IsVecExpr R> auto operator>=(const L &l, const R &r) {
  return expr::BinaryOp<L, R, detail::ComparisonGreaterEqual>(l, r);
}

template <IsVecExpr L, Scalar R> auto operator>=(const L &l, const R &r) {
  using ScalarExpr = VecScalar<R, L::dim>;
  return expr::BinaryOp<L, ScalarExpr, detail::ComparisonGreaterEqual>(
      l, ScalarExpr(r));
}

template <Scalar L, IsVecExpr R> auto operator>=(const L &l, const R &r) {
  using ScalarExpr = VecScalar<L, R::dim>;
  return expr::BinaryOp<ScalarExpr, R, detail::ComparisonGreaterEqual>(
      ScalarExpr(l), r);
}

template <IsVecExpr L, IsVecExpr R> auto operator<=(const L &l, const R &r) {
  return expr::BinaryOp<L, R, detail::ComparisonLessEqual>(l, r);
}

template <IsVecExpr L, Scalar R> auto operator<=(const L &l, const R &r) {
  using ScalarExpr = VecScalar<R, L::dim>;
  return expr::BinaryOp<L, ScalarExpr, detail::ComparisonLessEqual>(
      l, ScalarExpr(r));
}

template <Scalar L, IsVecExpr R> auto operator<=(const L &l, const R &r) {
  using ScalarExpr = VecScalar<L, R::dim>;
  return expr::BinaryOp<ScalarExpr, R, detail::ComparisonLessEqual>(
      ScalarExpr(l), r);
}

template <IsVecExpr L, IsVecExpr R> auto operator&&(const L &l, const R &r) {
  return expr::BinaryOp<L, R, detail::LogicalAnd>(l, r);
}

template <IsVecExpr L, IsVecExpr R> auto operator||(const L &l, const R &r) {
  return expr::BinaryOp<L, R, detail::LogicalOr>(l, r);
}

template <IsVecExpr V> auto operator!(const V &v) {
  return expr::UnaryOp<V, detail::LogicalNot>(v);
}
} // namespace vect::core
