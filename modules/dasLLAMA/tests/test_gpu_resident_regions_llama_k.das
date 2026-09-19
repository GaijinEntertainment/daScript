options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Llama-3.2-1B-Instruct Q4_K_M: the batched-step cells of the rig on a K-quant carrier, so the N-row token
// command's K-quant GEMV leaves and its split Q8_K sites serve two rows against the sessions alone - at one
// window, and past the unsplit position so the split attention form records. The rows hold the split bar with
// the one-token-off control, not bit for bit: the N-row command's Q8_K sites round apart from the one-row
// command's (`../followup_vulkan.md` item 75). Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver
// serves, or without the file under models_dir().
let private REGIONS_FILE = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"

[test]
def test_regions_batched_steps_k(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE, false)
}

[test]
def test_regions_batched_steps_split_k(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE, false)
}
