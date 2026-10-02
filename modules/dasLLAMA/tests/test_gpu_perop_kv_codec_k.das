options gen2
options stack = 524288
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen3)
require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require _model_tier
require _kv_codec_cells   // cell_perop_codec

// The per-op rails under a block-codec session on a K-quant carrier (Qwen3-4B-Instruct Q4_K_M, `DASLLAMA_GPU=1`, the
// whole-model route pinned off), one of the three weight formats a codec's parity owes (Q8_0, a K-quant, a grid format): a tq4 session and a q8_0 session
// at a 40-token prompt. Every cell skips when interpreted, without the model, without the armed tier, and where the tier took
// no attention layer of the model.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU arms prefill on the CPU by design
}

let private CARRIER = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"
//! the stored bytes differ from the CPU chain's at 0 to 4e-5 of them; the device's summed distance from the f16 chain reads 0.28 of
//! the CPU codec chain's own under tq4 (3.3 against 12.0) and 0.80 under q8_0 (3.3 against 4.1), the f16 rows one step off 5.7 and 16 times it
let private BARS = PeropBars(rows = 0.05, dist_rel = 1.0)

def private carrier_path() : string => path_join(models_dir(), CARRIER)

[test]
def test_gpu_perop_k_tq4(t : T?) {
    t |> run("Qwen3-4B Q4_K_M on the per-op rails, a tq4 session: the device prefill's rows and the steps' rows land rotated in the cache") @(t : T?) {
        cell_perop_codec(t, carrier_path(), 40l, KVDtype.tq4, BARS)
    }
}

[test]
def test_gpu_perop_k_q8(t : T?) {
    t |> run("Qwen3-4B Q4_K_M on the per-op rails, a q8_0 session: the device prefill and the steps over the block's f16 mirror") @(t : T?) {
        cell_perop_codec(t, carrier_path(), 40l, KVDtype.q8_0, BARS)
    }
}
