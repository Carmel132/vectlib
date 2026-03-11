#pragma once
#include "vect/core/vector.hpp"
#include "vect/expr/vector_operator.hpp"
#include "vect/core/scalar.hpp"
#include "vect/expr/vector_method.hpp"

namespace vect
{

    template <typename T, size_t N>
    using Vector = core::Vector<T, N>;

    using expr::dot;
    using expr::cross;
}