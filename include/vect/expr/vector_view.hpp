#pragma once
#include "vect/core/vec_expr.hpp"
#include "vect/expr/capture_strategy.hpp"
namespace vect::expr {
template <typename V> class VectorView : public core::VecExpr<VectorView<V>> {
  core::capture_t<V> v_;

public:
  using valueType = V::valueType;
  static constexpr size_t dim = V::dim;

  VectorView(const V &v) : v_(v) {}

  [[nodiscard]] auto operator[](size_t idx) const { return v_[idx]; }

  auto loadPacket(size_t idx) const { return v_.loadPacket(idx); }

  [[nodiscard]] constexpr size_t size() const { return dim; }
};
} // namespace vect::expr