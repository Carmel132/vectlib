// #include <array>
#include <print>
#include <vect/vector.hpp>

template <typename T>
void printVector(const vect::core::VecExpr<T> &vec)
{
    for (size_t idx{}; idx < vec.size(); ++idx)
    {
        std::print("{}, ", vec[idx]);
    }
}

int main()
{
    vect::Vector<double, 3> v1{1, 2, 3};
    vect::Vector<double, 3> v2{1, 0, 2};

    //vect::Vector<double, 3> v3 = v1 * 5.;

    auto d = v1 ^ v2;
    std::print("dot: {}\n", d);
    //printVector(v3);
    return 0;
}