options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions
require dasllama/dasllama_kv_dtype

// gemma-3-4b Q8_0 at two regions of 131072 positions - the model's whole context - on a q8_0 mirror: the 34 layers'
// blocks at that context run past a block codec's 32-bit byte span a binding, so the plan seats them in three slab
// pairs, every window-chain store and shadow pass bound to its layer's slab. The same cells as the llama file: the slab
// cell on the gemma-3 dense base bit for bit, a 12288-token prompt and its steps against the CPU chain at the gemma
// forced-feed bar (16%), a 120000-token prompt continued from 65536, and two device-home streams at 36700 and 60293
// positions batched at every step against each alone within the split bar (past the wide position the one-row command's
// wide twin and the N-row command's split pieces sum the keys in another order; the slab cell's short prompts stay bit for
// bit). Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver
// serves, without the file under models_dir(), where the environment arms another mirror codec over q8_0, and on a card
// whose room does not hold the mirror (19 GB of K/V beside the weights).
let private REGIONS_FILE = "gemma-3-4b-it-Q8_0.gguf"
let private LONG = LongCtx(regions = 2l, ctx = 131072l, cpu_n = 12288l, cpu_bar_rel = 0.16, split_n = 120000l, split_cut = 65536l, streams_bar = LogitsBar.split_bar)

[init]
def private gemma3_init() {
    pin_file_kv(KVDtype.q8_0)
}

[finalize]
def private gemma3_restore() {
    unpin_file_kv()
}

[test]
def test_regions_slabbed_mirror_gemma3_4b(t : T?) {
    cell_regions_slabbed_mirror(t, REGIONS_FILE, LONG)
}

[test]
def test_long_prompt_against_cpu_gemma3_4b(t : T?) {
    cell_long_prompt_against_cpu(t, REGIONS_FILE, LONG)
}

[test]
def test_long_continuation_gemma3_4b(t : T?) {
    cell_long_continuation(t, REGIONS_FILE, LONG)
}

[test]
def test_long_streams_gemma3_4b(t : T?) {
    cell_long_streams(t, REGIONS_FILE, LONG)
}

[test]
def test_long_rows_through_host_gemma3_4b(t : T?) {
    cell_long_rows_through_host(t, REGIONS_FILE, LONG)
}
