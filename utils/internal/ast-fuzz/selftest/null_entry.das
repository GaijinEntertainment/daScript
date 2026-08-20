options gen2
// lint-skip-file: null_entry_breaker corrupts the AST and verify_module reports 50503 by design
require null_entry_breaker

[export]
def main {
    // dropped by the breaker, so nothing below may reference it
    var probe = 1
    print("ok\n")
}
