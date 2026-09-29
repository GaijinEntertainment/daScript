// Fixture for test_inline_errors.das — intentionally fails to compile: [never_fast_math] implies
// [never_inline], so a later [inline] on the same function is contradictory.
expect 20800

options gen2

[never_fast_math, inline]
def bad(x : float) : float {
    return x * 2.0 + 1.0;
}

[export]
def main() {
    let y = bad(1.0);
    print("{y}\n");
}
