#pragma once
#include "vect/detail/simd_mask.hpp"

namespace vect::detail
{
    inline auto simdSelect(Mask4f mask, Packet4f ifTrue, Packet4f ifFalse) -> Packet4f
    {
        return Packet4f{_mm_blendv_ps(ifFalse.reg, ifTrue.reg, mask.reg)};
    }

    inline auto simdSelect(Mask8f mask, Packet8f ifTrue, Packet8f ifFalse) -> Packet8f
    {
        return Packet8f{_mm256_blendv_ps(ifFalse.reg, ifTrue.reg, mask.reg)};
    }

    inline auto simdSelect(Mask2d mask, Packet2d ifTrue, Packet2d ifFalse) -> Packet2d
    {
        return Packet2d{_mm_blendv_pd(ifFalse.reg, ifTrue.reg, mask.reg)};
    }

    inline auto simdSelect(Mask4d mask, Packet4d ifTrue, Packet4d ifFalse) -> Packet4d
    {
        return Packet4d{_mm256_blendv_pd(ifFalse.reg, ifTrue.reg, mask.reg)};
    }

    inline auto simdSelect(Mask4i mask, Packet4i ifTrue, Packet4i ifFalse) -> Packet4i
    {
        return Packet4i{_mm_blendv_epi8(ifFalse.reg, ifTrue.reg, mask.reg)};
    }

    inline auto simdSelect(Mask8i mask, Packet8i ifTrue, Packet8i ifFalse) -> Packet8i
    {
        return Packet8i{_mm256_blendv_epi8(ifFalse.reg, ifTrue.reg, mask.reg)};
    }
}