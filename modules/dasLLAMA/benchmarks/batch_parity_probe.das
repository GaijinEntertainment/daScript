options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — the CPU truth half only

require dasllama/dasllama   // the facade: arch [init] registrations + load_model's prepared-image (.dlim) cache
require dasllama/dasllama_math   // setup_dasllama_jobque_ + pin_kernel_backend + parallel_argmax
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require daslib/strings_boost
require daslib/strings_convert
require strings
require math
require dasllama/dasllama_env

// The batched-decode NUMERIC parity probe — the teacher-forced instrument that caught the 3d
// V-copy/v_norm race (Δ15-22 where the noise floor is ~0.5). CPU truth runs greedy and records
// its feeds; the metal pass REPLAYS those feeds (identical inputs, so a near-tie flip cannot
// fork the sequences), and the final-step logits compare within --tol. Prompts are tokenizer-
// encoded counting text at UNEVEN per-stream depths — random-id prompts push the state far
// off-distribution and inflate the noise floor ~20x (measured 8.9 vs 0.4 on gemma2), burying
// the signal; in-distribution prompts keep the floor at ~0.5 so a real defect (the race was
// 15-22) stands clear. The CPU truth is the expensive half (minutes at 12B) — --truth caches
// it to disk ONCE (per model!) and every later A/B rerun of the metal side loads it.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/batch_parity_probe.das -- \
//       -m <model.gguf> [--bs 1,2,4] [-n 4] [--tol 8.0] [--truth /path/per-model-prefix]

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path (maps the prepared .dlim image when present)"
    model : string

    @clarg_doc = "Comma list of batch sizes; 1 = the single-stream driver (the noise-floor reference)"
    bs : string = "1,2,4"

    @clarg_short = "n"
    @clarg_doc = "Teacher-forced decode steps (default: 4)"
    steps : int = 4

    @clarg_doc = "Max |dlogit| gate (default: 8.0 — the gemma4 cont-cell tolerance)"
    tol : float = 8.0

    @clarg_short = "t"
    @clarg_doc = "CPU-truth cache path prefix — computed and SAVED once, loaded on every rerun (default: no cache)"
    truth : string = ""

    @clarg_doc = "Compute and save the CPU truths, skip the Metal replay — mint truths UNPINNED (fast arm64-gen backend, ~3-5x quicker at 12B); the pinned-portable replay run then loads the cache"
    truth_only : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// stream i counts from start_i for 30 + 7*i numbers — uneven depths, in-distribution content
def private stream_prompt(t : Model; i : int64) : array<int64> {
    let start = [1, 7, 3, 11, 5, 9, 2, 8][int(i % 8l)] + int(i / 8l)
    let last = start + 29 + int(i) * 7
    let text = build_string() $(var w) {
        for (v in range(start, last + 1)) {
            w |> write(v)
            w |> write(v < last ? ", " : ",")
        }
    }
    return <- encode(t, text, true, false)
}

def private greedy_top(lg : array<float>; vocab : int64) : int64 {
    return parallel_argmax(lg, vocab)
}

// greedy when `feeds` is empty (recording the fed token per stream per step into feeds_out);
// replay otherwise. B == 1 runs the single-stream driver (forward), B > 1 the batch driver.
// Returns the final-step logits, streams concatenated.
def private teacher_logits(t : Model; nb, nsteps : int64; feeds : array<int64>; var feeds_out : array<int64>) : array<float> {
    let c = t.config
    let vocab = c.vocab_size
    let replay = !empty(feeds)
    var out : array<float>
    out |> reserve(vocab * nb)
    if (nb == 1l) {
        var s = make_run_state(c, KVDtype.f16, KVDtype.f16)
        var prompt <- stream_prompt(t, 0l)
        eval_(t, s, prompt)
        delete prompt
        for (st in range64(nsteps)) {
            let tok = replay ? feeds[st] : greedy_top(s.logits, vocab)
            if (!replay) {
                feeds_out |> push(tok)
            }
            forward(t, s, tok, s.n_past)
            s.n_past++
        }
        out |> push_from(s.logits)
        delete s
        return <- out
    }
    var ss : array<Session>
    ss |> reserve(nb)
    for (i in range64(nb)) {
        ss |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
        var prompt <- stream_prompt(t, i)
        eval_(t, ss[i], prompt)
        delete prompt
    }
    var ws <- create_batch_workspace_(t)
    var sess : array<Session?>
    sess |> reserve(nb)
    var cur : array<int64>
    cur |> reserve(nb)
    for (i in range64(nb)) {
        unsafe {
            sess |> push(addr(ss[i]))
        }
        cur |> push(0l)
    }
    for (st in range64(nsteps)) {
        for (i in range64(nb)) {
            cur[i] = replay ? feeds[st * nb + i] : greedy_top(ss[i].logits, vocab)
            if (!replay) {
                feeds_out |> push(cur[i])
            }
        }
        eval_batch_(t, ws, sess, cur)
    }
    for (i in range64(nb)) {
        out |> push_from(ss[i].logits)
    }
    sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
    unsafe {
        delete ws
        delete sess
    }
    delete cur
    delete ss
    return <- out
}

def private truth_path(prefix : string; nb, nsteps : int64) : string {
    return "{prefix}.b{nb}.n{nsteps}.tsv"
}

def private save_truth(path : string; feeds : array<int64>; logits : array<float>) {
    fopen(path, "wb") $(f) {
        if (f == null) {
            print("cannot write truth cache {path}\n")
            return
        }
        fwrite(f, build_string() $(var w) {
            w |> write("feeds")
            for (tk in feeds) {
                w |> write(" ")
                w |> write(tk)
            }
            w |> write("\n")
        })
        var at = 0
        while (at < length(logits)) {
            let hi = min(at + 4096, length(logits))
            let lo = at
            fwrite(f, build_string() $(var w) {
                w |> write("logits")
                for (i in range(lo, hi)) {
                    w |> write(" ")
                    w |> write(fmt(":.9g", logits[i]))
                }
                w |> write("\n")
            })
            at = hi
        }
    }
    print("truth cache saved: {path}\n")
}

def private load_truth(path : string; var feeds : array<int64>; var logits : array<float>) : bool {
    if (!stat(path).is_valid) {
        return false
    }
    var ok = false
    fopen(path, "rb") $(f) {
        if (f == null) {
            return
        }
        let text = fread(f)
        for (line in split(text, "\n")) {
            if (starts_with(line, "feeds")) {
                for (tk in split_by_chars(slice(line, 6), " ")) {
                    let parsed = try_to_int64(tk)
                    if (is_err(parsed)) {   // corrupt/truncated cache — reject it whole, the caller recomputes+resaves
                        feeds |> clear()
                        logits |> clear()
                        return
                    }
                    feeds |> push(unwrap(parsed))
                }
            } elif (starts_with(line, "logits")) {
                for (v in split_by_chars(slice(line, 7), " ")) {
                    let parsed = try_to_float(v)
                    if (is_err(parsed)) {
                        feeds |> clear()
                        logits |> clear()
                        return
                    }
                    logits |> push(unwrap(parsed))
                }
            }
        }
        ok = !empty(feeds) && !empty(logits)
    }
    return ok
}

[export]
def main() : int {   // nolint:STYLE038 — flat probe driver: arg handling, then one A/B cell per --bs entry
    allow_cpu_prefill()   // both halves prefill the streams on the CPU by design (truth + replay)
    if (!jit_enabled()) {
        print("This probe is JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "batch_parity_probe")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "batch_parity_probe")
        return 0
    }
    if (!empty(g_env_engine.pin_backend)) {
        pin_kernel_backend(g_env_engine.pin_backend)
    }
    let nsteps = max(int64(cfg.steps), 1l)
    print("loading {cfg.model}\n")
    var t <- load_model(cfg.model, QuantMode.q8)
    t.config.seq_len = min(t.config.seq_len, 1024l)   // B=16's deepest stream (30+7*15 numbers) needs ~500 + steps
    print("config: dim={t.config.dim} layers={t.config.n_layers} vocab={t.config.vocab_size}  backend {active_kernel_backend()}\n")
    // the replay half rides the blob-only drivers: a second facade load under metal mode maps
    // (or mints) the METAL flavor beside the planar truth model — flavors are self-sufficient
    var tg = Model()
    if (!cfg.truth_only) {
        set_metal_mode(MetalMode.when_available)
        var tg2 <- load_model(cfg.model, QuantMode.q8)
        set_metal_mode(MetalMode.off)   // overrides select per phase below
        tg <- tg2   // nolint:PERF030 — tg is still the default-constructed Model, nothing to release
        tg.config.seq_len = min(tg.config.seq_len, 1024l)
        if (!tg.metal_blob) {
            print("WARNING: the replay model is NOT blob-form (a CPU-flavor .dlim?) - the GPU replay will decline `planar` and run on the CPU\n")
        }
    }
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque_()
        for (tok in split(cfg.bs, ",")) {
            let nb = int64(to_int(tok))
            if (nb < 1l) {
                continue
            }
            var feeds : array<int64>
            var truth : array<float>
            let cpath = empty(cfg.truth) ? "" : truth_path(cfg.truth, nb, nsteps)
            var cached = false
            if (!empty(cpath)) {
                cached = load_truth(cpath, feeds, truth)
            }
            if (!cached) {
                select_decode_override("")
                var sink : array<int64>
                delete truth   // a rejected cache can leave rows behind (feeds empty, logits parsed)
                truth <- teacher_logits(t, nb, nsteps, sink, feeds)
                delete sink
                if (!empty(cpath)) {
                    save_truth(cpath, feeds, truth)
                }
            }
            if (cfg.truth_only) {
                print("B={nb}: truth {cached ? "already cached" : "minted"}\n")
                delete feeds
                delete truth
                continue
            }
            // the replay's prompt prefills ride the GPU too — CPU-portable prefill of the deep
            // streams (~4300 tokens at B=16) was minutes per run vs seconds of actual replay,
            // and sharing the GPU prefill across A/B arms keeps rail-on/off diffs prefill-free
            select_prefill_override("metal")
            select_decode_override("metal")
            var sink2 : array<int64>
            var gpu <- teacher_logits(tg, nb, nsteps, feeds, sink2)
            select_prefill_override("")
            select_decode_override("")
            delete sink2
            var d = 0.0f
            for (i in range(length(truth))) {
                d = max(d, abs(truth[i] - gpu[i]))
            }
            let verdict : string = d < cfg.tol ? "ok" : "EXCEEDS tol {cfg.tol}"
            let src : string = cached ? "  (cached truth)" : ""
            print("B={nb}: teacher max|dlogit| = {d}  [{verdict}]{src}\n")
            if (d >= cfg.tol) {
                rc = 1
            }
            delete feeds
            delete truth
            delete gpu
        }
    }
    delete t
    delete tg
    return rc
}
