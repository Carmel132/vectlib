// #include <array>
#include <print>
#include <vect/vector.hpp>

template <size_t N>
void printVector(const vect::Vector<double, N> &vec)
{
    for (size_t idx{}; idx < N; ++idx)
    {
        std::print("{}, ", vec[idx]);
    }
}

int main()
{
    vect::Vector<double, 3> v1{1, 2, 3};
    vect::Vector<double, 3> v2{1, 0, 2};

    auto v3 = vect::operator+(v1, v2);
    printVector<3>(v2);
    return 0;
}