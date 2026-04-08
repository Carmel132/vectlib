#pragma once

#include "vect/core/mat_expr.hpp"
#include "vect/expr/capture_strategy.hpp"
#include "vect/expr/mat_column_view.hpp"
namespace vect::expr {

template <typename M>
class TransposeExpr
    : public core::MatExpr<TransposeExpr<M>, typename M::valueType, M::columns,
                           M::rows> {
  core::capture_t<M> m_;

public:
  TransposeExpr(const M &m) : m_{m} {}

  [[nodiscard]] auto at(size_t r, size_t c) const { return m_.at(c, r); }

  [[nodiscard]] auto getRow(size_t r) const { return MatColumnView<M>(m_, r); }
};

template <core::IsMatExpr M> auto transpose(const M &m) {
  return TransposeExpr<M>(m);
}
} // namespace vect::expr