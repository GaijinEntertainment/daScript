options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _cpu_gpu_model   // with_cpu_and_gpu_model, device_form
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX, LogitsBar
require _resident_feed   // Held, held_within
require _metal_mtp_cells   // the Metal MTP rail's arms on the 9B's served image
require _kernel_coverage_cells   // the Vulkan coverage census's 9B row

// The deltanet hybrids past the 0.8B's Q8_0 planes, a carrier a weight-format family the 0.8B does not reach, each ONE block
// (`with_cpu_and_gpu_model`) at two mirror regions with no CPU form - the rig's batched cells read the device alone. The rows
// hold the split bar with the one-token-off control, not bit for bit (the bar's reading is `LogitsBar`'s, `_resident_regions.das`). The 9B runs only
// where the run names its category (`--category qwen35-9b`, `../TEST_CATEGORIES.md`); the two 27B carriers are layer cuts of
// the whole files (their rows in `../performance/model_specs.das`: the first eight trunk blocks - attention at blocks 3 and 7 -
// and the NextN block), in no category, so every run opens them. Every test skips off the JIT, without DASLLAMA_GPU=1 on a
// device the driver serves, without the file under models_dir(), or where the plan does not fit the card at two regions.
// - Qwen3.5-9B-MTP UD-Q5_K_XL: recurrent projections on K-quant planes, so the N-row command's recurrent form takes the rows'
//   Q8_K feed at every recurrent head - the site the rows' next-site stamp must leave unfused - beside the gated q over the
//   rows; its K-quant draft head through the pool's moves. Its block also loads the Vulkan
//   home's default form for the coverage census's row and the Metal home's served image for
//   the MTP rail's arms.
// - Qwen3.8-27B i1-IQ3_S, eight blocks: FFN and attention q/k/o planes IQ3_S (the recurrent qkv and the attention v planes Q4_K, the
//   classifier Q6_K) - the grid format's N-column leaf under the rows form beside the k4 leaf, the Q8_K feed at every
//   recurrent head.
// - Qwen3.8-27B UD-Q3_K_XL, eight blocks: planes mixing seven low-bit formats - Q3_K, Q2_K, IQ3_S, IQ3_XXS, IQ2_S, IQ2_XS
//   and IQ4_NL - across the attention, FFN and recurrent planes (the whole file's IQ2_XXS, first at block 14, and its trunk
//   Q6_K planes, from block 63, sit past the cut), so one load runs those low-format N-column leaves under the rows form,
//   the k-lattice and the grid decodes side by side.

let private CUT_BLOCKS = 8l   //! the 27B cuts' trunk blocks, as their names state

[test(description = "On the Qwen3.5-9B K-quant hybrid the N-row recurrent form with its Q8_K feed at every recurrent head batches two sessions within the split bar of each alone, and past the unsplit position the split N-row command does the same, the one-token-off control past it; an opening moved down to the host pool and up again carries the draft head's rows, and the fresh session drafts what the session that went down drafts; on the Vulkan home's default form the coverage census's head-256 row; on the Metal home's served image, one load on a das_metal build, the Metal MTP rail's verify rows within 0.5 of the plain GPU step on forced feed, plain-vs-plain reading zero, and speculative free-run matching the plain run token for token; each form loads once")]
def test_regions_hybrid_9b(t : T?) {
    return if (!family_on(t, "qwen35"))
    var h : Held
    var forms <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    if (vk_census_asked()) {
        forms |> push(vk_census_form())
    }
    forms |> push(metal_form(image = true, ctx = METAL_MTP_CELLS_CTX))
    let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf", REGIONS_RIG_CTX,
            forms, false) $(var m, form) {
        if (form.on_metal) {
            mtp_parity_metal(t, m, form, mtp_parity_qwen35_9b())
        } elif (vk_census_form_is(form)) {
            coverage_vk_model(t, m, form, cov_vk_c61_qwen35_9b())
        } else {
            regions_batched_rows(h, m, LogitsBar.split_bar)
            regions_split_rows(h, m, LogitsBar.split_bar)
            //! the head's K-quant planes through the pool's moves
            regions_pool_head_cell(t, m, "Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf")
        }
    }
    if (ran) {
        held_within(t, h, "Qwen3.5-9B-MTP UD-Q5_K_XL, two regions: ", false)
    }
    delete h
}

[test(description = "On the eight-block cut of the Qwen3.8-27B IQ3_S hybrid the grid format's N-column leaf beside the k4 leaf batches two sessions within the split bar of each alone, and past the unsplit position the split N-row command does the same, the one-token-off control past it; the cut loads its eight trunk blocks; the form loads once")]
def test_regions_hybrid_27b_iq3s(t : T?) {
    return if (!family_on(t, "qwen35"))
    var h : Held
    let forms <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.8-27B-8L-i1-IQ3_S-local.gguf", REGIONS_RIG_CTX, forms, false) $(m, _form) {
        cut_held(h, m, CUT_BLOCKS)
        regions_batched_rows(h, m, LogitsBar.split_bar)
        regions_split_rows(h, m, LogitsBar.split_bar)
    }
    if (ran) {
        held_within(t, h, "Qwen3.8-27B-8L IQ3_S, two regions: ", false)
    }
    delete h
}

[test(description = "On the eight-block cut of the Qwen3.8-27B UD-Q3_K_XL hybrid, mixing Q3_K, Q2_K, IQ3_S, IQ3_XXS, IQ2_S, IQ2_XS and IQ4_NL, every one of those N-column leaves batches two sessions within the split bar of each alone, and past the unsplit position the split N-row command does the same, the one-token-off control past it; the cut loads its eight trunk blocks; the form loads once")]
def test_regions_hybrid_27b_q3kxl(t : T?) {
    return if (!family_on(t, "qwen35"))
    var h : Held
    let forms <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.8-27B-8L-UD-Q3_K_XL-local.gguf", REGIONS_RIG_CTX, forms, false) $(m, _form) {
        cut_held(h, m, CUT_BLOCKS)
        regions_batched_rows(h, m, LogitsBar.split_bar)
        regions_split_rows(h, m, LogitsBar.split_bar)
    }
    if (ran) {
        held_within(t, h, "Qwen3.8-27B-8L UD-Q3_K_XL, two regions: ", false)
    }
    delete h
}
