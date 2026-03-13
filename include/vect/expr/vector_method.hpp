#pragma once 
#include "vect/core/vector.hpp"
#include "vect/core/vec_expr.hpp"
#include "vect/expr/capture_strategy.hpp"
#include "vect/core/scalar.hpp"
#include "vect/expr/binary_op.hpp"
#include "vect/expr/unary_op.hpp"

#include "vect/detail/vector_method.hpp"

#include <algorithm>
namespace vect::expr {
    /**
     * @brief Computes the dot product of two vector expressions.
     */
    template <core::IsVecExpr L, core::IsVecExpr R>
    constexpr auto dot(const L& l, const R& r)
    {
        static_assert(L::dim == R::dim, "Dot product is only defined for vectors of the same dimension");
        return detail::dot_fold_impl(l, r, std::make_index_sequence<L::dim>{});
    }

    template <typename L, typename R> 
    class CrossProduct : public core::VecExpr<CrossProduct<L, R>> {
        core::capture_t<L> l_;
        core::capture_t<R> r_;
        
    public:
        using valueType = std::common_type_t<typename L::valueType, typename R::valueType>;
        static constexpr size_t dim = 3;
        constexpr CrossProduct(const L& l, const R& r) : l_{l}, r_{r} {
            if constexpr (L::dim != 3) {
                static_assert(L::dim == 3, "Cross product is only defined for vectors of dimension 3");
            }
        }

        constexpr auto operator[](size_t idx) const -> valueType {
            size_t i = (idx + 1) % 3;
            size_t j = (idx + 2) % 3;
            return l_[i] * r_[j] - l_[j] * r_[i]; // neat lil trick
        }

        [[nodiscard]] constexpr auto size() const -> size_t { return 3; }
    };

    /// @brief Computes the cross product of two 3D vector expressions
    /// @note The function is only defined for 3D vectors
    template <core::IsVec3Expr L, core::IsVec3Expr R>
    constexpr auto cross(const L& l, const R& r) {
        return CrossProduct<L, R>(l, r);
    }

    template <core::IsVecExpr V>
    constexpr auto sum(const V& v) {
        return detail::sum_fold_impl(v, std::make_index_sequence<V::dim>{});
    }

    template <core::IsVecExpr V>
    constexpr auto normSquared(const V& v) {
        return dot(v, v);
    }

    template <core::IsVecExpr V>
    constexpr auto norm(const V& v) {
        return std::sqrt(normSquared(v));
    }

    template <core::IsVecExpr V>
    constexpr auto normalize(const V& v) {
        return v / norm(v);
    }

    template <core::IsVecExpr A, core::IsVecExpr B, core::Scalar S>
    constexpr auto lerp(const A& a, const B& b, S t) {
        return (b - a) * t + a;
    }

    template <core::IsVecExpr A, core::IsVecExpr B>
    constexpr auto project(const A& a, const B& b) {
        return (dot(a, b) / dot(b, b)) * b;
    }

    template <core::IsVecExpr A, core::IsVecExpr B>
    constexpr auto reject(const A& a, const B& b) {
        return a - project(a, b);
    }

    template <core::IsVecExpr A, core::IsVecExpr B>
    constexpr auto reflect(const A& i, const B& n) {
        return i - n * (2.0 * dot(i, n));
    }

    template <core::IsVecExpr V, typename F>
    constexpr auto map(V&& v, F&& f) {
        using ExprType = std::decay_t<V>;
        using FuncType = std::decay_t<F>;

        return UnaryOp<ExprType, FuncType>(std::forward<V>(v), std::forward<F>(f));
    }

    template <core::IsVecExpr V, core::Scalar S>
    constexpr auto max(const V& v, S s) {
        return map(v, [s](auto x) { return x > s ? x : s; });
    }

    template <core::IsVecExpr V, core::Scalar S>
    constexpr auto min(const V& v, S s) {
        return map(v, [s](auto x) { return x < s ? x : s; });
    }

    template <core::IsVecExpr V>
    constexpr auto abs(const V& v) {
        return map(v, [](auto x) { 
            using std::abs;
            return abs(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto sqrt(const V& v) {
        return map(v, [](auto x) { 
            using std::sqrt;
            return sqrt(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto exp(const V& v) {
        return map(v, [](auto x) { 
            using std::exp;
            return exp(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto log(const V& v) {
        return map(v, [](auto x) { 
            using std::log;
            return log(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto negate(const V& v) {
        return map(v, [](auto x) { return -x; });
    }

    template <core::IsVecExpr V>
    constexpr auto sin(const V& v) {
        return map(v, [](auto x) { 
            using std::sin;
            return sin(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto cos(const V& v) {
        return map(v, [](auto x) { 
            using std::cos;
            return cos(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto tan(const V& v) {
        return map(v, [](auto x) { 
            using std::tan;
            return tan(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto asin(const V& v) {
        return map(v, [](auto x) { 
            using std::asin;
            return asin(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto acos(const V& v) {
        return map(v, [](auto x) { 
            using std::acos;
            return acos(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto atan(const V& v) {
        return map(v, [](auto x) { 
            using std::atan;
            return atan(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto floor(const V& v) {
        return map(v, [](auto x) { 
            using std::floor;
            return floor(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto ceil(const V& v) {
        return map(v, [](auto x) { 
            using std::ceil;
            return ceil(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto round(const V& v) {
        return map(v, [](auto x) { 
            using std::round;
            return round(x);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto sign(const V& v) {
        return map(v, [](auto x) { 
            return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
        });
    }

    template <core::IsVecExpr V>
    constexpr auto fract(const V& v) {
        return map(v, [](auto x) { 
            using std::floor;
            return x - floor(x);
        });
    }

    template <core::IsVecExpr V, core::Scalar S>
    constexpr auto clamp(const V& v, S lo, S hi) {
        return map(v, [lo, hi](auto x) {
            return std::clamp(x, static_cast<decltype(x)>(lo), static_cast<decltype(x)>(hi));
        });
    }

    template <typename E>
    requires core::IsVecExpr<E>
    bool all(const E& expr) {
        using T = typename E::valueType;
        using Traits = detail::SimdTraits<T, E::dim>;

        size_t idx = 0;
        if constexpr (Traits::available) {
            using Packet = typename Traits::packetType;
            for (; idx <= E::dim - Traits::width; idx += Traits::width) {
                Packet packet = expr.loadPacket(idx);
                
                using detail::all;
                if (!all(packet)) return false;
            }
        }

        for (; idx < E::dim; ++idx) {
            if (!expr[idx]) return false;
        }

        return true;
    }

    template <typename E>
    requires core::IsVecExpr<E>
    bool any(const E& expr) {
        using T = typename E::valueType;
        using Traits = detail::SimdTraits<T, E::dim>;
        size_t idx = 0;
        if constexpr (Traits::available) {
            for (; idx <= E::dim - Traits::width; idx += Traits::width) {
                auto packet = expr.loadPacket(idx);

                using detail::any;
                if (any(packet)) return true;
            }
        }

        for (; idx < E::dim; ++idx) {
            if (expr[idx]) return true;
        }
        
        return false;
    }


}