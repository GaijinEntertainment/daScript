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

// The K/V mirror's block codecs on a K-quant carrier (Llama-3.2-1B-Instruct Q4_K_M under DASLLAMA_GPU=1), one of the three
// weight formats a codec's parity owes (Q8_0, a K-quant, a grid format): the whole-model driver's prefill and decode over a q8_0 mirror in the
// forced-feed form and over a tq4 mirror in the distance form, a 40-token prompt and eight fed tokens. Every cell skips when
// interpreted, without the model, without the armed tier, and where DASLLAMA_VK_KV32=1 armed an f32 mirror.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arms (the model dropped off the device) prefill on the CPU by design
}

let private CARRIER = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"
let private Q8KV_BAR_REL = 0.10     //! of the step's max |logit|: reads 0.25-0.37 of an 8.4-9.3 max logit
let private CODEC_DIST_REL = 1.15   //! the device's distance from the f16 chain, of the CPU codec chain's own: reads 0.99, the f16 rows one step off 4.7
//! the two codec chains' distance from each other, of the CPU codec chain's from f16: reads 1.02, the CPU codec rows one step off
//! 4.9. The codec moves this carrier's logits a point a row, three times the kernels' rounding, so the chains part as two
//! independent quantizations do - under the square root of two.
let private CODEC_PAIR_REL = 1.4

[test]
def test_gpu_resident_codec_llama_k_q8kv(t : T?) {
    t |> run("Llama-3.2-1B Q4_K_M: the q8_0 K/V mirror - resident prefill + decode vs the CPU chain on q8_0 sessions") @(t : T?) {
        cell_codec_forced_feed(t, CARRIER, 40l, KVDtype.q8_0, Q8KV_BAR_REL)
    }
}

[test]
def test_gpu_resident_codec_llama_k_tq4kv(t : T?) {
    t |> run("Llama-3.2-1B Q4_K_M: the tq4 K/V mirror - resident prefill + decode against the CPU chain's tq4 and f16 rows") @(t : T?) {
        cell_codec_distance(t, CARRIER, 40l, KVDtype.tq4, false, CODEC_DIST_REL, CODEC_PAIR_REL)
    }
}
