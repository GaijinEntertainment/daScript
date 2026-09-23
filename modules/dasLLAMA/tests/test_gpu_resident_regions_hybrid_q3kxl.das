options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen3.8-27B UD-Q3_K_XL (large tier, DASLLAMA_PARITY_FULL=1): the batched-step cells of the rig on a deltanet hybrid
// whose planes mix eight low-bit formats - Q3_K, Q2_K, IQ3_S, IQ3_XXS, IQ2_S, IQ2_XS, IQ2_XXS and IQ4_NL across the
// attention, FFN and recurrent planes - so one load runs every low-format N-column leaf under the rows form, the
// k-lattice and the grid decodes side by side. The rows hold the split bar with the one-token-off control, not bit
// for bit (`../followup_vulkan.md` item 75). Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves,
// or without the file under models_dir().
let private REGIONS_FILE = "Qwen3.8-27B-UD-Q3_K_XL.gguf"

[test]
def test_regions_batched_steps_hybrid_q3kxl(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE, LogitsBar.split_bar)
}

[test]
def test_regions_batched_steps_split_hybrid_q3kxl(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE, LogitsBar.split_bar)
}
