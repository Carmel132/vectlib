#pragma once
#include "vect/core/vec_expr.hpp"
#include "vect/detail/simd_traits.hpp"
#include "vect/expr/capture_strategy.hpp"
namespace vect::expr {
    template <typename M>
    class MatColumnView : public core::VecExpr<MatColumnView<M>>
    {
        core::capture_t<M> m_;
        size_t columnIndex_;
    public:
        using valueType = typename M::valueType;
        static constexpr size_t dim = M::rows;

        MatColumnView(const M& m, size_t columnIndex) : m_{m}, columnIndex_{columnIndex} {}
    
        [[nodiscard]] constexpr valueType operator[](size_t idx) const {
            return m_.at(idx, columnIndex_);
        }

        [[nodiscard]] constexpr auto size() const -> size_t { return dim; }

        [[nodiscard]] auto loadPacket(size_t idx) const {
            return loadPacketUnaligned(idx);
        }

        [[nodiscard]] auto loadPacketUnaligned(size_t idx) const {
            using Traits = detail::SimdTraits<valueType, dim>;
            using Packet = typename Traits::packetType;

        #ifdef VECT_HAS_AVX
            if constexpr (Traits::width == 8 && std::is_same_v<valueType, float>) {
                return loadPacketGather(idx);
            }    
        #endif
            alignas(Traits::alignment) valueType vals[Traits::width];
            for (size_t i = 0; i < Traits::width; ++i) {
                if (idx + i < dim) {
                    vals[i] = m_.at(idx + i, columnIndex_);
                } else {
                    vals[i] = valueType{0};
                }

            }
            return Packet::load(vals);
        }
    private:
        auto loadPacketGather(size_t idx) const {
            constexpr int stride = detail::matrix_stride_v<valueType, M::columns>;

            __m256i offsets = _mm256_set_epi32(
                7 * stride * sizeof(float), 6 * stride * sizeof(float),
                5 * stride * sizeof(float), 4 * stride * sizeof(float),
                3 * stride * sizeof(float), 2 * stride * sizeof(float),
                stride * sizeof(float), 0
            );

            const float* base = &m_.at(idx, columnIndex_);
            return _mm256_i32gather_ps(base, offsets, 1);
        }
    };
}