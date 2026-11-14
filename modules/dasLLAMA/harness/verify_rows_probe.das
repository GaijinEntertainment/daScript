options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// The same-slab verify against the one-row step on the armed GPU home: a plain greedy walk of the prompt records every
// step's logits, then the same walk again through speculative rounds whose drafts are the plain walk's own next tokens (an
// oracle in the block drafter's seat), at each depth of --rows. A row reports the verify's wall a round against the plain
// steps it replaces, and how far the verify's rows sit from the one-row steps' logits: the widest difference as a share of the
// step's largest logit, the rows whose pick differs, and the narrowest top-two margin among the plain steps.
//
//   DASLLAMA_GPU=1 [DASLLAMA_CUDA=1] bin/daslang -jit modules/dasLLAMA/harness/verify_rows_probe.das -- -m <model.gguf>
//       [--rows 1,3,7] [--steps 96] [--ctx 8192] [--prompt "<text>"]

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama
require dasllama/dasllama_common   // the block drafter's seams, mtp_spec_round_k
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_resident   // set_gpu_resident_seat_accept
require ?cuda dasllama/dasllama_cuda_decode   // nolint:STYLE030,LINT019 - set_cuda_rows_mma_min, inside the CUDA half
require ?cuda dasllama/dasllama_cuda_common   // nolint:STYLE030,LINT019 - the launch census, inside the CUDA half
require ?cuda _cuda_time   // nolint:STYLE030,LINT019 - the profile rail's report, inside the CUDA half
require _forced_drafter   // the oracle drafter in the block drafter's seat
require _prompt_files   // id_list
require ../tests/_compares.das   // top2_margin, logits_maxabs
require daslib/jobque_boost
require daslib/clargs
require math

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "The model's GGUF"
    model : string
    @clarg_doc = "Drafts a round, comma-separated: a round verifies one row more"
    rows : string = "1,3,7"
    @clarg_doc = "Generated tokens a walk"
    steps : int = 96
    @clarg_doc = "Cap the sessions' context at this many positions"
    ctx : int = 8192
    @clarg_doc = "CUDA home: the token rows from which a Q4_K site takes the rows tensor leaf (0 = never; -1 = the driver's own)"
    mma : int = -1
    @clarg_doc = "CUDA home: the same for a Q8_0 site"
    mma_q8 : int = -1
    @clarg_doc = "CUDA home: print the kernels a plain step and a round of each depth launch as their commands are first recorded"
    census : bool
    @clarg_doc = "CUDA home: after the walks, each kernel's wall time over 8 plain steps and 8 rounds of the deepest depth, every launch waited"
    times : bool
    @clarg_doc = "The prompt's text"
    prompt : string = "Explain how a binary search works, step by step."
    @clarg_short = "h"
    @clarg_doc = "Show this help and exit"
    help : bool
}

var private g_census : table<string; int64>

//! the CUDA home's launch counts since the last call, a kernel a line under `what` (a command's launches as it is first
//! recorded: the replays launch a graph)
[cold_path]
def private census_delta(what : string; say : bool) {
    static_if (typeinfo module_exists(dasllama_cuda_common)) {
        var names <- cud_kernel_names()
        var total = 0l
        for (name in names) {
            let now = cud_kernel_census_of(name)
            let was = g_census?[name] ?? 0l
            if (say && now > was) {
                print("CENSUS {what}: {now - was} x {name}\n")
                total += now - was
            }
            g_census[name] = now
        }
        if (say) {
            print("CENSUS {what}: {total} launches\n")
        }
        delete names
    }
}

//! eight plain steps and eight rounds of `d` drafts under the profile rail
[cold_path]
def private times_walk(m : Model; prompt : array<int64>; d : int64) {
    static_if (typeinfo module_exists(dasllama_cuda_common)) {
        cud_launch_timing(true)
        var one : array<int64>
        one |> resize(1l)
        var s <- create_session(m)
        eval(m, s, prompt)
        cuda_times_drop()
        for (i in range(8)) {
            one[0] = g_forced_seq[i]
            eval(m, s, one)
        }
        cuda_times_report("plain step", 8l, 0.0lf, "ms in the timed launches")
        delete s
        var cs <- create_session(m)
        eval(m, cs, prompt)
        set_mtp_depth(d)
        var pos = 0l
        mtp_spec_round_k(m, cs, g_forced_seq[0], d)   //! the round that warms the verify's kernels
        cuda_times_drop()
        for (_i in range(8)) {
            pos += d + 1l
            break if (pos + d + 1l >= long_length(g_forced_seq))
            mtp_spec_round_k(m, cs, g_forced_seq[pos], d)
        }
        cuda_times_report("round of {d + 1l} rows", 8l, 0.0lf, "ms in the timed launches")
        delete cs
        delete one
        cud_launch_timing(false)
    }
}

[cold_path]
def private walk_rows(m : Model; prompt : array<int64>; plain_logits : array<float>; k : int64; plain_ms_step : double) {
    let vocab = m.config.vocab_size
    var s <- create_session(m)
    eval(m, s, prompt)
    var worst = 0.0
    var flips = 0l
    var rows = 0l
    var rounds = 0l
    var accepted = 0l
    var i = 0l   // the anchor's index in g_forced_seq
    let t0 = ref_time_ticks()
    var cmp_us = 0
    while (i + 1l < long_length(g_forced_seq)) {
        let a = mtp_spec_round_k(m, s, g_forced_seq[i], k)
        rounds++
        accepted += a
        let tc = ref_time_ticks()
        let landed = long_length(s.mtp_logits_b) / vocab
        for (r in range64(min(landed, a + 1l))) {   // row r is the step at index i + r: its logits pick g_forced_seq[i + r + 1]
            let pi = i + r + 1l
            break if (pi >= long_length(g_forced_seq))
            var mag = 0.0
            var dmax = 0.0
            for (e in range64(vocab)) {
                let pv = plain_logits[pi * vocab + e]
                mag = max(mag, abs(pv))
                dmax = max(dmax, abs(s.mtp_logits_b[r * vocab + e] - pv))
            }
            worst = max(worst, dmax / max(mag, 1.0e-30))
            flips += parallel_argmax(unsafe(addr(s.mtp_logits_b[r * vocab])), vocab) != parallel_argmax(unsafe(addr(plain_logits[pi * vocab])), vocab) ? 1l : 0l
            rows++
        }
        cmp_us += get_time_usec(tc)
        i += a + 1l
    }
    let ms = double(get_time_usec(t0) - cmp_us) / 1000.0lf
    let toks = i
    let per_round = ms / double(max(rounds, 1l))
    print("drafts {k}: {rounds} rounds, {double(accepted) / double(max(rounds, 1l)):.2f} accepted a round, {per_round:.2f} ms a round = {per_round / plain_ms_step:.2f} plain steps, {double(toks) * 1000.0lf / ms:.1f} tok/s against {1000.0lf / plain_ms_step:.1f} plain; rows {rows}: widest logit difference {worst * 100.0:.3f}% of the step's largest, picks differing {flips}\n")
    delete s
}

[cold_path]
def private run(a : Args) : int {   // nolint:STYLE038 — one linear measurement sweep
    let rc = 0
    var depths <- id_list(a.rows, 1l)
    var deepest = 1l
    for (d in depths) {
        deepest = max(deepest, d)
    }
    set_mtp_depth(deepest)   // ahead of the load: a device home sizes its verify's rows to the depth it finds then
    static_if (typeinfo module_exists(dasllama_cuda_decode)) {
        if (a.mma >= 0) {
            set_cuda_rows_mma_min(int64(a.mma), int(KqFmt.k4))
        }
        if (a.mma_q8 >= 0) {
            set_cuda_rows_mma_min(int64(a.mma_q8), int(KqFmt.q8))
        }
    }
    set_gpu_resident_seat_accept(false)   // the probe compares every verify row's logits: they land
    set_mtp_spec(true)
    with_dasllama_jobque_() {
        var m <- load_model(a.model, QuantMode.q8)
        m.config.seq_len = min(m.config.seq_len, int64(a.ctx))
        let vocab = m.config.vocab_size
        var prompt <- encode(m, a.prompt)
        g_forced_base = long_length(prompt)
        var plain_logits : array<float>
        plain_logits |> reserve(int64(a.steps + 1) * vocab)
        plain_logits |> resize(int64(a.steps + 1) * vocab)
        var margin = 1.0
        {
            var s <- create_session(m)
            eval(m, s, prompt)
            var one : array<int64>
            one |> resize(1l)
            var step_us = 0
            for (i in range(a.steps + 1)) {
                for (e in range64(vocab)) {
                    plain_logits[int64(i) * vocab + e] = s.logits[e]
                }
                margin = min(margin, top2_margin(s.logits, vocab) / max(logits_maxabs(s.logits), 1.0e-30))
                let tok = parallel_argmax(s.logits, vocab)
                g_forced_seq |> push(tok)
                break if (i == a.steps)
                one[0] = tok
                if (a.census && i == 0) {
                    census_delta("", false)
                }
                let ts = ref_time_ticks()
                eval(m, s, one)
                step_us += get_time_usec(ts)
                if (a.census && i == 0) {
                    census_delta("plain step", true)
                }
            }
            let plain_ms = double(step_us) / 1000.0lf / double(a.steps)
            print("plain: {a.steps} steps at {plain_ms:.2f} ms ({1000.0lf / plain_ms:.1f} tok/s); the narrowest top-two margin {margin * 100.0:.3f}% of the step's largest logit\n")
            delete one
            delete s
            attach_forced_drafter([0l], deepest)
            for (d in depths) {
                set_mtp_depth(d)
                if (a.census) {
                    var cs <- create_session(m)
                    eval(m, cs, prompt)
                    census_delta("", false)
                    mtp_spec_round_k(m, cs, g_forced_seq[0], d)
                    census_delta("round of {d + 1l} rows", true)
                    delete cs
                }
                walk_rows(m, prompt, plain_logits, d, plain_ms)
            }
            if (a.times) {
                times_walk(m, prompt, deepest)
            }
            unregister_mtp_block_drafter()
        }
        delete plain_logits
        delete prompt
        delete m
    }
    delete depths
    return rc
}

[export, cold_path]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<Args>), "verify_rows_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model)) {
        print_help(get_command_info(type<Args>), "verify_rows_probe")
        return a.help ? 0 : 2
    }
    return run(a)
}
