options gen2
options indenting = 4

// Phase 0 (user-function lowering): the UserFns fixture's emitted text carries every new
// surface — plain-function prototypes then bodies, device/threadgroup/thread-ref parameter
// address spaces from the das signature, write-set through the callee signature, hidden
// builtin threading, generic twin instances, value returns. The behavioral GPU-vs-CPU twin
// is tests/metal/test_metal_functions.das.

require dastest/testing_boost public
require _msl_common
require strings

def private has(t : T?; needle : string) {
    t |> success(find(userfns_msl, needle) >= 0, "emitted MSL carries `{needle}`")
    if (find(userfns_msl, needle) < 0) {
        to_log(LOG_ERROR, "MISSING `{needle}` in:\n{userfns_msl}\n")
    }
}

[test]
def test_msl_functions(t : T?) {
    t |> run("user functions lower 1:1 — signatures, address spaces, hidden builtins") <| @(t : T?) {
        // free function with a value return, called nested
        has(t, "float ufx_sky(float d)")
        has(t, "float ufx_shade(float d, int bounce)")
        // hidden builtin threaded as a trailing by-value parameter, at decl and call
        has(t, "uint ufx_lane(uint3 gl_GlobalInvocationID)")
        has(t, "ufx_lane(gl_GlobalInvocationID)")
        // write-through var array param lowers device non-const; call in statement position
        has(t, "void ufx_splat(device float* yo, uint i, float v)")
        has(t, "ufx_splat(yo, i, acc);")
        // read-only array param lowers device const — the generic's two instances split names
        has(t, "device const float* src")
        has(t, "device const half* src")
        // threadgroup fixed-array param keeps its address space (const from the signature)
        has(t, "float ufx_sum_tg(threadgroup const float* part, uint n)")
        // thread-space reference param
        has(t, "void ufx_accum(thread float& acc, float v)")
        // prototypes precede bodies: the first ufx_sky mention ends with ';'
        let proto = find(userfns_msl, "float ufx_sky(float d);")
        let body = find(userfns_msl, "float ufx_sky(float d) \{")
        t |> success(proto >= 0 && body > proto, "prototypes precede bodies")
    }
    t |> run("both generic instances emit with distinct names") <| @(t : T?) {
        // one instance keeps the base name, the twin gets a suffixed one; both are called
        let base = find(userfns_msl, "float ufx_twice(")
        let twin = find(userfns_msl, "half ufx_twice_")
        t |> success(base >= 0, "float instance emitted")
        t |> success(twin >= 0, "half instance emitted under a suffixed name")
    }
}
