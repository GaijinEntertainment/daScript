options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions
require dasllama/dasllama_vulkan_common

// gemma-4-26B-A4B Q4_K_M (large tier, DASLLAMA_PARITY_FULL=1): the two batched-step cells of the rig on the gemma-4 MoE
// form - the routed feed and the router off their own norms of x, the parallel dense shared expert as the layer's own
// FFN triple, the per-expert down scale folded into the routing weights, the combine norming both branches and their
// sum under the layer's output scale - so the N-row command's routed block serves that form's rows; K-quant projections
// and experts, so the wide bar with the one-token-off control, as the Qwen3-30B file (`../followup_vulkan.md` item 75,
// the bar's reading `_resident_regions.das`'s). The cells pin the one-row command's fused forms off for their load: the
// rows take the split forms - the routed down GEMV a slot at a time under the combine's sum, where the fused one-row form
// folds the slots inside the GEMV - and on this carrier the two sums' rounding turns router near-ties into whole-position
// flips (five of thirty-two compares off by up to 0.13 of the peak, the argmax with them), as its CPU-vs-resident
// compares do (`CLAUDE.md`); the fused forms are held to the split ones by their own cells, and `../followup_vulkan.md`
// item 82 names the rows' own folded down. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or
// without the file under models_dir().
let private REGIONS_FILE = "gemma-4-26B-A4B-it-Q4_K_M.gguf"

//! the cell over the split one-row forms, the fuse knob pinned off for the load and unpinned after
def private split_forms_cell(t : T?; cell : function<(t : T?; file : string; bar : LogitsBar; served : bool) : void>) {
    g_vk_fuse = 0
    invoke(cell, t, REGIONS_FILE, LogitsBar.wide_bar, true)
    g_vk_fuse = -1
}

[test]
def test_regions_batched_steps_g4moe(t : T?) {
    split_forms_cell(t, @@cell_regions_batched_steps)
}

[test]
def test_regions_batched_steps_split_g4moe(t : T?) {
    split_forms_cell(t, @@cell_regions_batched_steps_split)
}
