#pragma once
#include "vect/core/mat_expr.hpp"
#include "vect/core/vector.hpp"
namespace vect::core
{

    template <typename T, size_t R, size_t C>
    class Matrix : public MatExpr<Matrix<T, R, C>, T, R, C>
    {
        std::array<Vector<T, C>, R> rows_{};

    public:
        Matrix() = default;

        Matrix(std::initializer_list<T> list)
        {
            auto it = list.begin();
            for (size_t r = 0; r < R; ++r)
            {
                for (size_t c = 0; c < C; ++c)
                {
                    if (it != list.end())
                    {
                        rows_[r][c] = *it++;
                    }
                }
            }
        }

        const T &at(size_t r, size_t c) const { return rows_[r][c]; }
        T &at(size_t r, size_t c) { return rows_[r][c]; }

        const auto &getRow(size_t r) const { return rows_[r]; }

        const T &operator()(size_t r, size_t c) const { return rows_[r][c]; }
        T &operator()(size_t r, size_t c) { return rows_[r][c]; }

        template <typename E>
        Matrix &operator=(const MatExpr<E, T, R, C> &expr)
        {
            for (size_t r = 0; r < R; ++r)
            {
                rows_[r] = expr.row(r);
            }
            return *this;
        }
    };
}