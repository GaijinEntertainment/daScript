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
// (twelve and twenty for the groups cells). The groups cells run with the CUDA home's rows tensor leaf off - the column
// tiles on every home - and the leaf cell holds the same twelve rows on the leaf, skipping on a home with none.
let private LLAMA_K_REGIONS_FILE = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"

[test(description = "On Llama-3.2-1B Q4_K_M the N-row command's K-quant GEMV leaves and split Q8_K sites batch two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_k(t : T?) {
    cell_regions_batched_steps(t, LLAMA_K_REGIONS_FILE, LogitsBar.split_bar)
}

[test(description = "On Llama-3.2-1B Q4_K_M past the unsplit position, the split N-row command batches two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_split_k(t : T?) {
    cell_regions_batched_steps_split(t, LLAMA_K_REGIONS_FILE, LogitsBar.split_bar)
}

//! twelve rows over twelve regions: one command, its GEMVs in a tile of eight columns and one of four
[test(description = "Twelve Llama-3.2-1B Q4_K_M sessions batched in one N-row command run its GEMVs in eight- and four-column tiles, and every row holds the split bar of its session alone")]
def test_regions_batched_groups_k(t : T?) {
    cell_regions_batched_groups(t, LLAMA_K_REGIONS_FILE, 12l)
}

[test(description = "Twelve Llama-3.2-1B Q4_K_M sessions step on the CUDA home's rows tensor leaf over its Q4_K and Q6_K planes, a leaf stamp launched, every row nearer its session alone than its previous step")]
def test_regions_batched_leaf_k(t : T?) {
    cell_regions_batched_leaf(t, LLAMA_K_REGIONS_FILE, 12l)
}

//! twenty rows over twenty regions: past the N-row command's sixteen the step runs two commands of ten, never a row at a time
[test(description = "Twenty Llama-3.2-1B Q4_K_M sessions, past the N-row command's sixteen, step as two commands of ten, never a row at a time, every row within the split bar of its session alone")]
def test_regions_batched_groups_wide_k(t : T?) {
    cell_regions_batched_groups(t, LLAMA_K_REGIONS_FILE, 20l)
}

// Phi-3.5-mini-instruct Q4_K_M: the batched-step cells of the rig on the phi3 family - the fused qkv plane, the fused
// gate-up plane, the long-rope factors - on a K-quant carrier, so the N-row token command's K-quant GEMV leaves and
// its split Q8_K sites serve two rows against the sessions alone, at one window and past the unsplit position. The
// rows hold the split bar with the one-token-off control, not bit for bit (`../followup_vulkan.md` item 75). Skips
// off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private PHI3_REGIONS_FILE = "Phi-3.5-mini-instruct-Q4_K_M.gguf"

[test(description = "On Phi-3.5-mini Q4_K_M with fused qkv and gate-up planes and long-rope factors, a batched step over two sessions holds the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_phi3(t : T?) {
    cell_regions_batched_steps(t, PHI3_REGIONS_FILE, LogitsBar.split_bar)
}

[test(description = "On Phi-3.5-mini Q4_K_M past the unsplit position, the split N-row command batches two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_split_phi3(t : T?) {
    cell_regions_batched_steps_split(t, PHI3_REGIONS_FILE, LogitsBar.split_bar)
}
