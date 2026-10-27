// The CUDA resident files' forced-feed cell: the same fixed tokens through the CUDA chain and the all-CPU chain, the
// prefill logits and every step's within the bar, the one-step-off control past it, with the witnesses that the CUDA
// driver served every position, ran the windows the prompt takes and replayed the captured graph past the first step,
// a CPU run with the model off the device moving none of them (the control). One body for every carrier family the
// home serves (`test_gpu_resident_cuda_llama.das`, `test_gpu_resident_cuda_phi3.das`, `test_gpu_resident_cuda_qwen3.das`).
// Requiring it declares the CUDA resident files' CPU-prefill intent and pins their prompt window (its `[init]`); it also
// carries the MMQ route pin and the FP4 rung's skip the FP4 cells take.

options gen2
options _dasllama_internal = true

module _cuda_feed shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/jobque_boost
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, RD_UNSPLIT_POS
require dasllama/dasllama_cuda_decode   // cud_rdec_stats, set_cuda_mmq_pin / cuda_mmq_pin
require dasllama/dasllama_cuda_common   // cud_init, g_cud.sm, FP4_RUNG_SM: the FP4 rung's skip
require dasllama/dasllama_common   // allow_cpu_prefill
require dasllama/dasllama_env   // set_pf_window_rows_pin
require _model_tier
require _resident_feed

//! the CUDA resident files' intent: the control arm (the model off the device) prefills on the CPU, and the prompt window is
//! PF_WINDOW_ROWS rows where nothing pinned one - the cells size their prompts against it, and the plan's own pick would run the
//! two-window prompt as one
[init]
def private cuda_feed_init() {
    allow_cpu_prefill()
    if (pf_window_rows_pin() <= 0l) {
        set_pf_window_rows_pin(PF_WINDOW_ROWS)
    }
}

let LOGIT_BAR_REL = 0.06   //! the dense carriers' bar, of the step's max |logit|: every CUDA resident file reads it

//! the DASLLAMA_CUDA_MMQ value `v` by its name, for a cell's label
def cuda_mmq_name(v : int64) : string {
    return "CUDA_MMQ_OFF" if (v == CUDA_MMQ_OFF)
    return "CUDA_MMQ_WINNERS" if (v == CUDA_MMQ_WINNERS)
    return "CUDA_MMQ_ALL" if (v == CUDA_MMQ_ALL)
    return "CUDA_MMQ_UNPINNED"
}

//! `blk` with the MMQ route pinned to `v` (`set_cuda_mmq_pin`), the pin put back to what `cuda_mmq_pin` read before
def with_cuda_mmq_pin(v : int64; blk : block<() : void>) {
    let was = cuda_mmq_pin()
    set_cuda_mmq_pin(v)
    invoke(blk)
    set_cuda_mmq_pin(was)
}

//! true where the CUDA home is asked for and its card carries the FP4 tensor rung; else the cell has registered the skip
//! naming why and its caller returns
def fp4_rung_on(t : T?) : bool {
    return false if (!cuda_home_asked(t))
    if (!cud_init()) {
        t |> skip("no CUDA device the home arms (the cuda tier's log names why)")
        return false
    }
    if (g_cud.sm < FP4_RUNG_SM) {
        t |> skip("the FP4 tensor rung needs compute capability {FP4_RUNG_SM}: this card is {g_cud.sm}")
        return false
    }
    return true
}

//! `file` of family `arch` through the CUDA chain and the CPU chain over an `n`-token prompt and eight steps, at `bar` of the step's max |logit|
[cold_path]
def cuda_forced_feed(t : T?; file, arch : string; n, windows : int64; bar : float) {
    let path = cuda_model_path(t, file)
    if (empty(path) || !assert_pf_window(t)) return
    var tr <- load_resident(path, 1l)
    t |> equal(tr.arch, arch, "the file is a {arch}-family model")
    return if (!resident_admitted(t, "the whole-model driver armed on the model (the load log names any decline)", ""))
    let before = cud_rdec_stats()
    with_dasllama_jobque_() {
        forced_feed_over(t, tr, "{file} cuda ", n, 8l, bar) $ {
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call of the resident run: {passes}")
            delete passes
            let st = cud_rdec_stats()
            t |> success(st.tokens >= n + 8l, "the CUDA driver served every position of the prompt and every fed token ({st.tokens} positions)")
            t |> equal(st.windows - before.windows, windows, "the prompt ran as {windows} window chain(s) on the CUDA driver")
            t |> success(st.graph, "the token step replays as a captured graph past the first step")
            let unsplit_want = st.unsplit_form && n + 8l <= RD_UNSPLIT_POS ? 8l : 0l
            t |> equal(st.unsplit - before.unsplit, unsplit_want, "the attention's unsplit form served {unsplit_want} of the fed steps (the keys {n + 8l <= RD_UNSPLIT_POS ? "under" : "past"} the unsplit position{st.unsplit_form ? "" : "; the device splits no keys"})")
        }
        var prompt <- resident_prompt(40l)
        var toks <- resident_feed(8l)
        let off0 = cud_rdec_stats()
        var cpu_rows <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
        let off1 = cud_rdec_stats()
        t |> equal(off1.tokens, off0.tokens, "the counters' control: a CPU run with the model off the device serves no position on the CUDA driver")
        t |> equal(off1.windows, off0.windows, "the counters' control: ... and runs no window chain on it")
        t |> success(!off1.graph, "the counters' control: ... and holds no captured graph")
        delete cpu_rows
        delete prompt
        delete toks
    }
    delete tr
}
