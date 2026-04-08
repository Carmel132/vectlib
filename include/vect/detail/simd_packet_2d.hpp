#pragma once
#include <cmath>
#include <immintrin.h>
namespace vect::detail
{
    struct Packet2d
    {
        union
        {
            __m128d reg;
            double vals[2];
        };

        void store(double *dest) const
        {
            _mm_store_pd(dest, reg);
        }

        void storeUnaligned(double *dest) const
        {
            _mm_storeu_pd(dest, reg);
        }

        static auto load(const double *src) -> Packet2d
        {
            return {_mm_load_pd(src)};
        }
        static auto loadUnaligned(const double *src) -> Packet2d
        {
            return {_mm_loadu_pd(src)};
        }

        static auto broadcast(double val) -> Packet2d
        {
            return {_mm_set1_pd(val)};
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

    inline auto dot(Packet2d a, Packet2d b) -> double
    {
        __m128d prod = _mm_mul_pd(a.reg, b.reg);
        __m128d sum = _mm_hadd_pd(prod, prod);
        return _mm_cvtsd_f64(sum);
    }

    inline auto sum(Packet2d a) -> double
    {
        __m128d sum = _mm_hadd_pd(a.reg, a.reg);
        return _mm_cvtsd_f64(sum);
    }

    inline auto sqrt(Packet2d a) -> Packet2d
    {
        return {_mm_sqrt_pd(a.reg)};
    }

    inline auto norm(Packet2d a) -> double
    {
        return std::sqrt(dot(a, a));
    }

    inline auto max(Packet2d a, Packet2d b) -> Packet2d
    {
        return {_mm_max_pd(a.reg, b.reg)};
    }

    inline auto min(Packet2d a, Packet2d b) -> Packet2d
    {
        return {_mm_min_pd(a.reg, b.reg)};
    }

    inline auto min_element(Packet2d a) -> double
    {
        __m128d min = _mm_min_pd(a.reg, _mm_shuffle_pd(a.reg, a.reg, 1));
        return _mm_cvtsd_f64(min);
    }

    inline auto max_element(Packet2d a) -> double
    {
        __m128d max = _mm_max_pd(a.reg, _mm_shuffle_pd(a.reg, a.reg, 1));
        return _mm_cvtsd_f64(max);
    }

    inline auto cross(Packet2d a, Packet2d b) -> double
    {
        // a.x * b.y - a.y * b.x
        __m128d prod1 = _mm_mul_pd(a.reg, _mm_shuffle_pd(b.reg, b.reg, 1)); // a.x*b.y, a.y*b.x
        __m128d prod2 = _mm_mul_pd(_mm_shuffle_pd(a.reg, a.reg, 1), b.reg); // a.y*b.x, a.x*b.y
        __m128d diff = _mm_sub_pd(prod1, prod2);
        return _mm_cvtsd_f64(diff);
    }

    inline auto normalize(Packet2d a) -> Packet2d
    {
        double n = norm(a);
        if (n == 0.0)
            return a;
        __m128d inv_n = _mm_set1_pd(1.0 / n);
        return {_mm_mul_pd(a.reg, inv_n)};
    }

    inline auto all(Packet2d a) -> bool
    {
        __m128d zero = _mm_setzero_pd();
        __m128d cmp = _mm_cmpneq_pd(a.reg, zero);
        int mask = _mm_movemask_pd(cmp);
        return mask == 3;
    }

    inline auto any(Packet2d a) -> bool
    {
        __m128d zero = _mm_setzero_pd();
        __m128d cmp = _mm_cmpneq_pd(a.reg, zero);
        int mask = _mm_movemask_pd(cmp);
        return mask != 0;
    }
}