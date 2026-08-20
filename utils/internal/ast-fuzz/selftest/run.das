options gen2
// lint-skip-file: verify_breaker corrupts the AST and verify_module reports 50503 by design
require verify_breaker

[export]
def main {
    // var (not let) so `x + x` is not const-folded — the breaker needs a live
    // ExprOp2 to null; using x on both sides keeps it used after the repair.
    var x = 1
    print("{x + x}\n")
}
