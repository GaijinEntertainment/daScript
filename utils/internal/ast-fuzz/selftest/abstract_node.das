options gen2
// lint-skip-file: abstract_node_breaker corrupts the AST and verify_module reports it by design
require abstract_node_breaker

[export]
def main {
    var x = 1
    print("{x + x}\n")
}
