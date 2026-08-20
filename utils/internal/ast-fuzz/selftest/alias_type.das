options gen2
// lint-skip-file: alias_type_breaker verifies a deliberately aliased throwaway expression - verify_expression reports 50503 by design

require alias_type_breaker

[export]
def main {
    print("{1 + 2}\n")
}
