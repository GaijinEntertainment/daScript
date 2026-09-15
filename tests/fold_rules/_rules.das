options gen2
options indenting = 4

module _rules shared public

//! Fixture rules for the fold-rules engine tests. Each rule exists to probe one engine property:
//! operator resolution per type family, an existing subtree returned unchanged, a constant result,
//! a chain that must retry, and a rewrite the C++ passes must still reduce in the next round.

require daslib/fold_rules public

[fold_rules]
class ProbeFolds : FoldRules {
    def override fold(var expr : ExpressionPtr) : ExpressionPtr {
        qmatch(expr) {
            if (pow($e(x), 2.0)) {
                if (is_pure(x)) return qmacro($e(x) * $e(x))
            }
            if (pow($e(x), 2.0lf)) {
                if (is_pure(x)) return qmacro($e(x) * $e(x))
            }
            if (pow($e(x), float3(2.0))) {
                if (is_pure(x)) return qmacro($e(x) * $e(x))
            }
            if (pow($e(x), 1.0)) {
                return x
            }
            if (pow($e(x), 0.0)) {
                if (is_pure(x)) return qmacro(1.0)
            }
            if (sqrt(abs($e(x)))) {
                if (is_pure(x)) return qmacro(sqrt($e(x) * $e(x)))
            }
            if (cbrt($e(x))) {
                return qmacro($e(x) * 1.0)
            }
        }
        return null
    }
}
