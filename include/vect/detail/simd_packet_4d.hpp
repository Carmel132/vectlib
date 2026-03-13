#pragma once
#include <immintrin.h>
#include <cmath>
namespace vect::detail
{
    struct Packet4d {
        union {
            __m256d reg;
            double vals[4];
        };
        void store(double* dest) const 
        {
            _mm256_store_pd(dest, reg);
        }
        friend auto operator+(Packet4d a, Packet4d b) -> Packet4d
        {
            return {_mm256_add_pd(a.reg, b.reg)};
        }
        friend auto operator-(Packet4d a, Packet4d b) -> Packet4d
        {
            return {_mm256_sub_pd(a.reg, b.reg)};
        }
        friend auto operator*(Packet4d a, Packet4d b) -> Packet4d
        {
            return {_mm256_mul_pd(a.reg, b.reg)};
        }
        friend auto operator/(Packet4d a, Packet4d b) -> Packet4d
        {
            return {_mm256_div_pd(a.reg, b.reg)};
        }
        friend auto operator-(Packet4d a) -> Packet4d
        {
            return {_mm256_sub_pd(_mm256_setzero_pd(), a.reg)};
        }
    };

    inline double dot(Packet4d a, Packet4d b) {
        __m256d prod = _mm256_mul_pd(a.reg, b.reg);
        __m256d sum1 = _mm256_hadd_pd(prod, prod);
        __m256d sum2 = _mm256_hadd_pd(sum1, sum1);
        return _mm256_cvtsd_f64(sum2);
    }

    inline double sum(Packet4d a) {
        __m256d sum1 = _mm256_hadd_pd(a.reg, a.reg);
        __m256d sum2 = _mm256_hadd_pd(sum1, sum1);
        return _mm256_cvtsd_f64(sum2);
    }

    inline Packet4d sqrt(Packet4d a) {
        return {_mm256_sqrt_pd(a.reg)};
    }

    inline double norm(Packet4d a) {
        return std::sqrt(dot(a, a));
    }

    inline double min_element(Packet4d a) {
        __m256d min1 = _mm256_min_pd(a.reg, _mm256_shuffle_pd(a.reg, a.reg, 5)); // 0b0101
        __m256d min2 = _mm256_min_pd(min1, _mm256_permute2f128_pd(min1, min1, 1));
        return _mm256_cvtsd_f64(min2);
    }

    inline double max_element(Packet4d a) {
        __m256d max1 = _mm256_max_pd(a.reg, _mm256_shuffle_pd(a.reg, a.reg, 5));
        __m256d max2 = _mm256_max_pd(max1, _mm256_permute2f128_pd(max1, max1, 1));
        return _mm256_cvtsd_f64(max2);
    }

    inline Packet4d cross(Packet4d a, Packet4d b) {
        alignas(32) double avals[4];
        alignas(32) double bvals[4];
        _mm256_store_pd(avals, a.reg);
        _mm256_store_pd(bvals, b.reg);
        alignas(32) double result[4] = {
            avals[1] * bvals[2] - avals[2] * bvals[1],
            avals[2] * bvals[0] - avals[0] * bvals[2],
            avals[0] * bvals[1] - avals[1] * bvals[0],
            0.0
        };
        return {_mm256_load_pd(result)};
    }

    inline Packet4d normalize(Packet4d a) {
        double n = norm(a);
        if (n == 0.0) return a;
        __m256d inv_n = _mm256_set1_pd(1.0 / n);
        return {_mm256_mul_pd(a.reg, inv_n)};
    }

    inline bool all(Packet4d a) {
        __m256d zero = _mm256_setzero_pd();
        __m256d cmp = _mm256_cmp_pd(a.reg, zero, _CMP_NEQ_OQ);
        int mask = _mm256_movemask_pd(cmp);
        return mask == 15; // all 4 bits set
    }

    inline bool any(Packet4d a) {
        __m256d zero = _mm256_setzero_pd();
        __m256d cmp = _mm256_cmp_pd(a.reg, zero, _CMP_NEQ_OQ);
        int mask = _mm256_movemask_pd(cmp);
        return mask != 0;
    }
}