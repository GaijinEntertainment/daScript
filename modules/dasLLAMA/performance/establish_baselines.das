options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options indenting = 4

// Batch reference-baseline establisher for the dasLLAMA profiling suite. Runs the REFERENCE tools once
// per box (llama.cpp llama-bench for LLMs here; the ASR CLIs land in a later phase) and writes their
// tok/s to a machine-readable per-box baseline that gen_profile then merges with the freshly-measured
// das side. This exists so establishing on a new box is one command, never a manual llama-bench sweep.
//
// Parsec MUST be off — this runs the reference tool, which is a measurement.
//   bin/daslang modules/dasLLAMA/performance/establish_baselines.das -- -t small

require profile_common
require daslib/clargs
require strings
require llvm/daslib/llvm_tune   // [tune_policy]: this tool benches llama.cpp only — never auto-tune
require dasllama/dasllama_env

[CommandLineArgs]
struct Args {
    @clarg_short = "t"
    @clarg_doc = "Tier: small | large | all (default small)"
    tier : string = "small"

    @clarg_doc = "Threads for llama-bench -t (0 = per-box default: 16, m1 8)"
    threads : int = 0

    @clarg_doc = "Prompt tokens for the prefill test (default 512)"
    nprompt : int = 512

    @clarg_doc = "Generated tokens for the emission test (default 128)"
    ngen : int = 128

    @clarg_short = "o"
    @clarg_doc = "Only models whose display name contains this substring (default: whole tier)"
    only : string = ""

    @clarg_doc = "GPU layers to offload (0 = CPU baseline; 99 = full Metal offload -> baseline_llm_<box>_metal.tsv)"
    ngl : int = 0

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// llama-bench binary; override with LLAMA_BENCH. Defaults are the clean builds (upstream ebd048f, no
// Accelerate/AMX/BLAS, no op-profile instrumentation): the CPU-only build for ngl==0 — the fair
// kernel-vs-kernel reference — and the Metal-enabled build for GPU-offload sweeps.
def llama_bench_bin(ngl : int) : string {
    let env = g_env_prof.llama_bench
    if (!empty(env)) {
        return env
    }
    return (ngl != 0 ? "/Users/borisbatkin/Work/llama.cpp-clean/build-metal/bin/llama-bench"
                     : "/Users/borisbatkin/Work/llama.cpp-clean/build/bin/llama-bench")
}

// Run llama-bench for one model (via profile_common's shared runner) and fold its two result rows
// (n_gen=0 prefill, n_prompt=0 decode) into an LlmBaseline. prefill/emission stay -1 on failure.
def bench_one(display, path : string; threads, nprompt, ngen, ngl : int) : LlmBaseline {
    // affinity args pin the bench to distinct physical cores on the boxes that pin (see profile_common)
    var args <- [llama_bench_bin(ngl), "-m", path, "-ngl", "{ngl}", "-t", "{threads}"]
    args |> push_from(lcpp_affinity_argv(threads), ["-p", "{nprompt}", "-n", "{ngen}", "-o", "json"])
    var rows : array<LlamaBenchRow>
    if (!run_llama_bench(args, rows)) {
        return LlmBaseline(model = display, prefill_tok_s = -1.0lf, emission_tok_s = -1.0lf, lcpp_commit = "", n_params = 0l)
    }
    var prefill = -1.0lf
    var emission = -1.0lf
    var commit = ""
    var nparams = 0l
    for (r in rows) {
        commit = r.build_commit
        nparams = r.model_n_params
        if (r.n_gen == 0 && r.n_prompt > 0) {
            prefill = r.avg_ts
        }
        if (r.n_prompt == 0 && r.n_gen > 0) {
            emission = r.avg_ts
        }
    }
    return LlmBaseline(model = display, prefill_tok_s = prefill, emission_tok_s = emission, lcpp_commit = commit, n_params = nparams)
}

// Replace the row for the same model, or append it. Keeps the baseline one-row-per-model.
def private upsert_baseline(var rows : array<LlmBaseline>; b : LlmBaseline) {
    for (row in rows) {
        if (row.model == b.model) {
            row = b
            return
        }
    }
    rows |> push(b)
}

// fallback: this tool only runs llama-bench — das gen-kernels never execute, so an
// untuned box must NOT trigger the (minutes-long) auto-tune the measuring tools get
[export, tune_policy(missing = "fallback")]
def main() {
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "establish_baselines")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "establish_baselines")
        return
    }
    let tier = parse_tier(cfg.tier)
    let threads = cfg.threads > 0 ? cfg.threads : profile_threads()
    let suffix = cfg.ngl != 0 ? "_metal" : ""
    logger_set_name(prof_log_name())
    logger_info("profile.llm.baseline", "run start", JV({"box" => JV(box_name()), "tier" => JV(cfg.tier),
        "threads" => JV(threads), "affinity" => JV(affinity_on()), "ngl" => JV(cfg.ngl), "bench" => JV(llama_bench_bin(cfg.ngl))}))
    print("establishing LLM baselines: box={box_name()} tier={cfg.tier} threads={threads} affinity={affinity_on()} ngl={cfg.ngl}\n")
    print("log: {logger_get_path()}  (tail for live, timestamped progress)\n")
    print("NOTE: this runs llama.cpp llama-bench (a measurement) — Parsec must be off.\n\n")
    // The baseline is persisted after EVERY model (upsert into the loaded file, rewrite): a crash or
    // hang on any model never costs the ones already measured, and partial / repeat runs accumulate.
    var merged <- read_llm_baselines(suffix)
    var n = 0
    for (m in llm_catalog()) {
        if (!empty(cfg.only) && find(m.display, cfg.only) < 0) {
            continue
        }
        var sz = 0ul
        if (!model_selected(m, tier, sz)) {
            continue
        }
        let mb = int(sz / (1024ul * 1024ul))
        logger_info("profile.llm.baseline", "model start", JV({"model" => JV(m.display), "size_mb" => JV(mb)}))
        let t0 = ref_time_ticks()
        let b = bench_one(m.display, model_path(m), threads, cfg.nprompt, cfg.ngen, cfg.ngl)
        let ms = double(get_time_usec(t0)) / 1000.0lf
        if (b.prefill_tok_s < 0.0lf || b.emission_tok_s < 0.0lf) {
            logger_error("profile.llm.baseline", "model failed (llama-bench parse)", JV({"model" => JV(m.display), "elapsed_ms" => JV(ms)}))
        } else {
            logger_info("profile.llm.baseline", "model done", JV({"model" => JV(m.display), "elapsed_ms" => JV(ms),
                "prefill_tok_s" => JV(b.prefill_tok_s), "emission_tok_s" => JV(b.emission_tok_s), "lcpp" => JV(b.lcpp_commit)}))
        }
        upsert_baseline(merged, b)
        write_llm_baselines(merged, suffix)   // persist now — before moving to the next model
        n++
    }
    logger_info("profile.llm.baseline", "run done", JV({"measured" => JV(n), "rows" => JV(length(merged)), "path" => JV(baseline_llm_path(suffix))}))
    print("measured {n} model(s); baseline now {length(merged)} row(s) -> {baseline_llm_path(suffix)}\n")
}
