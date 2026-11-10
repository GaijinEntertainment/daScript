options gen2
options stack = 524288
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

// The host-layers form reached by a pin (`DASLLAMA_GPU=1`, large tier): each fixture asks the plan for at least its first
// four routed layers' experts on the host (`set_gpu_host_layers`), so the split token command and the cut window chain run
// on every card the file loads on, not only on one whose room forces the form - a card that holds the rest moves exactly
// four, a smaller one moves more. The three carriers reach the three device gathers of the token table: Q8_0, a K-quant
// table and a grid-format one.

require dastest/testing_boost public
require _moe_resident   // stream_twin_within, resident_ready through its public _resident_feed
require _model_tier   // models_dir
require daslib/fio   // path_join
require dasllama/dasllama_kv_dtype   // KVDtype: the cells pin the f16 mirror
require dasllama/dasllama
require daslib/jobque_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require _resident_feed
require _kv_codec_cells

let private PINNED_Q8 = MoeFixture(file = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l)
let private PINNED_K = MoeFixture(file = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l)
let private PINNED_IQ = MoeFixture(file = "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l)

def private pinned_cells(t : T?; tag : string; fx : MoeFixture) {
    t |> run("{tag} with four routed layers' experts pinned to the host: the split token command and the cut window chain vs the CPU chain, one window") @(ct : T?) {
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            moe_forced_feed(ct, 40l, fx)
        }
    }
    t |> run("{tag} with four routed layers' experts pinned to the host: the same across two windows") @(ct : T?) {
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            return if (!assert_pf_window(ct))
            moe_forced_feed(ct, PF_WINDOW_ROWS + 88l, fx)
        }
    }
    t |> run("{tag} with four routed layers' experts pinned to the host: the streamed windows' rows against the host's sums' at the stream twin's bar, the device claiming first") @(ct : T?) {
        let path = path_join(models_dir(), fx.file)
        return if (!resident_ready(ct, path))
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            stream_twin_within(ct, path, 4l, 40l, 8l, STREAM_TWIN_BAR_REL, "{tag} ")
        }
    }
}


[test(description = "Qwen1.5-MoE Q8_0 with four routed layers pinned to the host holds the CPU chain within the routed bar over one and two windows, and its streamed window rows sit within the stream twin's bar of the host sums")]
def test_gpu_resident_moe_host_pinned_q8(t : T?) {
    pinned_cells(t, "Qwen1.5-MoE-A2.7B Q8_0", PINNED_Q8)
}

[test(description = "Qwen1.5-MoE Q4_K_M, a K-quant token table, with four routed layers pinned to the host holds the CPU chain within the routed bar over one and two windows and the stream twin's bar on streamed rows")]
def test_gpu_resident_moe_host_pinned_kq(t : T?) {
    pinned_cells(t, "Qwen1.5-MoE-A2.7B Q4_K_M", PINNED_K)
}

[test(description = "Qwen3-30B UD-IQ2_XXS, a grid-format token table, with four routed layers pinned to the host holds the CPU chain within the routed bar over one and two windows and the stream twin's bar on streamed rows")]
def test_gpu_resident_moe_host_pinned_iq(t : T?) {
    pinned_cells(t, "Qwen3-30B-A3B UD-IQ2_XXS", PINNED_IQ)
}

// The host-layers form's K/V mirror (`DASLLAMA_GPU=1`, Qwen3-30B-A3B Q4_K_M at 32768 positions asked whole, so the first routed
// layers' experts serve on the host and the window chain is cut at them): layer 0's stored rows after a 40-token prompt and the
// fed steps against the CPU cache's, through the codec rig's stored-rows cell - the instrument that says whether the cut window
// chain and the split token command write the mirror the decode reads.

let private CARRIER = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"
let private STRICT_CTX = 32768l
//! the device prefill's f16 K/V rows read 0.006 (K) and 0.013 (V) of the CPU rows' magnitude from them, a row held to the next 0.68
let private ROWS_BARS = StoredRowsBars(k = 0.02, v_step = 0.05, v_prefill_of_ctrl = 0.6, f16_dist = 0.05)

[test(description = "On Qwen3-30B Q4_K_M with host-layer experts at a strict 32768 context, layer 0's f16 mirror rows written by the cut window chain and split token command sit within 2% (K) and 5% (V) of the CPU cache's")]
def test_gpu_resident_moe_host_rows(t : T?) {
    let pins = gpu_ctx_pins()
    set_gpu_ctx_max(STRICT_CTX)
    set_gpu_ctx_strict(true)
    cell_codec_stored_rows(t, CARRIER, KVDtype.f16, ROWS_BARS)
    set_gpu_ctx_max(pins.ctx_max)
    set_gpu_ctx_strict(pins.strict)
}
