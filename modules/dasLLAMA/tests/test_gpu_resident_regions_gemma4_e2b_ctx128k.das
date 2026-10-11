options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions
require dasllama/dasllama_kv_dtype

// gemma-4-E2B Q8_0 at four regions of 131072 positions - the model's whole context - on an f16 mirror: the fifteen
// layers that own rows (the twenty above them share the K/V below) run past one binding's range at four regions, so the
// plan seats them in two slab pairs and every sharer takes its donor's seat. The same cells as the llama file: the slab
// cell on the E-series carrier bit for bit, a 12288-token prompt and its steps against the CPU chain at the gemma
// forced-feed bar (16%), a 120000-token prompt continued from 65536, and four device-home streams at 36700 to 107479
// positions batched at every step against each alone within the wide bar: past the wide position the one-row command's
// wide twin and the N-row command's split pieces sum the keys in another order, and on the E-series that reads 3-9% of
// the max logit (RTX PRO 4500: 0.4-1.2 of 13-20), the same class as its own rows against the CPU chain at 12288 tokens
// (5-8%: 0.9-2.1 of a bar of 2.6-4.6), where the controls read 10-37. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver
// serves, without the file under models_dir(), where the environment arms another mirror codec over f16, and on a card
// whose room does not hold the mirror (10 GB of K/V beside the weights).
let private REGIONS_FILE = "gemma-4-E2B-it-Q8_0.gguf"
let private LONG = LongCtx(regions = 4l, ctx = 131072l, cpu_n = 12288l, cpu_bar_rel = 0.16, split_n = 120000l, split_cut = 65536l, streams_bar = LogitsBar.wide_bar)

[init]
def private e2b_init() {
    pin_file_kv(KVDtype.f16)
}

[finalize]
def private e2b_restore() {
    unpin_file_kv()
}

[test]
def test_regions_slabbed_mirror_gemma4_e2b(t : T?) {
    cell_regions_slabbed_mirror(t, REGIONS_FILE, LONG)
}

[test]
def test_long_prompt_against_cpu_gemma4_e2b(t : T?) {
    cell_long_prompt_against_cpu(t, REGIONS_FILE, LONG)
}

[test]
def test_long_continuation_gemma4_e2b(t : T?) {
    cell_long_continuation(t, REGIONS_FILE, LONG)
}

[test]
def test_long_streams_gemma4_e2b(t : T?) {
    cell_long_streams(t, REGIONS_FILE, LONG)
}

[test]
def test_long_rows_through_host_gemma4_e2b(t : T?) {
    cell_long_rows_through_host(t, REGIONS_FILE, LONG)
}
