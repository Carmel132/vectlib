#pragma once
#include <immintrin.h>
#include <algorithm>
namespace vect::detail
{
    struct Packet8i
  {
    union
    {
        __m256i reg;
        int vals[8];
    };
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

    inline int dot(Packet8i a, Packet8i b) {
        __m256i prod = _mm256_mullo_epi32(a.reg, b.reg);
        alignas(32) int vals[8];
        _mm256_store_si256((__m256i*)vals, prod);
        int sum = 0;
        for (int i = 0; i < 8; ++i) sum += vals[i];
        return sum;
    }

    inline int sum(Packet8i a) {
        alignas(32) int vals[8];
        _mm256_store_si256((__m256i*)vals, a.reg);
        int sum = 0;
        for (int i = 0; i < 8; ++i) sum += vals[i];
        return sum;
    }

    inline int min_element(Packet8i a) {
        alignas(32) int vals[8];
        _mm256_store_si256((__m256i*)vals, a.reg);
        int min = vals[0];
        for (int i = 1; i < 8; ++i) min = std::min(min, vals[i]);
        return min;
    }

    inline int max_element(Packet8i a) {
        alignas(32) int vals[8];
        _mm256_store_si256((__m256i*)vals, a.reg);
        int max = vals[0];
        for (int i = 1; i < 8; ++i) max = std::max(max, vals[i]);
        return max;
    }

    inline bool all(Packet8i a) {
        __m256i zero = _mm256_setzero_si256();
        __m256i cmp = _mm256_cmpeq_epi32(a.reg, zero);
        alignas(32) int vals[8];
        _mm256_store_si256((__m256i*)vals, cmp);
        for (int i = 0; i < 8; ++i) if (vals[i] != 0) return false;
        return true;
    }

    inline bool any(Packet8i a) {
        __m256i zero = _mm256_setzero_si256();
        __m256i cmp = _mm256_cmpeq_epi32(a.reg, zero);
        alignas(32) int vals[8];
        _mm256_store_si256((__m256i*)vals, cmp);
        for (int i = 0; i < 8; ++i) if (vals[i] != 0) return true;
        return false;
    }
}