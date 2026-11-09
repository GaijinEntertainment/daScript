options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tune
require daslib/fio
require daslib/module_path
require strings
require math
require llvm/daslib/llvm_tune

def private reference(a, b : array<float>) : float {
    var s = 0f
    for (av, bv in a, b) {
        s += av * bv
    }
    return s
}

// Part 2 (final assembly): one source of truth, reconstituted into the real symbol.
def template sample_dot_template(a, b : array<float>) : float {
    var s = 0f
    for [tune = 1] (i in range(length(a))) {   // nolint:PERF029 - the tuned kernel: perm hints stamp this index loop, zipping would change what is measured
        s += a[i] * b[i]
    }
    return s
}

// [tuned] clones sample_dot_template's body into sample_dot and stamps the
// "vec8_u2" permutation's hints. Callers just call sample_dot — untouched.
[tuned(perm = "vec8_u2")]
def sample_dot(a, b : array<float>) : float {}

// `fallback=` supplies a kernel's own hand hints when they aren't the global vec8_u2; the box
// profile still overrides it (precedence: perm > profile > fallback > vec8_u2). Validation proof:
// an unknown fallback name fails the compile with "tuned: fallback perm ... not in grid", so this
// compiling proves the fallback names a valid grid perm — the one consulted whenever no profile
// entry / perm pin applies (and no profile carries this test-local kernel's key).
[tuned(fallback = "u2", src = "sample_dot_template")]
def sample_dot_u2(a, b : array<float>) : float {}

[test(description = "a [tuned] function pinned to a perm, and one with a fallback perm, both reconstitute the template body and match the reference dot")]
def test_tuned_reconstitutes(t : T?) {
    let a = [for (i in range(64)); float(i)]
    let b = [for (_i in range(64)); 1.0f]
    t |> equal(sample_dot(a, b), reference(a, b), "tuned sample_dot == reference")
    t |> equal(sample_dot_u2(a, b), reference(a, b), "tuned default=u2 sample_dot_u2 == reference")
}

//! the sidecar arm: an out-of-grid sidecar perm stamps the fallback instead of failing the compile - the explicit perm= pin twin is cant_tuned_bad_pin.das
[test(description = "a tune sidecar naming a perm outside the grid stamps the fallback with a stale-mint notice, and the client still compiles and runs", timeout = 1200)]
def test_tuned_stale_sidecar_perm(t : T?) {
    let args <- get_command_line_arguments()
    let bin = args[0]
    let tmpRes = create_temp_file_result("tuned_stale_perm_", ".json")
    if (tmpRes is error) {
        t |> failure("could not create a temp file: {tmpRes as error}")
        return
    }
    let manifest = tmpRes as value
    fwrite(manifest, "\{\"kernels\": \{\"stale_probe_dot\": \"no_such_perm\"\}, \"provenance\": \{\"box\": \"{tune_box_identity()}\"\}\}")
    let client = "{get_this_module_dir()}/_tuned_stale_perm_client.das"
    let env = (get_platform_name() == "windows"
        ? "set \"DAS_TUNE_MANIFEST={manifest}\"&& "
        : "DAS_TUNE_MANIFEST='{manifest}' ")
    let cmd = "{env}\"{bin}\" \"{client}\""
    var outp = ""
    let rc = unsafe(popen_timeout(cmd, 300.0) $(f) {
        if (f != null) {
            outp = fread(f)
        }
    })
    t |> equal(rc, 0, "the stale-sidecar client compiles and runs")
    t |> success(find(outp, "stale mint?") >= 0, "the fallback stamp is announced (got: {outp})")
    t |> success(find(outp, "STALE_CLIENT_OK") >= 0, "the client ran to completion (got: {outp})")
    remove(manifest)
}

def private spawn_capture(cmd : string; var outp : string&) : int {
    outp = ""
    let rc = unsafe(popen_timeout(cmd, 300.0) $(f) {
        if (f != null) {
            outp = fread(f)
        }
    })
    return rc
}

// The module-cache pin is the mechanism's ONLY production wiring (read_manifest ->
// add_module_cache_dependency, ARCHITECTURE.md sec.5): a [tuned] client compiled under
// -module-cache with a tune sidecar must REPARSE when the sidecar's content changes -
// served-from-cache stamps minted against the old sidecar are the silent failure this exists
// to prevent.
[test(description = "a [tuned] client compiled under -module-cache is served from the cache while its tune sidecar is unchanged and reparsed once the sidecar's content changes", timeout = 1200)]
def test_tuned_sidecar_invalidates_module_cache(t : T?) {
    let args <- get_command_line_arguments()
    let bin = args[0]
    let tmpRes = create_temp_file_result("tuned_mc_manifest_", ".json")
    if (tmpRes is error) {
        t |> failure("could not create a temp file: {tmpRes as error}")
        return
    }
    let manifest = tmpRes as value
    let mcRes = create_temp_file_result("tuned_mc_cache_", ".bin")
    if (mcRes is error) {
        t |> failure("could not create a temp cache file: {mcRes as error}")
        return
    }
    let mc = mcRes as value
    remove(mc)   // the cache path must start absent so the cold run writes the stream head
    fwrite(manifest, "\{\"kernels\": \{\}, \"provenance\": \{\"box\": \"{tune_box_identity()}\"\}\}")
    let client = "{get_this_module_dir()}/_tuned_stale_perm_client.das"
    let env = (get_platform_name() == "windows"
        ? "set \"DAS_TUNE_MANIFEST={manifest}\"&& "
        : "DAS_TUNE_MANIFEST='{manifest}' ")
    let cmd = "{env}\"{bin}\" -module-cache \"{mc}\" \"{client}\""
    var run_out = ""
    var rc = spawn_capture(cmd, run_out)
    t |> equal(rc, 0, "cold run compiles and runs")
    t |> success(find(run_out, "ser: wrote") >= 0, "cold run wrote the module cache (got: {run_out})")
    rc = spawn_capture(cmd, run_out)
    t |> equal(rc, 0, "warm run compiles and runs")
    t |> success(find(run_out, "deser: clean") >= 0, "unchanged sidecar serves from cache (got: {run_out})")
    fwrite(manifest, "\{\"kernels\": \{\"stale_probe_dot\": \"u2\"\}, \"provenance\": \{\"box\": \"{tune_box_identity()}\"\}\}")
    rc = spawn_capture(cmd, run_out)
    t |> equal(rc, 0, "post-mint run compiles and runs")
    t |> success(find(run_out, "ser: macro dependency changed") >= 0, "a re-minted sidecar invalidates the cached [tuned] stamps (got: {run_out})")
    remove(manifest)
    remove(mc)
}

// [dasllama_grid] emits, from a single `def template`, one clone per grid permutation plus a
// <name>_variants() : array<tuple<string; <sig>>> fn-ptr registry — the boilerplate the
// profiler harness used to hand-write (per-perm [tuned] stubs + a variants() array).
def template grid_dot_template(a, b : array<float>) : float {
    var s = 0f
    for [tune = 1] (i in range(length(a))) {
        s += a[i] * b[i]
    }
    return s
}

[dasllama_grid(src = "grid_dot_template")]
struct GridDot {}

[test(description = "[dasllama_grid] emits all seven grid permutations of a template into its variants registry, each callable and within tolerance of the reference dot")]
def test_grid_registry(t : T?) {
    let a = [for (i in range(64)); float(i)]
    let b = [for (_i in range(64)); 1.0f]
    let want = reference(a, b)

    let vs <- grid_dot_template_variants()
    t |> equal(length(vs), 7, "grid emits all 7 permutations (plain, u2, vec8, vec8_u2, vec16, vec16_u2, vec4_u4)")

    // Every emitted variant is callable and numerically correct (vectorize reassociates FP,
    // so compare within tolerance rather than bit-exact).
    var seen_default = false
    for (v in vs) {
        let f = v._1
        t |> success(abs(f(a, b) - want) < 1.0e-2f, "variant {v._0} == reference")
        if (v._0 == "vec8_u2") { seen_default = true }
    }
    t |> success(seen_default, "grid includes the default vec8_u2 permutation")
}

// A VOID-returning template must also produce a working registry: function_to_type carries the
// declared `: void` through, so the emitted variants() tuple type is a concrete function<...:void>.
// Regression — an undeclared/auto return produced a non-concrete function<...:auto> registry type
// that failed to compile (the in-place kernels axpy/add/mul/scale/copy hit exactly this).
def template grid_scale_template(var d : float ?; k : float; n : int64) : void {
    for [tune = 1] (i in range64(n)) {
        unsafe {
            d[i] *= k
        }
    }
}

[dasllama_grid(src = "grid_scale_template")]
struct GridScale {}

[test(description = "[dasllama_grid] over a void template emits a concrete seven-row registry whose variants run in place")]
def test_grid_void_registry(t : T?) {
    let vs <- grid_scale_template_variants()
    t |> equal(length(vs), 7, "void-template grid emits all 7 permutations")

    var d : array<float>
    d |> resize(8)
    for (i in range(8)) {
        d[i] = float(i)
    }
    var dp = unsafe(addr(d[0]))
    let f = vs[0]._1               // "plain" perm — scales d in place by k
    invoke(f, dp, 2.0f, 8l)
    t |> equal(d[3], 6.0f, "void variant {vs[0]._0} scaled d[3] (3*2) in place")
}
