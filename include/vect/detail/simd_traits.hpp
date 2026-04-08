#pragma once
#include "vect/detail/simd_packet.hpp"
#include "vect/detail/simd_packet_4d.hpp"

#include <bit>
#include <type_traits>
namespace vect::detail {
// TODO: test code without SSE support
#if defined(__SSE__) || defined(_M_AMD64) || defined(_M_X64)
#define VECT_HAS_SSE
#endif

// TODO: test this with sde
#if defined(__AVX__) || defined(__AVX2__)
#define VECT_HAS_AVX
#endif

template <typename T, size_t N> struct SimdTraits {

  static constexpr bool available = false;
  static constexpr bool isAligned = false;
  using packetType = T;
  static constexpr size_t width = 1;
  static constexpr size_t alignment = alignof(T);
};

template <size_t N> struct SimdTraits<float, N> {
#ifdef VECT_HAS_AVX
  static constexpr bool avxAvailable = (N >= 8);
  static constexpr bool sseAvailable = (N >= 4);
  static constexpr bool available = avxAvailable || sseAvailable;
  static constexpr size_t width = avxAvailable ? 8 : (sseAvailable ? 4 : 1);
#elifdef VECT_HAS_SSE
  static constexpr bool available = (N >= 4);
  static constexpr size_t width = available ? 4 : 1;
#else
  static constexpr bool available = false;
  static constexpr size_t width = 1;
#endif

  using packetType = std::conditional_t<
      width == 8, detail::Packet8f,
      std::conditional_t<width == 4, detail::Packet4f, float>>;

  static constexpr size_t simdAlign = (width == 8) ? 32 : 16;
  ;
  static constexpr size_t scalarAlign = std::bit_ceil(sizeof(float) * N);
  static constexpr size_t alignment =
      (scalarAlign > simdAlign) ? simdAlign : scalarAlign;
  static constexpr bool isAligned = true;
};

template <size_t N> struct SimdTraits<double, N> {
  static constexpr bool canUseAVX =
#ifdef VECT_HAS_AVX
      (N >= 4);
#else
      false;
#endif
  static constexpr bool canUseSSE = (N >= 2);
  static constexpr bool available = canUseAVX || canUseSSE;

  using packetType =
      std::conditional_t<canUseAVX, Packet4d,
                         std::conditional_t<canUseSSE, Packet2d, double>>;
  static constexpr size_t width = canUseAVX ? 4 : (canUseSSE ? 2 : 1);
  static constexpr size_t simdAlign = canUseAVX ? 32 : 16;

  static constexpr size_t scalarAlign = std::bit_ceil(sizeof(double) * N);
  static constexpr size_t alignment =
      (scalarAlign > simdAlign) ? simdAlign : scalarAlign;
  static constexpr bool isAligned = false;
};

template <size_t N> struct SimdTraits<int, N> {
#ifdef VECT_HAS_AVX
  static constexpr bool available = (N >= 4);
  static constexpr bool useAVX = (N >= 8);
  using packetType =
      std::conditional_t<useAVX, Packet8i,
                         std::conditional_t<available, Packet4i, int>>;
  static constexpr size_t width = useAVX ? 8 : (available ? 4 : 1);
  static constexpr size_t simdAlign = useAVX ? 32 : 16;
#else
  static constexpr bool available = (N >= 4);
  using packetType = std::conditional_t<available, Packet4i, int>;
  static constexpr size_t width = available ? 4 : 1;
  static constexpr size_t simdAlign = 16;
#endif

  static constexpr size_t scalarAlign = std::bit_ceil(sizeof(int) * N);
  static constexpr size_t alignment =
      (scalarAlign > simdAlign) ? simdAlign : scalarAlign;
  static constexpr bool isAligned = false;
};

template <typename T, size_t C>
static constexpr size_t matrix_stride_v =
    (C + SimdTraits<T, C>::width - 1) / SimdTraits<T, C>::width *
    SimdTraits<T, C>::width;

template <typename T, typename Traits> auto broadcast(T val) {
  using Packet = typename Traits::packetType;
  if constexpr (std::is_arithmetic_v<Packet>) {
    return static_cast<Packet>(val);
  } else {
    return Packet::broadcast(val);
  }
}
template <typename T, typename Traits> auto load(const T *ptr) {
  using Packet = typename Traits::packetType;
  if constexpr (std::is_arithmetic_v<Packet>) {
    return *ptr;
  } else {
    return Packet::load(ptr);
  }
}

template <typename T, typename Traits> auto loadUnaligned(const T *ptr) {
  using Packet = typename Traits::packetType;
  if constexpr (std::is_arithmetic_v<Packet>) {
    return *ptr;
  } else {
    return Packet::loadUnaligned(ptr);
  }
}

template <typename T> T sum(T f) { return f; }
} // namespace vect::detail