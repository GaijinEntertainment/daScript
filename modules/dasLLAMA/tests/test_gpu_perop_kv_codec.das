options gen2
options stack = 524288
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen3)
require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require _model_tier
require _kv_codec_cells   // cell_perop_codec

// The per-op rails under a block-codec session, the Q8_0 carrier (Qwen3-0.6B Q8_0, `DASLLAMA_GPU=1`, the whole-model route
// pinned off): the device's attention block keeps an f16 mirror of the rows, the session's cache keeps the codec's bytes. Each
// cell holds the cache rows the device passes wrote to the CPU chain's own, and the logits to the CPU chain at f16 - the device
// attends the rows before their quantization, so it reads nearer f16 than the CPU's codec chain does. Every cell skips when
// interpreted, without the model, without the armed tier, and where the tier took no attention layer of the model.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU arms, and the short prompt's prefill under the device's window floor
}

let private CARRIER = "Qwen3-0.6B-Q8_0.gguf"
//! layer 0's cache bytes, the device arm's against the CPU chain's: one input, so they agree but for a quant on a rounding edge
//! (differ at 0 of the bytes under tq4, 2e-5 under q8_0). The device's summed distance from the f16 chain reads 2.3 against the CPU
//! codec chain's 48.7 where the device prefilled and 38.4 against 42.6 over rows the CPU quantized; the f16 rows one step off sum to 80-95.
let private BARS = PeropBars(rows = 0.05, dist_rel = 1.0)

def private carrier_path() : string => path_join(models_dir(), CARRIER)

[test]
def test_gpu_perop_tq4(t : T?) {
    t |> run("Qwen3-0.6B on the per-op rails, a tq4 session: the device prefill's rows and the steps' rows land rotated in the cache") @(t : T?) {
        cell_perop_codec(t, carrier_path(), 40l, KVDtype.tq4, BARS)
    }
    t |> run("... and a prompt under the device prefill's floor: the block hydrates from the tq4 rows the CPU prefill wrote") @(t : T?) {
        cell_perop_codec(t, carrier_path(), 20l, KVDtype.tq4, BARS)
    }
}

[test]
def test_gpu_perop_q8(t : T?) {
    t |> run("Qwen3-0.6B on the per-op rails, a q8_0 session: the device prefill and the steps over the block's f16 mirror") @(t : T?) {
        cell_perop_codec(t, carrier_path(), 40l, KVDtype.q8_0, BARS)
    }
}
