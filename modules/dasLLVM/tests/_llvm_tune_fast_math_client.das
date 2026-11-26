options gen2
options fast_math = true

require daslib/tune

// The child fixture for the never_fast_math clone test of
// llvm_tune.das. add_plus_k declines the (float?, int) : float
// signature, so the function itself, every grid clone
// (DAS_TUNE_MODE=test) and every fat clone (DAS_TUNE_MODE=fat, -exe)
// compile this das body. Every fourth element is 1e8 and the rest
// 1.0: the k-ordered sum drops every 1.0 into 1e8's rounding, while a
// reassociated sum keeps them in three vector lanes. `fast_sum` is
// the same body unmarked - the witness that fast math is on and would
// change the bits.

[never_fast_math, tune_perm(k = 0), tune_perm(k = 1),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k1"), hint(noinline, noalias = p)]
def precise_sum(p : float?; n : int) : float {
    var acc = 0.0
    for (i in range(n)) {
        acc += unsafe(p[i])
    }
    return acc
}

// no fallback and no profile: a fat build clones the reference body
// itself, with no [llvm_code] stamp
[never_fast_math, tune_perm(k = 0), tune(gen = "llvm_code_selftest::add_plus_k"), hint(noinline, noalias = p)]
def precise_ref_sum(p : float?; n : int) : float {
    var acc = 0.0
    for (i in range(n)) {
        acc += unsafe(p[i])
    }
    return acc
}

[hint(noinline, noalias = p)]
def fast_sum(p : float?; n : int) : float {
    var acc = 0.0
    for (i in range(n)) {
        acc += unsafe(p[i])
    }
    return acc
}

// hand-written under a name shaped like precise_sum's fat clone: no
// clone of it, so it carries nothing of precise_sum's
[hint(noinline, noalias = p)]
def precise_sum__fat_handwritten(p : float?; n : int) : float {
    var acc = 0.0
    for (i in range(n)) {
        acc += unsafe(p[i])
    }
    return acc
}

// the same body marked and never tuned: the oracle a fat exe has no
// other copy of
[never_fast_math, hint(noinline, noalias = p)]
def precise_oracle(p : float?; n : int) : float {
    var acc = 0.0
    for (i in range(n)) {
        acc += unsafe(p[i])
    }
    return acc
}

def bits(f : float) : uint => unsafe(reinterpret<uint>(f))

[export]
def main {
    var xs : array<float>
    let n = 4096 + length(get_command_line_arguments())   // not a constant, so no call folds
    xs |> resize(n)
    for (i in range(n)) {
        xs[i] = (i % 4 == 0) ? 1.0e8 : 1.0
    }
    let p = unsafe(addr(xs[0]))
    print("FAST {bits(fast_sum(p, n))}\n")
    print("PRECISE {bits(precise_oracle(p, n))}\n")
    print("HAND {bits(precise_sum__fat_handwritten(p, n))}\n")
    print("DIRECT {bits(precise_sum(p, n))}\n")
    print("REFERENCE {bits(precise_ref_sum(p, n))}\n")
    static_if (typeinfo module_exists(llvm_tune)) {
        var vs <- precise_sum_variants()
        for (v in vs) {
            print("VARIANT {v._0} {bits(invoke(v._1, p, n))}\n")
        }
        delete vs
        var rs <- precise_ref_sum_variants()
        for (v in rs) {
            print("RVARIANT {v._0} {bits(invoke(v._1, p, n))}\n")
        }
        delete rs
    }
}
