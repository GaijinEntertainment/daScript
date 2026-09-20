options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen3-0.6B Q8_0: the batched-step cells of the rig on a q/k-norm carrier, so the N-row token command's per-head
// q/k rms over the rows' projection rows serves two rows against the sessions alone, bit for bit - at one window,
// and past the unsplit position so the split attention form records. Skips off the JIT, without DASLLAMA_GPU=1 on
// a device the driver serves, or without the file under models_dir().
let private REGIONS_FILE = "Qwen3-0.6B-Q8_0.gguf"

[test]
def test_regions_batched_steps_qwen3(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_split_qwen3(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}
