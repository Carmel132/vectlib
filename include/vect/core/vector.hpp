#pragma once
#include <cstddef>
#include <array>
#include <cmath>
namespace vect::core{
template <typename T, std::size_t N>
class Vector {
    public:
    std::array<T, N> data;
};
}