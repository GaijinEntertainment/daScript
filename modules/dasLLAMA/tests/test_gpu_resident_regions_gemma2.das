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
// driver serves, or without the file under models_dir(); the stamp cell skips under DASLLAMA_CUDA=1 (its slab pick and census
// names are the Vulkan driver's) and on a build with no kernel census.
let private REGIONS_FILE = "gemma-2-2b-it-Q8_0.gguf"

[test(description = "On gemma-2-2b with the attention softcap and the final logit softcap, a batched step over two sessions reads bit for bit what each session reads alone")]
def test_regions_batched_steps_g2(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test(description = "On gemma-2-2b with both prompts past the unsplit position, the N-row command's split attention form batches two sessions bit for bit what each reads alone")]
def test_regions_batched_steps_split_g2(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}

[test(description = "On gemma-2-2b a batched step before any one-row step, one-row steps, then a batched step again all read what each session reads alone")]
def test_regions_batched_first_g2(t : T?) {
    cell_regions_batched_first(t, REGIONS_FILE)
}

//! the stamp pick observed: a group of two heads (gemma-2's 8 q heads over 4 kv heads) steps through the two-head slab
//! stamp on the armed mirror codec - the logits compares above cannot tell the slabs apart, the census can
[test(description = "gemma-2-2b's two q heads per kv head make a driver step dispatch the two-head slab decode attention stamp of the armed mirror codec, as the kernel census shows")]
def test_regions_two_head_slab_served_g2(t : T?) {
    cell_regions_stamp_served(t, REGIONS_FILE, 2)
}
