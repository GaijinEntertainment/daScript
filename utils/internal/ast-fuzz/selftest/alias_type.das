options gen2
// lint-skip-file: alias_type_breaker corrupts the AST and verify_module reports 50503 by design

require alias_type_breaker

[export]
def main {
    print("{1 + 2}\n")
}
