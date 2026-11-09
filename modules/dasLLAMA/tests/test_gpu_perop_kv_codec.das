options gen2
options stack = 524288
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2, qwen3)
require dasllama/dasllama
require daslib/jobque_boost
require _model_tier
require _kv_codec_cells   // cell_perop_codec

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU arms, and the short prompt's prefill under the device's window floor
}

// The per-op rails under a block-codec session, the Q8_0 carrier (Qwen3-0.6B Q8_0, `DASLLAMA_GPU=1`, the whole-model route
// pinned off): the device's attention block keeps an f16 mirror of the rows, the session's cache keeps the codec's bytes. Each
// cell holds the cache rows the device passes wrote to the CPU chain's own, and the logits to the CPU chain at f16 - the device
// attends the rows before their quantization, so it reads nearer f16 than the CPU's codec chain does. Every cell skips when
// interpreted, without the model, without the armed tier, and where the tier took no attention layer of the model.

let private QWEN3_06B_CARRIER = "Qwen3-0.6B-Q8_0.gguf"
//! layer 0's cache bytes, the device arm's against the CPU chain's: one input, so they agree but for a quant on a rounding edge
//! (differ at 0 of the bytes under tq4, 2e-5 under q8_0). The device's summed distance from the f16 chain reads 2.3 against the CPU
//! codec chain's 48.7 where the device prefilled and 38.4 against 42.6 over rows the CPU quantized; the f16 rows one step off sum to 80-95.
let private QWEN3_06B_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)

[test(description = "On the per-op rails with Qwen3-0.6B Q8_0 under a tq4 session, layer 0's cache bytes differ from the CPU chain's at no more than 5 percent and the device reads no further from the f16 chain than the CPU codec chain, both above and below the device prefill floor")]
def test_gpu_perop_tq4(t : T?) {
    t |> run("Qwen3-0.6B on the per-op rails, a tq4 session: the device prefill's rows and the steps' rows land rotated in the cache") @(t : T?) {
        cell_perop_codec(t, QWEN3_06B_CARRIER, 40l, KVDtype.tq4, QWEN3_06B_BARS)
    }
    t |> run("... and a prompt under the device prefill's floor: the block hydrates from the tq4 rows the CPU prefill wrote") @(t : T?) {
        cell_perop_codec(t, QWEN3_06B_CARRIER, 20l, KVDtype.tq4, QWEN3_06B_BARS)
    }
}

[test(description = "On the per-op rails with Qwen3-0.6B Q8_0 under a q8_0 session, layer 0's cache bytes differ from the CPU chain's at no more than 5 percent and the device's f16-mirror reads sit no further from the f16 chain than the CPU codec chain")]
def test_gpu_perop_q8(t : T?) {
    t |> run("Qwen3-0.6B on the per-op rails, a q8_0 session: the device prefill and the steps over the block's f16 mirror") @(t : T?) {
        cell_perop_codec(t, QWEN3_06B_CARRIER, 40l, KVDtype.q8_0, QWEN3_06B_BARS)
    }
}

// The per-op rails under a block-codec session on a grid-format carrier (Qwen2.5-1.5B-Instruct IQ3_XS, `DASLLAMA_GPU=1`, the
// whole-model route pinned off), one of the three weight formats a codec's parity owes (Q8_0, a K-quant, a grid format): a tq4 session and a q8_0 session
// at a 40-token prompt. Every cell skips when interpreted, without the model, without the armed tier, and where the tier took
// no attention layer of the model.

let private QWEN25_IQ_CARRIER = "Qwen2.5-1.5B-Instruct-IQ3_XS.gguf"
//! the device's summed distance from the f16 chain, as a ratio of the CPU codec chain's own: reads 0.79 (8.6 against 11.0), the
//! f16 rows one step off 6.2 times it
let private QWEN25_IQ_Q8_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)
//! the serving rule on the biased carrier: a tq4 ask serves at q8_0 on both chains (the carrier's K bias outliers would set
//! every 4-bit block's scale), so the session holds the q8_0 bars, and the cache's rows are the CPU's q8_0 rows
[test(description = "On the per-op rails with Qwen2.5-1.5B IQ3_XS, a tq4 ask served at q8_0 by the serving rule keeps layer 0's cache bytes within 5 percent of the CPU chain's and the device no further from the f16 chain than the CPU codec chain")]
def test_gpu_perop_iq_tq4(t : T?) {
    t |> run("Qwen2.5-1.5B IQ3_XS on the per-op rails, a tq4 ask serving at q8_0 by the serving rule: the device prefill and the steps over the block's f16 mirror") @(t : T?) {
        cell_perop_codec(t, QWEN25_IQ_CARRIER, 40l, KVDtype.tq4, QWEN25_IQ_Q8_BARS)
    }
}

[test(description = "On the per-op rails with Qwen2.5-1.5B IQ3_XS under a q8_0 session, layer 0's cache bytes stay within 5 percent of the CPU chain's and the device reads no further from the f16 chain than the CPU codec chain")]
def test_gpu_perop_iq_q8(t : T?) {
    t |> run("Qwen2.5-1.5B IQ3_XS on the per-op rails, a q8_0 session: the device prefill and the steps over the block's f16 mirror") @(t : T?) {
        cell_perop_codec(t, QWEN25_IQ_CARRIER, 40l, KVDtype.q8_0, QWEN25_IQ_Q8_BARS)
    }
}

// The per-op rails under a block-codec session on a K-quant carrier (Qwen3-4B-Instruct Q4_K_M, `DASLLAMA_GPU=1`, the
// whole-model route pinned off), one of the three weight formats a codec's parity owes (Q8_0, a K-quant, a grid format): a tq4 session and a q8_0 session
// at a 40-token prompt. Every cell skips when interpreted, without the model, without the armed tier, and where the tier took
// no attention layer of the model.

let private QWEN3_4B_CARRIER = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"
//! the stored bytes differ from the CPU chain's at 0 to 4e-5 of them; the device's summed distance from the f16 chain reads 0.28 of
//! the CPU codec chain's own under tq4 (3.3 against 12.0) and 0.80 under q8_0 (3.3 against 4.1), the f16 rows one step off 5.7 and 16 times it
let private QWEN3_4B_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)

[test(description = "On the per-op rails with Qwen3-4B Q4_K_M under a tq4 session, layer 0's rotated cache bytes stay within 5 percent of the CPU chain's and the device reads no further from the f16 chain than the CPU codec chain")]
def test_gpu_perop_k_tq4(t : T?) {
    t |> run("Qwen3-4B Q4_K_M on the per-op rails, a tq4 session: the device prefill's rows and the steps' rows land rotated in the cache") @(t : T?) {
        cell_perop_codec(t, QWEN3_4B_CARRIER, 40l, KVDtype.tq4, QWEN3_4B_BARS)
    }
}

[test(description = "On the per-op rails with Qwen3-4B Q4_K_M under a q8_0 session, layer 0's cache bytes stay within 5 percent of the CPU chain's and the device reads no further from the f16 chain than the CPU codec chain")]
def test_gpu_perop_k_q8(t : T?) {
    t |> run("Qwen3-4B Q4_K_M on the per-op rails, a q8_0 session: the device prefill and the steps over the block's f16 mirror") @(t : T?) {
        cell_perop_codec(t, QWEN3_4B_CARRIER, 40l, KVDtype.q8_0, QWEN3_4B_BARS)
    }
}
