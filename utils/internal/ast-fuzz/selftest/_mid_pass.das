options gen2
// lint-skip-file: deliberately malformed by mid_pass_breaker - compiled without the verifier it crashes the inferer
require mid_pass_breaker

// mid_pass_breaker nulls the right operand of `a + b` between two inference passes: plain
// --ast-verify reports it from the pre-infer walk of the next pass; --ast-verify-batch has none.

def lost_mid_pass(a, b : int) : int {
    return a + b
}

[export]
def main {
    print("{lost_mid_pass(1, 2)}\n")
}
