options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen3-30B-A3B Q4_K_M (large tier, DASLLAMA_PARITY_FULL=1): the two batched-step cells of the rig on a MoE carrier with
// no shared expert - the N-row command's routed block over K-quant expert stacks, whose feed is the Q8_K requant of the
// rows - held to the wide bar with the one-token-off control rather than bit for bit: the carrier's K-quant projection
// leaves take the N-column form, which rounds apart from the one-row command's (`../followup_vulkan.md` item 75), and
// its expert planes carry that rounding through every layer (the bar's reading is `_resident_regions.das`'s). Skips off
// the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private REGIONS_FILE = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"

[test]
def test_regions_batched_steps_q30(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE, LogitsBar.wide_bar)
}

[test]
def test_regions_batched_steps_split_q30(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE, LogitsBar.wide_bar)
}
