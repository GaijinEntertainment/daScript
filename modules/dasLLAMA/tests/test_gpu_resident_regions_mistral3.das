options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Mistral-Small-3.1-24B-Instruct Q4_K_M: the batched-step cells of the rig on the mistral3 family (large tier,
// DASLLAMA_PARITY_FULL=1) on a K-quant carrier, so the N-row token command's K-quant GEMV leaves and its split Q8_K
// sites serve two rows against the sessions alone, at one window and past the unsplit position. The rows hold the
// split bar with the one-token-off control, not bit for bit (`../followup_vulkan.md` item 75). Skips off the JIT,
// without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private REGIONS_FILE = "Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf"

[test]
def test_regions_batched_steps_m3(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE, LogitsBar.split_bar)
}

[test]
def test_regions_batched_steps_split_m3(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE, LogitsBar.split_bar)
}
