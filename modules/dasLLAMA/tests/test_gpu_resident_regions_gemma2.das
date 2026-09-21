options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// gemma-2-2b-it Q8_0: the batched-step cells of the rig on a carrier with the attention logit softcap and the
// classifier epilogue (the final softcap over every row's logits), so the N-row token command serves two rows
// bit for bit against the sessions alone - at one window, past the unsplit position so the split attention form
// records, and a batched step before any one-row step. Skips off the JIT, without DASLLAMA_GPU=1 on a device the
// driver serves, or without the file under models_dir().
let private REGIONS_FILE = "gemma-2-2b-it-Q8_0.gguf"

[test]
def test_regions_batched_steps_g2(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_split_g2(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}

[test]
def test_regions_batched_first_g2(t : T?) {
    cell_regions_batched_first(t, REGIONS_FILE)
}

//! the stamp pick observed: a group of two heads (gemma-2's 8 q heads over 4 kv heads) steps through the two-head slab
//! stamp on the armed mirror codec - the logits compares above cannot tell the slabs apart, the census can
[test]
def test_regions_two_head_slab_served_g2(t : T?) {
    cell_regions_stamp_served(t, REGIONS_FILE, "da_attn_f16_g2_cls_spv", "da_attn_g2_cls_spv")
}
