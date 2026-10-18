options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the umbrella fires each arch [init] registration (qwen35): the load needs it, not a symbol
require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_env
require ?vulkan dasllama/dasllama_vulkan_common
require _model_tier
require _resident_feed

// A long prompt through the whole-model driver at a prompt window past a recurrent window's rows: the window chain cuts a recurrent model's window at DN_WINDOW rows, the rows the deltanet smalls planes and the scan workspace hold.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm prefills on the CPU by design
}

let private HYBRID = "Qwen3.5-0.8B-Q4_K_M.gguf"
let private PROMPT = 3000l
let private WINDOW = 2048l   //! past DN_WINDOW: two deltanet chunks a window were the cap absent
let private BAR_REL = 0.10   //! the device chain against the CPU's on a plain hybrid: the arms read 3%, the broken window 30-72%

[test]
def test_gpu_resident_long_window(t : T?) {
    t |> run("Qwen3.5-0.8B: a 3000-token prompt at a 2048-row prompt window against the CPU chain - the recurrent window cut at DN_WINDOW") @(t : T?) {
        let path = path_join(models_dir(), HYBRID)
        if (!resident_ready(t, path)) return
        set_pf_window_rows_pin(WINDOW)
        var tr <- load_resident(path)
        t |> success(tr.config.recr_mask != 0ul, "the carrier is the hybrid whose recurrent window the cap bounds")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on it (the load log names any decline)")
        t |> success(WINDOW > DN_WINDOW, "the window asked passes DN_WINDOW ({WINDOW} over {DN_WINDOW}): the cut is what the cell reads")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(PROMPT)
            var short <- [for (i in range64(PROMPT - 1l)); prompt[i]]
            var sd <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sd, prompt, PROMPT, 0l)
            var dev <- [for (x in sd.logits); x]
            delete sd
            moe_gpu_drop_model()
            var sh <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sh, prompt, PROMPT, 0l)
            var cpu <- [for (x in sh.logits); x]
            delete sh
            var sx <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sx, short, PROMPT - 1l, 0l)
            var control <- [for (x in sx.logits); x]
            delete sx
            let bar = row_bar(cpu, BAR_REL)
            control_past(t, "the prompt's last row", cpu, control, bar)
            row_within(t, "the device's last row at a {WINDOW}-row window against the CPU chain", dev, cpu, bar)
            delete control
            delete cpu
            delete dev
            delete short
            delete prompt
        }
        delete tr
        set_pf_window_rows_pin(0l)
    }
}
