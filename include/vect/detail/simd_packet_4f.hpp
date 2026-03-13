#pragma once
#include <immintrin.h>
namespace vect::detail {
    struct Packet4f
  {
    __m128 reg;

    void store(float* dest) const 
    {
      _mm_store_ps(dest, reg);
    }

    friend auto operator+(Packet4f a, Packet4f b) -> Packet4f
    {
      return {_mm_add_ps(a.reg, b.reg)};
    }
    friend auto operator-(Packet4f a, Packet4f b) -> Packet4f
    {
      return {_mm_sub_ps(a.reg, b.reg)};
    }
    friend auto operator*(Packet4f a, Packet4f b) -> Packet4f
    {
      return {_mm_mul_ps(a.reg, b.reg)};
    }
    friend auto operator/(Packet4f a, Packet4f b) -> Packet4f
    {
      return {_mm_div_ps(a.reg, b.reg)};
    }
    friend auto operator-(Packet4f a) -> Packet4f
    {
      return {_mm_sub_ps(_mm_setzero_ps(), a.reg)};
    }
  };
}