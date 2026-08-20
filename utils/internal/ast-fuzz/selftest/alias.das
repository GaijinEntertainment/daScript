options gen2
// lint-skip-file: alias_breaker corrupts the AST and verify_module reports 50503 by design

require alias_breaker

def aliased(a, b : int) : int {
    return a + b
}

[export]
def main {
    print("{aliased(1, 2)}\n")
}
