options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// gpt-oss-20b mxfp4 (large tier, DASLLAMA_PARITY_FULL=1): the batched-step cells of the rig on a MoE carrier - the N-row
// command's routed block (the router's columns, the per-row top-k, the expert GEMVs over the rows' slots, the combine per
// row) on the native-MXFP4 expert stacks with the biased router, the biased act and the attention sinks - so two rows step
// bit for bit against the sessions alone, at one window, past the unsplit position, and a batched step before any
// one-row step. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under
// models_dir().
let private REGIONS_FILE = "gpt-oss-20b-mxfp4.gguf"

[test]
def test_regions_batched_steps_gptoss(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_split_gptoss(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}

[test]
def test_regions_batched_first_gptoss(t : T?) {
    cell_regions_batched_first(t, REGIONS_FILE)
}
