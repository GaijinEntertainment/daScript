options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions
require dasllama/dasllama_kv_dtype

// Llama-3.2-3B-Instruct Q4_K_M at four regions of 32768 positions on an f16 mirror - a server's four streams at its
// context: the 28 layers' rows at that context run past one binding's range, so the plan seats them in two K/V mirror
// slab pairs. The slab cell: the slab count, the region count and the unshortened context asserted, then the interleaved
// single steps and the batched steps over two sessions on the split bar with the one-token-off control (a K-quant
// carrier, `../followup_vulkan.md` item 75). The long-context cells: a 12288-token prompt and its steps against the CPU
// chain at the 10% bar, a 30000-token prompt continued from 20480 against the one-call prefill, and four device-home
// streams at 9175 to 26624 positions batched at every step against each alone. Skips off the JIT, without
// DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(), where the environment arms
// another mirror codec over f16, and on a card whose room does not hold the mirror (15 GB of K/V beside the weights).
let private REGIONS_FILE = "Llama-3.2-3B-Instruct-Q4_K_M.gguf"
let private LONG = LongCtx(regions = 4l, ctx = 32768l, cpu_n = 12288l, cpu_bar_rel = 0.10, split_n = 30000l, split_cut = 20480l, bar = LogitsBar.split_bar)

[init]
def private llama3b_init() {
    pin_file_kv(KVDtype.f16)
}

[finalize]
def private llama3b_restore() {
    unpin_file_kv()
}

[test]
def test_regions_slabbed_mirror_llama3b(t : T?) {
    cell_regions_slabbed_mirror(t, REGIONS_FILE, LONG)
}

[test]
def test_long_prompt_against_cpu_llama3b(t : T?) {
    cell_long_prompt_against_cpu(t, REGIONS_FILE, LONG)
}

[test]
def test_long_continuation_llama3b(t : T?) {
    cell_long_continuation(t, REGIONS_FILE, LONG)
}

[test]
def test_long_streams_llama3b(t : T?) {
    cell_long_streams(t, REGIONS_FILE, LONG)
}

[test]
def test_long_rows_through_host_llama3b(t : T?) {
    cell_long_rows_through_host(t, REGIONS_FILE, LONG)
}
