options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)

require profile_common
require daslib/clargs
require daslib/fio
require math

// The official record driver (modules/dasLLAMA/PUBLIC_BENCH_PLAN.md, step 3): loops the public
// catalog x pair legs, runs lcpp_bench as a CHILD per cell (one model process at a time), and
// upserts the --json-path records into the per-box store — written after EVERY cell, so a crash
// never costs a measured cell. Each board ratio is one das cell + its matched ref run ADJACENT:
//   metal : das metal            vs stock -ngl 99          ("as shipped")
//   neon  : das cpu              vs clean-cpu -ngl 0       ("generic neon-64", kernel vs kernel)
//   amx   : das cpu --accel      vs stock -ngl 0 -nopo 1   ("cpu + AMX" — -nopo keeps lcpp's Metal
//           op-offload from serving 'CPU' rows on the GPU; nopo == nometal-build, window-proven)
//   bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- [-o substr] [--legs neon,amx,metal]
// Ref binaries come from setup_lcpp_ref: --ref-clean/--ref-stock or LLAMA_BENCH_CLEAN/LLAMA_BENCH_STOCK.

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
    var out = ""
    let rc = run_and_capture(args, out)
    var cell : array<BenchModel>
    if (rc != 0 || !read_bench_records(cellPath, cell)) {
        let tail = slice(out, max(0, length(out) - 400))
        to_log(LOG_ERROR, "gen_bench_records: FAILED {file} das {tag} rc={rc} ({elapsed_ms(t0)} ms) — tail: {tail}\n")
        failed |> push("{file} das {tag}")
        return
    }
    absorb_cell(models, cell, store, note)
    remove(cellPath)
    to_log(LOG_INFO, "gen_bench_records: DONE {file} das {tag} ({elapsed_ms(t0)} ms)\n")
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
    var rows : array<LlamaBenchRow>
    if (!run_llama_bench(cmd, rows) || empty(rows)) {
        to_log(LOG_ERROR, "gen_bench_records: FAILED {file} ref {flavor} {backend} ({elapsed_ms(t0)} ms)\n")
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
    let apple = is_apple_box()
    if (cfg.backend == "metal" && !apple) {   // fail fast instead of running metal cells that all decline on non-Apple boxes
        to_log(LOG_ERROR, "gen_bench_records: --backend metal requested on a non-Apple box — Metal is macOS-only\n")
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
    if (!empty(failed)) {
        let list = join(failed, ", ")
        to_log(LOG_ERROR, "gen_bench_records: {length(failed)} FAILED cell(s): {list}\n")
        return 1
    }
    return 0
}
