options gen2
options indenting = 4

module _rules_cycle shared public

//! A deliberately non-monotonic rule: it swaps the operands of every addition, so its own output
//! matches it again. The engine's retry cap is what turns this into a diagnostic.

require daslib/fold_rules public

[fold_rules]
class CycleFolds : FoldRules {
    def override fold(var expr : ExpressionPtr) : ExpressionPtr {
        qmatch(expr) {
            if ($e(a) + $e(b)) {
                return qmacro($e(b) + $e(a))
            }
        }
        return null
    }
}
