options gen2
options _dasllama_internal = true

module _ffn_slice_cells shared public

//! the last layer's FFN slice against the whole window on a served model; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_math          // with_dasllama_jobque_ — the cell's queue (engine-level, not re-exported)
require dasllama/dasllama_gpu_resident  // moe_gpu_resident_active — the slice cell needs the driver armed
require ?vulkan dasllama/dasllama_vulkan_prefill   // g_pf_ffn_slice — the slice's A/B arm
require ?vulkan dasllama/dasllama_vulkan_common    // g_cm2_splitk — both arms under one k order
require daslib/jobque_boost

// The last layer's FFN slice against the whole window: one 512-token prefill each way on the
// resident Vulkan driver, logits exact - the frozen fixtures' 5-20-token prompts never reach it.
// Split-k is pinned off for both arms: the wave model splits the whole arm's GEMMs on a box with
// SMs to spare (82 on the RTX PRO 4500) where a sliced region never splits, and the two k orders
// round apart - the claim is the slice's row arithmetic, not the pick's rounding.

//! on the resident Vulkan driver serving Llama-3.2-1B `t8`, a 512-token prefill with the last layer's FFN sliced yields logits
//! bit-identical to the whole-window prefill, split-k pinned off for both arms; a skip where the driver is not armed
def last_layer_slice_cell(t : T?; var t8 : Model) {
    unsafe {
        t |> run("the last layer's 32-row FFN slice reproduces the whole window's logits bit for bit") @capture(& t8)(t : T ?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!moe_gpu_resident_active()) {
                    t |> skip("the resident Vulkan driver is not armed (DASLLAMA_GPU=1 on a coopmat2 device)")
                    return
                }
                let plen = 512l
                var prompt : array<int64>
                prompt |> resize(int(plen))
                for (i in range(int(plen))) {
                    prompt[i] = 1000l + ((int64(i) * 17l + 999l * 131l) % 5000l)
                }
                var want, got : array<float>
                var served_whole = false
                var served_slice = false
                let splitk_before = g_cm2_splitk
                g_cm2_splitk = 1l
                with_dasllama_jobque_() {
                    g_pf_ffn_slice = false
                    var s0 <- make_run_state(t8.config, KVDtype.f16, KVDtype.f16)
                    forward_prefill(t8, s0, prompt, plen, 0l)
                    served_whole = s0.rdec_gen != 0l   // the driver stamps the session it served; a declined call leaves 0
                    want := s0.logits
                    delete s0
                    g_pf_ffn_slice = true
                    var s1 <- make_run_state(t8.config, KVDtype.f16, KVDtype.f16)
                    forward_prefill(t8, s1, prompt, plen, 0l)
                    served_slice = s1.rdec_gen != 0l
                    got := s1.logits
                    delete s1
                }
                g_cm2_splitk = splitk_before
                t |> success(served_whole && served_slice, "both arms were served by the resident driver, not the CPU rail")
                var bad = 0
                for (w, g in want, got) {
                    if (w != g) {
                        bad++
                    }
                }
                t |> success(length(want) == int(t8.config.vocab_size) && bad == 0,
                    "the sliced last layer reproduces the whole window ({bad} of {length(want)} logits diverge)")
                delete want
                delete got
                delete prompt
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}
