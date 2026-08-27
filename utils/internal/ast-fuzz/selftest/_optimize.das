options gen2
// lint-skip-file
require optimize_breaker

def lost_at_optimize(a, b : int) : int {
    return a + b
}

[export]
def main {
    print("{lost_at_optimize(2, 3)}\n")
}
