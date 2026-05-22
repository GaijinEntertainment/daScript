// Primes-loop benchmark — ported from dasProfile/tests/primes.das.
// Trial-division prime count up to 14000.
options gen2

def isprime(n) {
    for (i in range(2, n)) {
        return false if (n % i == 0)
    }
    return true
}

[sideeffects]
def primes(n) {
    var count = 0
    for (i in range(2, n + 1)) {
        if (isprime(i)) {
            ++count
        }
    }
    return count
}

[export]
def main {
    var f1 = 0
    profile(10, "primes loop") {
        f1 = primes(14000)
    }
    assert(f1 == 1652)
}
