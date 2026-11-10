options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen1.5-MoE-A2.7B Q8_0 (large tier, DASLLAMA_PARITY_FULL=1): the batched-step cells of the rig on a MoE carrier whose
// experts are Q8_0 and unbiased beside a shared expert - the routed block's plain act and the shared expert's Q8_0
// requant of the routed feed - so the N-row command's rows step against the sessions alone on the arms the mxfp4 and
// K-quant MoE files never reach. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without
// the file under models_dir().
let private QWEN2MOE_REGIONS_FILE = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"

[test(description = "On Qwen1.5-MoE Q8_0 with unbiased experts beside a shared expert, the N-row command's plain act and shared-expert requant batch two sessions bit for bit what each reads alone")]
def test_regions_batched_steps_qwen2moe(t : T?) {
    cell_regions_batched_steps(t, QWEN2MOE_REGIONS_FILE)
}

[test(description = "On Qwen1.5-MoE Q8_0 with both prompts past the unsplit position, the split N-row command batches two sessions bit for bit what each reads alone")]
def test_regions_batched_steps_split_qwen2moe(t : T?) {
    cell_regions_batched_steps_split(t, QWEN2MOE_REGIONS_FILE)
}

// Qwen3-30B-A3B Q4_K_M (large tier, DASLLAMA_PARITY_FULL=1): the two batched-step cells of the rig on a MoE carrier with
// no shared expert - the N-row command's routed block over K-quant expert stacks, whose feed is the Q8_K requant of the
// rows - held to the wide bar with the one-token-off control rather than bit for bit: the carrier's K-quant projection
// leaves take the N-column form, which rounds apart from the one-row command's (`../followup_vulkan.md` item 75), and
// its expert planes carry that rounding through every layer (the bar's reading is `_resident_regions.das`'s). Skips off
// the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private QWEN3MOE_REGIONS_FILE = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"

[test(description = "On Qwen3-30B Q4_K_M with no shared expert, the N-row routed block over K-quant stacks fed Q8_K batches two sessions within the wide bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_q30(t : T?) {
    cell_regions_batched_steps(t, QWEN3MOE_REGIONS_FILE, LogitsBar.wide_bar)
}

[test(description = "On Qwen3-30B Q4_K_M past the unsplit position, the split N-row command's routed block batches two sessions within the wide bar of each alone, the one-token-off control past it")]
def test_regions_batched_steps_split_q30(t : T?) {
    cell_regions_batched_steps_split(t, QWEN3MOE_REGIONS_FILE, LogitsBar.wide_bar)
}
