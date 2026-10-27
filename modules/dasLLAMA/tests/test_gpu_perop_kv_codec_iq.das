options gen2
options stack = 524288
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2)
require dasllama/dasllama
require daslib/jobque_boost
require _model_tier
require _kv_codec_cells   // cell_perop_codec

// The per-op rails under a block-codec session on a grid-format carrier (Qwen2.5-1.5B-Instruct IQ3_XS, `DASLLAMA_GPU=1`, the
// whole-model route pinned off), one of the three weight formats a codec's parity owes (Q8_0, a K-quant, a grid format): a tq4 session and a q8_0 session
// at a 40-token prompt. Every cell skips when interpreted, without the model, without the armed tier, and where the tier took
// no attention layer of the model.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU arms prefill on the CPU by design
}

let private CARRIER = "Qwen2.5-1.5B-Instruct-IQ3_XS.gguf"
//! the device's summed distance from the f16 chain, as a ratio of the CPU codec chain's own: reads 0.79 (8.6 against 11.0), the
//! f16 rows one step off 6.2 times it
let private Q8_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)
//! the serving rule on the biased carrier: a tq4 ask serves at q8_0 on both chains (the carrier's K bias outliers would set
//! every 4-bit block's scale), so the session holds the q8_0 bars, and the cache's rows are the CPU's q8_0 rows
[test]
def test_gpu_perop_iq_tq4(t : T?) {
    t |> run("Qwen2.5-1.5B IQ3_XS on the per-op rails, a tq4 ask serving at q8_0 by the serving rule: the device prefill and the steps over the block's f16 mirror") @(t : T?) {
        cell_perop_codec(t, CARRIER, 40l, KVDtype.tq4, Q8_BARS)
    }
}

[test]
def test_gpu_perop_iq_q8(t : T?) {
    t |> run("Qwen2.5-1.5B IQ3_XS on the per-op rails, a q8_0 session: the device prefill and the steps over the block's f16 mirror") @(t : T?) {
        cell_perop_codec(t, CARRIER, 40l, KVDtype.q8_0, Q8_BARS)
    }
}
