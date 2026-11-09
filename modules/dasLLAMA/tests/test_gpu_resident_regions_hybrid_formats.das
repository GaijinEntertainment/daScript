options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen3.5-9B-MTP UD-Q5_K_XL (large tier, DASLLAMA_PARITY_FULL=1): the batched-step cells of the rig on a deltanet hybrid
// whose recurrent projections are K-quant planes, so the N-row command's recurrent form takes the rows' Q8_K feed at
// every recurrent head - the site the rows' next-site stamp must leave unfused - beside the gated q over the rows. The
// rows hold the split bar with the one-token-off control, not bit for bit (`../followup_vulkan.md` item 75). Skips
// off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private K_REGIONS_FILE = "Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf"

[test(description = "On the Qwen3.5-9B K-quant hybrid the N-row recurrent form with its Q8_K feed at every recurrent head batches two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_hybrid_k(t : T?) {
    cell_regions_batched_steps(t, K_REGIONS_FILE, LogitsBar.split_bar)
}

[test(description = "On the Qwen3.5-9B K-quant hybrid past the unsplit position, the split N-row command batches two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_split_hybrid_k(t : T?) {
    cell_regions_batched_steps_split(t, K_REGIONS_FILE, LogitsBar.split_bar)
}

[test(description = "On the Qwen3.5-9B-MTP K-quant hybrid an opening moved down to the host pool and up again carries the draft head's rows, and the fresh session drafts what the session that went down drafts")]
def test_device_pool_head_rows_hybrid_k(t : T?) {
    cell_device_pool_head_rows(t, K_REGIONS_FILE)   // the head's K-quant planes through the pool's moves
}

// Qwen3.8-27B i1-IQ3_S (large tier, DASLLAMA_PARITY_FULL=1): the batched-step cells of the rig on a deltanet hybrid whose
// FFN planes and attention q/k/o planes are IQ3_S (the recurrent qkv and the attention v planes Q4_K, the classifier
// Q6_K) - the grid format's N-column leaf under the rows form beside the k4 leaf, with the Q8_K feed at every recurrent
// head. The rows hold the split bar with the one-token-off control, not bit for bit (`../followup_vulkan.md` item 75).
// Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private IQ3S_REGIONS_FILE = "Qwen3.8-27B.i1-IQ3_S.gguf"

[test(description = "On the Qwen3.8-27B IQ3_S hybrid the grid format's N-column leaf beside the k4 leaf batches two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_hybrid_iq3s(t : T?) {
    cell_regions_batched_steps(t, IQ3S_REGIONS_FILE, LogitsBar.split_bar)
}

[test(description = "On the Qwen3.8-27B IQ3_S hybrid past the unsplit position, the split N-row command batches two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_split_hybrid_iq3s(t : T?) {
    cell_regions_batched_steps_split(t, IQ3S_REGIONS_FILE, LogitsBar.split_bar)
}

// Qwen3.8-27B UD-Q3_K_XL (large tier, DASLLAMA_PARITY_FULL=1): the batched-step cells of the rig on a deltanet hybrid
// whose planes mix eight low-bit formats - Q3_K, Q2_K, IQ3_S, IQ3_XXS, IQ2_S, IQ2_XS, IQ2_XXS and IQ4_NL across the
// attention, FFN and recurrent planes - so one load runs every low-format N-column leaf under the rows form, the
// k-lattice and the grid decodes side by side. The rows hold the split bar with the one-token-off control, not bit
// for bit (`../followup_vulkan.md` item 75). Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves,
// or without the file under models_dir().
let private Q3KXL_REGIONS_FILE = "Qwen3.8-27B-UD-Q3_K_XL.gguf"

[test(description = "On the Qwen3.8-27B UD-Q3_K_XL hybrid, mixing eight low-bit formats, every low-format N-column leaf batches two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_hybrid_q3kxl(t : T?) {
    cell_regions_batched_steps(t, Q3KXL_REGIONS_FILE, LogitsBar.split_bar)
}

[test(description = "On the Qwen3.8-27B UD-Q3_K_XL hybrid past the unsplit position, the split N-row command batches two sessions within the split bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_split_hybrid_q3kxl(t : T?) {
    cell_regions_batched_steps_split(t, Q3KXL_REGIONS_FILE, LogitsBar.split_bar)
}
