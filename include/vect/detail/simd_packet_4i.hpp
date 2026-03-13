#pragma once
#include <immintrin.h>
namespace vect::detail {
    struct Packet4i {
        __m128i reg;
        void store(int* dest) const
        {
            _mm_store_si128((__m128i*)dest, reg);
        }
        friend auto operator+(Packet4i a, Packet4i b) -> Packet4i
        {
            return {_mm_add_epi32(a.reg, b.reg)};
        }
        friend auto operator-(Packet4i a, Packet4i b) -> Packet4i
        {
            return {_mm_sub_epi32(a.reg, b.reg)};
        }
        friend auto operator*(Packet4i a, Packet4i b) -> Packet4i
        {
            return {_mm_mullo_epi32(a.reg, b.reg)};
        }
        friend auto operator/(Packet4i a, Packet4i b) -> Packet4i
        {
            alignas(16) int avals[4];
            alignas(16) int bvals[4];
            _mm_store_si128((__m128i*)avals, a.reg);
            _mm_store_si128((__m128i*)bvals, b.reg);
            alignas(16) int resultVals[4];
            for (int j = 0; j < 4; ++j) {
                resultVals[j] = avals[j] / bvals[j];
            }
            return {_mm_load_si128((__m128i*)resultVals)};
        }
        friend auto operator-(Packet4i a) -> Packet4i
        {
            return {_mm_sub_epi32(_mm_setzero_si128(), a.reg)};
    }
};}