#pragma once

#include "vect/core/vec_expr.hpp"
#include "vect/expr/capture_strategy.hpp"
namespace vect::expr {
template <typename M, size_t Start, size_t Stop, size_t Step>
class MatStridedRowView
    : public core::VecExpr<MatStridedRowView<M, Start, Stop, Step>> {

  core::capture_t<M> m_;
  size_t rowIdx_;

public:
  using valueType = typename M::valueType;
  static constexpr size_t dim = (Stop - Start + Step - 1) / Step;

  MatStridedRowView(const M &m, size_t r) : m_(m), rowIdx_(r) {}

  [[nodiscard]] valueType operator[](size_t idx) const {
    return m_.at(rowIdx_, Start + idx * Step);
  }

  [[nodiscard]] constexpr size_t size() const { return dim; }

  [[nodiscard]] auto loadPacket(size_t idx) const {
    using Traits = detail::SimdTraits<valueType, dim>;
    using Packet = typename Traits::packetType;

    if constexpr (Step == 1) {
      return m_.getRow(rowIdx_).loadPacketUnaligned(Start + idx);
    }

#ifdef VECT_HAS_AVX
    if constexpr (Traits::width == 8 && std::is_same_v<valueType, float>) {
      __m256i offsets = _mm256_set_epi32(7 * Step, 6 * Step, 5 * Step, 4 * Step,
                                         3 * Step, 2 * Step, 1 * Step, 4

      );

      const float *base = &m_.at(rowIdx_, Start + idx * Step);
      return _mm256_i32gather_ps(base, offsets, 4);
    }
#endif

    alignas(Traits::alignment) valueType vals[Traits::width];
    for (size_t i = 0; i < Traits::width; ++i) {
      vals[i] =
          (idx + i < dim) ? m_.at(rowIdx_, Start + (idx + i) * Step) : 0.0f;
    }
    return detail::load<Packet, Traits>(vals);
  }

  [[nodiscard]] auto loadPacketUnaligned(size_t idx) const {
    return loadPacket(idx);
  }
};
} // namespace vect::expr