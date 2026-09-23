options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen3.8-27B i1-IQ3_S (large tier, DASLLAMA_PARITY_FULL=1): the batched-step cells of the rig on a deltanet hybrid whose
// every attention, FFN and recurrent plane is IQ3_S - the grid format's N-column leaf under the rows form at every
// site the one-row command runs its one-column class, with the Q8_K feed at every recurrent head. The rows hold the
// split bar with the one-token-off control, not bit for bit (`../followup_vulkan.md` item 75). Skips off the JIT,
// without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private REGIONS_FILE = "Qwen3.8-27B.i1-IQ3_S.gguf"

[test]
def test_regions_batched_steps_hybrid_iq3s(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE, LogitsBar.split_bar)
}

[test]
def test_regions_batched_steps_split_hybrid_iq3s(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE, LogitsBar.split_bar)
}
