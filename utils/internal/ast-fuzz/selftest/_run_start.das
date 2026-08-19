options gen2
// lint-skip-file: deliberately malformed by run_start_breaker - compiled without the verifier it crashes the inferer
require run_start_breaker

// run_start_breaker nulls the right operand of `a + b` after inference and asks for a new run:
// plain --ast-verify reports it from that run's pass 0 walk; --ast-verify-batch has none.

def lost_at_run_start(a, b : int) : int {
    return a + b
}

[export]
def main {
    print("{lost_at_run_start(1, 2)}\n")
}
