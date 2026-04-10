#pragma once

#include "vect/core/mat_expr.hpp"
#include "vect/core/vec_expr.hpp"
#include "vect/expr/capture_strategy.hpp"
#include "vect/expr/mat_strided_row_view.hpp"
#include "vect/expr/swizzle.hpp"
namespace vect::expr {

template <typename M, size_t rStart = 0, size_t rEnd = M::rows - 1,
          size_t cStart = 0, size_t cEnd = M::columns - 1, size_t rStep = 1,
          size_t cStep = 1>
class MatSlice : public core::MatExpr<
                     MatSlice<M, rStart, rEnd, cStart, cEnd, rStep, cStep>,
                     typename M::valueType, (rEnd - rStart + rStep - 1) / rStep,
                     (cEnd - cStart + cStep - 1) / cStep> {
  core::capture_t<M> m_;

  static_assert(rEnd <= M::rows, "Row slice out of bounds");
  static_assert(cEnd <= M::columns, "Column slice out of bounds");
  static_assert(rStart < rEnd, "Row start must be less than end");
  static_assert(cStart < cEnd, "Column start must be less than end");

public:
  static constexpr size_t rows = (rEnd - rStart + rStep - 1) / rStep;
  static constexpr size_t cols = (cEnd - cStart + cStep - 1) / cStep;

  MatSlice(const M &m) : m_(m) {}

  [[nodiscard]] auto at(size_t r, size_t c) const {
    return m_.at(rStart + r * rStep, cStart + c * cStep);
  }

  [[nodiscard]] auto getRow(size_t r) const {
    return MatStridedRowView<M, cStart, cEnd, cStep>(m_, rStart + r * rStep);
  }
};
} // namespace vect::expr

namespace vect::core {
template <typename M, typename T, size_t R, size_t C>
template <size_t rL, size_t rR, size_t cU, size_t cD, size_t rS, size_t cS>
[[nodiscard]] constexpr auto MatExpr<M, T, R, C>::slice() const {
  return expr::MatSlice<M, rL, rR, cU, cD, rS, cS>(
      static_cast<const M &>(*this));
}
} // namespace vect::core