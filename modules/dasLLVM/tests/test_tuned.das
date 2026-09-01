options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tune
require daslib/fio
require daslib/module_path
require strings
require llvm/daslib/llvm_tune   // tune_box_identity - the stale-sidecar cell's manifest must pass the box gate

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
// an unknown fallback name fails the compile with "tuned: unknown perm" (negative-probed), so this
// compiling proves the fallback names a valid grid perm — the one consulted whenever no profile
// entry / perm pin applies (and no profile carries this test-local kernel's key).
[tuned(fallback = "u2", src = "sample_dot_template")]
def sample_dot_u2(a, b : array<float>) : float {}

def private reference(a, b : array<float>) : float {
    var s = 0f
    for (av, bv in a, b) {
        s += av * bv
    }
    return s
}

[test]
def test_tuned_reconstitutes(t : T?) {
    let a = [for (i in range(64)); float(i)]
    let b = [for (_i in range(64)); 1.0f]
    t |> equal(sample_dot(a, b), reference(a, b), "tuned sample_dot == reference")
    t |> equal(sample_dot_u2(a, b), reference(a, b), "tuned default=u2 sample_dot_u2 == reference")
}

// The sidecar is measured data, not law: a manifest entry naming a perm the grid no longer
// offers (a grid rework since the mint) must not brick the compile - the macro stamps the
// fallback and says so, so the tuner that would re-race the entry can always run. The hard
// refusal survives only for an explicit perm= pin (cant_tuned_bad_pin.das).
[test]
def test_tuned_stale_sidecar_perm(t : T?) {
    let args <- get_command_line_arguments()  // locked view - do not delete
    let bin = args[0]
    let tmpRes = create_temp_file_result("tuned_stale_perm_", ".json")
    if (tmpRes is error) {
        t |> failure("could not create a temp file: {tmpRes as error}")
        return
    }
    let manifest = tmpRes as value
    fwrite(manifest, "\{\"kernels\": \{\"stale_probe_dot\": \"no_such_perm\"\}, \"provenance\": \{\"box\": \"{tune_box_identity()}\"\}\}")
    let client = "{get_this_module_dir()}/tuned_stale_perm_client.das"
    let env = (get_platform_name() == "windows"
        ? "set DAS_TUNE_MANIFEST={manifest}&& "
        : "DAS_TUNE_MANIFEST={manifest} ")
    let cmd = "{env}\"{bin}\" \"{client}\""
    var outp = ""
    var rc : int
    unsafe {
        rc = popen_timeout(cmd, 300.0) $(f) {
            if (f != null) {
                outp = fread(f)
            }
        }
    }
    t |> equal(rc, 0, "the stale-sidecar client compiles and runs")
    t |> success(find(outp, "stale mint?") >= 0, "the fallback stamp is announced (got: {outp})")
    t |> success(find(outp, "STALE_CLIENT_OK") >= 0, "the client ran to completion (got: {outp})")
    remove(manifest)
}
