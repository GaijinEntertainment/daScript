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

def private cuda_mtp_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
    set_pf_window_rows_pin(MTP_WINDOW)   // before every load of the file: the plan and the window chain read it
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures
}

[test(description = "On the CUDA driver Qwen3.5-0.8B-MTP's resident prefill, each step and each N-row batched row land the post-norm hidden carry matching the CPU chain's, and with speculation off no carry lands while logits stay within the deltanet bar")]
def test_gpu_resident_cuda_mtp_carry(t : T?) {
    cuda_mtp_intent()
    return if (!cuda_home_asked(t))
    mtp_carry_cell(t)
    mtp_carry_batched_cell(t)
    mtp_carry_gate_cell(t)
}

[test(description = "On the CUDA driver a NextN draft through the resident head matches the CPU forward_mtp on the same inputs, uploads host rows when it runs past the device's, pins away a suppressed pick, and is bit-identical despite NaN-poisoned host residual rows")]
def test_gpu_resident_cuda_mtp_draft(t : T?) {
    cuda_mtp_intent()
    return if (!cuda_home_asked(t))
    mtp_draft_cell(t)
    mtp_draft_upload_cell(t)
    mtp_draft_suppressed_cell(t)
    mtp_xb_poison_cell(t)
}

[test(description = "On the CUDA driver the same-slab verify rows sit within twice the deltanet bar of one-row steps with the same argmax, the pick-ask rounds match the logits-landing rounds, and a rejected verify rolls back on the device with the next round on the device again")]
def test_gpu_resident_cuda_mtp_verify(t : T?) {
    cuda_mtp_intent()
    return if (!cuda_home_asked(t))
    mtp_verify_cell(t)
    mtp_verify_asked_cell(t)
    mtp_verify_reject_cell(t)
    mtp_verify_reject_asked_cell(t)
}

[test(description = "On the CUDA driver the NextN head's K rows warmed by the window chain match the CPU warm's within twice the deltanet bar over one and two windows, the row one down landing past it")]
def test_gpu_resident_cuda_mtp_head_warm(t : T?) {
    cuda_mtp_intent()
    return if (!cuda_home_asked(t))
    mtp_head_warm_cell(t)
}

[test(description = "On the CUDA driver six speculative rounds draft the CPU round's tokens or a near-tie with equal accepts and positions, declined drafts hydrate head rows within the bar, and under the depth-8 knob the driver prepares nine verify rows and its greedy walk emits the plain decode")]
def test_gpu_resident_cuda_mtp_rounds(t : T?) {
    cuda_mtp_intent()
    return if (!cuda_home_asked(t))
    mtp_rounds_cell(t)
    mtp_depth8_cell(t)
}

[test(description = "On the CUDA driver a NaN carry makes the draft panic by row, f32, unhomed, gap and rewind rounds pass to the CPU rails by name (the rewind refused), and with the head off the device the verify passes as verify_rows without panic")]
def test_gpu_resident_cuda_mtp_gates(t : T?) {
    cuda_mtp_intent()
    return if (!cuda_home_asked(t))
    mtp_nonfinite_cell(t)
    mtp_pass_arms_cell(t)
    mtp_rewind_refused_cell(t)
    mtp_head_off_cell(t)
}

[test(description = "On the CUDA driver the NextN head's draft, verify, warmed rows and six rounds hold their bars over a q8_0 K/V mirror, and the verify and a forced reject hold against the driver's own one-row steps over a tq4 mirror")]
def test_gpu_resident_cuda_mtp_block_codec(t : T?) {
    cuda_mtp_intent()
    return if (!cuda_home_asked(t))
    mtp_block_codec_cell(t)
}

[test(description = "On the CUDA driver at depth 2 a sampled stream (temp 0.7, top-p 0.9) through the device's selection decodes the rows-landed stream token for token, every step's selection served on the device")]
def test_gpu_resident_cuda_mtp_sampled_selection(t : T?) {
    cuda_mtp_intent()
    return if (!cuda_home_asked(t))
    mtp_sampled_selection_cell(t)
}
