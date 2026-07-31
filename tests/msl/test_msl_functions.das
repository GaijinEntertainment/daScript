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

def private hasc(t : T?; needle : string) {
    t |> success(find(ufxcls_msl, needle) >= 0, "emitted MSL carries `{needle}`")
    if (find(ufxcls_msl, needle) < 0) {
        to_log(LOG_ERROR, "MISSING `{needle}` in:\n{ufxcls_msl}\n")
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
        // threadgroup fixed-array param keeps its address space; `var` decides const, exactly
        // as it does for a device buffer, and the helper may rendezvous inside
        has(t, "float ufx_sum_tg(threadgroup const float* part, uint n)")
        has(t, "void ufx_scale_tg(threadgroup float* part, float k, uint lid, uint n)")
        has(t, "part[i] *= k;")
        let bar = find(userfns_msl, "void ufx_scale_tg(threadgroup float* part, float k, uint lid, uint n) \{")
        t |> success(bar >= 0 && find(userfns_msl, "threadgroup_barrier(mem_flags::mem_threadgroup);", bar) > bar,
            "barrier() inside a user function lowers in place")
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
    t |> run("class methods + inheritance — the one flat struct, statically devirtualized") <| @(t : T?) {
        // the class emits as one flat struct: inherited members and methods live in the
        // concrete class; @uniform members become by-value fields; the entry fills self
        hasc(t, "struct UfxKid \{")
        hasc(t, "device const float* xs;")
        hasc(t, "device float* ys;")           // written only through the store METHOD body
        hasc(t, "float gain;")
        hasc(t, "threadgroup float * tmp;")
        hasc(t, "uint3 gl_GlobalInvocationID;")
        // methods as member functions; the base skeleton calls the KID's override
        hasc(t, "float combo(uint i) \{")
        hasc(t, "return stage(pick(i));")
        hasc(t, "return (xs[i] + zs[i]);")     // Kid's pick — the devirtualized body
        t |> success(find(ufxcls_msl, "return xs[i];") < 0, "Base's un-overridden pick never emits")
        // the kernel body itself is a member function; the entry fills self and calls it
        hasc(t, "void ufxcls() \{")
        hasc(t, "UfxKid self_;")
        hasc(t, "self_.xs = xs;")
        hasc(t, "self_.gain = gain;")
        hasc(t, "self_.gl_GlobalInvocationID = gl_GlobalInvocationID;")
        hasc(t, "self_.ufxcls();")
        // a free function called from the kernel body still emits at global scope
        hasc(t, "float ufx_sky(float d)")
    }
}
