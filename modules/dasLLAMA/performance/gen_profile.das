options gen2
options stack = 524288   // the forward/prefill chain needs more than the 16K default; the 122B/Coder-Next hybrid eval frames outgrew the old 64K (scratch probes needed 512K)
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); the profile is a speed measurement

// The das-side half of the profiling suite: for each tier-selected model it measures prefill (one
// batched forward_prefill) and emission (a greedy decode loop) tok/s under the tuned per-box runtime,
// pulls the matching llama.cpp reference from the committed baseline TSV (never re-run here — see
// establish_baselines.das), and MERGES the row into the persistent per-box profile JSON. Partial runs
// (small now, large later) merge into the same file. JIT-only, Parsec off.
//   bin/daslang -jit modules/dasLLAMA/performance/gen_profile.das -- -t small

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math          // setup_dasllama_jobque_ + apply_box_profile_runtime knobs (engine-level, not re-exported)
require dasllama/dasllama_tune          // box_profile_path (the applied per-box manifest, recorded in the platform block)
require llvm/daslib/llvm_tune           // [tune_policy] + log_tune_status — the box tunes itself before measuring
require daslib/jobque_boost
require profile_common
require daslib/clargs
require daslib/fio
require math

[CommandLineArgs]
struct Args {
    @clarg_short = "t"
    @clarg_doc = "Tier: small | large | all (default small)"
    tier : string = "small"

    @clarg_doc = "Dispatch lanes: worker cap + computing main (0 = per-box default: 16, m1 8)"
    threads : int = 0

    @clarg_doc = "Parsec was off for this run — recorded in the platform block (default true)"
    parsec_off : bool = true

    @clarg_short = "o"
    @clarg_doc = "Only models whose display name contains this substring (default: whole tier)"
    only : string = ""

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private build_prompt(n : int64) : array<int64> {
    return <- [for (i in range64(n)); 1000l + (i % 5000l)]
}

def private greedy_top(s : Session; vocab : int64) : int64 {
    var best = 0l
    var bestv = s.logits[0]
    for (i in range64(vocab)) {
        if (s.logits[i] > bestv) {
            bestv = s.logits[i]
            best = i
        }
    }
    return best
}

// Measure prefill (one batched forward over `prefill_ctx` synthetic tokens) and emission (a greedy
// decode of `emission_ctx` tokens from position 0) tok/s for one loaded model, both from a warmed
// JIT + live jobque. Token content doesn't affect speed, so synthetic ids are fine.
def private measure(var t : Model; prefill_ctx, emission_ctx : int) : tuple<prefill : double; emission : double> {
    // F16 KV — the create_session facade default; llama-bench's KV default is F16 too, so the
    // reference rows in the baseline TSV are measured at the same codec.
    var s <- make_run_state(t.config, KVDtype.f16, KVDtype.f16)
    var pf = 0.0lf
    var em = 0.0lf
    with_job_que() {
        setup_dasllama_jobque_()
        var warm <- build_prompt(8l)
        for (p in range64(8l)) {
            forward(t, s, warm[p], p)
        }
        forward_prefill(t, s, warm, 8l, 0l)
        delete warm
        // PREFILL — one batched forward_prefill (matches llama-bench pp<N>)
        let np = int64(prefill_ctx)
        var prompt <- build_prompt(np)
        let t0 = ref_time_ticks()
        forward_prefill(t, s, prompt, np, 0l)
        let us_pf = get_time_usec(t0)
        pf = double(np) / (double(us_pf) * 1.0e-6lf)
        delete prompt
        // EMISSION — greedy decode from position 0 (matches llama-bench tg<N>)
        let ng = int64(emission_ctx)
        var tok = 1l
        let t1 = ref_time_ticks()
        for (i in range64(ng)) {
            forward(t, s, tok, i)
            tok = greedy_top(s, t.config.vocab_size)
        }
        let us_em = get_time_usec(t1)
        em = double(ng) / (double(us_em) * 1.0e-6lf)
    }
    delete s   // free the run-state (KV cache) — measure() runs once per model
    return (prefill = pf, emission = em)
}

// The applied per-box profile filename (or "default" when none is on disk) for the platform block.
def private box_profile_tag() : string {
    let bp = box_profile_path()
    return stat(bp).is_valid ? base_name(bp) : "default"
}

// Pull the reference tok/s for `display` from the established baselines. Non-positive (unmeasured)
// stays 0.0 — @optional in the JSON means the reference cell is then simply absent.
def private ref_for(baselines : array<LlmBaseline>; display : string; var prefill, emission : double&) {
    prefill = 0.0lf
    emission = 0.0lf
    for (b in baselines) {
        if (b.model == display) {
            prefill = max(b.prefill_tok_s, 0.0lf)    // unmeasured (-1) collapses to 0 => absent in JSON
            emission = max(b.emission_tok_s, 0.0lf)
            return
        }
    }
}

// tune_policy: measuring an untuned box silently publishes fallback-tier numbers (it cost a
// day to notice). missing="auto" runs the scope's tuner and re-execs with the winners stamped;
// set DASLLAMA_CONFIRM_MODEL=<q8 gguf> so a divergent crown must prove itself in real prefill.
[export, tune_policy(missing = "auto")]
def main() {
    allow_cpu_prefill()   // CPU rows are this tool's product — disarm the Metal-build tripwire
    if (!jit_enabled()) {
        print("gen_profile is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "gen_profile")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "gen_profile")
        return
    }
    let tier = parse_tier(cfg.tier)
    let threads = cfg.threads > 0 ? cfg.threads : profile_threads()

    // 16-cores-with-affinity is the standing methodology, enforced here rather than left to the
    // environment: cap the future que's workers to threads-1 (the computing main is the last lane)
    // and pin lanes to distinct physical cores. Explicit DAS_JOBQUE_THREADS / DAS_JOBQUE_AFFINITY
    // envs still win (the A/B rail). Must run before any with_job_que — applied at worker spawn.
    if (!has_env_variable("DAS_JOBQUE_THREADS")) {
        set_jobque_threads_cap(max(threads - 1, 1))
    }
    if (affinity_on() && !has_env_variable("DAS_JOBQUE_AFFINITY")) {
        // 2 = HARD mask, mirroring llama-bench --cpu-strict. Mode 1 (hint) re-rolls the placement
        // lottery on every fresh que: lanes migrate onto SMT siblings and lockstep prefill goes
        // bimodal (~halved) while memory-bound decode barely notices — measured on the 3990X.
        set_jobque_affinity(2)
    }

    // reference baselines (lcpp tok/s + commit) — established once per box, never re-run here
    let baselines <- read_llm_baselines()
    var lcpp_commit = ""
    for (b in baselines) {
        if (!empty(b.lcpp_commit)) {
            lcpp_commit = b.lcpp_commit
            break
        }
    }

    // load-or-start the persistent per-box profile (the merge target)
    var prof : LlmProfile
    read_llm_profile(prof)
    let date = format_time(get_clock(), "%Y-%m-%d")   // portable (no shell) — Windows cmd `date` prompts and hangs
    prof.platform = gather_platform(threads, lcpp_commit, date, box_profile_tag(), cfg.parsec_off)

    apply_box_profile_runtime()   // the tuned per-box stack (backend pin + dispatch knobs)
    log_tune_status("gen_profile")

    logger_set_name(prof_log_name())
    // The das side runs on the jobque, NOT llama-bench's -t: the worker cap + affinity were declared
    // above, but probe the ACTUAL lane count, record it (not the advisory flag), and warn on mismatch
    // (an explicit DAS_JOBQUE_THREADS still redirects the run).
    var lanes = 0
    with_job_que() {
        setup_dasllama_jobque_()
        lanes = get_dispatch_lanes()
    }
    prof.platform.threads = lanes
    // effective das affinity mode (env rail wins over the declared default) — recorded in the profile
    var aff = max(get_jobque_affinity(), 0)
    if (has_env_variable("DAS_JOBQUE_AFFINITY")) {
        aff = to_int(get_env_variable("DAS_JOBQUE_AFFINITY"))
    }
    if (aff > 0) {
        let lcpp_aff = argv_line(lcpp_affinity_argv(threads))
        prof.platform.affinity = empty(lcpp_aff) ? "das jobque mode {aff}" : "das jobque mode {aff}; llama-bench {lcpp_aff}"
    }
    logger_info("profile.llm.gen", "run start", JV({"box" => JV(box_name()), "tier" => JV(cfg.tier),
        "requested_threads" => JV(threads), "dispatch_lanes" => JV(lanes), "affinity" => JV(aff), "lcpp" => JV(lcpp_commit)}))
    if (lanes != threads) {
        logger_warning("profile.llm.gen", "dispatch lanes != requested threads",
            JV({"dispatch_lanes" => JV(lanes), "requested_threads" => JV(threads)}))
        print("WARNING: das dispatch lanes = {lanes}, requested {threads}.\n")
    }
    print("gen_profile: box={box_name()} tier={cfg.tier} threads={threads} das_lanes={lanes} affinity={aff}\n")
    print("log: {logger_get_path()}  (tail for live, timestamped progress)\n")
    print("baselines: {length(baselines)} row(s) from {baseline_llm_path()} (lcpp {lcpp_commit})\n\n")
    for (m in llm_catalog()) {
        if (!empty(cfg.only) && find(m.display, cfg.only) < 0) {
            continue
        }
        var sz = 0ul
        if (!model_selected(m, tier, sz)) {
            continue
        }
        logger_info("profile.llm.gen", "model start", JV({"model" => JV(m.display), "size_mb" => JV(int(sz / (1024ul * 1024ul)))}))
        let t0 = ref_time_ticks()
        var t <- load_gguf(model_path(m), m.quant)
        t.config.seq_len = min(t.config.seq_len, 2048l)   // cap KV; both phases fit under this
        let meas = measure(t, m.prefill_ctx, m.emission_ctx)
        let ms = double(get_time_usec(t0)) / 1000.0lf
        var pf_ref = 0.0lf
        var em_ref = 0.0lf
        ref_for(baselines, m.display, pf_ref, em_ref)
        let rec = LlmRec(
            model = m.display,
            arch = t.arch,
            quant = model_quant_str(m),
            active_b = active_params_b(t),
            prefill = PhaseRec(context = m.prefill_ctx, das_tok_s = meas.prefill, llama_cpp_tok_s = pf_ref),
            emission = PhaseRec(context = m.emission_ctx, das_tok_s = meas.emission, llama_cpp_tok_s = em_ref))
        upsert_llm(prof.tests, rec)
        write_llm_profile(prof)   // persist now — a crash on a later model never costs this one
        logger_info("profile.llm.gen", "model done", JV({"model" => JV(m.display), "elapsed_ms" => JV(ms),
            "das_prefill_tok_s" => JV(meas.prefill), "das_emission_tok_s" => JV(meas.emission),
            "lcpp_prefill_tok_s" => JV(pf_ref), "lcpp_emission_tok_s" => JV(em_ref)}))
        delete t   // free the model's weights before the next one (else all models stay resident)
    }
    logger_info("profile.llm.gen", "run done", JV({"profiled" => JV(length(prof.tests)), "path" => JV(profile_llm_path())}))
    print("profiled {length(prof.tests)} model row(s) -> {profile_llm_path()}\n")
}
