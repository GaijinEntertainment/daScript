options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — matches how llama.cpp is benched

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ + apply_box_profile_runtime (engine-level, not re-exported)
require dasllama/dasllama_image   // load_model_cached — the .dlim rail, without the full facade's compile weight
require ?das_accelerate dasllama/dasllama_math_accelerate   // nolint:STYLE030 — the --accel leg; Apple-only C++ module
require ../performance/profile_common.das   // profile_threads + affinity_on + bench records — the standing per-box methodology
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require daslib/md_boost
require math

// llama-bench mirror — EXACTLY llama.cpp's test shapes, rep counts, and timing boundaries, and
// NOTHING else. Two independent rows, each = one untimed warmup + -r timed reps, mean ± stdev:
//   pp: one batched forward_prefill of -p tokens from an empty cache per rep
//   tg: -n single-token forwards at pos 0.. per rep, feeding synthetic ids with NO logit read
//       (llama-bench never samples — sampling cost is excluded by design)
//   ours:   bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <model.gguf>
//   theirs: --ref <llama-bench> runs it on the same GGUF with the same -t/-p/-n/-r/--ngl
// `-o json` emits public bench records (PUBLIC_BENCH_PLAN.md) on stdout — progress goes to
// stderr then, like llama-bench itself. `--ngl` != 0 is the whole graph on Metal
// (MetalMode.required — a CPU fallback fails the run, never silently mismeasures).

enum private OutFmt {
    txt
    md
    json
}

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_short = "p"
    @clarg_doc = "Prompt tokens per pp rep (default: 512, llama-bench -p)"
    plen : int = 512

    @clarg_short = "n"
    @clarg_doc = "Generated tokens per tg rep (default: 128, llama-bench -n)"
    ngen : int = 128

    @clarg_short = "r"
    @clarg_doc = "Timed repetitions per row (default: 5, llama-bench -r)"
    reps : int = 5

    @clarg_short = "t"
    @clarg_doc = "Worker threads (default: the box profile; DAS_JOBQUE_THREADS still wins)"
    threads : int = 0

    @clarg_doc = "GPU layers, llama-bench spelling: 0 = CPU, anything else = Metal, required (default: 0)"
    ngl : int = 0

    @clarg_doc = "Arm the accelerate (+AMX/SME) float-batch tier for the das run — the 'cpu + AMX' board row (records flavor 'accel'; CPU only, fails where das_accelerate is not built)"
    accel : bool

    @clarg_short = "o"
    @clarg_doc = "Output format: txt, md (llama-bench-style table), json (public bench records) (default: txt)"
    output : OutFmt = OutFmt.txt

    @clarg_doc = "Path to a llama-bench binary — also bench llama.cpp on the same GGUF, same shapes"
    ref : string

    @clarg_name = "ref-flavor"
    @clarg_doc = "The --ref binary's build flavor tag for the record: stock | clean-cpu (default: stock)"
    ref_flavor : string = "stock"

    @clarg_name = "json-path"
    @clarg_doc = "With -o json: also write the records to this file — the machine rail (stdout carries engine logs too, and an untuned box's tune-relaunch merges stderr into it)"
    json_path : string

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_doc = "A/B the dense GPU rail in-process: every row runs twice, dense routing OFF then ON. Needs DASLLAMA_GPU_DENSE=1 (uploads + marks); both arms share one process/residency, so cross-run pp drift cancels"
    dense_ab : bool

    @clarg_doc = "A/B the device-side moe combine in-process: every row runs twice, combine OFF then ON (same drift-cancel rationale as --dense-ab; mutually exclusive with it)"
    comb_ab : bool

    @clarg_doc = "A/B the deltanet GPU chain in-process: every row runs twice, DN routing OFF then ON. Needs DASLLAMA_GPU_DN=1 (uploads + marks); mutually exclusive with the other A/B levers"
    dn_ab : bool

    @clarg_doc = "A/B the attention GPU chain in-process: every row runs twice, attention routing OFF then ON. Needs DASLLAMA_GPU_ATTN=1 (uploads + marks); mutually exclusive with the other A/B levers"
    attn_ab : bool

    @clarg_doc = "A/B the GPU MoE prefill arm in-process: every row runs twice, device-side prefill OFF then ON. Needs resident expert layers (DASLLAMA_GPU_MOE_LAYERS); decode has no npos gate so tg reads as a wash. Mutually exclusive with the other A/B levers"
    prefill_ab : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// deterministic synthetic ids (vocab-safe low range, the decode_prof convention); llama-bench
// feeds std::rand() — token content doesn't affect speed, determinism keeps runs comparable
def private synth_id(i, seed : int64) : int64 => 1000l + ((i * 17l + seed * 131l) % 5000l)

def private build_prompt(n : int64; seed : int64) : array<int64> {
    return <- [for (i in range64(n)); synth_id(i, seed)]
}

// progress line — stdout in txt mode, stderr when stdout carries the md table / json records
def private say(fmt : OutFmt; text : string) {
    if (fmt == OutFmt.txt) {
        print(text)
    } else {
        fwrite(fstderr(), text)
    }
}

struct private RowStat {
    test : string
    mean : double
    sd : double
}

def private row_stat(tag : string; tps : array<double>) : RowStat {
    var sum = 0.0lf
    for (v in tps) {
        sum += v
    }
    let n = length(tps)
    let mean = sum / double(n)
    var vsum = 0.0lf
    for (v in tps) {
        vsum += (v - mean) * (v - mean)
    }
    let sd = n > 1 ? double(sqrt(float(vsum / double(n - 1)))) : 0.0lf
    return RowStat(test = tag, mean = mean, sd = sd)
}

def private tests_of(rows : array<RowStat>) : table<string; BenchTest> {
    return <- { for (r in rows); r.test => BenchTest(tok_s = r.mean, stddev = r.sd) }
}

[export]
def main() {
    if (!jit_enabled()) {
        print("This benchmark is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "lcpp_bench")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "lcpp_bench")
        return
    }
    let gpu = cfg.ngl != 0
    if (!gpu) {
        allow_cpu_prefill()   // CPU-protocol bench by intent — disarm the Metal-build tripwire
    }
    let plen = max(int64(cfg.plen), 0l)
    let ngen = max(int64(cfg.ngen), 0l)
    let reps = max(cfg.reps, 1)
    if ((cfg.dense_ab ? 1 : 0) + (cfg.comb_ab ? 1 : 0) + (cfg.dn_ab ? 1 : 0) + (cfg.attn_ab ? 1 : 0) + (cfg.prefill_ab ? 1 : 0) > 1) {
        print("warning: --dense-ab / --comb-ab / --dn-ab / --attn-ab / --prefill-ab are mutually exclusive — running the first only\n")
    }
    let ab = cfg.dense_ab || cfg.comb_ab || cfg.dn_ab || cfg.attn_ab || cfg.prefill_ab
    var fmt = cfg.output
    if (ab && fmt != OutFmt.txt) {
        print("warning: -o {cfg.output} with an A/B lever — records cover the plain protocol only, falling back to txt\n")
        fmt = OutFmt.txt
    }
    // the standing per-box methodology, enforced here rather than left to the environment
    // (gen_profile's pattern): cap the future que's workers (computing main is the last lane) and
    // hard-pin lanes to distinct physical cores on the boxes that pin. -t is the explicit ask, but
    // DAS_JOBQUE_THREADS / DAS_JOBQUE_AFFINITY envs still win the que's actual lane count.
    let t_env = has_env_variable("DAS_JOBQUE_THREADS")
    var threads = cfg.threads > 0 ? cfg.threads : (t_env ? to_int(get_env_variable("DAS_JOBQUE_THREADS")) : profile_threads())
    if (t_env && to_int(get_env_variable("DAS_JOBQUE_THREADS")) != threads) {
        threads = to_int(get_env_variable("DAS_JOBQUE_THREADS"))
        say(fmt, "warning: DAS_JOBQUE_THREADS overrides -t — running {threads} lanes\n")
    } elif (!t_env) {
        set_jobque_threads_cap(max(threads - 1, 1))
    }
    let pinned = affinity_on() && !has_env_variable("DAS_JOBQUE_AFFINITY")
    if (pinned) {
        set_jobque_affinity(2)   // 2 = hard mask, the llama-bench --cpu-strict mirror (mode 1 re-rolls SMT placement)
    }
    apply_box_profile_runtime()   // the tuned per-box stack — what load_model users get
    if (cfg.accel && gpu) {
        print("--accel is a CPU-tier lever — drop it or run with -ngl 0\n")
        return
    }
    if (cfg.accel) {
        static_if (typeinfo builtin_module_exists(das_accelerate)) {
            dasllama_accelerate_enable(32l)   // the shipped +AMX defaults: min ntok 32, work floor 512 MMAC
        } else {
            print("--accel requested but das_accelerate is not built on this platform — no accel row to measure\n")
            return
        }
    }
    if (gpu) {
        set_metal_mode(MetalMode.required)   // before load_model — pins the portable backend, activates the metal overrides
    }
    if (cfg.dense_ab && !has_env_variable("DASLLAMA_GPU_DENSE")) {
        print("warning: --dense-ab without DASLLAMA_GPU_DENSE=1 — no dense marks, both arms identical\n")
    }
    if (cfg.dn_ab && !has_env_variable("DASLLAMA_GPU_DN")) {
        print("warning: --dn-ab without DASLLAMA_GPU_DN=1 — no deltanet marks, both arms identical\n")
    }
    if (cfg.attn_ab && !has_env_variable("DASLLAMA_GPU_ATTN")) {
        print("warning: --attn-ab without DASLLAMA_GPU_ATTN=1 — no attention marks, both arms identical\n")
    }
    if (cfg.prefill_ab && !has_env_variable("DASLLAMA_GPU_MOE_LAYERS") && !has_env_variable("DASLLAMA_GPU_MOE_STREAM")) {
        print("warning: --prefill-ab without DASLLAMA_GPU_MOE_LAYERS/_STREAM — no resident expert layers, both arms identical\n")
    }
    say(fmt, "loading {cfg.model}\n")
    var results : array<RowStat>
    var modelArch = ""
    var activeB = 0.0lf
    var execFmt = ""
    with_job_que() {
        setup_dasllama_jobque_()
        // load INSIDE the que, on the image rail: the .dlim maps in ms, and the GPU tier's
        // gather/upload parallelizes over the lanes (que-less it runs single-threaded — at
        // STREAM=35 that gather was the dominant per-run cost)
        var t <- load_model_cached(cfg.model, cfg.quant)
        t.config.seq_len = min(t.config.seq_len, max(plen, ngen) + 8l)   // cap the KV allocation
        let c = t.config
        say(fmt, "config: dim={c.dim} layers={c.n_layers} vocab={c.vocab_size} | backend {active_kernel_backend()} (batch {active_batch_backend()}) | t{threads} affinity {pinned ? "hard" : "env/off"} | pp{plen} tg{ngen} x {reps} reps\n")
        modelArch = "{t.arch}"
        activeB = active_params_b(t)
        execFmt = model_exec_fmt(t)
        var s <- make_run_state(c)
        for (arm in range(ab ? 2 : 1)) {
            var dtag = ""
            if (cfg.dense_ab) {
                dtag = arm == 0 ? " dense=off" : " dense=on"
                set_moe_gpu_dense_route(arm != 0)
            } elif (cfg.comb_ab) {
                dtag = arm == 0 ? " comb=off" : " comb=on"
                set_gpu_combine_route(arm != 0)
            } elif (cfg.dn_ab) {
                dtag = arm == 0 ? " dn=off" : " dn=on"
                set_moe_gpu_dn_route(arm != 0)
            } elif (cfg.attn_ab) {
                dtag = arm == 0 ? " attn=off" : " attn=on"
                set_moe_gpu_attn_route(arm != 0)
            } elif (cfg.prefill_ab) {
                dtag = arm == 0 ? " gpuprefill=off" : " gpuprefill=on"
                set_moe_gpu_prefill_route(arm != 0)
            }
            if (plen > 0l) {
                let warm <- build_prompt(plen, 999l)
                forward_prefill(t, s, warm, plen, 0l)   // warmup — untimed full-size run, as llama-bench does
                var tps : array<double>
                for (rep in range(reps)) {
                    let prompt <- build_prompt(plen, int64(rep))
                    let t0 = ref_time_ticks()
                    forward_prefill(t, s, prompt, plen, 0l)
                    let us = get_time_usec(t0)
                    tps |> push(double(plen) * 1.0e6lf / double(us))
                }
                let st = row_stat("pp{plen}{dtag}", tps)
                say(fmt, "{st.test}: {st.mean:.2f} ± {st.sd:.2f} tok/s ({reps} reps)\n")
                if (has_env_variable("DASLLAMA_BENCH_VERBOSE")) {
                    say(fmt, "  reps: {tps}\n")
                }
                results |> push(st)
                delete tps
            }
            if (ngen > 0l) {
                forward(t, s, synth_id(0l, 999l), 0l)   // warmup — one untimed token, llama-bench's test_gen(ctx, 1)
                var tps : array<double>
                for (rep in range(reps)) {
                    let t0 = ref_time_ticks()
                    for (i in range64(ngen)) {
                        forward(t, s, synth_id(i, int64(rep)), i)
                    }
                    let us = get_time_usec(t0)
                    tps |> push(double(ngen) * 1.0e6lf / double(us))
                }
                let st = row_stat("tg{ngen}{dtag}", tps)
                say(fmt, "{st.test}: {st.mean:.2f} ± {st.sd:.2f} tok/s ({reps} reps)\n")
                if (has_env_variable("DASLLAMA_BENCH_VERBOSE")) {
                    say(fmt, "  reps: {tps}\n")
                }
                results |> push(st)
                delete tps
            }
        }
        delete s
        delete t
    }
    // the reference runs AFTER our model is freed — one model process at a time, per the protocol
    var refRows : array<LlamaBenchRow>
    var refCmd = ""
    if (!empty(cfg.ref)) {
        let aff = (!gpu && affinity_on()) ? lcpp_affinity_args(threads) : ""
        refCmd = "\"{cfg.ref}\" -m \"{cfg.model}\" -ngl {cfg.ngl} -t {threads}{aff} -p {cfg.plen} -n {cfg.ngen} -r {reps} -o json"
        say(fmt, "ref: {refCmd}\n")
        if (!run_llama_bench(refCmd, refRows)) {
            say(fmt, "warning: llama-bench run/parse failed — no ref rows\n")
        }
        for (rr in refRows) {
            let tag = rr.n_gen == 0 ? "pp{rr.n_prompt}" : "tg{rr.n_gen}"
            say(fmt, "ref {tag}: {rr.avg_ts:.2f} tok/s\n")
        }
    }
    if (fmt == OutFmt.txt) {
        return
    }
    let backend = gpu ? "metal" : "cpu"
    let gguf = base_name(cfg.model)
    if (fmt == OutFmt.md) {
        let headers <- ["engine", "model", "backend", "ngl", "threads", "test", "t/s"]
        let aligns <- [ColAlign.left, ColAlign.left, ColAlign.left, ColAlign.right, ColAlign.right, ColAlign.left, ColAlign.right]
        var rows : array<array<string>>
        rows |> reserve(length(results) + length(refRows))
        for (st in results) {
            var row <- ["das", gguf, backend, "{cfg.ngl}", "{threads}", st.test, "{st.mean:.2f} ± {st.sd:.2f}"]
            rows |> emplace(row)
        }
        for (rr in refRows) {
            let tag = rr.n_gen == 0 ? "pp{rr.n_prompt}" : "tg{rr.n_gen}"
            let sd = rr.stddev_ts
            var row <- ["llama.cpp", gguf, backend, "{cfg.ngl}", "{threads}", tag, "{rr.avg_ts:.2f} ± {sd:.2f}"]
            rows |> emplace(row)
        }
        print(render_md_table(headers, aligns, rows))
        return
    }
    // -o json: the public bench records — one BenchModel, das run + optional llama.cpp sibling
    let hw = gather_hardware()
    let date = format_time(get_clock(), "%Y-%m-%d")
    let box = box_name()
    var m = BenchModel(
        gguf = gguf,
        arch = modelArch,
        quant = quant_str(cfg.quant),
        active_b = activeB,
        size_bytes = int64(stat(cfg.model).size))
    if (!empty(refRows)) {
        m.params_b = double(refRows[0].model_n_params) / 1.0e9lf
    }
    let argstr = join(get_cli_arguments(), " ")
    var dr = BenchRun(
        engine = "das",
        backend = backend,
        flavor = cfg.accel ? "accel" : "tuned",
        box = box,
        threads = threads,
        date = date,
        cmd = "bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- {argstr}",
        sha = run_capture("git rev-parse --short HEAD", true),
        version = das_version(),
        tune = tune_summary(),
        exec_fmt = execFmt,
        hardware = hw,
        tests <- tests_of(results))
    m.runs |> emplace(dr)
    if (!empty(refRows)) {
        var rr = BenchRun(
            engine = "llama.cpp",
            backend = backend,
            flavor = cfg.ref_flavor,
            box = box,
            threads = threads,
            date = date,
            cmd = refCmd,
            sha = refRows[0].build_commit,
            exec_fmt = lcpp_exec_fmt(backend),
            hardware = hw)
        for (row in refRows) {
            let tag = row.n_gen == 0 ? "pp{row.n_prompt}" : "tg{row.n_gen}"
            rr.tests |> insert(tag, BenchTest(tok_s = row.avg_ts, stddev = row.stddev_ts))
        }
        m.runs |> emplace(rr)
    }
    var models : array<BenchModel>
    models |> emplace(m)
    let text = sprint_json(models, true)
    if (!empty(cfg.json_path)) {
        var wrote = false
        fopen(cfg.json_path, "wb") $(f) {
            if (f != null) {
                fwrite(f, text)
                wrote = true
            }
        }
        if (wrote) {
            say(fmt, "records written to {cfg.json_path}\n")
        } else {
            say(fmt, "warning: could not open {cfg.json_path} for writing — no records file\n")
        }
    }
    print("{text}\n")
}
