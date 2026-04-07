// #include <array>
#include "vect/core/matrix.hpp"
#include "vect/expr/mat_binary_op.hpp"
#include <print>
#include <vect/vector.hpp>
template <typename T>
void printMat(T mat)
{
    for (size_t r = 0; r < mat.rows; ++r)
    {
        std::cout << mat.getRow(r);
    }
}

auto main() -> int
{

    vect::core::Matrix<float, 3, 4> m{1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
    // printMat(m);
    printMat(vect::expr::MatBinaryOp<typeof m, typeof m, std::plus<>>{m, m});
    return 0;
}