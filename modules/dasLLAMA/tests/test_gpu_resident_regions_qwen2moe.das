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
let private REGIONS_FILE = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"

[test]
def test_regions_batched_steps_qwen2moe(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_split_qwen2moe(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}
