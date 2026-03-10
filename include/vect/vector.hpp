#pragma once
#include "vect/expr/vector.hpp"

namespace vect
{

    template <typename T, size_t N>
    using Vector = core::Vector<T, N>;

    void foo()
    {
        vect::Vector<double, 3> v1{1, 2, 3};
        vect::Vector<double, 3> v2{1, 0, 2};

        auto v3 = vect::operator+(v1, v2);
    }

}