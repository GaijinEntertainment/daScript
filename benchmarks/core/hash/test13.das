options gen2
options persistent_heap

// hash(string) microbenchmark. Under -jit, hash(k) lowers to the inlined FNV
// intrinsic (llvm_jit_intrin.das); under interp it is the bound C++ builtin.
// Short and long keys show how the inline-vs-call win scales with key length.
// To measure the bound JIT path for an A/B, disable the "$::hash" entry in
// g_intrin_lookup (and bump LLVM_JIT_CODEGEN_VERSION) and re-run.

require dastest/testing_boost

let N = 256

def build_keys(prefix : string) : array<string> {
    var keys <- [for (i in range(N)); "{prefix}{i}"]
    return <- keys
}

def run_hash(b : B?; name : string; keys : array<string>) {
    b |> run(name, N) {
        var acc = 0ul
        for (k in keys) {
            acc ^= hash(k)
        }
        if (acc == 0ul) {
            b->failNow()
        }
    }
}

[benchmark]
def hash_short(b : B?) {
    var keys <- build_keys("k")
    run_hash(b, "short/{N}", keys)
    delete keys
}

[benchmark]
def hash_long(b : B?) {
    var keys <- build_keys("this/is/a/fairly/long/identifier/")
    run_hash(b, "long/{N}", keys)
    delete keys
}
