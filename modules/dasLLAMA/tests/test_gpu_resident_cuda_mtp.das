options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the umbrella's [init] registrations arm the qwen35 arch and the CUDA home
require dasllama/dasllama_common
require dasllama/dasllama_env   // set_pf_window_rows_pin: the cells' 512-row window
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin: the file's MMQ route
require _resident_feed
require _mtp_resident_cells

// The NextN draft head on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`), the cells the Vulkan hybrid
// file runs on its home (`_mtp_resident_cells.das`): the carry the token step and the window chain land, the draft command
// against the CPU `forward_mtp`, the same-slab verify's rows against one-row steps of the same tokens - the recurrent layers
// stepping the rows one at a time with the rollback copies between -, the rejected verify's rollback on the device, the head's
// prompt warm on the window chain, six speculative rounds against the CPU round, the depth-8 verify, and the gates around the
// round; every load under the MMQ route pinned `CUDA_MMQ_WINNERS` (the file's `[init]`). Every cell skips interpreted, without
// DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, or without Qwen3.5-0.8B-MTP-Q8_0.gguf under
// models_dir(); the head's embed-gather compare in the verify cell skips on this home (the counters are the Vulkan driver's);
// the two-window head-warm cell skips where the prompt window is not the file's 512 rows; the pass cells' codec arm skips
// under DASLLAMA_VK_KV32=1 (the mirror is f32 there), and the block-codec cell skips under the same knob, which arms the f32
// mirror over the codec asked.

[init]
def private cuda_mtp_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
    set_pf_window_rows_pin(MTP_WINDOW)   // before every load of the file: the plan and the window chain read it
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures
}

[test]
def test_gpu_resident_cuda_mtp_carry(t : T?) {
    return if (!cuda_home_asked(t))
    mtp_carry_cell(t)
    mtp_carry_batched_cell(t)
    mtp_carry_gate_cell(t)
}

[test]
def test_gpu_resident_cuda_mtp_draft(t : T?) {
    return if (!cuda_home_asked(t))
    mtp_draft_cell(t)
    mtp_draft_upload_cell(t)
    mtp_draft_suppressed_cell(t)
    mtp_xb_poison_cell(t)
}

[test]
def test_gpu_resident_cuda_mtp_verify(t : T?) {
    return if (!cuda_home_asked(t))
    mtp_verify_cell(t)
    mtp_verify_asked_cell(t)
    mtp_verify_reject_cell(t)
    mtp_verify_reject_asked_cell(t)
}

[test]
def test_gpu_resident_cuda_mtp_head_warm(t : T?) {
    return if (!cuda_home_asked(t))
    mtp_head_warm_cell(t)
}

[test]
def test_gpu_resident_cuda_mtp_rounds(t : T?) {
    return if (!cuda_home_asked(t))
    mtp_rounds_cell(t)
    mtp_depth8_cell(t)
}

[test]
def test_gpu_resident_cuda_mtp_gates(t : T?) {
    return if (!cuda_home_asked(t))
    mtp_nonfinite_cell(t)
    mtp_pass_arms_cell(t)
    mtp_rewind_refused_cell(t)
    mtp_head_off_cell(t)
}

[test]
def test_gpu_resident_cuda_mtp_block_codec(t : T?) {
    return if (!cuda_home_asked(t))
    mtp_block_codec_cell(t)
}

[test]
def test_gpu_resident_cuda_mtp_sampled_selection(t : T?) {
    return if (!cuda_home_asked(t))
    mtp_sampled_selection_cell(t)
}
