options gen2

require _llvm_tune_carry_probe

// The client of llvm_tune.das's carry test: `carrier` already holds
// the hint `origin` carries, and takes the rest

[never_fast_math, hint(noinline)]
def origin(a : float) : float => a * 2.0

[hint(noinline), carry_twice(from = "origin")]
def carrier(a : float) : float => a * 2.0

[export]
def main {
    print("{origin(1.0) + carrier(1.0)}\n")
}
