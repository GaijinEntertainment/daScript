options gen2
// lint-skip-file: cycle_breaker corrupts the AST and verify_module reports 50503 by design
require cycle_breaker

[export]
def main {
    var x = 1 // nolint:LINT003
    print("{x + x}\n")
}
