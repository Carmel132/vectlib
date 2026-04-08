#pragma once
#include "vect/core/mat_expr.hpp"
#include "vect/detail/simd_traits.hpp"
#include <concepts>
#include <vect/core/vec_expr.hpp>
namespace vect::core {
template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

template <typename T, size_t N>
class VecScalar : public core::VecExpr<VecScalar<T, N>> {
  T val_;

public:
  static constexpr size_t dim = N; // size agnostic
  using valueType = T;

  template <typename U>
    requires std::is_arithmetic_v<U>
  explicit VecScalar(U v) : val_{static_cast<T>(v)} {}
  auto operator[](size_t idx) const -> T { return val_; }
  [[nodiscard]] auto size() const -> size_t { return dim; }

  [[nodiscard]] auto loadPacket(size_t idx) const {
    using Traits = detail::SimdTraits<T, N>;
    using Packet = typename Traits::packetType;
    return Packet::broadcast(val_);
  }

  [[nodiscard]] auto loadPacketUnaligned(size_t idx) const {
    return loadPacket(idx);
  }
};

template <typename T, size_t R, size_t C>
class MatScalar : public core::MatExpr<MatScalar<T, R, C>, T, R, C> {
  T val_;

public:
  explicit MatScalar(T v) : val_(v) {}

  T at(size_t r, size_t c) const { return val_; }

  auto getRow(size_t r) const { return VecScalar<T, C>(val_); }
};
} // namespace vect::core