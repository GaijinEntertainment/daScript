options gen2
options indenting = 4

// Phase 0 (user-function lowering): the UserFns fixture's emitted text carries every new
// surface — plain-function prototypes then bodies, device/threadgroup/thread-ref parameter
// address spaces from the das signature, write-set through the callee signature, hidden
// builtin threading, generic twin instances, value returns. The behavioral GPU-vs-CPU twin
// is tests/metal/test_metal_functions.das. The flatten half lives here too: the UfxKid splice
// shapes, the StateArr state kinds, and the derived (no name=) companion/entry naming.

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

def private hass(t : T?; needle : string) {
    t |> success(find(statearr_msl, needle) >= 0, "emitted MSL carries `{needle}`")
    if (find(statearr_msl, needle) < 0) {
        to_log(LOG_ERROR, "MISSING `{needle}` in:\n{statearr_msl}\n")
    }
}

[test]
def test_msl_functions(t : T?) {
    t |> run("user functions lower 1:1 - signatures, address spaces, hidden builtins") <| @(t : T?) {
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
        // pointer params: unmarked is device, @threadgroup opts in, the POINTEE's const is the
        // MSL const, and an inline addr(member[i]) argument lowers to plain pointer arithmetic
        has(t, "float ufx_ptr_walk(device const float* p, uint n)")
        has(t, "void ufx_ptr_store(device float* o, uint i, float v)")
        has(t, "float ufx_ptr_tg(threadgroup const float* p, uint n)")
        has(t, "ufx_ptr_tg((part + (0)), gl_WorkGroupSize.x)")
        has(t, "ufx_ptr_store((yp + (0)), i, acc);")
        // the space propagates through a forwarding helper — its own param declares device too
        has(t, "float ufx_ptr_fwd(device const float* p, uint n)")
        has(t, "return ufx_ptr_walk(p, n);")
        // a written-through member stays non-const; the read-only source keeps its const
        has(t, "device float* yp")
        has(t, "device const float * sp = src + (0);")
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
    t |> run("class methods flatten - bodies splice inline, statically devirtualized") <| @(t : T?) {
        // no struct, no self fill: members stay kernel parameters, the write-set flows
        // through the SPLICED method bodies (ys is written only inside store's body)
        t |> success(find(ufxcls_msl, "struct UfxKid") < 0, "no MSL struct emits")
        t |> success(find(ufxcls_msl, "self_") < 0, "no self fill emits")
        hasc(t, "device const float* xs [[buffer(0)]]")
        hasc(t, "device float* ys [[buffer(1)]]")
        hasc(t, "constant float& gain [[buffer(2)]]")
        // value-position splices: combo -> stage(pick(i)) collapses to one inline expression,
        // with the KID's override devirtualized into place
        hasc(t, "tmp[lid] = ((((((xs[i] + zs[i]))) * gain)));")
        t |> success(find(ufxcls_msl, "combo(") < 0, "no member-function call remains")
        t |> success(find(ufxcls_msl, "xs[i];") < 0, "Base's un-overridden pick never emits")
        // plain members = STATE: kernel-entry thread-locals (name-sorted, after the tg pointers)
        hasc(t, "float boost = 0.25f;")
        hasc(t, "device const float * cur = nullptr;")
        // the seed/advance statement splices: the carried-pointer walk survives as plain
        // pointer arithmetic on the entry-scope local — rider state across splices
        hasc(t, "cur = (xs + (i));")
        hasc(t, "cur = (cur + (1));")
        hasc(t, "boost += 0.5f;")
        // peek's value splice reads the carried state
        hasc(t, "(cur[0] * boost)")
        // store's computed argument hoists into a splice-local const bind
        hasc(t, "const float v_s")
        hasc(t, "ys[i] = v_s")
        // a free function called from the kernel body still emits at global scope
        hasc(t, "float ufx_sky(float d)")
    }
}

[test]
def test_msl_state_and_naming(t : T?) {
    t |> run("the other state kinds: a fixed-array thread-local and a mutable-pointee rider") <| @(t : T?) {
        // fixed-array state is a kernel-entry thread-local array, zero-init like a local
        hass(t, "float hist[4] = \{\};")
        // a MUTABLE pointee: taking the writable pointer IS the write declaration, so the root
        // buffer lowers non-const with no deref tracking
        hass(t, "device float * wcur = nullptr;")
        hass(t, "device float* out_ [[buffer(1)]]")
        hass(t, "wcur = (out_ + (i));")
        hass(t, "wcur[0] = (((hist[0] + hist[1]) + hist[2]) + hist[3]);")
        // the array state survives the splice boundary: written in one method, read in another
        hass(t, "hist[k] = xs[(i + uint(k))];")
    }
    t |> run("no name= derives the companion names AND a qualified entry point") <| @(t : T?) {
        t |> success(DerivedName_dkern_msl_entry == "DerivedName_dkern",
            "the entry point is <Class>_<method>, not the bare method name")
        t |> success(find(DerivedName_dkern_msl, "kernel void DerivedName_dkern(") >= 0,
            "the emitted signature carries the derived entry name")
    }
}
