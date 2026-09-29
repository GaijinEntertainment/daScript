// Fixture for test_inline_errors.das — intentionally fails to compile: [inline] splices the
// body into its callers, where the LLVM tiers stamp fast-math flags [never_fast_math] withholds.
expect 20800

options gen2

[inline, never_fast_math]
def bad(x : float) : float {
    return x * 2.0 + 1.0;
}

[export]
def main() {
    let y = bad(1.0);
    print("{y}\n");
}
