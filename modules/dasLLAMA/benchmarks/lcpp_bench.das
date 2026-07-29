options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — matches how llama.cpp is benched

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ + apply_box_profile_runtime (engine-level, not re-exported)
require dasllama/dasllama_image   // load_model_cached — the .dlim rail, without the full facade's compile weight
require dasllama/dasllama_tokenizer   // the tg-real row needs REAL tokens (see Lever.real)
require daslib/module_path            // get_this_module_dir — the default prompt corpus beside this bench
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

    @clarg_name = "mtp-ab"
    @clarg_doc = "A/B MTP/NextN self-speculation in-process, OFF then ON (needs a NextN head in the GGUF). Runs the tg-real row, NOT the llama-bench tg row — acceptance is meaningless on synthetic ids. Reports the draft accept rate; mutually exclusive with the other A/B levers"
    mtp_ab : bool

    @clarg_doc = "Prompt corpus for the tg-real row (segments split on lines of two percent signs; default: data/wikitext2_prompts.txt beside this bench)"
    prompts : string = ""

    @clarg_doc = "After each tg-real arm's timed reps, run one UNTIMED window with forward_profile buckets — the per-section time split of that arm. With --mtp-ab this is what attributes draft vs verify vs classifier"
    prof : bool

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

// One in-process A/B lever: both arms run under a SINGLE model load, so cross-run drift cancels and
// the pair is directly comparable. Arms ride the test name (" dense=off" / " dense=on"), which keeps
// both inside one BenchRun and leaves the (box, engine, backend, flavor) upsert identity intact.
// Adding a lever — including one pulled over from an archived bench — is one row in levers_of().
struct private Lever {
    flag : string    // the CLI spelling, for diagnostics: --<flag>-ab
    tag : string     // the row suffix key
    route : function<(on : bool) : void>
    env : string     // env that must arm the uploads first; "" when the lever needs none
    real : bool      // this lever is only meaningful under REAL greedy continuation, so it runs the
                     // tg-real row instead of the llama-bench pp/tg rows (synthetic ids would
                     // measure something the lever cannot affect and read as a valid result)
}

def private levers_of(cfg : Args) : array<Lever> {
    var out : array<Lever>
    if (cfg.dense_ab) {
        out |> emplace(Lever(flag = "dense", tag = "dense", route = @@set_moe_gpu_dense_route,
                             env = "DASLLAMA_GPU_DENSE"))
    }
    if (cfg.comb_ab) {
        out |> emplace(Lever(flag = "comb", tag = "comb", route = @@set_gpu_combine_route, env = ""))
    }
    if (cfg.dn_ab) {
        out |> emplace(Lever(flag = "dn", tag = "dn", route = @@set_moe_gpu_dn_route,
                             env = "DASLLAMA_GPU_DN"))
    }
    if (cfg.attn_ab) {
        out |> emplace(Lever(flag = "attn", tag = "attn", route = @@set_moe_gpu_attn_route,
                             env = "DASLLAMA_GPU_ATTN"))
    }
    if (cfg.prefill_ab) {
        out |> emplace(Lever(flag = "prefill", tag = "gpuprefill", route = @@set_moe_gpu_prefill_route,
                             env = "DASLLAMA_GPU_MOE_LAYERS"))
    }
    if (cfg.mtp_ab) {
        // set_mtp_spec must be live BEFORE the prefill — it warms the draft head's KV
        out |> emplace(Lever(flag = "mtp", tag = "mtp", route = @@set_mtp_spec, env = "", real = true))
    }
    return <- out
}

// Greedy pick over the last logits — the tg-real row's sampler. Runs INSIDE the timed window, but
// both A/B arms pay it identically so it cancels for judging.
def private argmax_logit(s : Session; vocab : int64) : int64 {
    var best = 0l
    var bv = s.logits[0]
    for (v in range64(vocab)) {
        if (s.logits[v] > bv) {
            bv = s.logits[v]
            best = v
        }
    }
    return best
}

// Prompt corpus: segments separated by lines of two percent signs, '#' segments skipped.
def private load_corpus(path : string) : array<string> {
    var texts : array<string>
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("lcpp_bench: can't open prompt corpus {path}")
        }
        for (p in split(fread(f), "%%")) {
            let tp = strip(p)
            if (!empty(tp) && !starts_with(tp, "#")) {
                texts |> push(clone_string(tp))
            }
        }
    }
    return <- texts
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
    var levers <- levers_of(cfg)
    if (length(levers) > 1) {
        var names <- [for (l in levers); "--{l.flag}-ab"]
        print("warning: A/B levers are mutually exclusive ({join(names, " ")}) — running --{levers[0].flag}-ab only\n")
        delete names
        levers |> resize(1)
    }
    let ab = !empty(levers)
    let real_row = ab && levers[0].real
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
    if (!tune_gate()) {   // never measure on fallback winners — the whole board would sandbag
        return
    }
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
    // an unarmed lever silently measures the SAME thing twice — the one A/B failure that looks
    // like a real result, so it warns rather than being left to the reader
    if (ab && !empty(levers[0].env) && !has_env_variable(levers[0].env)
            && !(cfg.prefill_ab && has_env_variable("DASLLAMA_GPU_MOE_STREAM"))) {
        print("warning: --{levers[0].flag}-ab without {levers[0].env}=1 — nothing armed, both arms identical\n")
    }
    // -m accepts a comma-separated list: one process benches every model in turn, so a sweep
    // pays the compile/startup cost once; a single -m path behaves exactly as before
    // trim: `-m "a.gguf, b.gguf"` is the natural spelling, and an untrimmed path fails to open
    var model_list <- [for (m in split(cfg.model, ",")); strip(m)]
    var mdRows : array<array<string>>
    var jsonModels : array<BenchModel>
    with_job_que() {
        setup_dasllama_jobque_()
        for (mpath in model_list) {
        continue if (empty(mpath))
        say(fmt, "loading {mpath}\n")
        var results : array<RowStat>
        var modelArch = ""
        var activeB = 0.0lf
        var execFmt = ""
        // load INSIDE the que, on the image rail: the .dlim maps in ms, and the GPU tier's
        // gather/upload parallelizes over the lanes (que-less it runs single-threaded — at
        // STREAM=35 that gather was the dominant per-run cost)
        var t <- load_model_cached(mpath, cfg.quant)
        // the tg-real row continues REAL prompts, so its KV must cover the longest one + the window
        var inscope toks : array<array<int64>>
        if (real_row) {
            var tk <- load_tokenizer_auto(mpath)
            var texts <- load_corpus(empty(cfg.prompts)
                ? path_join(get_this_module_dir(), "data/wikitext2_prompts.txt") : cfg.prompts)
            var maxlen = 0l
            for (tx in texts) {
                var ids <- encode(tk, tx, false)
                maxlen = max(maxlen, long_length(ids))
                toks |> emplace(ids)
            }
            delete texts
            delete tk
            if (empty(toks)) {
                panic("lcpp_bench: prompt corpus tokenized to nothing — --{levers[0].flag}-ab needs real prompts")
            }
            t.config.seq_len = min(t.config.seq_len, maxlen + ngen + 8l)
        } else {
            t.config.seq_len = min(t.config.seq_len, max(plen, ngen) + 8l)   // cap the KV allocation
        }
        let c = t.config
        // a model with no NextN head silently no-ops set_mtp_spec — both arms would be identical
        // and the row would read as a valid 1.00x result. Fail closed instead.
        if (cfg.mtp_ab && c.n_layer_nextn <= 0l) {
            panic("lcpp_bench: --mtp-ab needs a NextN draft head, and {mpath} has none")
        }
        say(fmt, "config: dim={c.dim} layers={c.n_layers} vocab={c.vocab_size} | backend {active_kernel_backend()} (batch {active_batch_backend()}) | t{threads} affinity {pinned ? "hard" : "env/off"} | pp{plen} tg{ngen} x {reps} reps\n")
        modelArch = "{t.arch}"
        activeB = active_params_b(t)
        execFmt = model_exec_fmt(t)
        var s <- make_run_state(c)
        for (arm in range(ab ? 2 : 1)) {
            var dtag = ""
            if (ab) {
                dtag = " {levers[0].tag}={arm == 0 ? "off" : "on"}"
                invoke(levers[0].route, arm != 0)
            }
            if (real_row) {
                // REAL greedy continuation: fresh session per prompt (no KV/recurrent bleed),
                // prefill untimed as llama-bench excludes the prompt, argmax inside the timed loop
                // (both arms pay it, so it cancels). NOT comparable to the synthetic tg row.
                let mtp_on = cfg.mtp_ab && arm != 0
                var drafted = 0l
                var accepted = 0l
                var tps : array<double>
                for (_rep in range(reps)) {
                    var total_us = 0l
                    var rep_gen = 0l
                    for (pi in range(length(toks))) {
                        var rs <- make_run_state(c)
                        let pl = long_length(toks[pi])
                        forward_prefill(t, rs, toks[pi], pl, 0l)
                        let t0 = ref_time_ticks()
                        if (mtp_on) {
                            rs.n_past = pl   // forward_prefill leaves n_past alone; mtp_spec_eval drives it
                            var tok = argmax_logit(rs, c.vocab_size)
                            var gen = 0l
                            while (gen < ngen) {
                                var acc = 0l   // the id is unused — only the 1-vs-2 advance is timed
                                gen += mtp_spec_eval(t, rs, tok, acc) ? 2l : 1l
                                tok = argmax_logit(rs, c.vocab_size)
                            }
                            rep_gen += gen   // a trailing accept may overshoot by 1 — count what ran
                        } else {
                            var pos = pl
                            for (_g in range64(ngen)) {
                                forward(t, rs, argmax_logit(rs, c.vocab_size), pos)
                                pos++
                            }
                            rep_gen += ngen
                        }
                        total_us += int64(get_time_usec(t0))
                        drafted += rs.mtp_drafted   // session-lifetime counters — harvest before the free
                        accepted += rs.mtp_accepted
                        delete rs
                    }
                    tps |> push(double(rep_gen) * 1.0e6lf / double(total_us))
                }
                let st = row_stat("tg-real{ngen}{dtag}", tps)
                say(fmt, "{st.test}: {st.mean:.2f} ± {st.sd:.2f} tok/s ({reps} reps, {length(toks)} prompts)\n")
                if (mtp_on) {
                    let rate = drafted > 0l ? double(accepted) * 100.0lf / double(drafted) : 0.0lf
                    say(fmt, "  mtp: {accepted}/{drafted} drafts accepted ({rate:.1f}%)\n")
                }
                results |> push(st)
                delete tps
                if (cfg.prof) {
                    // untimed bucket window for THIS arm — the prefill lands BEFORE the reset, so
                    // the report is pure decode (or pure draft+verify on the on arm). Running it
                    // per arm is the point: the off/on bucket delta is what attributes the draft.
                    var ps <- make_run_state(c)
                    let pl0 = long_length(toks[0])
                    forward_prefill(t, ps, toks[0], pl0, 0l)
                    forward_profile_reset()
                    if (mtp_on) {
                        ps.n_past = pl0
                        var ptok = argmax_logit(ps, c.vocab_size)
                        var pgen = 0l
                        while (pgen < ngen) {
                            var pacc = 0l
                            pgen += mtp_spec_eval(t, ps, ptok, pacc) ? 2l : 1l
                            ptok = argmax_logit(ps, c.vocab_size)
                        }
                    } else {
                        var ppos = pl0
                        for (_g in range64(ngen)) {
                            forward(t, ps, argmax_logit(ps, c.vocab_size), ppos)
                            ppos++
                        }
                    }
                    say(fmt, "--- forward_profile: {st.test} ---\n")
                    forward_profile_report()
                    delete ps
                }
                continue
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
        // the reference runs AFTER our model is freed — one model in memory at a time
        var refRows : array<LlamaBenchRow>
        var refCmd = ""
        if (!empty(cfg.ref)) {
            var refArgs <- [cfg.ref, "-m", mpath, "-ngl", "{cfg.ngl}", "-t", "{threads}"]
            if (!gpu) {
                refArgs |> push_from(lcpp_affinity_argv(threads))
            }
            refArgs |> push_from(["-p", "{cfg.plen}", "-n", "{cfg.ngen}", "-r", "{reps}", "-o", "json"])
            refCmd = argv_line(refArgs)
            say(fmt, "ref: {refCmd}\n")
            if (!run_llama_bench(refArgs, refRows)) {
                say(fmt, "warning: llama-bench run/parse failed — no ref rows\n")
            }
            for (rr in refRows) {
                let tag = rr.n_gen == 0 ? "pp{rr.n_prompt}" : "tg{rr.n_gen}"
                say(fmt, "ref {tag}: {rr.avg_ts:.2f} tok/s\n")
            }
        }
        if (fmt == OutFmt.txt) {
            delete results
            delete refRows
            continue
        }
        let backend = gpu ? "metal" : "cpu"
        let gguf = base_name(mpath)
        if (fmt == OutFmt.md) {
            for (st in results) {
                var row <- ["das", gguf, backend, "{cfg.ngl}", "{threads}", st.test, "{st.mean:.2f} ± {st.sd:.2f}"]
                mdRows |> emplace(row)
            }
            for (rr in refRows) {
                let tag = rr.n_gen == 0 ? "pp{rr.n_prompt}" : "tg{rr.n_gen}"
                let sd = rr.stddev_ts
                var row <- ["llama.cpp", gguf, backend, "{cfg.ngl}", "{threads}", tag, "{rr.avg_ts:.2f} ± {sd:.2f}"]
                mdRows |> emplace(row)
            }
            delete results
            delete refRows
            continue
        }
    // -o json: the public bench records — one BenchModel per gguf, das run + optional llama.cpp sibling
    let hw = gather_hardware()
    let date = format_time(get_clock(), "%Y-%m-%d")
    let box = box_name()
    var m = BenchModel(
        gguf = gguf,
        arch = modelArch,
        quant = quant_str(cfg.quant),
        active_b = activeB,
        size_bytes = int64(stat(mpath).size))
    if (!empty(refRows)) {
        m.params_b = double(refRows[0].model_n_params) / 1.0e9lf
    }
    var dr = BenchRun(
        engine = "das",
        backend = backend,
        flavor = cfg.accel ? "accel" : "tuned",
        box = box,
        threads = threads,
        date = date,
        cmd = bench_cmd_line(),
        env = bench_env_line(),
        sha = run_capture("git rev-parse --short=9 HEAD", true),
        version = das_version(),
        tune = tune_summary(),
        exec_fmt = execFmt,
        hardware = hw,
        tests <- tests_of(results))
    dr.files |> emplace(file_identity(mpath, "weights"))
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
        rr.files |> emplace(file_identity(mpath, "weights"))
        for (row in refRows) {
            let tag = row.n_gen == 0 ? "pp{row.n_prompt}" : "tg{row.n_gen}"
            rr.tests |> insert(tag, BenchTest(tok_s = row.avg_ts, stddev = row.stddev_ts))
        }
        m.runs |> emplace(rr)
    }
    jsonModels |> emplace(m)
    delete results
    delete refRows
    }   // model loop
    }   // with_job_que
    if (fmt == OutFmt.md) {
        let headers <- ["engine", "model", "backend", "ngl", "threads", "test", "t/s"]
        let aligns <- [ColAlign.left, ColAlign.left, ColAlign.left, ColAlign.right, ColAlign.right, ColAlign.left, ColAlign.right]
        print(render_md_table(headers, aligns, mdRows))
        return
    }
    if (fmt != OutFmt.json) {
        return
    }
    let text = sprint_json(jsonModels, true)
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
