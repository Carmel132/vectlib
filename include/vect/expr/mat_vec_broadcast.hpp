#pragma once
#include "vect/core/mat_expr.hpp"
#include "vect/core/scalar.hpp"
#include "vect/expr/capture_strategy.hpp"
#include "vect/expr/vector_view.hpp"
namespace vect::expr {
template <typename V, size_t C>
class MatBroadcastColumnVectorExpr
    : public core::MatExpr<MatBroadcastColumnVectorExpr<V, C>,
                           typename V::valueType, V::dim, C> {
  core::capture_t<V> v_;

public:
  MatBroadcastColumnVectorExpr(const V &v) : v_(v) {}

  [[nodiscard]] auto at(size_t r, size_t c) const { return v_[r]; }

  [[nodiscard]] auto getRow(size_t r) const {
    using VecScalar = core::VecScalar<typename V::valueType, C>;
    return VecScalar(v_[r]);
  }
};
template <typename V, size_t R>
class MatBroadcastRowVectorExpr
    : public core::MatExpr<MatBroadcastRowVectorExpr<V, R>,
                           typename V::valueType, R, V::dim> {
  core::capture_t<V> v_;

public:
  MatBroadcastRowVectorExpr(const V &v) : v_(v) {}

  [[nodiscard]] auto at(size_t r, size_t c) const { return v_[c]; }

  [[nodiscard]] auto getRow(size_t r) const { return VectorView(v_); }
};
} // namespace vect::expr