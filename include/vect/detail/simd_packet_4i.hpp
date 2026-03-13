#pragma once
#include <immintrin.h>
#include <algorithm>
namespace vect::detail {
    struct Packet4i {
        union
        {
            __m128i reg;
            int vals[4];
        };
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
};

    inline int dot(Packet4i a, Packet4i b) {
        __m128i prod = _mm_mullo_epi32(a.reg, b.reg);
        // Sum the 4 ints
        alignas(16) int vals[4];
        _mm_store_si128((__m128i*)vals, prod);
        return vals[0] + vals[1] + vals[2] + vals[3];
    }

    inline int sum(Packet4i a) {
        alignas(16) int vals[4];
        _mm_store_si128((__m128i*)vals, a.reg);
        return vals[0] + vals[1] + vals[2] + vals[3];
    }

    inline int min_element(Packet4i a) {
        alignas(16) int vals[4];
        _mm_store_si128((__m128i*)vals, a.reg);
        int min = vals[0];
        for (int i = 1; i < 4; ++i) min = std::min(min, vals[i]);
        return min;
    }

    inline int max_element(Packet4i a) {
        alignas(16) int vals[4];
        _mm_store_si128((__m128i*)vals, a.reg);
        int max = vals[0];
        for (int i = 1; i < 4; ++i) max = std::max(max, vals[i]);
        return max;
    }

    inline bool all(Packet4i a) {
        __m128i zero = _mm_setzero_si128();
        __m128i cmp = _mm_cmpeq_epi32(a.reg, zero);
        int mask = _mm_movemask_epi8(cmp); // for epi32, need to check
        alignas(16) int vals[4];
        _mm_store_si128((__m128i*)vals, cmp);
        return vals[0] == 0 && vals[1] == 0 && vals[2] == 0 && vals[3] == 0; // all equal to zero means all original were zero
    }

    inline bool any(Packet4i a) {
        __m128i zero = _mm_setzero_si128();
        __m128i cmp = _mm_cmpeq_epi32(a.reg, zero);
        alignas(16) int vals[4];
        _mm_store_si128((__m128i*)vals, cmp);
        return !(vals[0] == 0 && vals[1] == 0 && vals[2] == 0 && vals[3] == 0);
    }
}