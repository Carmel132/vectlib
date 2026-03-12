#pragma once
#include "vect/detail/simd_packet.hpp"

#include <bit>
namespace vect::detail {
    template <typename T, size_t N>
    struct SimdTraits
    {
    static constexpr bool available = (std::is_same_v<T, float> && N >= 4);
    using packetType = std::conditional_t<available, Packet4f, T>;

    static constexpr size_t simdAlign = available ? sizeof(packetType) : alignof(T);
    static constexpr size_t scalarAlign = std::bit_ceil(sizeof(T) * N);
    static constexpr size_t alignment = (scalarAlign > simdAlign) ? simdAlign : scalarAlign;
    };
}