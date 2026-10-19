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

// A long prompt through the whole-model driver on a recurrent model: the window chain cuts a recurrent model's window at
// DN_WINDOW rows, the rows the deltanet smalls planes and the scan workspace hold, where a window past them copied its rows
// past the planes into the next. One cell reads the default window, which is DN_WINDOW itself and runs uncut; the other a
// window pinned past it, which the cut bounds. The pin is read back as the witness that the driver saw it, and the window
// chain's prefill count that the device served the prompt. Skips: no file, no armed tier.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU reference and its control prefill on the CPU by design
}

let private HYBRID = "Qwen3.5-0.8B-Q4_K_M.gguf"
let private PROMPT = 3000l
let private PAST_WINDOW = 3072l   //! past DN_WINDOW: two deltanet chunks a window were the cap absent
let private BAR_REL = 0.10   //! the device chain against the CPU's on a plain hybrid: the arms read 3%; a window past the planes read 30-72%

//! the 3000-token prompt at a `window`-row prompt window: the device's last row against the CPU chain's, the one-token-short control past the bar
def private long_prompt_cell(t : T?; window : int64) {
    let path = path_join(models_dir(), HYBRID)
    if (!resident_ready(t, path)) return
    let pin_was = pf_window_rows_pin()
    set_pf_window_rows_pin(window)
    t |> equal(pf_window_rows(), window, "the pin reached the driver's window")
    var tr <- load_resident(path)
    t |> success(tr.config.recr_mask != 0ul, "the carrier is the hybrid whose recurrent window the cap bounds")
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on it (the load log names any decline)")
    with_dasllama_jobque_() {
        var prompt <- resident_prompt(PROMPT)
        var control_prompt <- [for (i in range64(PROMPT - 1l)); prompt[i]]
        let served0 = moe_gpu_resident_prefills()
        var sd <- create_session(tr, mirror_kv_dtype())
        forward_prefill(tr, sd, prompt, PROMPT, 0l)
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the window chain served the prompt")
        served_every_call(t)
        var dev <- [for (x in sd.logits); x]
        delete sd
        moe_gpu_drop_model()
        var sh <- create_session(tr, mirror_kv_dtype())
        forward_prefill(tr, sh, prompt, PROMPT, 0l)
        var cpu <- [for (x in sh.logits); x]
        delete sh
        var sx <- create_session(tr, mirror_kv_dtype())
        forward_prefill(tr, sx, control_prompt, PROMPT - 1l, 0l)
        var control <- [for (x in sx.logits); x]
        delete sx
        let bar = row_bar(cpu, BAR_REL)
        control_past(t, "the prompt's last row", cpu, control, bar)
        row_within_say(t, tr, "the device's last row at a {window}-row window against the CPU chain", dev, cpu, bar)
        delete control
        delete cpu
        delete dev
        delete control_prompt
        delete prompt
    }
    delete tr
    set_pf_window_rows_pin(pin_was)
}

[test]
def test_gpu_resident_long_window(t : T?) {
    t |> run("Qwen3.5-0.8B: a 3000-token prompt at the default prompt window against the CPU chain - the recurrent window uncut at DN_WINDOW") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> equal(PF_WINDOW_ROWS, DN_WINDOW, "the default window is one deltanet chunk ({PF_WINDOW_ROWS} rows, DN_WINDOW {DN_WINDOW}): the window runs uncut")
        }
        long_prompt_cell(t, PF_WINDOW_ROWS)
    }
    t |> run("Qwen3.5-0.8B: a 3000-token prompt at a {PAST_WINDOW}-row prompt window against the CPU chain - the recurrent window cut at DN_WINDOW") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> success(PAST_WINDOW > DN_WINDOW, "the window asked passes DN_WINDOW ({PAST_WINDOW} over {DN_WINDOW}): the cut is what the cell reads")
        }
        long_prompt_cell(t, PAST_WINDOW)
    }
}
