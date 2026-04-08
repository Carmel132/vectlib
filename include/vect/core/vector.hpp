#pragma once
#include "vect/detail/simd_resolver.hpp"
#include "vect/detail/simd_traits.hpp"
#include <array>
#include <span>
#include <utility>
#include <vect/core/vec_expr.hpp>
#include "vect/detail/defer_eval.hpp"

namespace vect::core {

template <typename T, size_t N>
class alignas(detail::SimdTraits<T, N>::alignment) Vector
    : public VecExpr<Vector<T, N>> {
public:
  Vector() = default;
  static constexpr size_t alignment = detail::SimdTraits<T, N>::alignment;

  alignas(alignment) std::array<T, N> data_{};
  using valueType = T;
  static constexpr size_t dim = N;

  /// @brief Direct `VecExpr` initializer
  /// @param expr the `VecExp` to initialize a vector from
  template <typename Expr>
  Vector(const VecExpr<Expr> &expr)
      : Vector(expr, std::make_index_sequence<N>{}) {}

  template <typename U> constexpr Vector(std::initializer_list<U> list) {
    size_t i = 0;
    for (const auto &val : list) {
      if (i >= N)
        break;
      data_[i++] = static_cast<T>(val);
    }
  }

  constexpr auto operator[](size_t idx) -> T & { return data_[idx]; }
  constexpr auto operator[](size_t idx) const -> const T & {
    return data_[idx];
  }

  [[nodiscard]] constexpr auto size() const -> size_t { return N; }

  template <typename V> Vector<T, N> &operator=(const VecExpr<V> &expr) {


    if constexpr (detail::DeferAssign<V>::value) {
      expr.evaluateTo(*this);
      return *this;
    }

    const V &derived = static_cast<const V &>(expr);
    using R = detail::SimdResolver<V>;
    using Traits = detail::SimdTraits<typename R::valueType, R::dim>;
    size_t idx = 0;

    if constexpr (Traits::available) {
      for (; idx <= N - Traits::width; idx += Traits::width) {
        if constexpr (R::isAligned) {
          auto packet = derived.loadPacket(idx);
          packet.store(&data_[idx]);
        } else {
          auto packet = derived.loadPacketUnaligned(idx);
          packet.storeUnaligned(&data_[idx]);
        }
      }
    }

    for (; idx < N; ++idx) {
      data_[idx] = derived[idx];
    }

    return *this;
  }

  Vector &operator=(T value) {
    fill(value);
    return *this;
  }

  [[nodiscard]] auto loadPacket(size_t idx) const {
    return detail::load<T, detail::SimdTraits<T, N>>(&data_[idx]);
  }

  [[nodiscard]] auto loadPacketUnaligned(size_t idx) const
    requires(detail::SimdTraits<T, N>::available)
  {
    using Packet = typename detail::SimdTraits<T, N>::packetType;
    return Packet::loadUnaligned(&data_[idx]);
  }

  operator std::span<const T, N>() const {
    return std::span<const T, N>(data_.data(), N);
  }

  operator std::span<T, N>() { return std::span<T, N>(data_.data(), N); }

  // For structured bindings if ur into that sorta thing
  template <size_t I> auto get() & -> T & {
    static_assert(I < N, "Index out of bounds");
    return data_[I];
  }

  template <size_t I> const T &get() const & {
    static_assert(I < N, "Index out of bounds");
    return data_[I];
  }

  template <size_t I> T get() && { return (*this)[I]; }

  // TODO: Add an simd implementation
  void fill(T value) { std::fill(data_.begin(), data_.end(), value); }

  template <typename Expr>
  void multiplyAccumulate(T scale, const VecExpr<Expr> &other) {
    using Traits = detail::SimdTraits<T, dim>;
    using Packet = typename Traits::packetType;

    auto scalePacket = Packet::broadcast(scale);
    const auto &derivedOther = other.self();

    size_t i = 0;
    if constexpr (dim >= Traits::width) {
      for (; i <= dim - Traits::width; i += Traits::width) {
        Packet dest = Packet::loadUnaligned(&data_[i]);
        Packet src = derivedOther.loadPacketUnaligned(i);
        Packet result = dest + (scalePacket * src);
        result.storeUnaligned(&data_[i]);
      }
    }
    for (; i < dim; ++i) {
      data_[i] += scale * derivedOther[i];
    }
  }

private:
  template <typename Expr, size_t... Ints>
  Vector(const VecExpr<Expr> &expr, std::index_sequence<Ints...>)
      : data_{expr.self()[Ints]...} {}
};
} // namespace vect::core