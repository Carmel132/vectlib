#pragma once

#include "vect/core/mat_expr.hpp"
#include "vect/expr/mat_binary_op.hpp"
#include "vect/expr/mat_unary_op.hpp"
#include "vect/expr/vector_method.hpp"
#include <functional>


namespace vect::expr {
    template <typename L, typename R>
    class MatVecMultExpr : public core::VecExpr<MatVecMultExpr<L, R>> {
        core::capture_t<L> l_;
        core::capture_t<R> r_;

    public:
        using valueType = std::common_type_t<typename L::valueType, typename R::valueType>;
        static constexpr size_t dim = L::rows;

        MatVecMultExpr(const L& l, const R& r) : l_{l}, r_{r} {
            static_assert(L::columns == R::dim, "Matrix columns must match vector dimension for multiplication");
        }

        [[nodiscard]] auto operator[](size_t idx) const -> valueType {
            valueType sum = valueType{};
            
            auto row = l_.getRow(idx);
            for (size_t c = 0; c < L::columns; ++c) {
                sum += row[c] * r_[c];
            }
            return sum;
        }

        [[nodiscard]] auto size() const -> size_t { return dim; }
    
        [[nodiscard]] auto loadPacket(size_t idx) const {
            return loadPacketUnaligned(idx);
        }

        [[nodiscard]] auto loadPacketUnaligned(size_t idx) const {
            using Traits = detail::SimdTraits<valueType, dim>;
            using Packet = typename Traits::packetType;

            alignas(Traits::alignment) valueType vals[Traits::width];

            for (size_t i = 0; i < Traits::width; ++i) {
                if (idx + + i < dim) {
                    vals[i] = expr::dot(l_.getRow(idx + i), r_);
                } else {
                    vals[i] = 0;
                }
            }

            return Packet::load(vals);
        }
    };
}

namespace vect::core
{

    template <IsMatExpr L, IsMatExpr R>
    auto operator+(const L &l, const R &r)
    {
        return expr::MatBinaryOp<L, R, std::plus<>>(l, r);
    }

    template <IsMatExpr L, IsMatExpr R>
    auto operator-(const L &l, const R &r)
    {
        return expr::MatBinaryOp<L, R, std::minus<>>(l, r);
    }

    template <IsMatExpr M>
    auto operator-(const M& m) {
        return expr::MatUnaryOp<M, std::negate<>>(m);
    }

    template <IsMatExpr M, IsVecExpr V>
    requires (M::columns == V::dim)
    auto operator*(const M& m, const V& v) {
        return expr::MatVecMultExpr<M, V>(m, v);
    }
}