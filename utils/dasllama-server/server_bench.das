options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — a serving benchmark, not the bit-exact suite

// Serving-latency benchmark for llm_scheduler — drives the scheduler directly through the
// SchedEvent seam (no HTTP), so every number is server-side truth:
//   1. aggregate tok/s + TTFT p50/p95 + inter-token p50/p95 vs batch size B
//   2. chunk interference: a decoding stream's inter-token p95 while a long prompt prefills,
//      per chunk size K — the data that tunes --chunk
//   3. warm vs cold TTFT for an identical prompt — the prefix-cache collapse
//   bin/daslang -jit utils/dasllama-server/server_bench.das -- -m <model.gguf> [-b 8] [-n 64] [--long-prompt 512]

require llm_scheduler           // the scheduler under bench (sibling — bare require, hyphenated dir)
require dasllama/dasllama       // facade: load_model / setup_dasllama_jobque
require daslib/jobque_boost
require daslib/clargs
require daslib/strings_boost    // split (the --chunks list)
require strings                 // to_int64 / strip
require math

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_short = "p"
    @clarg_doc = "Prompt tokens per stream in the B sweep (default: 64)"
    prompt : int = 64

    @clarg_short = "n"
    @clarg_doc = "Tokens generated per stream (default: 64)"
    ngen : int = 64

    @clarg_short = "b"
    @clarg_doc = "Largest batch size — sweeps 1,2,4,... up to this (default: 8)"
    bmax : int = 8

    @clarg_doc = "Long-prompt tokens for the interference and warm-TTFT arms (default: 512)"
    long_prompt : int = 512

    @clarg_doc = "Comma-separated chunk sizes for the interference arm (default 16,64,256)"
    chunks : string

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private build_prompt(n, salt : int64) : array<int64> {
    return <- [for (i in range64(n)); 1000l + ((i * 7l + salt * 131l) % 5000l)]
}

def private pct(var xs : array<int64>; p : float) : int64 {
    if (empty(xs)) {
        return 0l
    }
    xs |> sort()
    let idx = clamp(int(float(length(xs) - 1) * p + 0.5), 0, length(xs) - 1)
    return xs[idx]
}

def private ms(us : int64) : float => float(us) / 1000.0

struct private RoundStats {
    agg_tps : double            // Σ generated tokens / (last piece - first piece)
    ttft_us : array<int64>      // per stream, admit -> first sampled token
    itl_us : array<int64>       // inter-token deltas, all streams pooled
    cached : int64              // Σ n_cached across finished streams
}

// Submit nreq salted prompts, then step to idle, timestamping every piece as it leaves the
// scheduler — the same loop shape as the server, minus HTTP.
def private drive(m : Model; var sch : Scheduler; nreq, p_tokens, max_new, salt0 : int64) : RoundStats {
    var st = RoundStats()
    var events : array<SchedEvent>
    var last_piece : table<int64; int64>
    for (i in range64(nreq)) {
        var req <- PendingReq(id = salt0 * 1000l + i, max_new = max_new)
        req.prompt <- build_prompt(p_tokens, salt0 * 97l + i)
        if (!submit(m, sch, req)) {
            panic("server_bench: submit rejected (queue too small for the sweep)")
        }
    }
    let t0 = ref_time_ticks()
    var first_us = -1l
    var final_us = 0l
    var gen_total = 0l
    var guard = 0
    var running = true
    while (running) {   // the LAST step's events (the final finished) must still process
        running = scheduler_step(m, sch, events)
        let now = int64(get_time_usec(t0))
        for (e in events) {
            if (e.kind == SchedEventKind.piece) {
                if (first_us < 0l) {
                    first_us = now
                }
                final_us = now
                if (key_exists(last_piece, e.stream)) {
                    st.itl_us |> push(now - last_piece[e.stream])
                }
                last_piece[e.stream] = now
            } else {
                st.ttft_us |> push(e.ttft_us)
                st.cached += e.n_cached
                gen_total += e.n_gen
            }
        }
        events |> clear()
        guard ++
        if (guard > 1000000) {
            panic("server_bench: scheduler did not go idle")
        }
    }
    if (final_us > first_us && first_us >= 0l) {
        st.agg_tps = double(gen_total) * 1.0e6lf / double(final_us - first_us)
    }
    delete events
    delete last_piece
    return <- st
}

// Stream A decodes; a long prompt lands mid-run and prefills in K-token chunks. A's inter-token
// gaps during that window are the interference the chunk size buys or costs.
def private interference_run(m : Model; var sch : Scheduler; long_p, salt : int64) : array<int64> {
    var itl : array<int64>
    var events : array<SchedEvent>
    var req_a <- PendingReq(id = 1l, max_new = long_p)   // enough budget to outlast B's prefill
    req_a.prompt <- build_prompt(64l, salt * 7l + 1l)
    if (!submit(m, sch, req_a)) {
        panic("server_bench: submit A rejected")
    }
    let t0 = ref_time_ticks()
    var a_last = -1l
    var b_running = false
    var b_first_piece = false
    var guard = 0
    var running = true
    while (running && !b_first_piece) {
        running = scheduler_step(m, sch, events)
        let now = int64(get_time_usec(t0))
        for (e in events) {
            if (e.kind != SchedEventKind.piece) {
                continue
            }
            if (e.stream == 1l) {
                if (b_running && a_last >= 0l) {
                    itl |> push(now - a_last)   // only the window that overlaps B's prefill
                }
                a_last = now
                if (!b_running) {   // A is decoding — drop the long prompt on the scheduler
                    var req_b <- PendingReq(id = 2l, max_new = 2l)
                    req_b.prompt <- build_prompt(long_p, salt * 7l + 2l)
                    if (!submit(m, sch, req_b)) {
                        panic("server_bench: submit B rejected")
                    }
                    b_running = true
                }
            } elif (e.stream == 2l) {
                b_first_piece = true   // B's prefill is done — the window closes
            }
        }
        events |> clear()
        guard ++
        if (guard > 1000000) {
            panic("server_bench: interference run did not converge")
        }
    }
    evict(sch, 1l, events)
    evict(sch, 2l, events)
    var draining = true
    while (draining) {
        draining = scheduler_step(m, sch, events)
        events |> clear()
    }
    delete events
    return <- itl
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
        print_help(get_command_info(type<Args>), "server_bench")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "server_bench")
        return
    }
    let p = max(int64(cfg.prompt), 8l)
    let n = max(int64(cfg.ngen), 4l)
    let bmax = clamp(int64(cfg.bmax), 1l, 32l)
    let long_p = max(int64(cfg.long_prompt), 128l)
    var chunks : array<int64>
    if (!empty(cfg.chunks)) {
        var parts <- split(cfg.chunks, ",")
        for (s in parts) {
            let k = to_int64(strip(s))
            if (k > 0l) {
                chunks |> push(k)
            }
        }
        delete parts
    }
    if (empty(chunks)) {
        chunks <- [16l, 64l, 256l]
    }
    print("loading {cfg.model}\n")
    var m <- load_model(cfg.model, cfg.quant)
    // every arm must fit: the B sweep (p+n), the interference arm (64 + long_p of A-budget), warm TTFT
    let need = max(p + n + 8l, max(long_p + 16l, 64l + long_p + 8l))
    m.config.seq_len = min(m.config.seq_len, need)
    print("config: dim={m.config.dim} layers={m.config.n_layers} ctx={m.config.seq_len}\n\n")
    with_job_que() {
        setup_dasllama_jobque()
        // ===== 1. throughput + latency vs batch size (distinct prompts — no prefix effects) =====
        print("== tok/s + latency vs B  (prompt {p}, gen {n} per stream, paged P=64) ==\n")
        var salt = 1l
        {
            var warm_sch <- create_scheduler(m, 2l, 64l, 64l, 64l)   // warm the JIT + kernel dispatch
            var wst <- drive(m, warm_sch, 2l, 16l, 4l, salt)
            delete wst
            unsafe {
                delete warm_sch
            }
        }
        var nb = 1l
        while (nb <= bmax) {
            salt ++
            var sch <- create_scheduler(m, nb, 64l, 64l, 64l)
            var st <- drive(m, sch, nb, p, n, salt)
            print("B={nb}: {st.agg_tps} tok/s | ttft p50 {ms(pct(st.ttft_us, 0.5))}ms p95 {ms(pct(st.ttft_us, 0.95))}ms" +
                  " | inter-token p50 {ms(pct(st.itl_us, 0.5))}ms p95 {ms(pct(st.itl_us, 0.95))}ms\n")
            delete st
            unsafe {
                delete sch
            }
            nb *= 2l
        }
        // ===== 2. chunk interference: decode stall while a long prompt prefills =====
        print("\n== inter-token gap of a live stream while a {long_p}-token prompt prefills, per chunk K ==\n")
        for (k in chunks) {
            salt ++
            var sch <- create_scheduler(m, 2l, 4l, k, 64l)
            var itl <- interference_run(m, sch, long_p, salt)
            print("K={k}: {length(itl)} gaps | p50 {ms(pct(itl, 0.5))}ms | p95 {ms(pct(itl, 0.95))}ms | max {ms(pct(itl, 1.0))}ms\n")
            delete itl
            unsafe {
                delete sch
            }
        }
        // ===== 3. warm vs cold TTFT: the prefix-cache prefill collapse =====
        print("\n== warm vs cold TTFT for an identical {long_p}-token prompt (prefix cache) ==\n")
        {
            salt ++
            var sch <- create_scheduler(m, 1l, 4l, 64l, 64l)
            var cold <- drive(m, sch, 1l, long_p, 8l, salt)
            var warm <- drive(m, sch, 1l, long_p, 8l, salt)   // same salt -> same prompt
            let c_us = pct(cold.ttft_us, 0.5)
            let w_us = pct(warm.ttft_us, 0.5)
            let ratio = w_us > 0l ? double(c_us) / double(w_us) : 0.0lf
            print("cold ttft {ms(c_us)}ms | warm ttft {ms(w_us)}ms ({ratio}x) | warm cached {warm.cached} tokens\n")
            delete cold
            delete warm
            unsafe {
                delete sch
            }
        }
    }
    delete chunks
    delete m
}
