options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the umbrella's [init] registrations arm the llama arch and the CUDA home
require dasllama/dasllama_common
require daslib/jobque_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_env
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin - the FP4 arm's knob-off control
require _model_tier
require _resident_feed
require _cuda_feed

// The CUDA home's whole-model driver on Llama-3.2-1B Q8_0 (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the dense slice - one
// arena of Q8_0 planes, the f16 mirror, the token step as a captured graph, the prefill as the window chain over the
// cm2 tiles. The forced-feed logits-tolerance form of test_gpu_resident_llama: the same fixed tokens through the CUDA
// chain and the all-CPU chain, the prefill logits and every step's within the bar, the one-step-off control past it,
// at 40 tokens (one window) and at a prompt of two windows; the witnesses that the CUDA driver served every position,
// ran the windows the prompt takes and replayed the captured graph past the first step, a CPU run with the model off the
// device moving none of them (the control).
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without
// the file under models_dir(), or, on the forced-feed cells, where the prompt window is not PF_WINDOW_ROWS rows
// (`assert_pf_window`: DASLLAMA_GPU_PF_ROWS or a file's pin moved it).

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model off the device) prefills on the CPU
    if (pf_window_rows_pin() <= 0l) {   // the cells size their prompts against PF_WINDOW_ROWS; the plan's own pick would run the two-window prompt as one
        set_pf_window_rows_pin(PF_WINDOW_ROWS)
    }
}

let private FILE_1B = "Llama-3.2-1B-Instruct-Q8_0.gguf"
let private FILE_1B_K = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"   //! the K-quant planes (q4_K, q6_K): the kq GEMV leaves, the Q8_K feeds, the kq cm2 tiles
let private FILE_1B_Q51 = "Llama-3.2-1B-Instruct-Q5_1-local.gguf"   //! every dense plane q51 (the mint's recipe row in `../performance/model_specs.das`): the per-32 GEMV leaves, the s tile, the q51 table gather
let private FILE_1B_Q50 = "Llama-3.2-1B-Instruct-Q5_0-local.gguf"   //! the q5032 planes
let private FILE_1B_IQ4XS = "Llama-3.2-1B-Instruct-IQ4_XS.gguf"      //! the iq4xs planes: the codebook decode through the home's permute
let private FILE_1B_IQ4NL = "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf"   //! the iq4nl superblock planes (on the 256 lattice), the same decode
let private FILE_1B_Q40 = "Llama-3.2-1B-Instruct-Q4_0-local.gguf"       //! the q40 superblock planes: the signed nibble codes under the d pairs
let private FILE_1B_NV4 = "Llama-3.2-1B-Instruct-NVFP4-local.gguf"      //! the nv4 planes: the window on the FP4 tensor rung (W4A4 - the activations quantized to e2m1 per token), the steps on the nv4 GEMV leaves
let private FILE_3B = "Llama-3.2-3B-Instruct-Q8_0.gguf"   //! head size 128: the window's attention on the h128 flash tile

[test]
def test_gpu_resident_cuda_llama_1b(t : T?) {
    t |> run("{FILE_1B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_1B}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_llama_1b_k4(t : T?) {
    t |> run("{FILE_1B_K}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_K, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_1B_K}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_K, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_llama_1b_q51(t : T?) {
    t |> run("{FILE_1B_Q51}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_Q51, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_1B_Q51}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_Q51, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_llama_1b_q50(t : T?) {
    t |> run("{FILE_1B_Q50}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_Q50, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_llama_1b_iq4xs(t : T?) {
    t |> run("{FILE_1B_IQ4XS}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_IQ4XS, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_llama_1b_iq4nl(t : T?) {
    t |> run("{FILE_1B_IQ4NL}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_IQ4NL, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_llama_1b_q40(t : T?) {
    t |> run("{FILE_1B_Q40}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_Q40, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
}

//! the FP4 leaf's bar: W4A4's e2m1 activations are a step the f32 CPU chain does not take - the 1B's rows read up to 0.135 of the
//! step's max logit (the argmax matched; llama.cpp measures the same step at +0.7% perplexity) and the tightest one-step-off control
//! at 0.158, the bar between them; the same carrier's cm2-tiles arm below holds the dense bar, so the extra is the feed's
let private LOGIT_BAR_REL_FP4 = 0.14

[test]
def test_gpu_resident_cuda_llama_1b_nv4(t : T?) {
    t |> run("{FILE_1B_NV4}: one window (40 tokens) and eight steps on the CUDA driver, the window on the FP4 tensor leaf") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_NV4, "llama", 40l, 1l, LOGIT_BAR_REL_FP4)
    }
    t |> run("{FILE_1B_NV4}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver, the windows on the FP4 tensor leaf") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_NV4, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL_FP4)
    }
    t |> run("{FILE_1B_NV4}: one window (40 tokens) and eight steps with the MMQ knob off - the cm2 tiles over f16 activations hold the dense bar (the FP4 arm's control)") @(t : T?) {
        set_cuda_mmq_pin(0l)
        cuda_forced_feed(t, FILE_1B_NV4, "llama", 40l, 1l, LOGIT_BAR_REL)
        set_cuda_mmq_pin(-1l)
    }
}

//! the short window's peel through the N-row command on the CUDA driver (`peel_cell`): tails of 1, 2, 5 and 8 rows after a 40-row
//! opening, one row past the per-row planes and the eight-row tail with the lever off taking the window chain
[cold_path]
def private cuda_peel(t : T?; file : string) {
    let path = cuda_model_path(t, file)
    if (empty(path)) return
    var tr <- load_resident(path, 1l)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the model (the load log names any decline)")
    with_dasllama_jobque_() {
        peel_cell(t, tr, SPLIT_BAR_REL, "{file} cuda, ", true)
    }
    delete tr
}

[test]
def test_gpu_resident_cuda_llama_peel(t : T?) {
    t |> run("{FILE_1B}: a short tail peels through the N-row command on the CUDA driver and reads what the one-row steps read") @(t : T?) {
        cuda_peel(t, FILE_1B)
    }
    t |> run("{FILE_1B_K}: the same on the K-quant planes' N-column leaves and the rows' split Q8_K step") @(t : T?) {
        cuda_peel(t, FILE_1B_K)
    }
    t |> run("{FILE_1B_Q51}: the same on the per-32 planes' N-column leaves") @(t : T?) {
        cuda_peel(t, FILE_1B_Q51)
    }
}

[test]
def test_gpu_resident_cuda_llama_3b(t : T?) {
    t |> run("{FILE_3B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_3B, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_3B}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_3B, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}
