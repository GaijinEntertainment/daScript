options gen2

require alias_breaker

def aliased(a, b : int) : int {
    return a + b
}

[export]
def main {
    print("{aliased(1, 2)}\n")
}
