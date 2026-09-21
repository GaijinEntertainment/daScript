options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// gemma-4-26B-A4B Q4_K_M (large tier, DASLLAMA_PARITY_FULL=1): the two batched-step cells of the rig on the gemma-4 MoE
// form - the routed feed and the router off their own norms of x, the parallel dense shared expert as the layer's own
// FFN triple, the per-expert down scale folded into the routing weights, the combine norming both branches and their
// sum under the layer's output scale - so the N-row command's routed block serves that form's rows; K-quant projections
// and experts, so the wide bar with the one-token-off control, as the Qwen3-30B file (`../followup_vulkan.md` item 75,
// the bar's reading `_resident_regions.das`'s). Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves,
// or without the file under models_dir().
let private REGIONS_FILE = "gemma-4-26B-A4B-it-Q4_K_M.gguf"

[test]
def test_regions_batched_steps_g4moe(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE, LogitsBar.wide_bar)
}

[test]
def test_regions_batched_steps_split_g4moe(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE, LogitsBar.wide_bar)
}
