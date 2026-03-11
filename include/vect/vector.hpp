#pragma once
#include "vect/core/vector.hpp"
#include "vect/expr/vector.hpp"
#include "vect/expr/vector_operator.hpp"
#include "vect/core/scalar.hpp"

namespace vect
{

    template <typename T, size_t N>
    using Vector = core::Vector<T, N>;
}