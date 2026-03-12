#pragma once
#include <immintrin.h>
#include <utility>
#include <type_traits>

namespace vect::detail {
    struct Packet4f {
      __m128 reg;
      
      friend auto operator+(Packet4f a, Packet4f b) -> Packet4f {
        return {_mm_add_ps(a.reg, b.reg)};
      }
      friend auto operator-(Packet4f a, Packet4f b) -> Packet4f {
        return {_mm_sub_ps(a.reg, b.reg)};
      }
    };

    template <typename T, size_t N>
    struct simdTraits {
        static constexpr bool available = (std::is_same_v<T, float> && N >= 4);
        using packetType = std::conditional_t<available, Packet4f, T>;
    };
}