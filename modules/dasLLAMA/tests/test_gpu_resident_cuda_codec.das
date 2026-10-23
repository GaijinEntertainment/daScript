options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // the umbrella's [init] registrations arm the llama arch and the CUDA home
require dasllama/dasllama
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_env
require dasllama/dasllama_kv_dtype
require _model_tier
require _resident_feed
require _kv_codec_cells

// The CUDA home's whole-model driver under a q8_0 K/V mirror (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the rope stamps stage the
// token's row, the codec's store quantizes it into the mirror, the decode attention's q8_0 stamps score the blocks, the window
// chain stores its rows and attends the f16 shadow - the codec rig's forced-feed form (`_kv_codec_cells`) on Llama-3.2-1B
// Q8_0 at one window and two, and on the Q4_K_M file, each at the 10% bar the Vulkan carriers hold (both chains quantize each
// query, each its own, so two roundings part). Every cell skips off the JIT, without DASLLAMA_CUDA=1, without the armed tier,
// without the file under models_dir(), or under DASLLAMA_VK_KV32=1 (the f32 mirror armed over the codec the cell asks).

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model off the device) prefills on the CPU
    if (pf_window_rows_pin() <= 0l) {   // the cells size their prompts against PF_WINDOW_ROWS; the plan's own pick would run the two-window prompt as one
        set_pf_window_rows_pin(PF_WINDOW_ROWS)
    }
}

let private FILE_1B = "Llama-3.2-1B-Instruct-Q8_0.gguf"
let private FILE_1B_K = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"
let private Q8KV_BAR_REL = 0.10   //! of the step's max |logit|: the Vulkan codec carriers' bar

def private cuda_codec_cell(t : T?; file : string; n : int64) {
    return if (!cuda_home_asked(t))
    cell_codec_forced_feed(t, file, n, KVDtype.q8_0, Q8KV_BAR_REL)
}

[test]
def test_gpu_resident_cuda_q8kv(t : T?) {
    t |> run("{FILE_1B}: a q8_0 mirror, one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_codec_cell(t, FILE_1B, 40l)
    }
    t |> run("{FILE_1B}: a q8_0 mirror, a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_codec_cell(t, FILE_1B, PF_WINDOW_ROWS + 88l)
    }
    t |> run("{FILE_1B_K}: a q8_0 mirror, one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_codec_cell(t, FILE_1B_K, 40l)
    }
}

//! tq4 in the distance form (no logit bar holds a 4-bit mirror): the device at tq4 against the f16 chain within `TQ4_DIST_REL` of
//! the CPU tq4 chain's own distance, the two tq4 chains within `TQ4_PAIR_REL` of it - the Vulkan llama codec file's bars
let private TQ4_DIST_REL = 1.15
let private TQ4_PAIR_REL = 1.4

[test]
def test_gpu_resident_cuda_tq4kv(t : T?) {
    t |> run("{FILE_1B_K}: a tq4 mirror, the distance form over 40 tokens and eight steps on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        cell_codec_distance(t, FILE_1B_K, 40l, KVDtype.tq4, false, TQ4_DIST_REL, TQ4_PAIR_REL)
    }
}
