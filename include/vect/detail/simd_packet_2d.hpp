#pragma once
#include <immintrin.h>
#include <cmath>
namespace vect::detail {
    struct Packet2d {
        union {
            __m128d reg;
            double vals[2];
        };

        void store(double* dest) const 
        {
            _mm_store_pd(dest, reg);
        }

        friend auto operator+(Packet2d a, Packet2d b) -> Packet2d
        {
            return {_mm_add_pd(a.reg, b.reg)};
        }
        friend auto operator-(Packet2d a, Packet2d b) -> Packet2d
        {
            return {_mm_sub_pd(a.reg, b.reg)};
        }
        friend auto operator*(Packet2d a, Packet2d b) -> Packet2d
        {
            return {_mm_mul_pd(a.reg, b.reg)};
        }
        friend auto operator/(Packet2d a, Packet2d b) -> Packet2d
        {
            return {_mm_div_pd(a.reg, b.reg)};
        }
        friend auto operator-(Packet2d a) -> Packet2d
        {
            return {_mm_sub_pd(_mm_setzero_pd(), a.reg)};
        }
    };

    inline double dot(Packet2d a, Packet2d b) {
        __m128d prod = _mm_mul_pd(a.reg, b.reg);
        __m128d sum = _mm_hadd_pd(prod, prod);
        return _mm_cvtsd_f64(sum);
    }

    inline double sum(Packet2d a) {
        __m128d sum = _mm_hadd_pd(a.reg, a.reg);
        return _mm_cvtsd_f64(sum);
    }

    inline Packet2d sqrt(Packet2d a) {
        return {_mm_sqrt_pd(a.reg)};
    }

    inline double norm(Packet2d a) {
        return std::sqrt(dot(a, a));
    }

    inline double min_element(Packet2d a) {
        __m128d min = _mm_min_pd(a.reg, _mm_shuffle_pd(a.reg, a.reg, 1));
        return _mm_cvtsd_f64(min);
    }

    inline double max_element(Packet2d a) {
        __m128d max = _mm_max_pd(a.reg, _mm_shuffle_pd(a.reg, a.reg, 1));
        return _mm_cvtsd_f64(max);
    }

    inline double cross(Packet2d a, Packet2d b) {
        // a.x * b.y - a.y * b.x
        __m128d prod1 = _mm_mul_pd(a.reg, _mm_shuffle_pd(b.reg, b.reg, 1)); // a.x*b.y, a.y*b.x
        __m128d prod2 = _mm_mul_pd(_mm_shuffle_pd(a.reg, a.reg, 1), b.reg); // a.y*b.x, a.x*b.y
        __m128d diff = _mm_sub_pd(prod1, prod2);
        return _mm_cvtsd_f64(diff);
    }

    inline Packet2d normalize(Packet2d a) {
        double n = norm(a);
        if (n == 0.0) return a;
        __m128d inv_n = _mm_set1_pd(1.0 / n);
        return {_mm_mul_pd(a.reg, inv_n)};
    }

    inline bool all(Packet2d a) {
        __m128d zero = _mm_setzero_pd();
        __m128d cmp = _mm_cmpneq_pd(a.reg, zero);
        int mask = _mm_movemask_pd(cmp);
        return mask == 3;
    }

    inline bool any(Packet2d a) {
        __m128d zero = _mm_setzero_pd();
        __m128d cmp = _mm_cmpneq_pd(a.reg, zero);
        int mask = _mm_movemask_pd(cmp);
        return mask != 0;
    }
}