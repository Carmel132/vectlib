#pragma once
#include <immintrin.h>
namespace vect::detail
{
    struct Packet8i
  {
    __m256i reg;
    void store(int* dest) const
    {
        _mm256_store_si256((__m256i*)dest, reg);
    }
    friend auto operator+(Packet8i a, Packet8i b) -> Packet8i
    {
        return {_mm256_add_epi32(a.reg, b.reg)};
    }
    friend auto operator-(Packet8i a, Packet8i b) -> Packet8i
    {
        return {_mm256_sub_epi32(a.reg, b.reg)};
    }
    friend auto operator*(Packet8i a, Packet8i b) -> Packet8i
    {
        return {_mm256_mullo_epi32(a.reg, b.reg)};
    }
    friend auto operator/(Packet8i a, Packet8i b) -> Packet8i
    {
        alignas(32) int avals[8];
        alignas(32) int bvals[8];
        alignas(32) int resultVals[8];
        _mm256_store_si256((__m256i*)avals, a.reg);
        _mm256_store_si256((__m256i*)bvals, b.reg);
        for (int j = 0; j < 8; ++j)
        {
            resultVals[j] = avals[j] / bvals[j];
        }
        return {_mm256_load_si256((__m256i*)resultVals)};
    }
    friend auto operator-(Packet8i a) -> Packet8i
    {
        return {_mm256_sub_epi32(_mm256_setzero_si256(), a.reg)};
    }
  };
}