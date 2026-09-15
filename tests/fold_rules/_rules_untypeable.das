options gen2
options indenting = 4

module _rules_untypeable shared public

//! A rule whose replacement names a function the engine cannot resolve. The engine must refuse it
//! rather than splice an untyped node into the optimized tree.

require daslib/fold_rules public

[fold_rules]
class UntypeableFolds : FoldRules {
    def override fold(var expr : ExpressionPtr) : ExpressionPtr {
        qmatch(expr) {
            if ($e(a) - $e(b)) {
                return qmacro(no_such_builtin($e(a), $e(b)))
            }
        }
        return null
    }
}
