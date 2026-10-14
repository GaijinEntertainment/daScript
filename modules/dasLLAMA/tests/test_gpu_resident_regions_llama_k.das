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
// serves, without the file under models_dir(), or on a card whose room the plan does not fit at the regions a cell asks
// (twelve and twenty for the groups cells).
let private REGIONS_FILE = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"

[test]
def test_regions_batched_steps_k(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE, LogitsBar.split_bar)
}

[test]
def test_regions_batched_steps_split_k(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE, LogitsBar.split_bar)
}

//! twelve rows over twelve regions: one command, its GEMVs in a tile of eight columns and one of four
[test]
def test_regions_batched_groups_k(t : T?) {
    cell_regions_batched_groups(t, REGIONS_FILE, 12l)
}

//! twenty rows over twenty regions: past the N-row command's sixteen the step runs two commands of ten, never a row at a time
[test]
def test_regions_batched_groups_wide_k(t : T?) {
    cell_regions_batched_groups(t, REGIONS_FILE, 20l)
}
