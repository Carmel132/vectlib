#pragma once
#include "vect/detail/simd_packet.hpp"

#include <bit>
namespace vect::detail
{
// TODO: test code without SSE support
#if defined(__SSE__) || defined(_M_AMD64) || defined(_M_X64)
#define VECT_HAS_SSE
#endif

// TODO: test this with sde
#if defined(__AVX__) || defined(__AVX2__)
#define VECT_HAS_AVX
#endif

    template <typename T, size_t N>
    struct SimdTraits
    {

        static constexpr bool available = false;
        using packetType = T;
        static constexpr size_t width = 1;
        static constexpr size_t alignment = alignof(T);
    };

    template <size_t N>
    struct SimdTraits<float, N>
    {
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

        using packetType = std::conditional_t<width == 8, detail::Packet8f, std::conditional_t<width == 4, detail::Packet4f, float>>;

        static constexpr size_t simdAlign = (width == 8) ? 32 : 16;
        ;
        static constexpr size_t scalarAlign = std::bit_ceil(sizeof(float) * N);
        static constexpr size_t alignment = (scalarAlign > simdAlign) ? simdAlign : scalarAlign;
    };

    template <size_t N>
    struct SimdTraits<double, N>
    {
        #ifdef VECT_HAS_AVX
        static constexpr bool available = (N>=4);
        static constexpr size_t width = available ? 4 : 1;
        using packetType = std::conditional_t<available, Packet4d, double>;
        static constexpr size_t simdAlign = 32;
        #else
        static constexpr bool available = (N>= 2);
        static constexpr size_t width = available ? 2 : 1;
        using packetType = std::conditional_t<available, Packet2d, double>;
        static constexpr size_t simdAlign = 16;
        #endif

        static constexpr size_t scalarAlign = std::bit_ceil(sizeof(double) * N);
        static constexpr size_t alignment = (scalarAlign > simdAlign) ? simdAlign : scalarAlign;
    };

    template <size_t N>
    struct SimdTraits<int, N>
    {
        #ifdef VECT_HAS_AVX
        static constexpr bool available = (N >= 8);
        static constexpr size_t width = available ? 8 : 1;
        using packetType = std::conditional_t<available, Packet8i, int>;
        static constexpr size_t simdAlign = 32;
        #else
        static constexpr bool available = (N >= 4);
        static constexpr size_t width = available ? 4 : 1;
        using packetType = std::conditional_t<available, Packet4i, int>;
        static constexpr size_t simdAlign = 16;
        #endif

        static constexpr size_t scalarAlign = std::bit_ceil(sizeof(int) * N);
        static constexpr size_t alignment = (scalarAlign > simdAlign) ? simdAlign : scalarAlign;
    };
}