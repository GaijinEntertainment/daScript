options gen2
require cycle_breaker

[export]
def main {
    var x = 1 // nolint:LINT003
    print("{x + x}\n")
}
