options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// gemma-4-12B-it Q8_0 (large tier, DASLLAMA_PARITY_FULL=1): the batched-step cells of the rig on a carrier whose
// global layers share the K/V of the layers below them (the N-row command's Q-only form over the donor's mirror
// rows) and whose classifier epilogue caps the logits and pins the suppressed ids, so the N-row token command
// serves two rows bit for bit against the sessions alone - at one window and past the unsplit position. Skips
// off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private REGIONS_FILE = "gemma-4-12B-it-Q8_0.gguf"

[test]
def test_regions_batched_steps_g4(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_split_g4(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}
