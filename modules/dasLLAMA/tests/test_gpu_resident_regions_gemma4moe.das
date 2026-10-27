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
// the bar's reading `_resident_regions.das`'s). The split-forms cells pin the one-row command's fused forms off for their
// load; the fold cell holds the rows against the fused one-row command the server runs, both commands folding the per-32
// routed down under the routing weights. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or
// without the file under models_dir().
let private REGIONS_FILE = "gemma-4-26B-A4B-it-Q4_K_M.gguf"

[test]
def test_regions_batched_steps_g4moe(t : T?) {
    cell_regions_batched_steps_split_forms(t, REGIONS_FILE, LogitsBar.ctrl_bar)
}

[test]
def test_regions_batched_down_fold_g4moe(t : T?) {
    cell_regions_batched_down_fold(t, REGIONS_FILE, LogitsBar.ctrl_bar)
}

[test]
def test_regions_batched_embed_gather_g4moe(t : T?) {
    cell_regions_batched_embed_gather(t, REGIONS_FILE)
}

[test]
def test_regions_batched_widen_g4moe(t : T?) {
    cell_regions_batched_widen(t, REGIONS_FILE, LogitsBar.ctrl_bar, split_forms = true)
}
