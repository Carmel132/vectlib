#pragma once
#include <concepts>
#include <vect/core/vec_expr.hpp>
#include "vect/detail/simd_traits.hpp"
namespace vect::core {
    template <typename T>
    concept Scalar = std::is_arithmetic_v<T>;

    template <typename T, size_t N>
    class VecScalar : public core::VecExpr<VecScalar<T, N>> {
        T val_;
    public:
        static constexpr size_t dim = N; // size agnostic
        using valueType = T;

        explicit VecScalar(T v) : val_{v} {}
        auto operator[](size_t idx) const -> T {return val_;}
        [[nodiscard]] auto size() const -> size_t {return 0;}

        auto loadPacket(size_t idx) const {
            using Traits = detail::SimdTraits<T, N>;
            using Packet = typename Traits::packetType;
            return Packet::broadcast(val_);
        }

        auto loadPacketUnaligned(size_t idx) const {
            return loadPacket(idx);
        }
    };
}