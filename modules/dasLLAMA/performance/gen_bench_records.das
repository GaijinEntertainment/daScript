options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)

require profile_common
require dasllama/dasllama_image   // dlim_clean — the automatic step-zero GC
require daslib/clargs
require daslib/fio
require math

// The official record driver — ONE rig for every workload: loops the catalogs x pair legs, runs
// the das side as a CHILD per cell (lcpp_bench for LLM, asr_bench_cell for audio; one model
// process at a time), and upserts the --json-path records into the per-box store — written after
// EVERY cell, so a crash never costs a measured cell. Each board ratio is one das cell + its
// matched ref run ADJACENT:
//   metal : das metal            vs stock -ngl 99          ("gpu")
//   neon  : das cpu              vs clean-cpu -ngl 0       ("cpu", kernel vs kernel)
//   amx   : das cpu --accel      vs stock -ngl 0 -nopo 1   ("cpu + accel" — -nopo keeps lcpp's
//           Metal op-offload from serving 'CPU' rows on the GPU; nopo == nometal-build)
//   asr   : das cpu (asr_bench_cell) vs each catalog reference tool, per clip
//   bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- [-o substr] [--workload llm|asr|all]
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

    @clarg_name = "ref-clean"
    @clarg_doc = "clean-cpu llama-bench binary (default: env LLAMA_BENCH_CLEAN)"
    ref_clean : string

    @clarg_name = "ref-stock"
    @clarg_doc = "stock llama-bench binary (default: env LLAMA_BENCH_STOCK)"
    ref_stock : string

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

    @clarg_short = "w"
    @clarg_doc = "Workloads to measure: llm | asr | all (default: all — absent models/tools skip with a warning)"
    workload : string = "all"

    @clarg_name = "asr-reps"
    @clarg_doc = "Timed repetitions per ASR clip, best kept (default: 2 — clips are minutes, not seconds)"
    asr_reps : int = 2

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct private PubEntry {
    file : string
    note : string   // board caveat, stamped into the model record ("" = none)
}

// The public catalog: community-popular GGUFs, one popular quant each — NOT the internal quant
// matrix. Curated to what people run THIS year (2026 spring/summer lists: Qwen3.6 family, gemma-4
// daily drivers, the standing staples). Absent files skip with a warning, so one list serves
// every box.
def private pub_catalog() : array<PubEntry> {
    return <- [
        PubEntry(file = "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf"),
        PubEntry(file = "Qwen3.6-27B-MTP-Q4_K_M.gguf"),
        PubEntry(file = "gemma-4-26B-A4B-it-Q4_K_M.gguf"),
        PubEntry(file = "gemma-4-12B-it-Q4_K_M.gguf"),
        PubEntry(file = "gemma-4-E4B-it-Q8_0.gguf"),
        PubEntry(file = "gpt-oss-20b-mxfp4.gguf"),
        PubEntry(file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"),
        PubEntry(file = "Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf")
    ]
}

def private daslang_bin() : string {
    let env = get_env_variable("DASLANG_BIN")
    return empty(env) ? "bin/daslang" : env
}

def private elapsed_ms(t0 : int64) : int {
    return get_time_usec(t0) / 1000
}

// Stamp source + the catalog note and upsert every run of `cell` into the store, then persist.
def private absorb_cell(var models : array<BenchModel>; var cell : array<BenchModel>; store, note : string) {
    for (c in cell) {
        for (r in c.runs) {
            r.source = "official"
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

def private das_cell(file, path, be : string; accel : bool; cfg : Args; threads : int; cellPath, note, store : string;
                     var models : array<BenchModel>; var failed : array<string>) {
    let t0 = ref_time_ticks()
    let tag = accel ? "{be} accel" : be
    to_log(LOG_INFO, "gen_bench_records: START {file} das {tag}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(tag)}))
    var args <- [daslang_bin(), "-jit", "modules/dasLLAMA/benchmarks/lcpp_bench.das", "--",
                 "-m", path, "-p", "{cfg.plen}", "-n", "{cfg.ngen}", "-r", "{cfg.reps}",
                 "-t", "{threads}", "-o", "json", "--json-path", cellPath]
    if (be == "metal") {
        args |> push("--ngl")
        args |> push("99")
    }
    if (accel) {
        args |> push("--accel")
    }
    var cell : array<BenchModel>
    for (attempt in range(2)) {
        var out = ""
        let rc = run_and_stream(args, out)
        cell |> clear()
        if (rc != 0 || !read_bench_records(cellPath, cell)) {
            let tail = slice(out, max(0, length(out) - 400))
            to_log(LOG_ERROR, "gen_bench_records: FAILED {file} das {tag} rc={rc} ({elapsed_ms(t0)} ms) — tail: {tail}\n")
            logger_error("bench.records", "cell FAILED", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(tag), "rc" => JV(rc), "elapsed_ms" => JV(elapsed_ms(t0))}))
            failed |> push("{file} das {tag}")
            return
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
            to_log(LOG_WARNING, "gen_bench_records: HIGH VARIANCE {file} das {tag} (cv {worst * 100.0lf:.1f}%) — cold-map warming suspect, re-running the cell warm\n")
            logger_warning("bench.records", "high variance — warm re-run", JV({"model" => JV(file), "leg" => JV(tag), "cv_pct" => JV(worst * 100.0lf)}))
            settle(cfg)
        } else {
            to_log(LOG_WARNING, "gen_bench_records: STILL HIGH VARIANCE {file} das {tag} (cv {worst * 100.0lf:.1f}%) after a warm re-run — keeping it, treat with suspicion\n")
        }
    }
    absorb_cell(models, cell, store, note)
    remove(cellPath)
    to_log(LOG_INFO, "gen_bench_records: DONE {file} das {tag} ({elapsed_ms(t0)} ms)\n")
    logger_info("bench.records", "cell done", JV({"model" => JV(file), "engine" => JV("das"), "leg" => JV(tag), "elapsed_ms" => JV(elapsed_ms(t0))}))
}

def private ref_pass(file, path, bin, flavor : string; ngl : int; nopo : bool; cfg : Args; threads : int;
                     hw : HardwareInfo; box, date, note, store : string;
                     var models : array<BenchModel>; var failed : array<string>) {
    if (empty(bin)) {
        return
    }
    let backend = ngl != 0 ? "metal" : "cpu"
    let t0 = ref_time_ticks()
    let aff = ngl == 0 && affinity_on() ? lcpp_affinity_args(threads) : ""
    // -nopo 1 = --no-op-offload: without it a stock build at -ngl 0 still ships big-batch ops
    // with host weights to Metal (26B: MTL0 193MB, +33% pp) — not a CPU number
    let np = nopo ? " -nopo 1" : ""
    let cmd = "\"{bin}\" -m \"{path}\" -ngl {ngl}{np} -t {threads}{aff} -p {cfg.plen} -n {cfg.ngen} -r {cfg.reps} -o json"
    to_log(LOG_INFO, "gen_bench_records: START {file} ref {flavor} {backend}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(file), "engine" => JV("llama.cpp"), "leg" => JV("{flavor} {backend}")}))
    var rows : array<LlamaBenchRow>
    if (!run_llama_bench(cmd, rows) || empty(rows)) {
        to_log(LOG_ERROR, "gen_bench_records: FAILED {file} ref {flavor} {backend} ({elapsed_ms(t0)} ms)\n")
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

// ===== ASR legs (the audio workloads, same adjacency discipline as the LLM legs) =====

// das side: one asr_bench_cell child per catalog model, absorbed via the same cell protocol
def private asr_das_cell(spec : AsrModelSpec; cfg : Args; threads : int; cellPath, store : string;
                         var models : array<BenchModel>; var failed : array<string>) {
    let t0 = ref_time_ticks()
    to_log(LOG_INFO, "gen_bench_records: START {spec.display} das asr\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(spec.display), "engine" => JV("das"), "leg" => JV("asr")}))
    let args <- [daslang_bin(), "-jit", "modules/dasLLAMA/benchmarks/asr/asr_bench_cell.das", "--",
                 "-m", spec.das_model, "-r", "{cfg.asr_reps}", "-t", "{threads}",
                 "--json-path", cellPath]
    var out = ""
    let rc = run_and_stream(args, out)
    var cell : array<BenchModel>
    if (rc != 0 || !read_bench_records(cellPath, cell)) {
        let tail = slice(out, max(0, length(out) - 400))
        to_log(LOG_ERROR, "gen_bench_records: FAILED {spec.display} das asr rc={rc} ({elapsed_ms(t0)} ms) — tail: {tail}\n")
        logger_error("bench.records", "cell FAILED", JV({"model" => JV(spec.display), "engine" => JV("das"), "leg" => JV("asr"), "rc" => JV(rc)}))
        failed |> push("{spec.display} das asr")
        return
    }
    absorb_cell(models, cell, store, "")
    remove(cellPath)
    to_log(LOG_INFO, "gen_bench_records: DONE {spec.display} das asr ({elapsed_ms(t0)} ms)\n")
    logger_info("bench.records", "cell done", JV({"model" => JV(spec.display), "engine" => JV("das"), "leg" => JV("asr"), "elapsed_ms" => JV(elapsed_ms(t0))}))
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
                         var models : array<BenchModel>; var failed : array<string>) {
    if (!asr_ref_available(ref.tool)) {
        to_log(LOG_WARNING, "gen_bench_records: SKIP {spec.display} ref {ref.tool} — tool not on this box\n")
        return
    }
    let t0 = ref_time_ticks()
    to_log(LOG_INFO, "gen_bench_records: START {spec.display} ref {ref.tool}\n")
    logger_info("bench.records", "cell start", JV({"model" => JV(spec.display), "engine" => JV(asr_engine_name(ref.tool)), "leg" => JV("asr")}))
    let model_path = "{spec.root}{spec.das_model}"
    var rr = BenchRun(
        engine = asr_engine_name(ref.tool),
        backend = "cpu",
        flavor = "stock",
        workload = asr_workload(spec),
        box = box,
        threads = threads,
        date = date,
        source = "official",
        versions = versions,
        hardware = hw)
    var measured = 0
    if (ref.tool == "onnx" || ref.tool == "nemo") {
        // the onnx/nemo weights are venv-managed (the variant/HF handle in `versions` names
        // them); only the clips are local files here
        var clips := spec.buckets
        var lib : array<string>
        if (!empty(spec.libri_short) && ref.tool == "onnx") {
            // the dictation aggregate pairs against onnx only: one amortized process covers all
            // 25 clips in ~30 s, where a per-clip cli process would cost ~9 min per sweep
            lib <- libri_wavs()
            if (empty(lib)) {
                to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref onnx — librispeech set absent/partial, leg skipped\n")
            } else {
                clips |> push_from(lib)
            }
        }
        to_log(LOG_INFO, "gen_bench_records:   {ref.tool} over {length(clips)} clip(s) in one process (python model load amortized)\n")
        let cmd = asr_pybench_cmd(ref.tool, ref.handle, clips, threads, cfg.asr_reps)
        rr.cmd = cmd
        let out = run_capture("{cmd} 2>&1", false)
        for (wav in spec.buckets) {
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
                to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref onnx — {lib_n}/{length(lib)} libri clips parsed, aggregate NOT recorded\n")
            }
        }
        delete lib
        delete clips
    } else {
        rr.files |> emplace(file_identity(model_path, "weights"))
        if (ref.tool == "mtmd") {
            rr.files |> emplace(file_identity("{spec.root}{ref.handle}", "mmproj"))
        }
        for (wav in spec.buckets) {
            let wav_path = "{asr_corpus_dir()}{wav}"
            to_log(LOG_INFO, "gen_bench_records:   {ref.tool} {wav} ({cfg.asr_reps} rep(s))...\n")
            var cmd = ""
            let ms = (ref.tool == "mtmd"
                ? run_asr_mtmd(model_path, "{spec.root}{ref.handle}", wav_path, threads, cfg.asr_reps, cmd)
                : run_asr_cli(ref.tool, model_path, wav_path, threads, cfg.asr_reps, cmd))
            rr.cmd = cmd   // last clip's line; they differ only in -f
            if (ms > 0.0lf) {
                to_log(LOG_INFO, "gen_bench_records:   {ref.tool} {wav} = {ms:.0f} ms\n")
                // audio seconds live on the das cell's matching test key; cli tools don't report it
                rr.tests |> insert("asr:{wav}", BenchTest(ms = ms))
                rr.files |> emplace(file_identity(wav_path, "clip"))
                measured++
            } else {
                to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref {ref.tool} {wav} — no timing parsed\n")
            }
        }
        if (!empty(spec.libri_short) && ref.tool == "parakeet-cli") {
            // ONE multi-file process: the load (~0.6 s) amortizes over the whole set and the
            // cumulative total − load IS the aggregate (see run_asr_cli_multi)
            var lib <- libri_wavs()
            if (empty(lib)) {
                to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref {ref.tool} — librispeech set absent/partial, leg skipped\n")
            } else {
                var paths <- [for (w in lib); "{asr_corpus_dir()}{w}"]
                to_log(LOG_INFO, "gen_bench_records:   {ref.tool} librispeech25 — one process over {length(lib)} clip(s) ({cfg.asr_reps} rep(s))...\n")
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
                    to_log(LOG_WARNING, "gen_bench_records: {spec.display} ref {ref.tool} librispeech25 — no timing parsed\n")
                }
                delete paths
            }
            delete lib
        }
    }
    if (measured == 0) {
        to_log(LOG_ERROR, "gen_bench_records: FAILED {spec.display} ref {ref.tool} — nothing measured ({elapsed_ms(t0)} ms)\n")
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

[export]
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
    to_log(LOG_INFO, "gen_bench_records: box={box_name()} workload={cfg.workload} — ndjson record: {logger_get_path()}\n")
    // ONE tune per box: every child (das cells, converter runs) reads the same env-pointed
    // manifest, so their image identities agree and the pre-bake serves every cell. Divergent
    // per-script mints (near-tie winners flip) were observed to fork identities — B7.
    if (empty(get_env_variable("DAS_TUNE_MANIFEST"))) {
        set_env_variable("DAS_TUNE_MANIFEST", rig_tune_manifest())
        to_log(LOG_INFO, "gen_bench_records: DAS_TUNE_MANIFEST={rig_tune_manifest()} (one tune per box)\n")
    }
    // STEP ZERO, automatic: GC image siblings that can never load again (BROKEN/version-stale)
    // across the dirs this sweep reads — a box once carried 274 GB of them. OTHER stays.
    let gc_llm = dlim_clean(models_dir(), true)
    if (gc_llm.removed > 0) {
        to_log(LOG_INFO, "gen_bench_records: GC freed {gc_llm.freed >> 20l} MB — {gc_llm.removed} stale image(s) in {models_dir()}\n")
    }
    if (cfg.workload == "asr" || cfg.workload == "all") {
        let gc_w = dlim_clean(whisper_models_dir(), true)
        if (gc_w.removed > 0) {
            to_log(LOG_INFO, "gen_bench_records: GC freed {gc_w.freed >> 20l} MB — {gc_w.removed} stale image(s) in {whisper_models_dir()}\n")
        }
    }
    let apple = is_apple_box()
    if (cfg.backend == "metal" && !apple) {   // fail fast instead of running metal cells that all decline on non-Apple boxes
        to_log(LOG_ERROR, "gen_bench_records: --backend metal requested on a non-Apple box — Metal is macOS-only\n")
        return 1
    }
    let doLlm = cfg.workload == "llm" || cfg.workload == "all"
    let doAsr = cfg.workload == "asr" || cfg.workload == "all"
    if (!doLlm && !doAsr) {
        to_log(LOG_ERROR, "gen_bench_records: unknown --workload `{cfg.workload}` (expected llm | asr | all)\n")
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
            if (tok == "neon") {
                legNeon = true
            } elif (tok == "amx") {
                legAmx = true
            } elif (tok == "metal") {
                legMetal = true
            } elif (!empty(tok)) {
                to_log(LOG_ERROR, "gen_bench_records: unknown leg `{tok}` (expected neon | amx | metal)\n")
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
    var refClean = empty(cfg.ref_clean) ? get_env_variable("LLAMA_BENCH_CLEAN") : cfg.ref_clean
    var refStock = empty(cfg.ref_stock) ? get_env_variable("LLAMA_BENCH_STOCK") : cfg.ref_stock
    if (!empty(refClean) && !stat(refClean).is_valid) {
        to_log(LOG_WARNING, "gen_bench_records: clean-cpu ref `{refClean}` not found — neon cells run das-only\n")
        refClean = ""
    }
    if (!empty(refStock) && !stat(refStock).is_valid) {
        to_log(LOG_WARNING, "gen_bench_records: stock ref `{refStock}` not found — no stock rows\n")
        refStock = ""
    }
    mkdir_rec("modules/dasLLAMA/performance/records")
    let store = bench_records_path()
    var models : array<BenchModel>
    read_bench_records(store, models)
    let hw = gather_hardware()
    let date = format_time(get_clock(), "%Y-%m-%d")
    let box = box_name()
    let threads = cfg.threads > 0 ? cfg.threads : profile_threads()
    let cellPath = "modules/dasLLAMA/performance/records/_cell_{box}.json"
    var failed : array<string>
    var cat <- pub_catalog()
    for (ent in cat) {
        if (!doLlm) {
            continue
        }
        let file = ent.file
        if (!empty(cfg.only) && find(file, cfg.only) < 0) {
            continue
        }
        let path = "{models_dir()}{file}"
        if (!stat(path).is_valid) {
            to_log(LOG_WARNING, "gen_bench_records: SKIP {file} — not on this box\n")
            continue
        }
        // thermal-fair pass order: each ratio's das cell and its ref run ADJACENT (the
        // measurement protocol's interleave rule at the pass level), and the metal pair goes
        // FIRST — the old [das cpu, das metal, refs...] order ran das metal straight off the
        // das-cpu heat soak while the lcpp metal ref ran minutes later on a different chip
        // state (27B: das 110.7 soaked vs 138.1 quiet — a phantom 0.91 board red). Refs still
        // run independently of das-cell failures.
        if (legMetal) {
            settle(cfg)
            das_cell(file, path, "metal", false, cfg, threads, cellPath, ent.note, store, models, failed)
            settle(cfg)
            ref_pass(file, path, refStock, "stock", 99, false, cfg, threads, hw, box, date, ent.note, store, models, failed)
        }
        if (legNeon) {
            settle(cfg)
            das_cell(file, path, "cpu", false, cfg, threads, cellPath, ent.note, store, models, failed)
            settle(cfg)
            ref_pass(file, path, refClean, "clean-cpu", 0, false, cfg, threads, hw, box, date, ent.note, store, models, failed)
        }
        if (legAmx) {
            settle(cfg)
            das_cell(file, path, "cpu", true, cfg, threads, cellPath, ent.note, store, models, failed)
            settle(cfg)
            ref_pass(file, path, refStock, "stock", 0, true, cfg, threads, hw, box, date, ent.note, store, models, failed)
        }
    }
    if (doAsr) {
        // per-tool version lines are session facts — capture once, not per model
        var toolVersions : table<string; string>
        for (spec in asr_catalog()) {
            if (!empty(cfg.only) && find(spec.display, cfg.only) < 0 && find(spec.das_model, cfg.only) < 0) {
                continue
            }
            if (!stat("{spec.root}{spec.das_model}").is_valid) {
                to_log(LOG_WARNING, "gen_bench_records: SKIP {spec.display} — model not on this box\n")
                continue
            }
            settle(cfg)
            asr_das_cell(spec, cfg, threads, cellPath, store, models, failed)
            for (rf in spec.refs) {
                if (!key_exists(toolVersions, rf.tool)) {
                    toolVersions |> insert(rf.tool, asr_ref_available(rf.tool) ? asr_tool_versions(rf.tool) : "")
                }
                settle(cfg)
                asr_ref_pass(spec, rf, toolVersions?[rf.tool] ?? "", cfg, threads, hw, box, date, store, models, failed)
            }
        }
    }
    if (!empty(failed)) {
        let list = join(failed, ", ")
        to_log(LOG_ERROR, "gen_bench_records: {length(failed)} FAILED cell(s): {list}\n")
        return 1
    }
    return 0
}
