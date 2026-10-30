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


[test]
def test_gpu_resident_moe_host_pinned_q8(t : T?) {
    pinned_cells(t, "Qwen1.5-MoE-A2.7B Q8_0", PINNED_Q8)
}

[test]
def test_gpu_resident_moe_host_pinned_kq(t : T?) {
    pinned_cells(t, "Qwen1.5-MoE-A2.7B Q4_K_M", PINNED_K)
}

[test]
def test_gpu_resident_moe_host_pinned_iq(t : T?) {
    pinned_cells(t, "Qwen3-30B-A3B UD-IQ2_XXS", PINNED_IQ)
}
