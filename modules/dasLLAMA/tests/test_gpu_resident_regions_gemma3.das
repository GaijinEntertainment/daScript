options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// gemma-3-1b-it Q8_0: the batched-step cells of the rig on the gemma-3 dense base - the sandwich norms, the sliding-window
// layers beside the global ones with their own rope class, the GeGLU FFN - so the N-row token command serves two rows
// bit for bit against the sessions alone: at one window, past the unsplit position so the split attention form
// records, and a batched step before any one-row step. Skips off the JIT, without DASLLAMA_GPU=1 on a device the
// driver serves, or without the file under models_dir().
let private REGIONS_FILE = "gemma-3-1b-it-Q8_0.gguf"

[test]
def test_regions_batched_steps_g3(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_split_g3(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}

[test]
def test_regions_batched_first_g3(t : T?) {
    cell_regions_batched_first(t, REGIONS_FILE)
}
