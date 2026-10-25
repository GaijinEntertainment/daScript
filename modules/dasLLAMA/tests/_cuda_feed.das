// The CUDA resident files' forced-feed cell: the same fixed tokens through the CUDA chain and the all-CPU chain, the
// prefill logits and every step's within the bar, the one-step-off control past it, with the witnesses that the CUDA
// driver served every position, ran the windows the prompt takes and replayed the captured graph past the first step,
// a CPU run with the model off the device moving none of them (the control). One body for every carrier family the
// home serves (`test_gpu_resident_cuda_llama.das`, `test_gpu_resident_cuda_phi3.das`, `test_gpu_resident_cuda_qwen3.das`).

options gen2
options _dasllama_internal = true

module _cuda_feed shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/jobque_boost
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, RD_UNSPLIT_POS
require dasllama/dasllama_cuda_decode   // cud_rdec_stats
require _model_tier
require _resident_feed

let LOGIT_BAR_REL = 0.06   //! the dense carriers' bar, of the step's max |logit|: every CUDA resident file reads it

//! `file` of family `arch` through the CUDA chain and the CPU chain over an `n`-token prompt and eight steps, at `bar` of the step's max |logit|
[cold_path]
def cuda_forced_feed(t : T?; file, arch : string; n, windows : int64; bar : float) {
    let path = cuda_model_path(t, file)
    if (empty(path) || !assert_pf_window(t)) return
    var tr <- load_resident(path, 1l)
    t |> equal(tr.arch, arch, "the file is a {arch}-family model")
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the model (the load log names any decline)")
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
