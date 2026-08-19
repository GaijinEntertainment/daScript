options gen2
options _dasllama_internal = true

require daslib/module_path
require daslib/command_line
require daslib/fio
require strings   // find — the NOISE GATE refusal detector in run_half
require daslib/jobque_boost   // get_total_hw_cores: the measurement-regime lane count
require daslib/json
require daslib/json_boost
require llvm/daslib/llvm_tune   // tune_history_archive + tune_median — the mint diff/archive
require tuner_cli               // --tune-paranoid, shared with both halves
require math
require dasllama/dasllama_env

// The dasllama scope tuner — the wrapper the policy rail (auto default / --tune / daspkg
// release) spawns. Runs BOTH halves as child daslang processes, each upserting the same
// env-pointed sidecar (DAS_TUNE_MODE/DAS_TUNE_MANIFEST inherit):
//   1. gen_tune_probe   — the [tune] generator-grid winners (q8q8 tile family)
//   2. tune_kernels     — the [tuned] loop-hint winners + the "runtime" knob snapshot
// A child failure is reported but does not abort the other half — the completeness check
// upstream names whatever ends up missing.

// Two modes only: default and `-- --tune-paranoid` (3x budget, 1% gate) — `daspkg release
// --paranoid` forwards the flag here. There is no fast RACE mode; the debugging concession is
// `daspkg release --quick`, which accepts the existing sidecar and never spawns this at all.
// Both halves take real measurements (the confirm gate is a full prefill), so they must run
// under the box measurement regime — 16 cores with affinity on the big-SMT x64 boxes, not the
// fresh-que default (all cores): a crown confirmed at the wrong width ships the wrong kernel.
// Explicit DAS_JOBQUE_* envs on the tuner itself still win — the children inherit them.
def private regime_env_prefix() : string {
    if (get_platform_name() != "windows") {
        return ""
    }
    var pfx = ""
    if (is_none(g_env_core.jobque_threads)) {
        let lanes = min(get_total_hw_cores(), 16)
        pfx = "set DAS_JOBQUE_THREADS={lanes}&& "
    }
    if (is_none(g_env_core.jobque_affinity)) {
        pfx = "{pfx}set DAS_JOBQUE_AFFINITY=2&& "   // hard mask — mode 1's hint re-rolls SMT placement per que
    }
    return pfx
}

// the newest race-table time recorded for (kernel, suffix), -1 when absent — the loop-hint
// half records med_us, the generator families tile_us/streamed_us
def private race_time(var race : JsonValue?; kernel, suffix : string) : double {
    var row = race?[kernel]?["rows"]?[suffix]
    var t = row?["med_us"] ?? -1.0lf
    if (t <= 0.0lf) {
        t = row?["tile_us"] ?? -1.0lf
    }
    if (t <= 0.0lf) {
        t = row?["streamed_us"] ?? -1.0lf
    }
    return t
}

// the mint diff: winners that changed (with margins from the NEW race tables) plus the median
// time shift over rows both mints raced — "uniform shift = box state; scattered flips past
// the floor = one of the mints was noisy; same-direction twin flips = estimator change"
def private print_mint_diff(oldText, newText : string) {
    var err = ""
    var oldDoc = read_json(oldText, err)
    var newDoc = read_json(newText, err)
    if (oldDoc == null || newDoc == null) {
        print("dasllama_tuner: diff skipped (previous sidecar unreadable: {err})\n")
        delete_json(oldDoc)
        delete_json(newDoc)
        return
    }
    var newK = newDoc?["kernels"]
    var flips = 0
    var fresh = 0
    if (newK != null && newK.value is _object) {
        for (k, v in keys(unsafe(newK.value as _object)), values(unsafe(newK.value as _object))) {
            let newS = v ?? ""
            let oldS = oldDoc?["kernels"]?[k] ?? ""
            if (empty(oldS)) {
                // a stale/absent previous doc contributes no baseline — count, don't compare
                fresh++
                continue
            }
            if (empty(newS) || oldS == newS) {
                continue
            }
            flips++
            let tOld = race_time(newDoc?["race"], k, oldS)
            let tNew = race_time(newDoc?["race"], k, newS)
            let margin = ((tOld > 0.0lf && tNew > 0.0lf)
                ? "  (old suffix runs {(tOld - tNew) / tNew * 100.0lf:0.2f}% vs new, per the new race table)" : "")
            print("dasllama_tuner: DIFF {k}: {oldS} -> {newS}{margin}\n")
        }
    }
    // median time shift across every (kernel, suffix) row BOTH mints raced
    var ratios : array<double>
    var newR = newDoc?["race"]
    if (newR != null && newR.value is _object) {
        for (k, kv in keys(unsafe(newR.value as _object)), values(unsafe(newR.value as _object))) {
            var rows = kv?["rows"]
            if (rows == null || !(rows.value is _object)) {
                continue
            }
            for (sfx in keys(unsafe(rows.value as _object))) {
                let tNew = race_time(newR, k, sfx)
                let tOld = race_time(oldDoc?["race"], k, sfx)
                if (tNew > 0.0lf && tOld > 0.0lf) {
                    ratios |> push(tNew / tOld)
                }
            }
        }
    }
    if (!empty(ratios)) {
        let shift = (tune_median(ratios) - 1.0lf) * 100.0lf
        print("dasllama_tuner: DIFF times moved {shift:0.2f}% median over {length(ratios)} shared rows\n")
    } elif (flips > 0) {
        print("dasllama_tuner: DIFF (no race tables in the previous sidecar - margins start next mint)\n")
    }
    let freshTxt = fresh > 0 ? ", {fresh} entry(ies) with no previous baseline" : ""
    print("dasllama_tuner: DIFF {flips} winner change(s){freshTxt}; uniform shift = box state, scattered past-floor flips = a noisy mint\n")
    delete ratios
    delete_json(oldDoc)
    delete_json(newDoc)
}

def private run_half(name : string; paranoid : bool) : bool {
    let t0 = ref_time_ticks()
    let pfx = regime_env_prefix()
    let user_args = paranoid ? " -- --tune-paranoid" : ""
    let cmd = "{pfx}\"{get_das_exe()}\" -jit \"{path_join(get_this_module_dir(), name)}\" -dasroot \"{get_das_root()}\"{user_args} 2>&1"
    // cmd.exe strips the first and last quote of a line that starts with a quote — wrap in a
    // sacrificial outer pair on windows UNLESS the env prefix already leads (the daspkg popen trap)
    let full = get_platform_name() == "windows" && empty(pfx) ? "\"{cmd}\"" : cmd
    print("dasllama_tuner: {cmd}\n")
    var rc : int
    var noiseRefused = false
    unsafe {
        rc = popen_timeout(full, 3600.0) $(f) {
            if (f == null) {
                return
            }
            while (!feof(f)) {
                let ln = fgets(f)
                if (find(ln, "NOISE GATE") >= 0) {
                    noiseRefused = true
                }
                print(ln)
            }
        }
    }
    let elapsed_ms = get_time_usec(t0) / 1000
    print("dasllama_tuner: {name} finished in {elapsed_ms} ms (rc={rc})\n")
    if (rc != 0) {
        // the refusal detail scrolled by minutes ago mid-stream — the closing line carries the
        // WHY and the escape hatches so a nonzero exit is self-diagnosing where you look.
        // Under NOISE_OVERRIDE the gate line prints but the mint proceeds — a later nonzero
        // exit is then some OTHER fault, and blaming noise would hide it
        if (noiseRefused && !tune_noise_override()) {
            print("dasllama_tuner: {name} REFUSED for noise (rc={rc}) - quiet the box and re-run; escapes: DAS_TUNE_NOISE_CV recalibrates the gate, DAS_TUNE_NOISE_OVERRIDE=1 mints anyway (stamped), DASLLAMA_ALLOW_UNTUNED=1 skips minting for dev runs\n")
        } else {
            print("dasllama_tuner: {name} FAILED (rc={rc})\n")
        }
    }
    return rc == 0
}

[export]
def main : int {
    let t0 = ref_time_ticks()
    let paranoid = tuner_paranoid_requested()
    print("dasllama_tuner: sidecar = {g_env_core.tune_manifest}\n")
    let modeDesc = (paranoid
        ? "PARANOID (kernels 20 screening / 240 max, generators best-of-18, 1% gate, validation)"
        : "default (kernels 20 screening / 80 max, generators best-of-6, validation)")
    print("dasllama_tuner: mode = {modeDesc}\n")
    // active overrides announce themselves up front: a mint an escape shaped must say so
    if (tune_noise_threshold_overridden()) {
        print("dasllama_tuner: OVERRIDE DAS_TUNE_NOISE_CV - noise gate recalibrated to {tune_noise_threshold_pct(paranoid):0.1f}%\n")
    }
    if (tune_noise_override()) {
        print("dasllama_tuner: OVERRIDE DAS_TUNE_NOISE_OVERRIDE - a failing noise gate mints anyway (sidecar stamped 'noise: overridden')\n")
    }
    // pre-mint snapshot: COPY, not rename — the halves UPSERT, and a rename would drop other
    // scopes' keys from the shared file. The post-mint diff reads it back.
    let sidecar = g_env_core.tune_manifest
    var oldText = ""
    if (!empty(sidecar)) {
        oldText = fread(sidecar)
        if (!empty(oldText) && fwrite("{sidecar}.bak", oldText)) {
            print("dasllama_tuner: previous sidecar snapshotted -> {sidecar}.bak\n")
        }
    }
    let ok1 = run_half("gen_tune_probe.das", paranoid)
    let ok2 = run_half("tune_kernels.das", paranoid)
    if (!empty(sidecar)) {
        if (ok1 && ok2) {
            if (!empty(oldText)) {
                print_mint_diff(oldText, fread(sidecar))
            }
            let arch = tune_history_archive(sidecar, "ok", "")
            if (!empty(arch)) {
                print("dasllama_tuner: archived -> {arch}\n")
            }
        } else {
            // the failed mint wrote nothing (deferred writes), but the FAILURE itself is a
            // data point the box's history must keep
            tune_history_archive(sidecar, "FAILED", "gen ok={ok1} kernels ok={ok2}")
        }
    }
    print("dasllama_tuner: total {get_time_usec(t0) / 1000} ms\n")
    return ok1 && ok2 ? 0 : 1
}
