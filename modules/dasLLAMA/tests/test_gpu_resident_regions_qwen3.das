options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen3-0.6B Q8_0: the rig's step cells on a q/k-norm carrier - the interleaved single steps, the batched steps at one
// window and past the unsplit position (the N-row token command's q/k norm in the form the one-row command takes,
// bit for bit against the sessions alone), a batched step before any one-row step, the single steps past the wide
// position where the wide twin records, and DASLLAMA_VK_FUSE_BISECT a bit at a time within the split bar of the fused
// run. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir(); the
// fuse-bisect and the fused q/k norm cells skip under DASLLAMA_CUDA=1 - the fuse knob is the Vulkan driver's.
let private REGIONS_FILE = "Qwen3-0.6B-Q8_0.gguf"

[test(description = "On the q/k-norm Qwen3-0.6B two sessions in separate regions, stepped one row at a time interleaved, each read what that session reads alone")]
def test_regions_interleaved_single_steps_qwen3(t : T?) {
    cell_regions_interleaved_single_steps(t, REGIONS_FILE)
}

[test(description = "On Qwen3-0.6B two sessions past the wide position take the token command's wide twin on one-row steps, each reading what that session reads alone")]
def test_regions_wide_single_steps_qwen3(t : T?) {
    cell_regions_wide_single_steps(t, REGIONS_FILE)
}

[test(description = "On Qwen3-0.6B a batched step before any one-row step, one-row steps, then a batched step again all read what each session reads alone")]
def test_regions_batched_first_qwen3(t : T?) {
    cell_regions_batched_first(t, REGIONS_FILE)
}

[test(description = "On Qwen3-0.6B every DASLLAMA_VK_FUSE_BISECT bit serves one-row logits within the split bar of the fused run, the one-token-off control outside, and the split-pair rows batch bit for bit")]
def test_regions_fuse_bisect_qwen3(t : T?) {
    cell_regions_fuse_bisect(t, REGIONS_FILE)
}

[test(description = "On Qwen3-0.6B the N-row command's q/k norm in the one-row form batches two sessions bit for bit what each session reads alone")]
def test_regions_batched_steps_qwen3(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test(description = "On Qwen3-0.6B with both prompts past the unsplit position, the split N-row command batches two sessions bit for bit what each reads alone")]
def test_regions_batched_steps_split_qwen3(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}

[test(description = "On Qwen3-0.6B the window's fused q/k norm, rope and store pass reads within the split bar of the split pair over a two-window prompt and fed steps, the census counting each form's own stamp")]
def test_window_qkn_fused(t : T?) {
    cell_window_qkn_fused(t, REGIONS_FILE)
}
