options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

// The host-layers form's K/V mirror (`DASLLAMA_GPU=1`, Qwen3-30B-A3B Q4_K_M at 32768 positions asked whole, so the first routed
// layers' experts serve on the host and the window chain is cut at them): layer 0's stored rows after a 40-token prompt and the
// fed steps against the CPU cache's, through the codec rig's stored-rows cell - the instrument that says whether the cut window
// chain and the split token command write the mirror the decode reads.

require dastest/testing_boost public
require dasllama/dasllama_transformer
require dasllama/dasllama
require daslib/jobque_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_kv_dtype
require _model_tier
require _resident_feed
require _kv_codec_cells

let private CARRIER = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"
let private STRICT_CTX = 32768l
let private ROWS_BARS = StoredRowsBars(k = 0.02, v_step = 0.05, v_prefill_of_ctrl = 0.6)

[test]
def test_gpu_resident_moe_host_rows(t : T?) {
    let pins = gpu_ctx_pins()
    set_gpu_ctx_max(STRICT_CTX)
    set_gpu_ctx_strict(true)
    cell_codec_stored_rows(t, CARRIER, KVDtype.f16, ROWS_BARS)
    set_gpu_ctx_max(pins.ctx_max)
    set_gpu_ctx_strict(pins.strict)
}
