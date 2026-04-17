#pragma once

#include "vect/core/mat_expr.hpp"
#include "vect/core/vec_expr.hpp"
#include "vect/detail/simd_select_mask.hpp"
#include "vect/expr/capture_strategy.hpp"
namespace vect::expr {
template <typename MaskE, typename TrueE, typename FalseE>
class WhereOp : public core::VecExpr<WhereOp<MaskE, TrueE, FalseE>> {
  core::capture_t<MaskE> maskExpr_;
  core::capture_t<TrueE> trueExpr_;
  core::capture_t<FalseE> falseExpr_;

public:
  using valueType = TrueE::valueType;
  static constexpr size_t dim = TrueE::dim;

  WhereOp(const MaskE &m, const TrueE &t, const FalseE &f)
      : maskExpr_{m}, trueExpr_{t}, falseExpr_{f} {}

  auto loadPacket(size_t idx) const {
    return detail::simdSelect(maskExpr_.loadPacket(idx),
                              trueExpr_.loadPacket(idx),
                              falseExpr_.loadPacket(idx));
  }

  auto loadPacketUnaligned(size_t idx) const {
    return detail::simdSelect(maskExpr_.loadPacketUnaligned(idx),
                              trueExpr_.loadPacketUnaligned(idx),
                              falseExpr_.loadPacketUnaligned(idx));
  }

  auto operator[](size_t idx) const {
    return maskExpr_[idx] ? trueExpr_[idx] : falseExpr_[idx];
  }

  [[nodiscard]] auto size() const -> size_t {
    return trueExpr_.size() != 0 ? trueExpr_.size() : falseExpr_.size();
  }
};

template <typename MaskE, typename TrueE, typename FalseE>
class MatWhereOp : public core::MatExpr<MatWhereOp<MaskE, TrueE, FalseE>,
                                        typename MaskE::valueType, MaskE::rows,
                                        MaskE::columns> {
  core::capture_t<MaskE> maskExpr_;
  core::capture_t<TrueE> trueExpr_;
  core::capture_t<FalseE> falseExpr_;

public:
  MatWhereOp(const MaskE &m, const TrueE &t, const FalseE &f)
      : maskExpr_(m), trueExpr_(t), falseExpr_(f) {}

  [[nodiscard]] auto at(size_t r, size_t c) const {
    return maskExpr_.at(r, c) ? trueExpr_.at(r, c) : falseExpr_.at(r, c);
  }

  [[nodiscard]] auto getRow(size_t r) const {
    return WhereOp(maskExpr_.getRow(r), trueExpr_.getRow(r),
                   falseExpr_.getRow(r));
  }
};
} // namespace vect::expr