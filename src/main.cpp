// #include <array>
#include "vect/core/matrix.hpp"
#include "vect/expr/mat_binary_op.hpp"
#include "vect/expr/mat_operator.hpp"
#include <print>
#include <vect/vector.hpp>
#include "vect/expr/mat_method.hpp"
template <typename T>
void printMat(T mat)
{
    for (size_t r = 0; r < T::rows; ++r)
    {
        std::cout << mat.getRow(r);
    }
}

auto main() -> int
{

    vect::core::Matrix<float, 3, 4> m{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    // printMat(m);
    //printMat(vect::expr::MatBinaryOp<decltype(m), decltype(m), std::plus<>>{m, m});

    vect::core::Matrix<float, 4, 3> res = vect::expr::transpose(m);
    printMat(res);
    return 0;
}