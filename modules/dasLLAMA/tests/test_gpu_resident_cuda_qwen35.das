options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the umbrella's [init] registrations arm the qwen35 arch and the CUDA home
require dasllama/dasllama_common
require daslib/jobque_boost
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_cuda_decode   // CUDA_MMQ_WINNERS / CUDA_MMQ_OFF: the pinned MMQ routes
require dasllama/dasllama_kv_dtype
require _model_tier
require _resident_feed
require _cuda_feed
require _kv_codec_cells

// The CUDA home's whole-model driver on the deltanet hybrid Qwen3.5-0.8B (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the recurrent
// layers through the window chain's conv, scan, gated out-norm and ring tail over the layer's device state slot, then the
// fused deltanet step a token; the attention layers through the gated-query arm of the fused q/k norm, rope and K/V store
// and the attention kernels. The forced-feed form the CUDA files share (`_cuda_feed.das`) at the deltanet bar the Vulkan
// hybrid file holds (`test_gpu_resident_hybrid.das`): the Q8_0 file over one window and two (the conv tail and the state
// carry across the window seam), the 4B shape, the Q4_K_M and q8out twins (the deltanet planes in their file formats), the
// NextN-headed trunk (the head stays off this home) and the NVFP4 twin (the nv4 planes under their `.scale` factors on the
// FP4 leaf, the cm2 arm as its control). The NVFP4 and K-quant cells pin the MMQ route (`set_cuda_mmq_pin`: the leaf arms
// `CUDA_MMQ_WINNERS`, the control `CUDA_MMQ_OFF`, the pin put back as found) and name the pin in their labels. Every cell
// skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without the file under
// models_dir(), or where the prompt window is not PF_WINDOW_ROWS rows; the two NVFP4 leaf cells also skip on a card below the
// FP4 rung (`FP4_RUNG_SM`, compute capability 120), where the nv4 planes stay on the cm2 tiles.

let private FILE_Q8 = "Qwen3.5-0.8B-Q8_0.gguf"   //! the deltanet hybrid: recurrent layers at ds 128, gated attention layers at head 256 with the per-head q/k norm and the partial rope
let private FILE_4B = "Qwen3.5-4B-Q8_0.gguf"   //! the larger shape: 32 layers over 2560, the same head geometry
let private FILE_KQ = "Qwen3.5-0.8B-Q4_K_M.gguf"   //! the K-quant twin: the deltanet qkv, z and out planes in their file formats
let private FILE_KQ_Q8OUT = "Qwen3.5-0.8B-Q4_K_M-q8out.gguf"   //! the mixed twin: K-quant qkv and z planes beside a Q8_0 out plane
let private FILE_MTP = "Qwen3.5-0.8B-MTP-Q8_0.gguf"   //! the NextN-headed twin: the trunk serves, the head stays off this home (no draft seat), so a plain decode is the same chain
let private FILE_NV4 = "Qwen3.5-0.8B-NVFP4-MTP.gguf"   //! the NVFP4 twin: nv4 qkv, z and out planes under their `.scale` factors beside K-quant and bf16 layers - the deltanet sites on the FP4 tensor leaf
let private DN_LOGIT_BAR_REL = 0.04   //! of the step's max |logit| - the deltanet bar the Vulkan hybrid file holds
let private DN_LOGIT_BAR_REL_KQ = 0.10   //! the K-quant twins' bar off the f16 feed (the Vulkan hybrid file's quant-feed bar): the window's K-quant sites read the int8 MMQ leaves' Q8_0 feed here
let private DN_LOGIT_BAR_REL_FP4 = 0.20   //! the FP4 leaf's W4A4 bar on the hybrid: the file's rows read 0.139 (one window) and 0.134 (two) of the step's max logit on the leaf and 0.040 / 0.030 on the cm2 tiles (the control arm, at the K-quant bar), the tightest one-step-off control at 0.45 (RTX 5060 Ti, `-jit`, the MMQ pin on; the 2026-10-26 PERF_LEDGER entry) - the extra is the e2m1 feed's rounding, which the recurrence carries into every step

//! the gated carrier under a block codec on this home - the Vulkan hybrid file's bars: the q8_0 mirror against the CPU chain
//! on q8_0 sessions; under tq4 the device's summed distance from the f16 chain against the CPU tq4 chain's own, and the two
//! tq4 chains' distance apart (the un-rotation applies the head gate: `crd_q_head_stride` feeds the tq4 store)
let private GATED_Q8KV_BAR_REL = 0.06
let private GATED_CODEC_DIST_REL = 1.15
let private GATED_CODEC_PAIR_REL = 0.8

[test]
def test_gpu_resident_cuda_qwen35_block_codecs(t : T?) {
    t |> run("{FILE_Q8}: a q8_0 K/V mirror under the gated attention, one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        cell_codec_forced_feed(t, FILE_Q8, 40l, KVDtype.q8_0, GATED_Q8KV_BAR_REL)
    }
    t |> run("{FILE_Q8}: a tq4 K/V mirror under the gated attention - the distance form over 40 tokens and eight steps on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        cell_codec_distance(t, FILE_Q8, 40l, KVDtype.tq4, false, GATED_CODEC_DIST_REL, GATED_CODEC_PAIR_REL)
    }
}

[test]
def test_gpu_resident_cuda_qwen35_q8(t : T?) {
    t |> run("{FILE_Q8}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_Q8, "qwen35", 40l, 1l, DN_LOGIT_BAR_REL)
    }
    t |> run("{FILE_Q8}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_Q8, "qwen35", PF_WINDOW_ROWS + 88l, 2l, DN_LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_qwen35_4b(t : T?) {
    t |> run("{FILE_4B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_4B, "qwen35", 40l, 1l, DN_LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_qwen35_mtp_trunk(t : T?) {
    t |> run("{FILE_MTP}: the trunk on the CUDA driver with the head off, one window (40 tokens) and eight plain steps") @(t : T?) {
        cuda_forced_feed(t, FILE_MTP, "qwen35", 40l, 1l, DN_LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_qwen35_nv4(t : T?) {
    t |> run("{FILE_NV4}: one window (40 tokens) and eight steps on the CUDA driver under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)}), the nv4 deltanet sites on the FP4 tensor leaf under the planes' factors") @(t : T?) {
        return if (!fp4_rung_on(t))
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_NV4, "qwen35", 40l, 1l, DN_LOGIT_BAR_REL_FP4)
        }
    }
    t |> run("{FILE_NV4}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)}), the FP4 tensor leaf") @(t : T?) {
        return if (!fp4_rung_on(t))
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_NV4, "qwen35", PF_WINDOW_ROWS + 88l, 2l, DN_LOGIT_BAR_REL_FP4)
        }
    }
    t |> run("{FILE_NV4}: one window (40 tokens) and eight steps under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_OFF)}) - the cm2 tiles over f16 activations hold the K-quant bar (the FP4 arm's control)") @(t : T?) {
        with_cuda_mmq_pin(CUDA_MMQ_OFF) {
            cuda_forced_feed(t, FILE_NV4, "qwen35", 40l, 1l, DN_LOGIT_BAR_REL_KQ)
        }
    }
}

//! the K-quant twins' window sites on the int8 MMQ leaves (`set_cuda_mmq_pin(CUDA_MMQ_WINNERS)`): the bar is the Q8_0 feed's
[test]
def test_gpu_resident_cuda_qwen35_kq(t : T?) {
    t |> run("{FILE_KQ}: one window (40 tokens) and eight steps on the CUDA driver under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)})") @(t : T?) {
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_KQ, "qwen35", 40l, 1l, DN_LOGIT_BAR_REL_KQ)
        }
    }
    t |> run("{FILE_KQ}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)})") @(t : T?) {
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_KQ, "qwen35", PF_WINDOW_ROWS + 88l, 2l, DN_LOGIT_BAR_REL_KQ)
        }
    }
    t |> run("{FILE_KQ_Q8OUT}: one window (40 tokens) and eight steps on the CUDA driver under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)})") @(t : T?) {
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_KQ_Q8OUT, "qwen35", 40l, 1l, DN_LOGIT_BAR_REL_KQ)
        }
    }
}
