options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _cyclomatic_complexity = 0   // orchestrator passes; reshaped by the single-rig consolidation
options _function_length = 0   // same
options _dasllama_internal = true

require profile_common
require dasllama/dasllama_env   // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require dasllama/dasllama_image   // dlim_wipe + dlim_inventory — the batch lifecycle and the re-mint tripwire
require llvm/daslib/llvm_tune   // [tune_policy]: the orchestrator never measures — it must never self-tune
require daslib/clargs
require daslib/fio
require math

// The official record driver — ONE rig for every workload: loops the catalogs x pair legs, runs
// the das side as a CHILD per cell (the ONE released lcpp_bench exe, --asr for audio; one model
// process at a time), and upserts the --json-path records into the per-box store — written after
// EVERY cell, so a crash never costs a measured cell. Each board ratio is one das cell + its
// matched ref run ADJACENT:
//   metal : das metal            vs stock -ngl 99          ("gpu")
//   neon  : das cpu              vs clean-cpu -ngl 0       ("cpu", kernel vs kernel)
//   amx   : das cpu --accel      vs stock -ngl 0 -nopo 1   ("cpu + accel" — -nopo keeps lcpp's
//           Metal op-offload from serving 'CPU' rows on the GPU; nopo == nometal-build)
//   asr   : das cpu (lcpp_bench --asr) vs each catalog reference tool, per clip
//   image : das cpu/accel/metal (lcpp_bench --image)  vs patched llama-mtmd-cli ("image-chat":
//           metal/amx vs the stock build, neon vs the CPU ref build — llama-bench itself has no
//           image mode; the picture is the pinned fixture)
//   bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- [-o substr] [--workload llm|asr|image|all] [--image-reps n]
// --oracle inverts the rig into a regression tripwire: the stored das rows re-measure once
// each (text cells frozen; ASR/image cells bake what they need; no refs, store never written)
// and gate against their stored means.
// Ref binaries come from setup_lcpp_ref: --ref-clean/--ref-stock or LLAMA_BENCH_CLEAN/LLAMA_BENCH_STOCK.
// ASR references resolve via WHISPER_CPP / MTMD_BIN / ONNX_PY / NEMO_PY (profile_common).

[CommandLineArgs]
struct Args {
    @clarg_short = "o"
    @clarg_doc = "Only catalog entries whose filename contains this substring"
    only : string

    @clarg_doc = "Backends to measure: cpu | metal | all (default: all — metal on Apple boxes only)"
    backend : string = "all"

    @clarg_doc = "Comma list of pair legs to run: neon | amx | metal (default: every leg --backend implies; cpu = neon,amx)"
    legs : string

    @clarg_doc = "Seconds to idle between passes (default: 12) — a dead child's 10-20GB map reclaims ASYNC after exit, and short cells bench right into that churn (12B pp ±33 at 0s settle, ±3 at 12s)"
    settle : int = 12

    @clarg_name = "das-settle"
    @clarg_doc = "Seconds to idle before each das cell (default: 180) — das tuned kernels run near the package power ceiling, and a heat-soaked box under-reads them with a CLEAN cv (gpt-oss tuned pp512 read −13.6% mid-board, 180s of idle restored it; llama.cpp refs were insensitive all along). The cv retry cannot catch a stable-low cell; only cool-slot entry can"
    das_settle : int = 180

    @clarg_name = "ref-clean"
    @clarg_doc = "clean-cpu llama-bench binary (default: env LLAMA_BENCH_CLEAN)"
    ref_clean : string

    @clarg_name = "ref-stock"
    @clarg_doc = "stock llama-bench binary (default: env LLAMA_BENCH_STOCK); its sibling llama-mtmd-cli serves as the metal/amx image-chat reference"
    ref_stock : string

    @clarg_name = "ref-no-affinity"
    @clarg_doc = "Drop --cpu-mask/--cpu-strict from reference invocations (zen2 3990X: driver-spawned pinned refs read ~18% low; solo runs show pins are worth nothing there anyway)"
    ref_no_affinity : bool

    @clarg_short = "t"
    @clarg_doc = "Worker threads (default: the box profile)"
    threads : int = 0

    @clarg_short = "p"
    @clarg_doc = "Prompt tokens (default: 512)"
    plen : int = 512

    @clarg_short = "n"
    @clarg_doc = "Generated tokens (default: 128)"
    ngen : int = 128

    @clarg_short = "r"
    @clarg_doc = "Timed repetitions per row (default: 5)"
    reps : int = 5

    @clarg_name = "asr-tier"
    @clarg_doc = "ASR model tier: small | large | all (default all). Declared per catalog entry, not derived from bytes — ASR cost is clip-duration x model-speed, and large-v3-turbo is SMALLER on disk than Parakeet yet 2.5x costlier"
    asr_tier : string = "all"

    @clarg_short = "w"
    @clarg_doc = "Workloads to measure: llm | asr | image | all (default: all — absent models/tools skip with a warning; image = only the image-chat cells of the llm sweep)"
    workload : string = "all"

    @clarg_name = "asr-reps"
    @clarg_doc = "Timed repetitions per ASR clip, best kept (default: 2 — clips are minutes, not seconds)"
    asr_reps : int = 2

    @clarg_name = "image-reps"
    @clarg_doc = "Timed repetitions per image-chat cell, best kept (default: 3 — a cell is one full image turn: encode, spliced prefill, decode)"
    image_reps : int = 3

    @clarg_doc = "Verify instead of measure: re-run each stored das row (text, ASR, image-chat) once and gate it against the store. The batch starts with an image wipe; a prepare pass bakes and warms each text cell's image so only the timed cell is frozen, and a model's images are deleted after its last cell. No refs, no store writes. Stops at the first FAIL"
    oracle : bool

    @clarg_doc = "Oracle: run every row and report the full board instead of stopping at the first FAIL"
    oracle_keep_going : bool

    @clarg_doc = "Oracle: WARN when a cell lands this far below its stored mean, percent (default: 3)"
    oracle_warn : float = 3.0

    @clarg_doc = "Oracle: FAIL when a cell lands this far below its stored mean, percent (default: 5)"
    oracle_fail : float = 5.0

    @clarg_doc = "Oracle: seconds to idle before the automatic solo re-run of a FAILED cell, whose verdict then stands (default: 180; 0 disables) — board-tail cells under-read on a hot chip (−6.5%..−34% observed; thermal recovery takes ~3 min), a solo re-run after settle reads true"
    oracle_retry_settle : int = 180

    @clarg_name = "oracle-allow-crossgen"
    @clarg_doc = "Oracle: compare across sidecar generations anyway (default: a tune_sha mismatch is INCOMPARABLE — the stored mean belongs to different winners — and fails the run)"
    oracle_allow_crossgen : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// The measurement child is the RELEASED exe, never a script: its tune winners are baked in at
// build time, so no cell can tune, re-exec, or drift onto different winners mid-sweep. Absent or
// older than the bench source is a hard stop with the build line — never a silent rebuild here,
// which would put minutes of hidden work inside a measurement run.
let RIG_EXE_OUT = "modules/dasLLAMA/performance/_rig"
let RIG_BENCH_SRC = "modules/dasLLAMA/benchmarks/lcpp_bench.das"
let RIG_ENGINE_SRC = "modules/dasLLAMA/dasllama"

def private rig_exe_path() : string {
    // macOS ships a clickable .app; every other platform gets <out>/<name>/<name>, .exe on Windows
    if (is_apple_box()) {
        return "{RIG_EXE_OUT}/dasllama-bench.app/Contents/MacOS/dasllama-bench"
    }
    // daslang -exe appends `.exe` on EVERY platform (daspkg bundle_exe_path) — and this path is
    // STAT'd, so it must be spelled out (zen4 bringup: the bare-Linux name never existed)
    return "{RIG_EXE_OUT}/dasllama-bench/dasllama-bench.exe"
}

def private rig_build_line() : string {
    return "bin/daslang utils/daspkg/main.das -- release --root modules/dasLLAMA/benchmarks --out {RIG_EXE_OUT}"
}

// The exe bakes the whole ENGINE, not just the bench driver, so the bench source alone is not the
// staleness axis: a refactor under dasllama/ with an untouched driver leaves an exe that measures
// the OLD engine and reports no regression. Newest source across both wins.
def private rig_newest_source() : tuple<string; clock> {
    var bestPath = RIG_BENCH_SRC
    var best = stat(RIG_BENCH_SRC).mtime
    dir(RIG_ENGINE_SRC) $(name : string) {
        return if (!(name |> ends_with(".das")))
        let full = path_join(RIG_ENGINE_SRC, name)
        let s = stat(full)
        if (s.is_valid && s.mtime > best) {
            best = s.mtime
            bestPath = full
        }
    }
    return (bestPath, best)
}

//! The exe must exist and be no older than every source it bakes. Returns "" (and logs the fix)
//! when it is not usable — the caller aborts rather than measuring something stale.
def private rig_exe_checked() : string {
    let exe = rig_exe_path()
    let st = stat(exe)
    if (!st.is_valid) {
        to_log(LOG_ERROR, "gen_bench_records: no bench exe at {exe}\n  build it: {rig_build_line()}\n")
        return ""
    }
    let newest = rig_newest_source()
    if (newest._1 > st.mtime) {
        to_log(LOG_ERROR, "gen_bench_records: bench exe is older than {newest._0} - it bakes that source AND the tune winners, so it would measure stale code\n  rebuild: {rig_build_line()}\n")
        return ""
    }
    return exe
}

// Re-mint tripwire. Exactly one process may write a given .dlim during a sweep: a second write of
// the SAME image means two cells disagreed about its identity, which silently re-measures one of
// them against different bytes. Counting mtimes per image catches it; the first repeat is fatal.
var private g_mint_seen : table<string; clock>
var private g_mint_count : table<string; int>

def private mint_snapshot(dir : string) {
    for (im in dlim_inventory_(dir)) {
        let st = stat(im.path)
        if (st.is_valid) {
            g_mint_seen[im.path] = st.mtime
        }
    }
}

def private mint_check(dir, whose : string) {
    for (im in dlim_inventory_(dir)) {
        let st = stat(im.path)
        continue if (!st.is_valid)
        // unseen path or a moved mtime = this cell wrote it
        if (key_exists(g_mint_seen, im.path)) {
            let prev = g_mint_seen[im.path]
            continue if (prev == st.mtime)
        }
        g_mint_seen[im.path] = st.mtime
        let n = (g_mint_count?[im.path] ?? 0) + 1
        g_mint_count[im.path] = n
        if (n > 1) {
            panic("gen_bench_records: {im.path} was minted {n} times in one sweep (last by {whose}) - two cells disagree on its identity, so their numbers are not comparable")
        }
        to_log(LOG_INFO, "gen_bench_records: minted {im.file} ({im.bytes >> 20l} MB) during {whose}\n")
    }
}

// STEP ZERO of a validated batch: the rigs own the model dirs for the whole run — start by
// deleting every image. Each model re-bakes when its first cell needs it (the cv warm-retry
// absorbs the cold map; --oracle's prepare pass bakes before the timed cell) and its images die
// after its last cell, so the disk high-water mark is ONE model's lanes, never the catalog's —
// a live two-lane working set once filled a 460 GB box to 97% and failed a bake mid-sweep.
// A fresh box pays nothing extra: it starts with no images either way.
def private batch_start_wipe(workload : string) {
    // scoped to what THIS batch will re-bake: an ASR-only run must not wipe LLM images it
    // never touches again, so it wipes per ASR carrier instead of the whole models dir
    // (image cells bake decoder + mmproj images in the llm dir, so they take the llm scope)
    if (workload == "llm" || workload == "all" || workload == "image") {
        let w_llm = dlim_wipe(models_dir())
        if (w_llm.removed > 0) {
            to_log(LOG_INFO, "gen_bench_records: lifecycle wipe - {w_llm.removed} image(s), {w_llm.freed >> 20l} MB in {models_dir()}\n")
        }
    } else {
        for (spec in asr_catalog()) {
            let w = dlim_wipe(asr_model_path(spec))
            if (w.removed > 0) {
                to_log(LOG_INFO, "gen_bench_records: lifecycle wipe - {spec.display}: {w.removed} image(s), {w.freed >> 20l} MB\n")
            }
        }
    }
    if (workload == "asr" || workload == "all") {
        // a one-dir box points WHISPER_CPP_MODELS at the llm dir — skip the whole-dir form
        // there on an ASR-only run (the per-carrier wipe above already covered it)
        let wdir = whisper_models_dir()
        if (workload == "all" || wdir != models_dir()) {
            let w_w = dlim_wipe(wdir)
            if (w_w.removed > 0) {
                to_log(LOG_INFO, "gen_bench_records: lifecycle wipe - {w_w.removed} image(s), {w_w.freed >> 20l} MB in {wdir}\n")
            }
        }
    }
}

def private daslang_bin() : string {
    return g_env_prof.daslang_bin
}

def private elapsed_ms(t0 : int64) : int {
    return get_time_usec(t0) / 1000
}

// Stamp source + the catalog note and upsert every run of `cell` into the store, then persist.
// the records directory carries each sidecar generation ONCE, by shasum: a row's tune_sha
// resolves to the FULL doc (winners + race tables + provenance) sitting beside the store
def private snapshot_tune_generation(exe : string) {
    let sc = sidecar_for(exe)
    let sha = sidecar_generation_sha(sc)
    return if (empty(sha))
    let dst = "modules/dasLLAMA/performance/records/{box_name()}.tune.{slice(sha, 0, 12)}.json"
    return if (stat(dst).is_valid)
    if (fwrite(dst, fread(sc))) {
        to_log(LOG_INFO, "gen_bench_records: sidecar generation archived -> {dst}\n")
    }
}

def private absorb_cell(var models : array<BenchModel>; var cell : array<BenchModel>; store, note : string;
                        var failed : array<string>&; parity : string = "") {
    let hw = gather_hardware()
    for (c in cell) {
        var debug_stamped = false
        var unstamped = false
        for (r in c.runs) {
            r.source = "official"
            // the driver's probe is the hardware truth — a child exe built before a probe fix
            // stamps stale/empty identity (zen4 bringup: every das cell read "unknown")
            r.hardware = hw
            if (!empty(parity) && r.engine == "das") {
                r.parity = parity   // the model's pregate verdict — cells only run after it passed
            }
            debug_stamped ||= r.flavor |> starts_with("debug-jit")
            unstamped ||= das_run_unstamped(r)
        }
        if (debug_stamped) {
            // belt + braces: the rig only spawns the released exe, but a debug-stamped row
            // must never survive into the store no matter how it arrived
            to_log(LOG_ERROR, "gen_bench_records: DROPPING debug-jit cell for {c.gguf} - debug runs are not records\n")
            continue
        }
        if (unstamped && !g_env_bench.allow_untuned) {
            // the store-time arm of the tune gate: the file-level gate passes a VALID-but-stale
            // shipped sidecar (the M4 mis-ship), the runtime then drops to fallback, and the
            // row arrives here with an empty stamp — unnameable winners are not records
            to_log(LOG_ERROR, "gen_bench_records: DROPPING {c.gguf} - a das run carries no tune stamp, the winners it measured under cannot be named (fix the release/manifest; DASLLAMA_ALLOW_UNTUNED=1 stores a dev row)\n")
            failed |> push("{c.gguf} unstamped")
            continue
        }
        upsert_bench_model(models, c)
        stamp_note(models, c.gguf, note)
    }
    write_bench_records(store, models)
}

// The catalog OWNS the note — stamp it verbatim after upsert (upsert's merge never clears, so a
// resolved caveat would otherwise stick to the stored record forever).
def private stamp_note(var models : array<BenchModel>; gguf, note : string) {
    for (m in models) {
        if (m.gguf == gguf) {
            m.note = note
            return
        }
    }
}

// idle gap before every pass — lets the kernel finish reclaiming the previous child's map
def private settle(cfg : Args) {
    if (cfg.settle > 0) {
        sleep(uint(cfg.settle * 1000))
    }
}

// cool-slot entry for das cells: thermal recovery, not map-reclaim churn — see --das-settle
def private settle_das(cfg : Args) {
    if (cfg.das_settle > 0) {
        sleep(uint(cfg.das_settle * 1000))
    }
}

// One lcpp_bench child with the cold-map cv retry — the shared engine of das_cell and the oracle
// cells. Fills `cell` from the child's --json-path records; false = the child failed or its
// records did not parse (already logged).
def private run_das_child(file, path, be, tag, exe : string; accel, frozen : bool; cfg : Args; threads : int;
                          cellPath : string; var cell : array<BenchModel>) : bool {
    let t0 = ref_time_ticks()
    var args <- [exe, "--",
                 "-m", path, "-p", "{cfg.plen}", "-n", "{cfg.ngen}", "-r", "{cfg.reps}",
                 "-t", "{threads}", "-o", "json", "--json-path", cellPath]
    if (be == "metal") {
        args |> push_from(["--ngl", "99"])
    }
    if (accel) {
        args |> push("--accel")
    }
    if (frozen) {
        args |> push("--frozen")
    }
    for (attempt in range(2)) {
        var out = ""
        let rc = run_and_stream(args, out)
        cell |> clear()
        if (rc != 0 || !read_bench_records(cellPath, cell)) {
            let tail = slice(out, max(0, length(out) - 400))
            to_log(LOG_ERROR, "gen_bench_records: FAILED {file} das {tag} rc={rc} ({elapsed_ms(t0)} ms) - tail: {tail}\n")
            logger_error("bench.records", "cell FAILED", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(tag), "rc" => JV(rc), "elapsed_ms" => JV(elapsed_ms(t0))}))
            return false
        }
        // variance tripwire, self-healing: the FIRST das process on a cold/churned .dlim map
        // warms across its reps (2026-07-24 35B: 150->198 monotone, mean 177 ± 21, while the
        // next process reads a flat 212 ± 1.7 — one in-process warmup pass cannot absorb a
        // 19GB map fighting page-cache eviction). A >3% cv cell is that signature: re-run the
        // child once — its predecessor just became the warmer — and keep the retry.
        var worst = 0.0lf
        for (c in cell) {
            for (r in c.runs) {
                if (r.engine != "das") {
                    continue
                }
                for (tv in values(r.tests)) {
                    if (tv.tok_s > 0.0lf) {
                        worst = max(worst, tv.stddev / tv.tok_s)
                    }
                }
            }
        }
        if (worst <= 0.03lf) {
            break
        }
        if (attempt == 0) {
            to_log(LOG_WARNING, "gen_bench_records: HIGH VARIANCE {file} das {tag} (cv {worst * 100.0lf:.1f}%) - cold-map warming suspect, re-running the cell warm\n")
            logger_warning("bench.records", "high variance - warm re-run", JV({"model" => JV(file), "leg" => JV(tag), "cv_pct" => JV(worst * 100.0lf)}))
            settle(cfg)
        } else {
            to_log(LOG_WARNING, "gen_bench_records: STILL HIGH VARIANCE {file} das {tag} (cv {worst * 100.0lf:.1f}%) after a warm re-run - keeping it, treat with suspicion\n")
        }
    }
    return true
}

def private das_cell(file, path, be, exe : string; accel : bool; cfg : Args; threads : int; cellPath, note, store : string;
                     var models : array<BenchModel>; var failed : array<string>) {
    let t0 = ref_time_ticks()
    let tag = accel ? "{be} accel" : be
    to_log(LOG_INFO, "gen_bench_records: START {file} das {tag}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(tag)}))
    var cell : array<BenchModel>
    mint_snapshot(models_dir())
    if (!run_das_child(file, path, be, tag, exe, accel, false, cfg, threads, cellPath, cell)) {
        failed |> push("{file} das {tag}")
        return
    }
    mint_check(models_dir(), "{file} das {tag}")
    absorb_cell(models, cell, store, note, failed, "ok")
    remove(cellPath)
    to_log(LOG_INFO, "gen_bench_records: DONE {file} das {tag} ({elapsed_ms(t0)} ms)\n")
    logger_info("bench.records", "cell done", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(tag), "elapsed_ms" => JV(elapsed_ms(t0))}))
}

// ===== Oracle mode (--oracle: the store rows ARE the oracle) =====

// One oracle cell: re-measure the stored row's leg under frozen artifacts and gate against it.
// The verdict line lands on the board; per-test detail logs live.
def private oracle_cell(gguf, be, tag, exe : string; accel : bool; stored : BenchRun; cfg : Args; threads : int;
                        cellPath : string; var board : array<string>) : OracleVerdict {
    let path = "{models_dir()}{gguf}"
    if (!stat(path).is_valid) {
        let miss = "FAIL {gguf} das {tag} - model file missing: {path}"
        to_log(LOG_ERROR, "gen_bench_records: ORACLE {miss}\n")
        board |> push(miss)
        return OracleVerdict.fail
    }
    to_log(LOG_INFO, "gen_bench_records: ORACLE START {gguf} das {tag}\n")
    logger_info("bench.records", "oracle cell start", JV({"model" => JV(gguf), "leg" => JV(tag)}))
    var cell : array<BenchModel>
    if (!run_das_child(gguf, path, be, tag, exe, accel, true, cfg, threads, cellPath, cell)) {
        board |> push("FAIL {gguf} das {tag} - cell did not measure (missing .dlim, unfit tune, or crash - see the log)")
        return OracleVerdict.fail
    }
    remove(cellPath)
    var verdict = OracleVerdict.fail
    var detail = "no das run in the fresh cell records"
    for (c in cell) {
        if (c.gguf != gguf) {
            continue
        }
        for (r in c.runs) {
            if (r.engine != "das") {
                continue
            }
            var lines : array<string>
            verdict = oracle_compare(r, stored, double(cfg.oracle_warn), double(cfg.oracle_fail), lines)
            detail = join(lines, "; ")
            delete lines
        }
    }
    let line = "{oracle_verdict_str(verdict)} {gguf} das {tag}: {detail}"
    let level = verdict == OracleVerdict.fail ? LOG_ERROR : (verdict == OracleVerdict.warn ? LOG_WARNING : LOG_INFO)
    to_log(level, "gen_bench_records: ORACLE {line}\n")
    logger_info("bench.records", "oracle cell done", JV({"model" => JV(gguf), "leg" => JV(tag), "verdict" => JV(oracle_verdict_str(verdict))}))
    board |> push(line)
    return verdict
}

// The ASR twin of oracle_cell: re-measure one stored audio row and gate it against itself. No
// reference tools run — this is the das side against its own record, like the LLM legs. The ms
// metrics gate through the same oracle_compare, mirrored (for a duration, worse is HIGHER).
def private oracle_asr_cell(spec : AsrModelSpec; stored : BenchRun; leg : string; cfg : Args; threads : int;
                            cellPath, exe : string; var board : array<string>) : OracleVerdict {
    let path = asr_model_path(spec)
    let leg_tag = empty(leg) ? "asr" : "asr {leg}"
    if (!stat(path).is_valid) {
        let miss = "FAIL {spec.display} das {leg_tag} - model file missing: {path}"
        to_log(LOG_ERROR, "gen_bench_records: ORACLE {miss}\n")
        board |> push(miss)
        return OracleVerdict.fail
    }
    to_log(LOG_INFO, "gen_bench_records: ORACLE START {spec.display} das {leg_tag}\n")
    logger_info("bench.records", "oracle asr cell start", JV({"model" => JV(spec.display), "leg" => JV(leg_tag)}))
    var args <- [exe, "--", "--asr", "-m", spec.das_model, "-r", "{cfg.asr_reps}", "-t", "{threads}",
                 "--json-path", cellPath]
    if (leg == "accel") {
        args |> push("--accel")
    } elif (leg == "metal") {
        args |> push_from(["--ngl", "1"])
    }
    var out = ""
    let rc = run_and_stream(args, out)
    var cell : array<BenchModel>
    if (rc != 0 || !read_bench_records(cellPath, cell)) {
        board |> push("FAIL {spec.display} das {leg_tag} - cell did not measure (rc={rc}, see the log)")
        return OracleVerdict.fail
    }
    remove(cellPath)
    var verdict = OracleVerdict.fail
    var detail = "no das {stored.backend} run in the fresh cell records"
    for (c in cell) {
        for (r in c.runs) {
            if (r.engine != "das" || empty(r.workload) || r.backend != stored.backend) {
                continue
            }
            var lines : array<string>
            verdict = oracle_compare(r, stored, double(cfg.oracle_warn), double(cfg.oracle_fail), lines)
            detail = join(lines, "; ")
            delete lines
        }
    }
    let line = "{oracle_verdict_str(verdict)} {spec.display} das {leg_tag}: {detail}"
    let level = verdict == OracleVerdict.fail ? LOG_ERROR : (verdict == OracleVerdict.warn ? LOG_WARNING : LOG_INFO)
    to_log(level, "gen_bench_records: ORACLE {line}\n")
    board |> push(line)
    return verdict
}

// The image twin of oracle_asr_cell: re-measure one stored image-chat row and gate it against
// itself. Like the ASR twin the cell is not frozen — it bakes the decoder and mmproj images it
// needs (the batch starts wiped); img:enc gates ms-mirrored, img:pp/img:tg as tok/s.
def private oracle_image_cell(gguf, mm, pic : string; stored : BenchRun; leg : string; cfg : Args; threads : int;
                              cellPath, exe : string; var board : array<string>) : OracleVerdict {
    let leg_tag = empty(leg) ? "image" : "image {leg}"
    let path = "{models_dir()}{gguf}"
    for (p in [path, "{models_dir()}{mm}", pic]) {
        if (!stat(p).is_valid) {
            let miss = "FAIL {gguf} das {leg_tag} - file missing: {p}"
            to_log(LOG_ERROR, "gen_bench_records: ORACLE {miss}\n")
            board |> push(miss)
            return OracleVerdict.fail
        }
    }
    to_log(LOG_INFO, "gen_bench_records: ORACLE START {gguf} das {leg_tag}\n")
    logger_info("bench.records", "oracle image cell start", JV({"model" => JV(gguf), "leg" => JV(leg_tag)}))
    var args <- [exe, "--", "-m", path, "--image-mmproj", "{models_dir()}{mm}", "--image", pic,
                 "-r", "{cfg.image_reps}", "-t", "{threads}", "--json-path", cellPath]
    if (leg == "accel") {
        args |> push("--accel")
    } elif (leg == "metal") {
        args |> push_from(["--ngl", "99"])
    }
    var out = ""
    let rc = run_and_stream(args, out)
    var cell : array<BenchModel>
    if (rc != 0 || !read_bench_records(cellPath, cell)) {
        board |> push("FAIL {gguf} das {leg_tag} - cell did not measure (rc={rc}, see the log)")
        return OracleVerdict.fail
    }
    remove(cellPath)
    var verdict = OracleVerdict.fail
    var detail = "no das {stored.backend} image-chat run in the fresh cell records"
    for (c in cell) {
        for (r in c.runs) {
            if (r.engine != "das" || r.workload != "image-chat" || r.backend != stored.backend) {
                continue
            }
            var lines : array<string>
            verdict = oracle_compare(r, stored, double(cfg.oracle_warn), double(cfg.oracle_fail), lines)
            detail = join(lines, "; ")
            delete lines
        }
    }
    let line = "{oracle_verdict_str(verdict)} {gguf} das {leg_tag}: {detail}"
    let level = verdict == OracleVerdict.fail ? LOG_ERROR : (verdict == OracleVerdict.warn ? LOG_WARNING : LOG_INFO)
    to_log(level, "gen_bench_records: ORACLE {line}\n")
    board |> push(line)
    return verdict
}

// The freeze exists so a cell cannot silently mint a DIFFERENT image and measure a config the
// stored row never used — but an ABSENT image (GC'd) has no identity to protect, and freezing
// there just makes the board unrunnable. The load walk mints it (never pre-mint by hand); this
// also hands the timed cell a WARM map, worth more than the mint: see the cv retry below.
def private oracle_prepare(gguf, be, tag, exe : string; accel : bool; threads : int; cellPath : string) {
    let path = "{models_dir()}{gguf}"
    return if (!stat(path).is_valid)   // the timed cell reports the missing model
    var args <- [exe, "--",
                 "-m", path, "-p", "8", "-n", "1", "-r", "1",
                 "-t", "{threads}", "-o", "json", "--json-path", cellPath]
    if (be == "metal") {
        args |> push_from(["--ngl", "99"])
    }
    if (accel) {
        args |> push("--accel")
    }
    let t0 = ref_time_ticks()
    var out = ""
    let rc = run_and_stream(args, out)
    remove(cellPath)
    if (rc != 0) {
        to_log(LOG_WARNING, "gen_bench_records: ORACLE PREPARE {gguf} {tag} rc={rc} - the frozen cell below reports it\n")
        return
    }
    // Under the batch lifecycle the image is EXPECTED absent (the batch starts with a wipe) —
    // the crossgen gate before this prepare already carried the pin-set proof via tune_sha.
    if (find(out, "prepared image streamed") >= 0) {
        to_log(LOG_INFO, "gen_bench_records: ORACLE PREPARE {gguf} {tag} - image baked ({elapsed_ms(t0)} ms)\n")
        return
    }
    to_log(LOG_INFO, "gen_bench_records: ORACLE PREPARE {gguf} {tag} - image mapped and warm ({elapsed_ms(t0)} ms)\n")
}

// The --oracle driver: the work list is the STORE's das LLM rows for this box (the published
// tables re-verify; coverage gaps fail loudly), each re-measured once and gated one-sided
// against its stored mean. Pure check — the store is never written.
// the cross-generation gate: a stored row minted under a DIFFERENT sidecar generation is not
// comparable — its mean belongs to different winners. Legacy rows (no tune_sha) warn and
// compare; a mismatch declines as INCOMPARABLE unless --oracle-allow-crossgen forces it.
def private crossgen_ok(sr : BenchRun; currentSha : string; allow : bool; what : string;
                        var board : array<string>; var fails : int&; var warns : int&) : bool {
    if (empty(currentSha)) {
        return true   // rig exe without a sidecar: the tune gate inside the cell reports it
    }
    if (empty(sr.tune_sha)) {
        board |> push("WARN {what}: legacy row (no sidecar generation recorded) - comparing anyway")
        warns++
        return true
    }
    if (sr.tune_sha == currentSha) {
        return true
    }
    if (allow) {
        board |> push("WARN {what}: cross-generation compare FORCED (stored {slice(sr.tune_sha, 0, 12)} vs current {slice(currentSha, 0, 12)})")
        warns++
        return true
    }
    board |> push("INCOMPARABLE {what}: sidecar generation changed (stored {slice(sr.tune_sha, 0, 12)} vs current {slice(currentSha, 0, 12)}) - re-record the row or --oracle-allow-crossgen")
    to_log(LOG_ERROR, "gen_bench_records: ORACLE INCOMPARABLE {what} - the stored mean belongs to different winners\n")
    fails++
    return false
}

def private oracle_main(cfg : Args; legNeon, legAmx, legMetal : bool; models : array<BenchModel>;
                        threads : int; cellPath, exe : string) : int {
    let box = box_name()
    let currentSha = sidecar_generation_sha(sidecar_for(exe))
    var doLlmOracle = false
    var doAsrOracle = false
    var doImageOracle = false
    workload_scope(cfg.workload, doLlmOracle, doAsrOracle, doImageOracle)   // validated at entry; the oracle shares the sweep's scope
    let asrTier = parse_tier(cfg.asr_tier)
    var board : array<string>
    var cells = 0
    var warns = 0
    var fails = 0
    var stopped = false
    for (m in models) {
        break if (stopped)
        if (!empty(cfg.only) && find(m.gguf, cfg.only) < 0) {
            continue
        }
        let cellsBefore = cells
        var imageVerified = false   // an image cell bakes the mmproj's images too - wipe them below
        for (sr in m.runs) {
            break if (stopped)
            if (sr.engine != "das" || sr.box != box) {
                continue
            }
            if (sr.workload == "image-chat") {   // image row: its own cell, mixed ms + tok/s gate
                if (!doImageOracle) {
                    continue
                }
                let mm = mmproj_companion(spec_by_file(m.gguf))
                if (empty(mm)) {
                    to_log(LOG_WARNING, "gen_bench_records: ORACLE skips stored run {m.gguf} image-chat - its spec carries no mmproj companion\n")
                    continue
                }
                var imgLeg = ""
                if (!stored_row_leg(sr.backend, sr.flavor, imgLeg)) {
                    to_log(LOG_WARNING, "gen_bench_records: ORACLE skips stored run {m.gguf} image-chat {sr.backend}/{sr.flavor} - no rig leg produces it\n")
                    continue
                }
                let imgWant = imgLeg == "metal" ? legMetal : (imgLeg == "accel" ? legAmx : legNeon)
                if (!imgWant) {
                    continue
                }
                cells++
                imageVerified = true
                if (!crossgen_ok(sr, currentSha, cfg.oracle_allow_crossgen, "{m.gguf} das image", board, fails, warns)) {
                    continue
                }
                settle(cfg)
                let pic = "{models_dir()}{bench_image_fixture()}"
                var vi = oracle_image_cell(m.gguf, mm, pic, sr, imgLeg, cfg, threads, cellPath, exe, board)
                if (vi == OracleVerdict.fail && cfg.oracle_retry_settle > 0) {
                    // same contract as the LLM legs below: the solo re-run's verdict stands
                    to_log(LOG_WARNING, "gen_bench_records: ORACLE FAIL {m.gguf} - auto solo re-run after {cfg.oracle_retry_settle}s settle (the retry is the verdict)\n")
                    board |> push("  ^ suspect FAIL - the auto solo re-run below is the verdict")
                    sleep(uint(cfg.oracle_retry_settle * 1000))
                    vi = oracle_image_cell(m.gguf, mm, pic, sr, imgLeg, cfg, threads, cellPath, exe, board)
                }
                if (vi == OracleVerdict.fail) {
                    fails++
                    if (!cfg.oracle_keep_going) {
                        to_log(LOG_ERROR, "gen_bench_records: ORACLE stopping at the first FAIL (--oracle-keep-going runs the full board)\n")
                        stopped = true
                    }
                } elif (vi == OracleVerdict.warn) {
                    warns++
                }
                continue
            }
            if (!empty(sr.workload)) {   // audio row: its own cell, its own ms-shaped gate
                if (!doAsrOracle) {
                    continue
                }
                var spec = AsrModelSpec()
                var found = false
                for (s in asr_catalog()) {
                    if (s.das_model == m.gguf && asr_model_selected(s, asrTier)) {
                        spec := s
                        found = true
                    }
                }
                if (!found) {
                    // tier narrowing is a quiet skip; a workload string no branch owns is not —
                    // an unknown modality's rows must never vanish from the board silently
                    if (sr.workload != "asr" && sr.workload != "audio-chat") {
                        to_log(LOG_WARNING, "gen_bench_records: ORACLE skips stored run {m.gguf} workload `{sr.workload}` - no oracle arm covers it\n")
                    }
                    continue
                }
                var asrLeg = ""
                if (!stored_row_leg(sr.backend, sr.flavor, asrLeg)) {
                    to_log(LOG_WARNING, "gen_bench_records: ORACLE skips stored run {m.gguf} {sr.backend}/{sr.flavor} - no rig leg produces it\n")
                    continue
                }
                cells++
                if (!crossgen_ok(sr, currentSha, cfg.oracle_allow_crossgen, "{m.gguf} das asr", board, fails, warns)) {
                    continue
                }
                settle(cfg)
                var va = oracle_asr_cell(spec, sr, asrLeg, cfg, threads, cellPath, exe, board)
                if (va == OracleVerdict.fail && cfg.oracle_retry_settle > 0) {
                    // same contract as the LLM legs below: the solo re-run's verdict stands
                    to_log(LOG_WARNING, "gen_bench_records: ORACLE FAIL {m.gguf} - auto solo re-run after {cfg.oracle_retry_settle}s settle (the retry is the verdict)\n")
                    board |> push("  ^ suspect FAIL - the auto solo re-run below is the verdict")
                    sleep(uint(cfg.oracle_retry_settle * 1000))
                    va = oracle_asr_cell(spec, sr, asrLeg, cfg, threads, cellPath, exe, board)
                }
                if (va == OracleVerdict.fail) {
                    fails++
                    if (!cfg.oracle_keep_going) {
                        to_log(LOG_ERROR, "gen_bench_records: ORACLE stopping at the first FAIL (--oracle-keep-going runs the full board)\n")
                        stopped = true
                    }
                } elif (va == OracleVerdict.warn) {
                    warns++
                }
                continue
            }
            if (!doLlmOracle) {   // -w narrows the oracle exactly like the publishing rig
                continue
            }
            var be = ""
            var tag = ""
            var accel = false
            var want = false
            if (sr.backend == "metal" && sr.flavor == "tuned") {
                be = "metal"
                tag = "metal"
                want = legMetal
            } elif (sr.backend == "cpu" && sr.flavor == "tuned") {
                be = "cpu"
                tag = "neon"
                want = legNeon
            } elif (sr.backend == "cpu" && sr.flavor == "accel") {
                be = "cpu"
                tag = "amx"
                accel = true
                want = legAmx
            } else {
                to_log(LOG_WARNING, "gen_bench_records: ORACLE skips stored run {m.gguf} {sr.backend}/{sr.flavor} - no rig leg produces it\n")
                continue
            }
            if (!want) {
                continue
            }
            cells++
            if (!crossgen_ok(sr, currentSha, cfg.oracle_allow_crossgen, "{m.gguf} das {tag}", board, fails, warns)) {
                continue
            }
            oracle_prepare(m.gguf, be, tag, exe, accel, threads, cellPath)
            settle(cfg)
            var v = oracle_cell(m.gguf, be, tag, exe, accel, sr, cfg, threads, cellPath, board)
            if (v == OracleVerdict.fail && cfg.oracle_retry_settle > 0) {
                // a tail-of-board FAIL is a re-run instruction, never a verdict (BRINGUP): the
                // solo re-run after settle is automatic, and ITS verdict stands
                to_log(LOG_WARNING, "gen_bench_records: ORACLE FAIL {m.gguf} - auto solo re-run after {cfg.oracle_retry_settle}s settle (the retry is the verdict)\n")
                board |> push("  ^ suspect FAIL - the auto solo re-run below is the verdict")
                sleep(uint(cfg.oracle_retry_settle * 1000))
                v = oracle_cell(m.gguf, be, tag, exe, accel, sr, cfg, threads, cellPath, board)
            }
            if (v == OracleVerdict.fail) {
                fails++
                if (!cfg.oracle_keep_going) {
                    to_log(LOG_ERROR, "gen_bench_records: ORACLE stopping at the first FAIL (--oracle-keep-going runs the full board)\n")
                    stopped = true
                }
            } elif (v == OracleVerdict.warn) {
                warns++
            }
        }
        if (cells > cellsBefore) {   // fully profiled here - its images are done serving
            let wm = dlim_wipe("{models_dir()}{m.gguf}")
            if (wm.removed > 0) {
                to_log(LOG_INFO, "gen_bench_records: lifecycle - {m.gguf} profiled, {wm.removed} image(s) deleted ({wm.freed >> 20l} MB)\n")
            }
            if (imageVerified) {   // the image cells baked the mmproj's images too - same lifecycle
                let wmm = dlim_wipe("{models_dir()}{mmproj_companion(spec_by_file(m.gguf))}")
                if (wmm.removed > 0) {
                    to_log(LOG_INFO, "gen_bench_records: lifecycle - {m.gguf} mmproj verified, {wmm.removed} image(s) deleted ({wmm.freed >> 20l} MB)\n")
                }
            }
        }
    }
    to_log(LOG_INFO, "gen_bench_records: ===== ORACLE BOARD ({box}) =====\n")
    for (line in board) {
        to_log(LOG_INFO, "gen_bench_records:   {line}\n")
    }
    if (cells == 0) {
        to_log(LOG_ERROR, "gen_bench_records: ORACLE found no stored das rows for the selected legs on {box} - nothing verified\n")
        return 1
    }
    let early = stopped ? " - STOPPED EARLY" : ""
    to_log(fails > 0 ? LOG_ERROR : LOG_INFO, "gen_bench_records: ORACLE {cells} cell(s): {fails} FAIL, {warns} WARN{early}\n")
    return fails > 0 ? 1 : 0
}

def private ref_pass(file, path, bin, flavor : string; ngl : int; nopo : bool; cfg : Args; threads : int;
                     hw : HardwareInfo; box, date, note, store : string;
                     var models : array<BenchModel>; var failed : array<string>; var refSkipped : array<string>) {
    if (empty(bin)) {
        refSkipped |> push("{file} ref {flavor} {ngl != 0 ? "metal" : "cpu"}")
        return
    }
    let backend = ngl != 0 ? "metal" : "cpu"
    let t0 = ref_time_ticks()
    var args <- [bin, "-m", path, "-ngl", "{ngl}"]
    // -nopo 1 = --no-op-offload: without it a stock build at -ngl 0 still ships big-batch ops
    // with host weights to Metal (26B: MTL0 193MB, +33% pp) — not a CPU number
    if (nopo) {
        args |> push_from(["-nopo", "1"])
    }
    args |> push_from(["-t", "{threads}"])
    if (ngl == 0 && !cfg.ref_no_affinity) {
        args |> push_from(lcpp_affinity_argv(threads))
    }
    args |> push_from(["-p", "{cfg.plen}", "-n", "{cfg.ngen}", "-r", "{cfg.reps}", "-o", "json"])
    let cmd = argv_line(args)
    to_log(LOG_INFO, "gen_bench_records: START {file} ref {flavor} {backend}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(file), "engine" => JV("llama.cpp"), "leg" => JV("{flavor} {backend}")}))
    var rows : array<LlamaBenchRow>
    var raw = ""
    if (!run_llama_bench(args, rows, raw) || empty(rows)) {
        // the tail is the whole diagnosis: a ref cell that dies has no other voice
        let tail = slice(raw, max(0, length(raw) - 400))
        // name WHY, not just the last 200 bytes: an empty capture is a dead child, output with
        // no result array is a child that only logged, and either beats a mid-token slice
        let why = (empty(strip(raw)) ? "child printed NOTHING (spawn or early death)"
            : (find(raw, "\n[") < 0 && !(strip(raw) |> starts_with("[")) ? "output carries NO json result array (only diagnostics?)"
                : "a result array was found but did not parse"))
        to_log(LOG_ERROR, "gen_bench_records: FAILED {file} ref {flavor} {backend} ({elapsed_ms(t0)} ms) - {why}\n  cmd: {cmd}\n  tail: {tail}\n")
        logger_error("bench.records", "cell FAILED", JV({"model" => JV(file), "engine" => JV("llama.cpp"), "leg" => JV("{flavor} {backend}"), "elapsed_ms" => JV(elapsed_ms(t0))}))
        failed |> push("{file} ref {flavor} {backend}")
        return
    }
    var rr = BenchRun(
        engine = "llama.cpp",
        backend = backend,
        flavor = flavor,
        box = box,
        threads = threads,
        date = date,
        cmd = cmd,
        sha = rows[0].build_commit,
        source = "official",
        exec_fmt = lcpp_exec_fmt(backend),
        hardware = hw)
    rr.files |> emplace(file_identity(path, "weights"))
    for (row in rows) {
        let tag = row.n_gen == 0 ? "pp{row.n_prompt}" : "tg{row.n_gen}"
        rr.tests |> insert(tag, BenchTest(tok_s = row.avg_ts, stddev = row.stddev_ts))
    }
    var bm = BenchModel(gguf = file, params_b = double(rows[0].model_n_params) / 1.0e9lf, note = note)
    bm.runs |> emplace(rr)
    upsert_bench_model(models, bm)
    stamp_note(models, file, note)
    write_bench_records(store, models)
    to_log(LOG_INFO, "gen_bench_records: DONE {file} ref {flavor} {backend} ({elapsed_ms(t0)} ms)\n")
    logger_info("bench.records", "cell done", JV({"model" => JV(file), "engine" => JV("llama.cpp"), "leg" => JV("{flavor} {backend}"), "elapsed_ms" => JV(elapsed_ms(t0))}))
}

// ===== Image legs (the image-chat cells of vision decoders; the ref is patched llama-mtmd-cli) =====

// One lcpp_bench --image child per (vision model, leg), absorbed via the same cell protocol.
// leg: "" = cpu (neon) | "accel" (amx) | "metal"
def private image_das_cell(file, path, mm, pic : string; cfg : Args; threads : int; cellPath, note, store, exe : string;
                           var models : array<BenchModel>; var failed : array<string>; leg : string = "") {
    let t0 = ref_time_ticks()
    let log_leg = empty(leg) ? "image" : "image {leg}"
    for (p in [mm, pic]) {
        if (!stat(p).is_valid) {
            to_log(LOG_WARNING, "gen_bench_records: SKIP {file} das {log_leg} - missing companion {p}\n")
            return
        }
    }
    to_log(LOG_INFO, "gen_bench_records: START {file} das {log_leg}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(log_leg)}))
    var args <- [exe, "--", "-m", path, "--image-mmproj", mm, "--image", pic,
                 "-r", "{cfg.image_reps}", "-t", "{threads}", "-o", "json", "--json-path", cellPath]
    if (leg == "accel") {
        args |> push("--accel")
    } elif (leg == "metal") {
        args |> push_from(["--ngl", "99"])
    }
    mint_snapshot(models_dir())
    var out = ""
    let rc = run_and_stream(args, out)
    var cell : array<BenchModel>
    if (rc != 0 || !read_bench_records(cellPath, cell)) {
        let tail = slice(out, max(0, length(out) - 400))
        to_log(LOG_ERROR, "gen_bench_records: FAILED {file} das {log_leg} rc={rc} ({elapsed_ms(t0)} ms) - tail: {tail}\n")
        logger_error("bench.records", "cell FAILED", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(log_leg), "rc" => JV(rc)}))
        failed |> push("{file} das {log_leg}")
        return
    }
    mint_check(models_dir(), "{file} das {log_leg}")
    absorb_cell(models, cell, store, note, failed, "ok")
    remove(cellPath)
    to_log(LOG_INFO, "gen_bench_records: DONE {file} das {log_leg} ({elapsed_ms(t0)} ms)\n")
    logger_info("bench.records", "cell done", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(log_leg), "elapsed_ms" => JV(elapsed_ms(t0))}))
}

// The image-chat reference cell: patched llama-mtmd-cli (MTMD_TIMING carries the split and the
// position counts) on the same decoder + mmproj + picture. Both engines price prefill and
// decode as own-positions over own-time; -n 32 pins the decode budget to the das cell's cap.
// Best over image_reps: min encode, max pp/tg — the das cell's own protocol.
def private image_ref_pass(file, path, mm, pic, bin, flavor : string; ngl : int; nopo : bool; cfg : Args; threads : int;
                           hw : HardwareInfo; box, date, note, store : string;
                           var models : array<BenchModel>; var failed : array<string>; var refSkipped : array<string>) {
    let backend = ngl != 0 ? "metal" : "cpu"
    if (empty(bin)) {
        refSkipped |> push("{file} ref image {flavor} {backend}")
        return
    }
    for (p_ in [mm, pic]) {
        if (!stat(p_).is_valid) {
            // the das half of this pair already warned and skipped on the same absence -
            // spawning a doomed child would red the sweep where the das side only skips
            to_log(LOG_WARNING, "gen_bench_records: SKIP {file} ref image {flavor} {backend} - missing companion {p_}\n")
            return
        }
    }
    let t0 = ref_time_ticks()
    var args <- [bin, "-m", path, "--mmproj", mm, "--image", pic,
                 "-p", image_turn_prompt(), "--temp", "0", "--jinja",
                 "-n", "{image_turn_steps()}", "-t", "{threads}", "-ngl", "{ngl}"]
    if (ngl == 0) {
        // -ngl/-nopo govern the DECODER; the mmproj tower rides the GPU regardless unless
        // told not to — without this flag a stock -ngl 0 llama-mtmd-cli run encoded E2B in
        // 107 ms (vs its own Metal arm's 116 ms and its honest CPU's 19946 ms) under a cpu
        // label. Same class as -nopo itself.
        args |> push("--no-mmproj-offload")
    }
    if (nopo) {
        args |> push("--no-op-offload")
    }
    let cmd = argv_line(args)
    to_log(LOG_INFO, "gen_bench_records: START {file} ref image {flavor} {backend}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(file), "engine" => JV("llama.cpp"), "leg" => JV("image {flavor} {backend}")}))
    var best_enc = -1.0lf
    var best_pp = -1.0lf
    var best_tg = -1.0lf
    for (_rep in range(cfg.image_reps)) {
        let out = run_capture_argv(args)
        let r = parse_mtmd_image(out)
        if (!r.ok) {
            let tail = slice(out, max(0, length(out) - 300))
            to_log(LOG_WARNING, "gen_bench_records: {file} ref image {flavor} {backend} - no timing parsed this rep - tail: {tail}\n")
            continue
        }
        if (best_enc < 0.0lf || r.enc_ms < best_enc) {
            best_enc = r.enc_ms
        }
        best_pp = max(best_pp, double(r.n_prefill) * 1000.0lf / r.prefill_ms)
        best_tg = max(best_tg, double(r.n_decode) * 1000.0lf / r.decode_ms)
    }
    if (best_enc < 0.0lf) {
        to_log(LOG_ERROR, "gen_bench_records: FAILED {file} ref image {flavor} {backend} - no rep parsed ({elapsed_ms(t0)} ms)\n")
        logger_error("bench.records", "cell FAILED", JV({"model" => JV(file), "engine" => JV("llama.cpp"), "leg" => JV("image {flavor} {backend}")}))
        failed |> push("{file} ref image {flavor} {backend}")
        return
    }
    var rr = BenchRun(
        engine = "llama.cpp",
        backend = backend,
        flavor = flavor,
        workload = "image-chat",
        box = box,
        threads = threads,
        date = date,
        cmd = cmd,
        source = "official",
        versions = mtmd_versions(bin),
        exec_fmt = lcpp_exec_fmt(backend),
        hardware = hw)
    rr.tests |> insert("img:enc", BenchTest(ms = best_enc))
    rr.tests |> insert("img:pp", BenchTest(tok_s = best_pp))
    rr.tests |> insert("img:tg", BenchTest(tok_s = best_tg))
    rr.files <- [file_identity(path, "weights"), file_identity(mm, "mmproj"), file_identity(pic, "image")]
    var bm = BenchModel(gguf = file, note = note)
    bm.runs |> emplace(rr)
    upsert_bench_model(models, bm)
    stamp_note(models, file, note)
    write_bench_records(store, models)
    to_log(LOG_INFO, "gen_bench_records: DONE {file} ref image {flavor} {backend} - enc {best_enc:.1f} ms | pp {best_pp:.1f} | tg {best_tg:.1f} tok/s ({elapsed_ms(t0)} ms)\n")
    logger_info("bench.records", "cell done", JV({"model" => JV(file), "engine" => JV("llama.cpp"), "leg" => JV("image {flavor} {backend}"), "elapsed_ms" => JV(elapsed_ms(t0))}))
}

// ===== ASR legs (the audio workloads, same adjacency discipline as the LLM legs) =====

// das side: one lcpp_bench --asr child per catalog model, absorbed via the same cell protocol
// the families the metal das leg serves end to end (tower + decoder); the Conformer and
// GGUF-tower rows would trip the --ngl anti-sandbag (their towers keep the CPU by design)
def private is_metal_served_family(spec : AsrModelSpec) : bool => find(spec.display, "Whisper") >= 0

def private asr_das_cell(spec : AsrModelSpec; cfg : Args; threads : int; cellPath, store, exe : string;
                         var models : array<BenchModel>; var failed : array<string>; leg : string = "") {
    let t0 = ref_time_ticks()
    let leg_tag = empty(leg) ? "" : " {leg}"
    let log_leg = "asr{leg_tag}"
    to_log(LOG_INFO, "gen_bench_records: START {spec.display} das asr{leg_tag}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(spec.display), "engine" => JV("das"), "leg" => JV(log_leg)}))
    var args <- [exe, "--", "--asr", "-m", spec.das_model, "-r", "{cfg.asr_reps}", "-t", "{threads}",
                 "--json-path", cellPath]
    if (leg == "accel") {
        args |> push("--accel")
    } elif (leg == "metal") {
        args |> push_from(["--ngl", "1"])
    }
    var out = ""
    let rc = run_and_stream(args, out)
    var cell : array<BenchModel>
    if (rc != 0 || !read_bench_records(cellPath, cell)) {
        let tail = slice(out, max(0, length(out) - 400))
        to_log(LOG_ERROR, "gen_bench_records: FAILED {spec.display} das {log_leg} rc={rc} ({elapsed_ms(t0)} ms) - tail: {tail}\n")
        logger_error("bench.records", "cell FAILED", JV({"model" => JV(spec.display), "engine" => JV("das"), "leg" => JV(log_leg), "rc" => JV(rc)}))
        failed |> push("{spec.display} das {log_leg}")
        return
    }
    absorb_cell(models, cell, store, "", failed)
    remove(cellPath)
    to_log(LOG_INFO, "gen_bench_records: DONE {spec.display} das {log_leg} ({elapsed_ms(t0)} ms)\n")
    logger_info("bench.records", "cell done", JV({"model" => JV(spec.display), "engine" => JV("das"), "leg" => JV(log_leg), "elapsed_ms" => JV(elapsed_ms(t0))}))
}

// True when the reference tool's binary/interpreter exists on this box (skip-with-warning gate)
def private asr_ref_available(tool : string) : bool {
    if (tool == "parakeet-cli" || tool == "whisper-cli") {
        return stat(whisper_cpp_bin(tool)).is_valid
    }
    if (tool == "mtmd") {
        return stat(mtmd_bin()).is_valid
    }
    if (tool == "onnx") {
        return stat(onnx_py()).is_valid
    }
    if (tool == "nemo") {
        return stat(nemo_py()).is_valid
    }
    return false
}

// reference side: one tool over the model's clips, one BenchRun with per-clip tests keys.
// cli tools fork per clip; onnx/nemo run ONE process over all clips (python load amortized).
def private asr_ref_pass(spec : AsrModelSpec; ref : AsrRef; versions : string; cfg : Args; threads : int;
                         hw : HardwareInfo; box, date, store : string;
                         var models : array<BenchModel>; var failed : array<string>; gpu : bool = false) {
    if (!asr_ref_available(ref.tool)) {
        to_log(LOG_WARNING, "gen_bench_records: SKIP {spec.display} ref {ref.tool} - tool not on this box\n")
        return
    }
    let t0 = ref_time_ticks()
    to_log(LOG_INFO, "gen_bench_records: START {spec.display} ref {ref.tool}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(spec.display), "engine" => JV(asr_engine_name(ref.tool)), "leg" => JV("asr")}))
    // cli tools time the ref's own carrier when the catalog names one (the q8_0 siblings);
    // mtmd's handle stays its mmproj, onnx/nemo weights are venv-managed
    let cli_tool = ref.tool == "parakeet-cli" || ref.tool == "whisper-cli"
    let model_path = (cli_tool && !empty(ref.handle)) ? "{spec.root}{ref.handle}" : "{spec.root}{spec.das_model}"
    // a slow reference caps its own clip ladder in the catalog; empty = the spec's
    var inscope rbuckets := spec.buckets
    if (!empty(ref.buckets)) {
        rbuckets := ref.buckets
    }
    var rr = BenchRun(
        engine = asr_engine_name(ref.tool),
        backend = gpu ? "metal" : "cpu",
        flavor = "stock",
        workload = asr_workload(spec),
        box = box,
        threads = threads,
        date = date,
        source = "official",
        versions = versions,
        exec_fmt = ref.exec_fmt,
        hardware = hw)
    var measured = 0
    if (ref.tool == "onnx" || ref.tool == "nemo") {
        // the onnx/nemo weights are venv-managed (the variant/HF handle in `versions` names
        // them); only the clips are local files here
        var clips := rbuckets
        var lib : array<string>   // the aggregate leg reads it again after the per-clip rows
        if (!empty(spec.libri_short) && ref.tool == "onnx") {
            // the dictation aggregate pairs against onnx only: one amortized process covers all
            // 25 clips in ~30 s, where a per-clip cli process would cost ~9 min per sweep
            lib <- libri_wavs()   // nolint:PERF030 — first write to the empty decl above
            if (empty(lib)) {
                to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref onnx - librispeech set absent/partial, leg skipped\n")
            } else {
                clips |> push_from(lib)
            }
        }
        to_log(LOG_INFO, "gen_bench_records:   {ref.tool} over {length(clips)} clip(s) in one process (python model load amortized)\n")
        let pyargs <- asr_pybench_args(ref.tool, ref.handle, clips, threads, cfg.asr_reps)
        rr.cmd = argv_line(pyargs)
        var out = run_capture_argv(pyargs)
        if (find(out, "BENCH") < 0) {
            // pipe came back rowless — fall back to the --out file (Windows: parakeet loaders
            // wreck the child's stdout pipe; the file survives everything)
            let sidecar = fread(asr_pybench_out_path())
            if (find(sidecar, "BENCH") >= 0) {
                to_log(LOG_INFO, "gen_bench_records:   {ref.tool} pipe was rowless - rows recovered from --out file\n")
                out = sidecar
            } else {
                let dtail = slice(out, max(0, length(out) - 400))
                to_log(LOG_WARNING, "gen_bench_records:   {ref.tool} output carries no BENCH rows - tail: {dtail}\n")
            }
        }
        for (wav in rbuckets) {
            var audio_s = -1.0lf
            let ms = parse_bench_best(out, wav, audio_s)
            if (ms > 0.0lf) {
                rr.tests |> insert("asr:{wav}", BenchTest(ms = ms, audio_s = audio_s))
                rr.files |> emplace(file_identity("{asr_corpus_dir()}{wav}", "clip"))
                measured++
            }
        }
        if (!empty(lib)) {
            // all-or-nothing: a partial sum would pair against das's full-set total and lie
            var lib_ms = 0.0lf
            var lib_audio = 0.0lf
            var lib_n = 0
            for (wav in lib) {
                var audio_s = -1.0lf
                let ms = parse_bench_best(out, base_name(wav), audio_s)
                if (ms > 0.0lf) {
                    lib_ms += ms
                    lib_audio += audio_s
                    lib_n++
                    rr.files |> emplace(file_identity("{asr_corpus_dir()}{wav}", "clip"))
                }
            }
            if (lib_n == length(lib)) {
                rr.tests |> insert(libri_test_key(), BenchTest(ms = lib_ms, audio_s = lib_audio))
                measured++
                to_log(LOG_INFO, "gen_bench_records:   onnx librispeech25 total {lib_ms:.0f} ms ({lib_n} clips)\n")
            } else {
                to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref onnx - {lib_n}/{length(lib)} libri clips parsed, aggregate NOT recorded\n")
            }
        }
        delete lib
        delete clips
    } else {
        rr.files |> emplace(file_identity(model_path, "weights"))
        if (ref.tool == "mtmd") {
            rr.files |> emplace(file_identity("{spec.root}{ref.handle}", "mmproj"))
        }
        for (wav in rbuckets) {
            let wav_path = "{asr_corpus_dir()}{wav}"
            to_log(LOG_INFO, "gen_bench_records:   {ref.tool} {wav} ({cfg.asr_reps} rep(s))...\n")
            var cmd = ""
            var enc_ms = 0.0lf
            var rtext = ""
            let ms = (ref.tool == "mtmd"
                ? run_asr_mtmd(model_path, "{spec.root}{ref.handle}", wav_path, threads, cfg.asr_reps, cmd, enc_ms, rtext)
                : run_asr_cli(ref.tool, model_path, wav_path, threads, cfg.asr_reps, cmd, gpu))
            rr.cmd = cmd   // last clip's line; they differ only in -f
            if (ms > 0.0lf) {
                to_log(LOG_INFO, "gen_bench_records:   {ref.tool} {wav} = {ms:.0f} ms\n")
                // audio seconds live on the das cell's matching test key; cli tools don't report it
                // (mtmd is the one ref that stores its transcript — parity must be readable, not hoped)
                rr.tests |> insert("asr:{wav}", BenchTest(ms = ms, encode_ms = enc_ms, text = rtext))
                rr.files |> emplace(file_identity(wav_path, "clip"))
                measured++
            } else {
                to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref {ref.tool} {wav} - no timing parsed\n")
            }
        }
        if (!empty(spec.libri_short) && ref.tool == "parakeet-cli") {
            // ONE multi-file process: the load (~0.6 s) amortizes over the whole set and the
            // cumulative total − load IS the aggregate (see run_asr_cli_multi)
            var lib <- libri_wavs()
            if (empty(lib)) {
                to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref {ref.tool} - librispeech set absent/partial, leg skipped\n")
            } else {
                var paths <- [for (w in lib); "{asr_corpus_dir()}{w}"]
                to_log(LOG_INFO, "gen_bench_records:   {ref.tool} librispeech25 - one process over {length(lib)} clip(s) ({cfg.asr_reps} rep(s))...\n")
                var cmd = ""
                let ms = run_asr_cli_multi(ref.tool, model_path, paths, threads, cfg.asr_reps, cmd)
                rr.cmd = cmd
                if (ms > 0.0lf) {
                    rr.tests |> insert(libri_test_key(), BenchTest(ms = ms))
                    rr.files |> reserve(length(rr.files) + length(lib))
                    for (w in lib) {
                        rr.files |> emplace(file_identity("{asr_corpus_dir()}{w}", "clip"))
                    }
                    measured++
                    to_log(LOG_INFO, "gen_bench_records:   {ref.tool} librispeech25 = {ms:.0f} ms\n")
                } else {
                    to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref {ref.tool} librispeech25 - no timing parsed\n")
                }
                delete paths
            }
            delete lib
        }
    }
    if (measured == 0) {
        to_log(LOG_ERROR, "gen_bench_records: FAILED {spec.display} ref {ref.tool} - nothing measured ({elapsed_ms(t0)} ms)\n")
        logger_error("bench.records", "cell FAILED", JV({"model" => JV(spec.display), "engine" => JV(asr_engine_name(ref.tool)), "leg" => JV("asr")}))
        failed |> push("{spec.display} ref {ref.tool}")
        return
    }
    var bm = BenchModel(gguf = spec.das_model, arch = spec.display,
        size_bytes = int64(stat(model_path).size))
    bm.runs |> emplace(rr)
    upsert_bench_model(models, bm)
    write_bench_records(store, models)
    to_log(LOG_INFO, "gen_bench_records: DONE {spec.display} ref {ref.tool} ({measured} clip(s), {elapsed_ms(t0)} ms)\n")
    logger_info("bench.records", "cell done", JV({"model" => JV(spec.display), "engine" => JV(asr_engine_name(ref.tool)), "leg" => JV("asr"), "clips" => JV(measured), "elapsed_ms" => JV(elapsed_ms(t0))}))
}

// The orchestrator never measures: it spawns the released exe per cell and reads its JSON, so its
// own kernels are irrelevant and tuning them would put minutes of hidden work in front of a
// measurement run — the trap this rig exists to avoid.
[export, tune_policy(missing = "fallback")]
def main : int {
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "gen_bench_records")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "gen_bench_records")
        return 0
    }
    // stdout is the live narration (cells stream their children through); the ndjson logger is
    // the timestamped machine-readable record beside it
    logger_set_name(prof_log_name())
    to_log(LOG_INFO, "gen_bench_records: box={box_name()} workload={cfg.workload} - ndjson record: {logger_get_path()}\n")
    // The child exe owns its tune state: winners baked in at build time, sidecar shipped beside it.
    // Pointing it at a shared box manifest re-opens exactly what that replaced — the manifest is
    // older than a freshly built exe, the staleness rule drops every kernel to fallback, and the
    // cell measures nothing real. So the rig CLEARS the override rather than setting one, and an
    // inherited value from the caller's shell is cleared too.
    if (!empty(g_env_core.tune_manifest)) {
        to_log(LOG_INFO, "gen_bench_records: clearing inherited DAS_TUNE_MANIFEST - the bench exe carries its own winners\n")
        set_env_variable("DAS_TUNE_MANIFEST", "")
    }
    // The lifecycle wipe (batch_start_wipe) runs AFTER each mode's exe gate below — a batch
    // that will refuse to measure must not destroy state first (bitten live: an exe-staleness
    // refusal right after a 323 GB wipe).
    let apple = is_apple_box()
    if (cfg.backend == "metal" && !apple) {   // fail fast instead of running metal cells that all decline on non-Apple boxes
        to_log(LOG_ERROR, "gen_bench_records: --backend metal requested on a non-Apple box - Metal is macOS-only\n")
        return 1
    }
    var doLlm = false
    var doAsr = false
    var doImage = false
    if (!workload_scope(cfg.workload, doLlm, doAsr, doImage)) {
        to_log(LOG_ERROR, "gen_bench_records: unknown --workload `{cfg.workload}` (expected llm | asr | image | all)\n")
        return 1
    }
    // pair legs: --legs wins outright; otherwise --backend implies its legs (amx is Apple-only —
    // both its sides need Accelerate)
    var legNeon = false
    var legAmx = false
    var legMetal = false
    if (!empty(cfg.legs)) {
        for (l in split(cfg.legs, ",")) {
            let tok = trim(l)
            if (tok == "cpu") {   // the documented alias: both CPU flavors, whatever the box has stored
                legNeon = true
                legAmx = true
            } elif (tok == "neon") {
                legNeon = true
            } elif (tok == "amx") {
                legAmx = true
            } elif (tok == "metal") {
                legMetal = true
            } elif (!empty(tok)) {
                to_log(LOG_ERROR, "gen_bench_records: unknown leg `{tok}` (expected cpu | neon | amx | metal)\n")
                return 1
            }
        }
    } else {
        legNeon = cfg.backend == "cpu" || cfg.backend == "all"
        legAmx = legNeon && apple
        legMetal = cfg.backend == "metal" || (cfg.backend == "all" && apple)
    }
    if ((legMetal || legAmx) && !apple) {
        to_log(LOG_ERROR, "gen_bench_records: legs metal/amx need an Apple box\n")
        return 1
    }
    if (!(legNeon || legAmx || legMetal)) {
        to_log(LOG_ERROR, "gen_bench_records: no legs selected (--backend {cfg.backend} --legs {cfg.legs})\n")
        return 1
    }
    if (cfg.oracle) {
        // GATE 1: no refs run (binaries not even required); GATE 2: one das pass per stored row
        // (the cv warm-retry stays — it REPLACES a bad cold measure); GATE 3: the timed cell is
        // frozen — its image is baked by the prepare pass, and tune_sha carries the pin-set proof.
        let ostore = bench_records_path()
        var omodels : array<BenchModel>
        if (!read_bench_records(ostore, omodels)) {
            to_log(LOG_ERROR, "gen_bench_records: --oracle needs an existing store - {ostore} is absent/unreadable\n")
            return 1
        }
        let othreads = cfg.threads > 0 ? cfg.threads : profile_threads()
        let oexe = rig_exe_checked()
        if (empty(oexe)) {
            return 1
        }
        batch_start_wipe(cfg.workload)
        return oracle_main(cfg, legNeon, legAmx, legMetal, omodels,
            othreads, "modules/dasLLAMA/performance/records/_cell_{box_name()}.json", oexe)
    }
    var refClean = empty(cfg.ref_clean) ? g_env_prof.llama_bench_clean : cfg.ref_clean
    var refStock = empty(cfg.ref_stock) ? g_env_prof.llama_bench_stock : cfg.ref_stock
    if (!empty(refClean) && !stat(refClean).is_valid) {
        to_log(LOG_WARNING, "gen_bench_records: clean-cpu ref `{refClean}` not found - neon cells run das-only\n")
        refClean = ""
    }
    if (!empty(refStock) && !stat(refStock).is_valid) {
        to_log(LOG_WARNING, "gen_bench_records: stock ref `{refStock}` not found - no stock rows\n")
        refStock = ""
    }
    // a ref-less sweep is legal but must never be a surprise: an unset ref silently minted a
    // whole das-only board once — say it up front, and the end summary names every skipped pair
    if (doLlm && empty(refClean)) {
        to_log(LOG_WARNING, "gen_bench_records: clean-cpu ref UNSET (--ref-clean / LLAMA_BENCH_CLEAN) - every neon cell mints das-only, no board ratio\n")
    }
    if (doLlm && empty(refStock)) {
        to_log(LOG_WARNING, "gen_bench_records: stock ref UNSET (--ref-stock / LLAMA_BENCH_STOCK) - no stock rows, metal/amx cells mint das-only\n")
    }
    // the image refs resolve off the SAME knobs — announce by env spelling where they change
    // what this sweep mints (the sibling rule for stock; MTMD_BIN or its layout for the cpu arm)
    if (doImage) {
        let sm = sibling_mtmd(refStock)
        if (empty(sm)) {
            to_log(LOG_WARNING, "gen_bench_records: stock llama-mtmd-cli UNRESOLVED (no sibling beside --ref-stock / LLAMA_BENCH_STOCK) - metal/amx image cells mint das-only\n")
        } else {
            to_log(LOG_INFO, "gen_bench_records: image refs - stock llama-mtmd-cli = {sm} (LLAMA_BENCH_STOCK sibling)\n")
        }
        let cm = mtmd_bin()
        if (!stat(cm).is_valid) {
            to_log(LOG_WARNING, "gen_bench_records: CPU ref llama-mtmd-cli ABSENT ({cm}; MTMD_BIN overrides) - neon image cells mint das-only\n")
        } else {
            to_log(LOG_INFO, "gen_bench_records: image refs - cpu llama-mtmd-cli = {cm} (MTMD_BIN)\n")
        }
    }
    let exe = rig_exe_checked()
    if (empty(exe)) {
        return 1
    }
    batch_start_wipe(cfg.workload)
    mkdir_rec("modules/dasLLAMA/performance/records")
    snapshot_tune_generation(exe)
    let store = bench_records_path()
    var models : array<BenchModel>
    read_bench_records(store, models)
    let hw = gather_hardware()
    let date = format_time(get_clock(), "%Y-%m-%d")
    let box = box_name()
    let threads = cfg.threads > 0 ? cfg.threads : profile_threads()
    let cellPath = "modules/dasLLAMA/performance/records/_cell_{box}.json"
    var failed : array<string>
    var refSkipped : array<string>
    var cat <- official_catalog()
    for (ent in cat) {
        if (!doLlm && !doImage) {
            continue
        }
        let file = ent.file
        if (!empty(cfg.only) && find(file, cfg.only) < 0) {
            continue
        }
        let mm = mmproj_companion(ent)
        if (!doLlm && empty(mm)) {   // -w image visits the vision rows only
            continue
        }
        let path = "{models_dir()}{file}"
        if (!stat(path).is_valid) {
            to_log(LOG_WARNING, "gen_bench_records: SKIP {file} - not on this box\n")
            continue
        }
        // profiling a model STARTS with its frozen pinned-greedy fixture (model_specs.das
        // evidence, the same rows test_parity gates); the verdict stamps every das row as `parity`
        to_log(LOG_INFO, "gen_bench_records: PARITY PREGATE {file}\n")
        var out = ""
        if (run_and_stream([exe, "--", "--parity", "-m", path], out) != 0) {
            to_log(LOG_ERROR, "gen_bench_records: PARITY PREGATE failed for {file} - skipping every cell of this model (pregate output above)\n")
            logger_error("bench.records", "parity pregate failed", JV({"model" => JV(file)}))
            failed |> push("{file} parity")
            continue
        }
        // thermal-fair pass order: each ratio's das cell and its ref run ADJACENT (the
        // measurement protocol's interleave rule at the pass level), and the metal pair goes
        // FIRST — the old [das cpu, das metal, refs...] order ran das metal straight off the
        // das-cpu heat soak while the lcpp metal ref ran minutes later on a different chip
        // state (27B: das 110.7 soaked vs 138.1 quiet — a phantom 0.91 board red). Refs still
        // run independently of das-cell failures.
        if (doLlm && legMetal) {
            settle_das(cfg)
            das_cell(file, path, "metal", exe, false, cfg, threads, cellPath, ent.note, store, models, failed)
            // the ref gets COOL-SLOT entry too: it is not insensitive on high-power parts —
            // zen2 35B read pp 81.7 twelve seconds behind a das cell vs 103.9 solo-cooled
            settle_das(cfg)
            ref_pass(file, path, refStock, "stock", 99, false, cfg, threads, hw, box, date, ent.note, store, models, failed, refSkipped)
        }
        if (doLlm && legNeon) {
            settle_das(cfg)
            das_cell(file, path, "cpu", exe, false, cfg, threads, cellPath, ent.note, store, models, failed)
            // the ref gets COOL-SLOT entry too: it is not insensitive on high-power parts —
            // zen2 35B read pp 81.7 twelve seconds behind a das cell vs 103.9 solo-cooled
            settle_das(cfg)
            ref_pass(file, path, refClean, "clean-cpu", 0, false, cfg, threads, hw, box, date, ent.note, store, models, failed, refSkipped)
        }
        if (doLlm && legAmx) {
            settle_das(cfg)
            das_cell(file, path, "cpu", exe, true, cfg, threads, cellPath, ent.note, store, models, failed)
            // the ref gets COOL-SLOT entry too: it is not insensitive on high-power parts —
            // zen2 35B read pp 81.7 twelve seconds behind a das cell vs 103.9 solo-cooled
            settle_das(cfg)
            ref_pass(file, path, refStock, "stock", 0, true, cfg, threads, hw, box, date, ent.note, store, models, failed, refSkipped)
        }
        // the image-chat legs of a vision decoder (an mmproj companion) — same
        // leg order and cool-slot discipline; the picture is the one pinned fixture
        if (doImage && !empty(mm)) {
            let mmPath = "{models_dir()}{mm}"
            let picPath = "{models_dir()}{bench_image_fixture()}"
            let stockMtmd = sibling_mtmd(refStock)
            // pairing mirrors the text legs: metal vs stock -ngl 99; neon vs the dedicated
            // CPU build (kernel vs kernel — stock ggml op-offloads to Metal even at -ngl 0);
            // amx vs stock -ngl 0 --no-op-offload
            if (legMetal) {
                settle_das(cfg)
                image_das_cell(file, path, mmPath, picPath, cfg, threads, cellPath, ent.note, store, exe, models, failed, "metal")
                settle_das(cfg)
                image_ref_pass(file, path, mmPath, picPath, stockMtmd, "stock", 99, false, cfg, threads, hw, box, date, ent.note, store, models, failed, refSkipped)
            }
            if (legNeon) {
                settle_das(cfg)
                image_das_cell(file, path, mmPath, picPath, cfg, threads, cellPath, ent.note, store, exe, models, failed)
                settle_das(cfg)
                let cpuMtmd = mtmd_bin()
                image_ref_pass(file, path, mmPath, picPath, stat(cpuMtmd).is_valid ? cpuMtmd : "", "clean-cpu", 0, false, cfg, threads, hw, box, date, ent.note, store, models, failed, refSkipped)
            }
            if (legAmx) {
                settle_das(cfg)
                image_das_cell(file, path, mmPath, picPath, cfg, threads, cellPath, ent.note, store, exe, models, failed, "accel")
                settle_das(cfg)
                image_ref_pass(file, path, mmPath, picPath, stockMtmd, "stock", 0, true, cfg, threads, hw, box, date, ent.note, store, models, failed, refSkipped)
            }
        }
        let wm = dlim_wipe(path)   // fully profiled - its images are done serving
        if (wm.removed > 0) {
            to_log(LOG_INFO, "gen_bench_records: lifecycle - {file} profiled, {wm.removed} image(s) deleted ({wm.freed >> 20l} MB)\n")
        }
        if (doImage && !empty(mm)) {   // the image legs' second carrier gets the same lifecycle
            let wmm = dlim_wipe("{models_dir()}{mm}")
            if (wmm.removed > 0) {
                to_log(LOG_INFO, "gen_bench_records: lifecycle - {mm} profiled, {wmm.removed} image(s) deleted ({wmm.freed >> 20l} MB)\n")
            }
        }
    }
    if (doAsr) {
        let asrTier = parse_tier(cfg.asr_tier)
        // per-tool version lines are session facts — capture once, not per model
        var toolVersions : table<string; string>
        for (spec in asr_catalog()) {
            if (!empty(cfg.only) && find(spec.display, cfg.only) < 0 && find(spec.das_model, cfg.only) < 0) {
                continue
            }
            if (!stat(asr_model_path(spec)).is_valid) {
                to_log(LOG_WARNING, "gen_bench_records: SKIP {spec.display} - model not on this box\n")
                continue
            }
            if (!asr_model_selected(spec, asrTier)) {
                to_log(LOG_INFO, "gen_bench_records: SKIP {spec.display} - not in --asr-tier {cfg.asr_tier}\n")
                continue
            }
            settle_das(cfg)
            asr_das_cell(spec, cfg, threads, cellPath, store, exe, models, failed)
            static_if (typeinfo builtin_module_exists(das_accelerate)) {
                settle_das(cfg)
                asr_das_cell(spec, cfg, threads, cellPath, store, exe, models, failed, "accel")
            }
            static_if (typeinfo builtin_module_exists(das_metal)) {
                if (is_metal_served_family(spec)) {   // GPU-served families only: the anti-sandbag would refuse the rest
                    settle_das(cfg)
                    asr_das_cell(spec, cfg, threads, cellPath, store, exe, models, failed, "metal")
                }
            }
            for (rf in spec.refs) {
                if (!key_exists(toolVersions, rf.tool)) {
                    toolVersions |> insert(rf.tool, asr_ref_available(rf.tool) ? asr_tool_versions(rf.tool) : "")
                }
                settle(cfg)
                asr_ref_pass(spec, rf, toolVersions?[rf.tool] ?? "", cfg, threads, hw, box, date, store, models, failed)
                static_if (typeinfo builtin_module_exists(das_metal)) {
                    if (rf.tool == "whisper-cli" && is_metal_served_family(spec)) {   // the metal das row's like-for-like pair
                        settle(cfg)
                        asr_ref_pass(spec, rf, toolVersions?[rf.tool] ?? "", cfg, threads, hw, box, date, store, models, failed, true)
                    }
                }
            }
            let wasr = dlim_wipe(asr_model_path(spec))   // same lifecycle for the audio images
            if (wasr.removed > 0) {
                to_log(LOG_INFO, "gen_bench_records: lifecycle - {spec.display} profiled, {wasr.removed} image(s) deleted ({wasr.freed >> 20l} MB)\n")
            }
        }
    }
    if (!empty(refSkipped)) {
        // the loud tail of the sweep-start UNSET warning: name every pair the board is missing
        to_log(LOG_WARNING, "gen_bench_records: {length(refSkipped)} ref cell(s) SKIPPED (refs unset - das-only rows, no board ratio): {join(refSkipped, ", ")}\n")
    }
    if (!empty(failed)) {
        let list = join(failed, ", ")
        to_log(LOG_ERROR, "gen_bench_records: {length(failed)} FAILED cell(s): {list}\n")
        return 1
    }
    return 0
}
