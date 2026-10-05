options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the model
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // resident_ready, the ramps, resident_then_cpu_feed, feed_steps_within
require _kv_codec_cells   // the K/V codec cells' rig

// The whole-model resident driver on a qwen2 (Qwen2.5-0.5B-Instruct-Q8_0 under DASLLAMA_GPU=1): the
// q/k/v projection bias folds into the rope stage on the device. The forced-feed logits-tolerance
// form of test_gpu_resident_hybrid: the same fixed tokens through the resident chain and the
// all-CPU chain, the prefill logits and every step's within the bar, the one-step-off control past
// it - a dropped or misplaced bias moves the logits far past the bar (the bias is a large share of
// a 0.5B's k rows), so the bar is the bias arm's gate. Two prompt lengths: one window, two windows.
// The K/V mirror's block codecs ride the same carrier, the Q8_0 one of the three weight formats a codec's parity owes: q8_0 in the forced-feed
// form on its own bar, and both codecs in the distance form and the stored-rows form. Every cell skips when interpreted,
// without the model, and without the armed tier; a codec cell also where DASLLAMA_VK_KV32=1 armed an f32 mirror.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}

let private CARRIER = "Qwen2.5-0.5B-Instruct-Q8_0.gguf"
//! of the step's max |logit|: the 0.5B's CPU-vs-device rounding sits flat at 0.30-0.50 of a 9.5-18 max logit
//! at every step, while a dropped bias lands whole logits off - the hybrid file's K-quant bar.
let private LOGIT_BAR_REL = 0.06
//! the q8_0 mirror's bar: the CPU chain quantizes each query to Q8 for its int8 dot where the device keeps it f32, so the arms
//! part by that noise on top of the f16 reading - 0.39-0.72 of a 9.4-18 max logit, the one-step-off controls 4.7-11
let private Q8KV_BAR_REL = 0.08
//! no logit bar holds a 4-bit mirror on a 0.5B, so the codec bars hold ratios of the rows' summed largest differences:
let private CODEC_DIST_REL = 1.15   //! the device's distance from the f16 chain, of the CPU codec chain's own: reads 0.95-1.01, the f16 rows one step off 1.6-3.7
let private CODEC_PAIR_REL = 0.8    //! the two codec chains' distance from each other, of the CPU codec chain's from f16: reads 0.20 (q8_0) to 0.61 (tq4), the CPU codec rows one step off 1.5-3.7
//! layer 0's stored bytes against the CPU cache's: the K rows (the bias carries them) and the steps' V rows (one Q8_0 feed on both
//! chains) differ at 0.002-0.01 of their bytes; the prefill's V rows at 0.39 (q8_0) and 0.12 (tq4) - its GEMM reads f16 rows where
//! the CPU reads Q8_0 blocks - against 0.97 for a row held to the next one (the control)
let private ROWS_BARS = StoredRowsBars(k = 0.02, v_step = 0.05, v_prefill_of_ctrl = 0.6)

[cold_path]
def private qwen2_forced_feed(t : T?; n : int64) {
    let path = path_join(models_dir(), CARRIER)
    if (!resident_ready(t, path)) return
    var tr <- load_model_(path, QuantMode.q8)
    t |> success(tr.config.attn_qkv_bias, "the qwen2 carries the q/k/v projection bias the arm exists for")
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the biased model (the load log names any decline)")
    with_dasllama_jobque_() {
        forced_feed_over(t, tr, "", n, 8l, LOGIT_BAR_REL) $ {}
    }
    delete tr
}


[test]
def test_gpu_resident_qwen2_bias(t : T?) {
    t |> run("Qwen2.5-0.5B: resident prefill + decode with the q/k/v bias on device vs the CPU chain, one window") @(t : T?) {
        qwen2_forced_feed(t, 40l)
    }
    t |> run("Qwen2.5-0.5B: the same across two windows") @(t : T?) {
        assert_pf_window(t)
        qwen2_forced_feed(t, PF_WINDOW_ROWS + 88l)
    }
}

[test]
def test_gpu_resident_qwen2_q8kv(t : T?) {
    t |> run("Qwen2.5-0.5B: the q8_0 K/V mirror - resident prefill + decode vs the CPU chain on q8_0 sessions, one window") @(t : T?) {
        cell_codec_forced_feed(t, CARRIER, 40l, KVDtype.q8_0, Q8KV_BAR_REL)
    }
    t |> run("Qwen2.5-0.5B: the q8_0 K/V mirror across two windows") @(t : T?) {
        assert_pf_window(t)
        cell_codec_forced_feed(t, CARRIER, PF_WINDOW_ROWS + 88l, KVDtype.q8_0, Q8KV_BAR_REL)
    }
}

[test]
def test_gpu_resident_qwen2_tq4kv(t : T?) {
    t |> run("Qwen2.5-0.5B: the tq4 K/V mirror - resident prefill + decode against the CPU chain's tq4 and f16 rows, one window") @(t : T?) {
        cell_codec_distance(t, CARRIER, 40l, KVDtype.tq4, false, CODEC_DIST_REL, CODEC_PAIR_REL)
    }
    t |> run("Qwen2.5-0.5B: the tq4 K/V mirror across two windows") @(t : T?) {
        assert_pf_window(t)
        cell_codec_distance(t, CARRIER, PF_WINDOW_ROWS + 88l, KVDtype.tq4, false, CODEC_DIST_REL, CODEC_PAIR_REL)
    }
}

[test]
def test_gpu_resident_qwen2_codec_rows_up(t : T?) {
    t |> run("Qwen2.5-0.5B: the device steps over q8_0 rows the CPU prefill wrote") @(t : T?) {
        cell_codec_distance(t, CARRIER, 40l, KVDtype.q8_0, true, CODEC_DIST_REL, CODEC_PAIR_REL)
    }
    t |> run("Qwen2.5-0.5B: the device steps over tq4 rows the CPU prefill wrote") @(t : T?) {
        cell_codec_distance(t, CARRIER, 40l, KVDtype.tq4, true, CODEC_DIST_REL, CODEC_PAIR_REL)
    }
}

[test]
def test_gpu_resident_qwen2_codec_rows(t : T?) {
    t |> run("Qwen2.5-0.5B: the q8_0 mirror's stored rows against the CPU cache's, layer 0") @(t : T?) {
        cell_codec_stored_rows(t, CARRIER, KVDtype.q8_0, ROWS_BARS)
    }
    t |> run("Qwen2.5-0.5B: the tq4 mirror's stored rows against the CPU cache's, layer 0") @(t : T?) {
        cell_codec_stored_rows(t, CARRIER, KVDtype.tq4, ROWS_BARS)
    }
}
