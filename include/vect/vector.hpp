#pragma once
#include "vect/core/vector.hpp"
#include "vect/expr/vector_operator.hpp"
#include "vect/core/scalar.hpp"
#include "vect/expr/vector_method.hpp"
#include "vect/expr/swizzle.hpp"
namespace vect
{

    template <typename T, size_t N>
    using Vector = core::Vector<T, N>;

    using expr::dot;
    using expr::cross;
    using expr::sum;
    using expr::norm;
    using expr::normSquared;
    using expr::normalize;
    using expr::lerp;
    using expr::project;
    using expr::reject;
    using expr::reflect;
    using expr::map;
    using expr::max;
    using expr::min;
    using expr::abs;
    using expr::sqrt;
    using expr::exp;
    using expr::log;
    using expr::negate;
    using expr::sin;
    using expr::cos;
    using expr::tan;
    using expr::asin;
    using expr::acos;
    using expr::atan;
    using expr::floor;
    using expr::ceil;
    using expr::round;
    using expr::sign;
    using expr::fract;

    using float2 = core::Vector<float, 2>;
    using float3 = core::Vector<float, 3>;

    using float4 = core::Vector<float, 4>;
}