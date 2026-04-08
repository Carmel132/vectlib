// #include <array>
#include "vect/core/matrix.hpp"
#include "vect/expr/mat_binary_op.hpp"
#include "vect/expr/mat_operator.hpp"
#include <cstddef>
#include <print>
#include <vect/vector.hpp>
#include "vect/expr/mat_method.hpp"
template <typename T>
void printMat(T mat)
{
    for (size_t r = 0; r < T::rows; ++r)
    {
        for (size_t c = 0; c < T::columns; ++c)
        {
            std::cout << mat.at(r, c) << " ";
        }
        std::cout << "\n";
    }
}

auto main() -> int
{

    vect::core::Matrix<float, 3, 4> m{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    // printMat(m);
    //printMat(vect::expr::MatBinaryOp<decltype(m), decltype(m), std::plus<>>{m, m});

    vect::core::Matrix<float, 4, 3> res = vect::expr::transpose(m);

    auto v = vect::Vector<float, 4>{1, 2, 3, 4};
    auto dotProd = vect::dot(v, v);
    std::cout << "result = " << dotProd << std::endl;


    auto tst = m * v;

    printMat(m);
    std::cout << "\n" << v << "\n";
    std::cout << tst;
    return 0;
}