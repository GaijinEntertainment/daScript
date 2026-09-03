options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// The batch step's cost curve in rows: ms per step at B = 1 (the single-stream decode) and B = 2, 4, 8
// (the batch driver, one session per row) on a served model - what a k-row speculative verify pays
// against a plain step, and on an MoE model the price of the expert union.
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_common
require dasllama/dasllama_batch   // eval_batch_ + create_batch_workspace_
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_image   // load_model_cached - the .dlim rail
require dasllama/dasllama_tokenizer
require ?das_metal dasllama/dasllama_metal_common   // finish_pending_step
require ?das_metal dasllama/dasllama_metal_decode
require ?das_metal dasllama/dasllama_metal_prefill
require ../performance/profile_common.das   // apply_box_profile_runtime
require daslib/jobque_boost
require daslib/strings_boost
require daslib/clargs
require math
require strings

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "GGUF model path"
    model : string

    @clarg_doc = "Timed steps per row count (two warm steps precede them)"
    steps : int = 32

    @clarg_doc = "Comma list of row counts"
    bs : string = "1,2,4,8"

    @clarg_doc = "Same-slab mode: the rows are B consecutive positions of ONE session through the verify entry (verify_batch_step) instead of B sessions"
    sameslab : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private time_rows(t : Model; prompt : array<int64>; nb, steps : int64; sameslab : bool) : double {
    let c = t.config
    var sess : array<Session>
    sess |> reserve(nb)
    for (_i in range64(nb)) {
        sess |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        select_prefill_override("metal")
        set_metal_prefill_min_npos(1l)
    }
    for (i in range64(nb)) {
        eval_(t, sess[i], prompt)
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        select_prefill_override("")
        set_metal_prefill_min_npos(64l)
        select_decode_override("metal")
    }
    var ws <- create_batch_workspace_(t)
    var ptrs : array<Session?>
    var cur : array<int64>
    ptrs |> reserve(nb)
    cur |> reserve(nb)
    for (i in range64(nb)) {
        unsafe {
            ptrs |> push(addr(sess[i]))
        }
        cur |> push(prompt[i % long_length(prompt)])
    }
    var t0 = 0l
    let plen = long_length(prompt)
    for (step in range64(steps + 2l)) {
        if (step == 2l) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                finish_pending_step()
            }
            t0 = ref_time_ticks()
        }
        for (i in range64(nb)) {
            cur[i] = prompt[(step + i) % plen]   // real-text tokens, deterministic - the routing stays realistic
        }
        if (nb == 1l) {
            forward(t, sess[0], cur[0], sess[0].n_past)
            sess[0].n_past++
        } elif (sameslab) {
            let pos = sess[0].n_past
            if (!verify_batch_step(t, ws, sess[0], cur, pos, nb)) {
                panic("batch_rows_probe: the same-slab verify declined at B={nb}")
            }
            sess[0].n_past = pos + nb
        } else {
            eval_batch_(t, ws, ptrs, cur)
        }
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        finish_pending_step()
    }
    let ms = double(get_time_usec(t0)) / double(steps) / 1000.0lf
    static_if (typeinfo builtin_module_exists(das_metal)) {
        select_decode_override("")
    }
    ptrs |> clear()
    unsafe {
        delete ptrs
    }
    delete ws
    delete cur
    delete sess
    return ms
}

[export]
def main() : int {
    if (!jit_enabled()) {
        print("This probe is JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "batch_rows_probe")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.model)) {
        print_help(get_command_info(type<Args>), "batch_rows_probe")
        return cfg.help ? 0 : 1
    }
    apply_box_profile_runtime()
    if (!tune_gate()) {   // never measure the ladder on fallback winners
        panic("batch_rows_probe: tune_gate refused - tune first (or point DAS_TUNE_MANIFEST at a complete sidecar)")
    }
    set_metal_mode(MetalMode.required)
    let tk <- load_tokenizer_auto(cfg.model)
    var measured = 0
    with_job_que() {
        setup_dasllama_jobque_()
        var t <- load_model_cached(cfg.model, QuantMode.q8)
        t.config.seq_len = min(t.config.seq_len, 512l)
        var prompt <- encode(tk, "The old lighthouse keeper watched the storm roll in from the west, counting the seconds between each flash and the thunder that followed, while the village below slept through it.", false)
        var base = 0.0lf
        for (bstr in split(cfg.bs, ",")) {
            let nb = int64(to_int(strip(bstr)))
            if (nb < 1l) {
                continue
            }
            let ms = time_rows(t, prompt, nb, int64(cfg.steps), cfg.sameslab && nb > 1l)
            if (nb == 1l) {
                base = ms
            }
            let ratio = base > 0.0lf ? ms / base : 0.0lf
            let mode = cfg.sameslab && nb > 1l ? " same-slab" : ""
            print("B={nb}{mode}: {ms:.2f} ms/step  x{ratio:.2f} of the single step  ({ms / double(nb):.2f} ms per row)\n")
            measured++
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            metal_decode_shutdown()
            metal_prefill_shutdown()
        }
        delete prompt
        delete t
    }
    if (measured == 0) {
        print("batch_rows_probe: no row count measured - --bs names none >= 1\n")
        return 1
    }
    return 0
}
