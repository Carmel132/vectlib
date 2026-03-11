#pragma once 
#include "vect/core/vector.hpp"
#include "vect/core/vec_expr.hpp"
#include "vect/expr/capture_strategy.hpp"

namespace vect::expr {
    /**
     * @brief Computes the dot product of two vector expressions.
     */
    template <core::IsVecExpr L, core::IsVecExpr R>
    constexpr auto dot(const L& l, const R& r)
    {
        using T = std::common_type_t<typename L::valueType, typename R::valueType>;
        T result{};
        for (size_t idx{}; idx < l.size(); ++idx) {
            result += l[idx] * r[idx];
        }
        return result;
    }

    template <typename L, typename R> 
    class CrossProduct : public core::VecExpr<CrossProduct<L, R>> {
        core::capture_t<L> l_;
        core::capture_t<R> r_;
        
    public:
        using valueType = std::common_type_t<typename L::valueType, typename R::valueType>;
        constexpr CrossProduct(const L& l, const R& r) : l_{l}, r_{r} {}

        constexpr auto operator[](size_t idx) const -> valueType {
            size_t i = (idx + 1) % 3;
            size_t j = (idx + 2) % 3;
            return l_[i] * r_[j] - l_[j] * r_[i]; // neat lil trick
        }

        [[nodiscard]] constexpr auto size() const -> size_t { return 3; }
    };
    /// @brief Computes the cross product of two 3D vector expressions
    /// @note The function is only defined for 3D vectors. For vector dimensions n>3, only the first three components are used. For n<3, the behavior is undefined.
    template <core::IsVecExpr L, core::IsVecExpr R>
    constexpr auto cross(const L& l, const R& r) {
        return CrossProduct<L, R>(l, r);
    }
}