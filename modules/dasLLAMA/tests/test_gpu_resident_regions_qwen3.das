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
// run. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private REGIONS_FILE = "Qwen3-0.6B-Q8_0.gguf"

[test]
def test_regions_interleaved_single_steps_qwen3(t : T?) {
    cell_regions_interleaved_single_steps(t, REGIONS_FILE)
}

[test]
def test_regions_wide_single_steps_qwen3(t : T?) {
    cell_regions_wide_single_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_first_qwen3(t : T?) {
    cell_regions_batched_first(t, REGIONS_FILE)
}

[test]
def test_regions_fuse_bisect_qwen3(t : T?) {
    cell_regions_fuse_bisect(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_qwen3(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_split_qwen3(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}
