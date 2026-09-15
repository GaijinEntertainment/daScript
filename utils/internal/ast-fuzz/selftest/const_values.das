options gen2
// lint-skip-file: const_values_breaker flags a literal with a variable element and the verifier reports it by design

require const_values_breaker

[export]
def main {
    var x = 5
    let a <- array<int>(1, x, 3)
    print("{a}\n")
}
