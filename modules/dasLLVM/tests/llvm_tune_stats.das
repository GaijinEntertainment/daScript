options gen2

require dastest/testing_boost
require llvm/daslib/llvm_tune
require daslib/fio
require daslib/json
require daslib/json_boost
require strings   // find — the history-archive content asserts
require math

// The measurement-statistics + noise-gate surface: median/cv math, the env-driven threshold,
// and the provenance-note round-trip through a sidecar write. All model-free and in-process
// (the env knobs are set BEFORE their first read — the read-once caches latch what we set).

[test]
def test_tune_median(t : T?) {
    var odd <- [3.0lf, 1.0lf, 2.0lf]
    t |> equal(tune_median(odd), 2.0lf)
    var even <- [4.0lf, 1.0lf, 3.0lf, 2.0lf]
    t |> equal(tune_median(even), 2.5lf)
    var one <- [7.0lf]
    t |> equal(tune_median(one), 7.0lf)
    var none : array<double>
    t |> equal(tune_median(none), 0.0lf)
    // the input survives (median sorts a copy)
    t |> equal(odd[0], 3.0lf)
    delete odd
    delete even
    delete one
}

[test]
def test_tune_cv_pct(t : T?) {
    var flat <- [5.0lf, 5.0lf, 5.0lf, 5.0lf]
    t |> equal(tune_cv_pct(flat), 0.0lf)
    // mean 10, sample variance 6/8 -> cv = 100 * sqrt(0.75) / 10
    var spread <- [9.0lf, 10.0lf, 11.0lf, 9.0lf, 10.0lf, 11.0lf, 9.0lf, 10.0lf, 11.0lf]
    t |> success(abs(tune_cv_pct(spread) - 100.0lf * sqrt(0.75lf) / 10.0lf) < 1.0e-9lf)
    var one <- [3.0lf]
    t |> equal(tune_cv_pct(one), 0.0lf)
    delete flat
    delete spread
    delete one
}

[test]
def test_tune_noise_env(t : T?) {
    // set BEFORE the first threshold read — the cache latches this value for both flavors
    set_env_variable("DAS_TUNE_NOISE_CV", "3.5")
    t |> equal(tune_noise_threshold_pct(false), 3.5lf)
    t |> equal(tune_noise_threshold_pct(true), 3.5lf)
    set_env_variable("DAS_TUNE_NOISE_OVERRIDE", "1")
    t |> success(tune_noise_override())
}

[test]
def test_tune_sidecar_section_merge(t : T?) {
    let tmpRes = create_temp_file_result("llvm_tune_race_", ".tune.json")
    if (tmpRes is error) {
        t |> failure("could not create a temp file: {tmpRes as error}")
        return
    }
    let sidecar = tmpRes as value
    set_env_variable("DAS_TUNE_MANIFEST", sidecar)
    t |> success(tune_manifest_set("some_fn", "vec8_u2"))
    var partA <- { "dot" => JV((med_us = 100.0lf, best_us = 98.0lf)) }
    t |> success(tune_sidecar_section_merge("race", partA))
    var partB <- { "gemm" => JV((tile_us = 5.0lf)) }
    t |> success(tune_sidecar_section_merge("race", partB))
    var jerr = ""
    var doc = read_json(fread(sidecar), jerr)
    t |> success(doc != null, "sidecar parses: {jerr}")
    // both merges' keys survive, and the kernels section is untouched. Regression guard: the
    // box cache is keyed path@mtime (1s granular) — a pre-created empty temp file once poisoned
    // it as box="", making every same-second re-read read STALE and reset the whole doc
    t |> equal(doc?["race"]?["dot"]?["med_us"] ?? 0.0lf, 100.0lf)
    t |> equal(doc?["race"]?["gemm"]?["tile_us"] ?? 0.0lf, 5.0lf)
    t |> equal(doc?["kernels"]?["some_fn"] ?? "", "vec8_u2")
    delete_json(doc)
    remove(sidecar)
}

[test]
def test_tune_history_archive(t : T?) {
    let dirRes = create_temp_directory_result("llvm_tune_history_")
    if (dirRes is error) {
        t |> failure("could not create a temp dir: {dirRes as error}")
        return
    }
    let hdir = dirRes as value
    set_env_variable("DAS_TUNE_HISTORY", hdir)   // before the first read — the location latches
    t |> equal(tune_history_dir(), hdir)
    let tmpRes = create_temp_file_result("llvm_tune_hist_", ".tune.json")
    if (tmpRes is error) {
        t |> failure("could not create a temp file: {tmpRes as error}")
        return
    }
    let sidecar = tmpRes as value
    set_env_variable("DAS_TUNE_MANIFEST", sidecar)
    t |> success(tune_manifest_set("hist_fn", "vec8_u2"))
    let okPath = tune_history_archive(sidecar, "ok", "")
    t |> success(!empty(okPath), "ok mint archives")
    t |> success(find(fread(okPath), "hist_fn") >= 0, "archive is the sidecar itself")
    let failPath = tune_history_archive(sidecar, "REFUSED (noise)", "cv 8.1% > 2%")
    t |> success(!empty(failPath) && find(failPath, ".FAILED.json") >= 0, "failure archives marked")
    t |> success(find(fread(failPath), "REFUSED") >= 0, "stub carries the verdict")
    remove(okPath)
    remove(failPath)
    remove(sidecar)
}

[test]
def test_tune_provenance_note_roundtrip(t : T?) {
    let tmpRes = create_temp_file_result("llvm_tune_stats_", ".tune.json")
    if (tmpRes is error) {
        t |> failure("could not create a temp file: {tmpRes as error}")
        return
    }
    let sidecar = tmpRes as value
    set_env_variable("DAS_TUNE_MANIFEST", sidecar)
    tune_provenance_note("noise", "overridden")
    tune_provenance_note("box", "spoofed-box")   // identity keys must win over notes
    t |> success(tune_manifest_set("stats_probe_fn", "vec8_u2"))
    var jerr = ""
    var doc = read_json(fread(sidecar), jerr)
    t |> success(doc != null, "sidecar parses: {jerr}")
    t |> equal(doc?["kernels"]?["stats_probe_fn"] ?? "", "vec8_u2")
    t |> equal(doc?["provenance"]?["noise"] ?? "", "overridden")
    let box = doc?["provenance"]?["box"] ?? ""
    t |> success(box != "spoofed-box", "a note must not overwrite the box identity")
    t |> success(!empty(box))
    delete_json(doc)
    remove(sidecar)
}
