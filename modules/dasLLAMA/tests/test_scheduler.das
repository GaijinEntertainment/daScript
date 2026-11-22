options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // each test frees its model/scheduler before the next loads
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_scheduler   // the scheduler under test
require _scheduler_rig   // nolint:STYLE029,LINT019 - make_req, drain, toks_of, read only inside the das_metal half
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the facade's load, read only inside the das_metal half
require dasllama/dasllama_blocks   // embed_text_rows: the media arms build rows from the prompt's own tokens
require _compares   // mismatch_exact
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, read only inside the das_metal half
require math   // nolint:STYLE030,LINT019 - min, read only inside the das_metal half
require _model_tier
require _scheduler_cells   // capture_stops_leg for the n-gram cell
require dasllama/dasllama_image   // nolint:STYLE030,LINT019 - load_model_cached: the n-gram capture cell's served image, inside the das_metal half
require dasllama/dasllama_gguf   // nolint:STYLE030,LINT019 - set_mtp_head_path: that image's split head, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_decode   // metal_decode_shutdown
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown

// The continuous-batching scheduler's tests: the model-free stop scan and idle chunk, and the Metal n-gram capture cell (the
// model cells of `_scheduler_cells.das` run on their models' blocks). The n-gram cell is model-gated and JIT-only: it skips
// without its model file under models_dir(), off a Metal build, outside its family and arm, and past the size tier.

// ===== stop strings: the hold-back scan on its own, then the stream it ends =====

[test(description = "`stop_scan` releases bytes that cannot begin a stop string, holds a growing partial match across pieces, cuts at a completed match, and with two stop strings cuts at the earliest")]
def test_scheduler_stop_scan(t : T?) {
    t |> run("stop_scan releases what cannot begin a stop string, holds what can, cuts at the first match") @(t : T?) {
        let stops <- ["</think", "STOP"]
        var hold = ""
        var emit = ""
        t |> success(!stop_scan(hold, "hello <", stops, emit), "no match in the first piece")
        t |> equal("hello ", emit, "the bytes that cannot begin a stop string are released")
        t |> equal("<", hold, "... the one that can is held")
        t |> success(!stop_scan(hold, "/thi", stops, emit), "still no match")
        t |> equal("", emit, "a hold that is all partial match releases nothing")
        t |> equal("</thi", hold, "the partial match grows")
        t |> success(stop_scan(hold, "nk> and more", stops, emit), "the match completes across three pieces")
        t |> equal("", emit, "nothing stood before it")
        t |> equal("", hold, "the hold is spent")
        t |> success(!stop_scan(hold, "a<b", stops, emit), "a false start")
        t |> equal("a<b", emit, "released whole: no suffix begins a stop string")
        t |> success(!stop_scan(hold, "xS", stops, emit), "the second stop string's first byte")
        t |> equal("x", emit)
        t |> equal("S", hold)
        t |> success(stop_scan(hold, "TOPPED", stops, emit), "completed by the next piece")
        t |> equal("", emit)
        t |> success(stop_scan(hold, "say STOP now", stops, emit), "a match inside one piece")
        t |> equal("say ", emit, "the text before the match")
        let two <- ["TOP", "ST"]
        t |> success(stop_scan(hold, "xSTOP", two, emit), "two stop strings match")
        t |> equal("x", emit, "the earliest match cuts")
    }
}

[test(description = "`idle_chunk_default` on a GPU is the larger of the 2048-token floor and the driver's prompt window, and on the CPU is the default chunk whatever the window")]
def test_idle_chunk_follows_the_window(t : T?) {
    t |> run("the idle chunk on a GPU is the wider of the 2048 floor and the driver's prompt window; the CPU's is one window") @(t : T?) {
        t |> equal(idle_chunk_default(true, 4096l), 4096l, "the wide window picks the chunk: a 3k prompt sweeps the expert stacks once")
        t |> equal(idle_chunk_default(true, 2048l), GPU_IDLE_CHUNK_TOKENS, "the narrow window is the floor")
        t |> equal(idle_chunk_default(true, 512l), GPU_IDLE_CHUNK_TOKENS, "a window under the floor never lowers it")
        t |> equal(idle_chunk_default(false, 4096l), DEFAULT_CHUNK_TOKENS, "the CPU's idle chunk is one window whatever a device would say")
    }
}

//! the n-gram side-input carrier the capture cell's Metal arm runs, served as its image with the split head beside it
let FLASH_NEXT_FILE = "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf"
let FLASH_NEXT_HEAD = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf"

[test(description = "On Metal with the Flash-Next n-gram carrier, four stops in one window attach and continue as a fresh prefill, and the captured side input holds exact last tokens and history within 1 percent of max")]
def test_scheduler_capture_stops_ngram(t : T?) {
    t |> run("Metal prefill: the four stops ride ONE window on the n-gram side-input carrier, each attaches and continues as a fresh prefill does (needs Qwen3.8-Flash-Next, DASLLAMA_PARITY_FULL=1)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), FLASH_NEXT_FILE)
            let hpath = path_join(models_dir(), FLASH_NEXT_HEAD)
            return if (!jit_only(t) || !family_on(t, "qwen4exp") || !arm_on(t, "fam-qwen4exp") || !model_available(t, path) || !model_available(t, hpath))
            let spec0 = get_mtp_spec()
            set_mtp_spec(false)
            let mode0 = get_metal_mode()
            set_metal_mode(MetalMode.required)
            set_mtp_head_path(hpath)
            var m <- load_model_cached(path, QuantMode.q8)
            clear_mtp_head_path()
            set_metal_mode(mode0)
            m.config.seq_len = min(m.config.seq_len, 1024l)
            t |> success(m.metal_blob && m.config.hyper_conn && has_ple_ngram(m.config), "the served image is the metal-blob hyper-connection carrier with the n-gram side input")
            with_dasllama_jobque() {
                t |> success(select_prefill_override("metal"), "the Metal prefill override registered")
                t |> success(select_decode_override("metal"), "the Metal decode override registered")
                var prompt <- counting_prompt(m, 60)
                let np = long_length(prompt)
                t |> success(np > 220l, "the prompt holds every stop with room past the last ({np} tokens)")
                let stops <- [48l, 96l, 160l, np - 1l]
                var cap <- capture_stops_leg(t, m, prompt, stops, 1024l, "ngram, one window")
                var live <- capture_stops_leg(t, m, prompt, stops, stops[1], "ngram, a window ending on stop {stops[1]}")
                //! the n-gram side input at the second stop - the last tokens and the conv history the pre-step reads - captured
                //! inside the window against what a window ending there leaves: the tokens bit for bit; the history rows are the
                //! gated projections of rows the trunk's stream gates, and the Metal window's tiles and scan groups round a 240-row
                //! window apart from a 96-row one (the recurrent state reads 0.9 apart on a magnitude of 6, logged below), so the
                //! history within a hundredth of its largest magnitude (reads 5e-3 on a magnitude of 12), the first stop's history
                //! outside that bar the control (reads 3.5). The CPU legs of `test_scheduler_capture_stops` hold the state bit for bit.
                let at = stops[1]
                t |> success(!empty(live[1].ple_prev) && !empty(live[1].ple_hist), "ngram: stop {at} carries the side input")
                t |> equal(mismatch_exact(cap[1].ple_prev, live[1].ple_prev), 0, "ngram: the last tokens captured at stop {at} are the ones a window ending there leaves ({cap[1].ple_prev} against {live[1].ple_prev})")
                let bar = 1e-2 * logits_maxabs(live[1].ple_hist)
                let hd = logits_maxdiff(cap[1].ple_hist, live[1].ple_hist)
                let hc = logits_maxdiff(cap[0].ple_hist, live[1].ple_hist)
                t |> success(hd <= bar, "ngram: the side input's conv history captured at stop {at} is the one a window ending there leaves ({hd} against a bar of {bar})")
                t |> success(hc > bar, "ngram: control - stop {stops[0]}'s side-input history lands outside the bar ({hc})")
                to_log(LOG_INFO, "ngram: stop {at}'s history, captured against a window's end: {hd} (stop {stops[0]}'s: {hc}, the bar {bar}); its state: {logits_maxdiff(cap[1].state, live[1].state)} (stop {stops[0]}'s: {logits_maxdiff(cap[0].state, live[1].state)}, magnitude {logits_maxabs(live[1].state)})\n")
                delete live
                delete cap
                delete prompt
                select_decode_override("")
                select_prefill_override("")
            }
            metal_decode_shutdown()
            metal_prefill_shutdown()
            delete m
            set_mtp_spec(spec0)
        }
    }
}

// ===== the forms: each model file loads once a form, its cells in turn on that load =====

