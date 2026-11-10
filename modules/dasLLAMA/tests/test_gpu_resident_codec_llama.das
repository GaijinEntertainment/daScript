options gen2
options stack = 524288
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (llama)
require dasllama/dasllama
require daslib/jobque_boost
require _model_tier
require _kv_codec_cells

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arms (the model dropped off the device) prefill on the CPU by design
}

// The K/V mirror's block codecs on a K-quant carrier (Llama-3.2-1B-Instruct Q4_K_M under DASLLAMA_GPU=1), one of the three
// weight formats a codec's parity owes (Q8_0, a K-quant, a grid format): the whole-model driver's prefill and decode over a q8_0 mirror in the
// forced-feed form and over a tq4 mirror in the distance form, a 40-token prompt and eight fed tokens. Every cell skips when
// interpreted, without the model, without the armed tier, and where DASLLAMA_VK_KV32=1 armed an f32 mirror.

let private K_CARRIER = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"
let private K_Q8KV_BAR_REL = 0.10     //! of the step's max |logit|: reads 0.25-0.37 of an 8.4-9.3 max logit
let private K_CODEC_DIST_REL = 1.15   //! the device's distance from the f16 chain, of the CPU codec chain's own: reads 0.99, the f16 rows one step off 4.7
//! the two codec chains' distance from each other, of the CPU codec chain's from f16: reads 1.02, the CPU codec rows one step off
//! 4.9. The codec moves this carrier's logits a point a row, three times the kernels' rounding, so the chains part as two
//! independent quantizations do - under the square root of two.
let private K_CODEC_PAIR_REL = 1.4

[test(description = "On Llama-3.2-1B Q4_K_M the whole-model driver's prefill and decode over a q8_0 K/V mirror keep forced-feed logits within 10 percent of max logit of the CPU chain on q8_0 sessions")]
def test_gpu_resident_codec_llama_k_q8kv(t : T?) {
    t |> run("Llama-3.2-1B Q4_K_M: the q8_0 K/V mirror - resident prefill + decode vs the CPU chain on q8_0 sessions") @(t : T?) {
        cell_codec_forced_feed(t, K_CARRIER, 40l, KVDtype.q8_0, K_Q8KV_BAR_REL)
    }
}

[test(description = "On Llama-3.2-1B Q4_K_M over a tq4 K/V mirror, the resident device sits within 1.15x the CPU tq4 chain's distance from the f16 chain and within 1.4x of it from the CPU tq4 chain, with one-step-off controls past both bounds")]
def test_gpu_resident_codec_llama_k_tq4kv(t : T?) {
    t |> run("Llama-3.2-1B Q4_K_M: the tq4 K/V mirror - resident prefill + decode against the CPU chain's tq4 and f16 rows") @(t : T?) {
        cell_codec_distance(t, K_CARRIER, 40l, KVDtype.tq4, false, K_CODEC_DIST_REL, K_CODEC_PAIR_REL)
    }
}

// The K/V mirror's block codecs on a grid-format carrier (Llama-3.2-1B-Instruct IQ4_XS under DASLLAMA_GPU=1), one of the three
// weight formats a codec's parity owes (Q8_0, a K-quant, a grid format): the whole-model driver's prefill and decode over a q8_0 mirror in the
// forced-feed form and over a tq4 mirror in the distance form, a 40-token prompt and eight fed tokens. Every cell skips when
// interpreted, without the model, without the armed tier, and where DASLLAMA_VK_KV32=1 armed an f32 mirror.

let private IQ_CARRIER = "Llama-3.2-1B-Instruct-IQ4_XS.gguf"
let private IQ_Q8KV_BAR_REL = 0.10     //! of the step's max |logit|: reads 0.29-0.42 of an 8.7-9.4 max logit
let private IQ_CODEC_DIST_REL = 1.15   //! the device's distance from the f16 chain, of the CPU codec chain's own: reads 1.00, the f16 rows one step off 5.0
//! the two codec chains' distance from each other, of the CPU codec chain's from f16: reads 1.13, the CPU codec rows one step off
//! 5.1. The codec moves this carrier's logits a point a row, three times the kernels' rounding, so the chains part as two
//! independent quantizations do - under the square root of two.
let private IQ_CODEC_PAIR_REL = 1.4

[test(description = "On Llama-3.2-1B IQ4_XS the whole-model driver's prefill and decode over a q8_0 K/V mirror keep forced-feed logits within 10 percent of max logit of the CPU chain on q8_0 sessions")]
def test_gpu_resident_codec_llama_iq_q8kv(t : T?) {
    t |> run("Llama-3.2-1B IQ4_XS: the q8_0 K/V mirror - resident prefill + decode vs the CPU chain on q8_0 sessions") @(t : T?) {
        cell_codec_forced_feed(t, IQ_CARRIER, 40l, KVDtype.q8_0, IQ_Q8KV_BAR_REL)
    }
}

[test(description = "On Llama-3.2-1B IQ4_XS over a tq4 K/V mirror, the resident device sits within 1.15x the CPU tq4 chain's distance from the f16 chain and within 1.4x of it from the CPU tq4 chain, with one-step-off controls past both bounds")]
def test_gpu_resident_codec_llama_iq_tq4kv(t : T?) {
    t |> run("Llama-3.2-1B IQ4_XS: the tq4 K/V mirror - resident prefill + decode against the CPU chain's tq4 and f16 rows") @(t : T?) {
        cell_codec_distance(t, IQ_CARRIER, 40l, KVDtype.tq4, false, IQ_CODEC_DIST_REL, IQ_CODEC_PAIR_REL)
    }
}
