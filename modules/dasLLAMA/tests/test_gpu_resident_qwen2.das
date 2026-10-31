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
//! the tq4 cells' carrier: the serving rule steps a tq4 ask down to q8_0 on the Q/K/V-biased Qwen2.5 (its K bias outliers set
//! every 4-bit block's scale), so the 4-bit arms run on the bias-free Qwen3-0.6B of the same size class
let private CARRIER_TQ4 = "Qwen3-0.6B-Q8_0.gguf"
//! of the step's max |logit|: the 0.5B's CPU-vs-device rounding sits flat at 0.30-0.50 of a 9.5-18 max logit
//! at every step, while a dropped bias lands whole logits off - the hybrid file's K-quant bar.
let private LOGIT_BAR_REL = 0.06
//! the q8_0 mirror's bar: both chains quantize each query to Q8 for their int8 dot - the device the query it computed, the CPU
//! its own - and two roundings of two nearby queries part by whole quant steps on a share of the elements, on top of the f16
//! reading: 0.42-0.89 of a 9.4-18 max logit, the one-step-off controls 4.7-11
let private Q8KV_BAR_REL = 0.10
//! no logit bar holds a 4-bit mirror on a 0.5B, so the codec bars hold ratios of the rows' summed largest differences:
let private CODEC_DIST_REL = 1.25   //! the device's distance from the f16 chain, of the CPU codec chain's own: reads 0.95-1.01, the tq4 rows 1.17 on the RTX PRO 4500 (driver 595), the f16 rows one step off 1.6-3.7
let private CODEC_PAIR_REL = 0.8    //! the two codec chains' distance from each other, of the CPU codec chain's from f16: reads 0.20 (q8_0) to 0.61 (tq4), the CPU codec rows one step off 1.5-3.7
//! layer 0's stored bytes against the CPU cache's: the K rows (the bias carries them) and the steps' V rows (one Q8_0 feed on both
//! chains) differ at 0.002-0.01 of their bytes; the prefill's V rows at 0.39 (q8_0) and 0.12 (tq4) - its GEMM reads f16 rows where
//! the CPU reads Q8_0 blocks - against 0.97 for a row held to the next one (the control)
let private ROWS_BARS = StoredRowsBars(k = 0.02, v_step = 0.05, v_prefill_of_ctrl = 0.6)
//! the tq4 carrier's own readings, the same on master's engine and this one: the two codec chains part further on Qwen3-0.6B
//! (0.82 of the CPU codec chain's distance from f16 against the biased carrier's 0.61 - the llama files read 1.0-1.1), and its
//! q/k norm before the rope lands the device's normed k on the 4-bit quant's edge on 0.049 of layer 0's K bytes
let private CODEC_PAIR_REL_TQ4 = 1.0
let private ROWS_BARS_TQ4 = StoredRowsBars(k = 0.08, v_step = 0.05, v_prefill_of_ctrl = 0.6)

//! the iq4nl32 carrier's bar: its dense planes serve off the per-32 planes, the window's GEMMs on the s tile's f16 feed, and the
//! 0.5B's rows read 0.69 of a 10.9 max logit on the RTX PRO 4500 (0.063), past the file's 0.06
let private IQ4NL32_BAR_REL = 0.08

[cold_path]
def private qwen2_forced_feed(t : T?; n : int64; file : string = CARRIER; dense : KqFmt = KqFmt.q8; bar : float = LOGIT_BAR_REL) {
    let path = path_join(models_dir(), file)
    if (!resident_ready(t, path)) return
    var tr <- load_resident(path)   //! the window pinned for the load: unpinned, the driver's own pick outlives the cell and sizes every later plan
    t |> success(tr.config.attn_qkv_bias, "the qwen2 carries the q/k/v projection bias the arm exists for")
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the biased model (the load log names any decline)")
    if (dense != KqFmt.q8) {
        dense_planes_tagged(t, tr, path, dense)
    }
    with_dasllama_jobque_() {
        forced_feed_over(t, tr, "{file} ", n, 8l, bar) $ {
            served_every_call(t)
        }
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

//! the IQ4_NL mint (`-local`, the catalog row's recipe): its 896-wide rows sit off the superblock lattice, so every dense plane
//! serves off the per-32 iq4nl32 planes - the window's GEMMs on the s tile on the f16 feed and the per-32 batch tile off it, the
//! steps on the per-32 GEMVs; the token table demotes to q8
[test]
def test_gpu_resident_qwen2_iq4nl32(t : T?) {
    let file = "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf"
    t |> run("{file}: the dense iq4nl32 planes, resident prefill + decode vs the CPU chain, one window") @(t : T?) {
        qwen2_forced_feed(t, 40l, file, KqFmt.iq4nl32, IQ4NL32_BAR_REL)
    }
    t |> run("{file}: the same across two windows") @(t : T?) {
        assert_pf_window(t)
        qwen2_forced_feed(t, PF_WINDOW_ROWS + 88l, file, KqFmt.iq4nl32, IQ4NL32_BAR_REL)
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
    t |> run("Qwen3-0.6B: the tq4 K/V mirror - resident prefill + decode against the CPU chain's tq4 and f16 rows, one window") @(t : T?) {
        cell_codec_distance(t, CARRIER_TQ4, 40l, KVDtype.tq4, false, CODEC_DIST_REL, CODEC_PAIR_REL_TQ4)
    }
    t |> run("Qwen3-0.6B: the tq4 K/V mirror across two windows") @(t : T?) {
        assert_pf_window(t)
        cell_codec_distance(t, CARRIER_TQ4, PF_WINDOW_ROWS + 88l, KVDtype.tq4, false, CODEC_DIST_REL, CODEC_PAIR_REL_TQ4)
    }
}

//! the serving rule on the biased carrier: a tq4 ask arms the q8_0 mirror, the session serves q8_0, and the forced feed holds
//! the q8_0 bar - the carrier's K bias outliers would set every 4-bit block's scale
[test]
def test_gpu_resident_qwen2_tq4_steps_down(t : T?) {
    t |> run("Qwen2.5-0.5B: a tq4 ask serves at q8_0") @(t : T?) {
        let path = path_join(models_dir(), CARRIER)
        return if (!resident_ready(t, path))
        var tr <- load_resident_at(path, KVDtype.tq4)
        if (mirror_kv_dtype() == KVDtype.f32) {
            t |> skip("DASLLAMA_VK_KV32=1 arms the f32 mirror over the codec asked - the cell needs it unset")
        } else {
            t |> success(moe_gpu_resident_active(), "the whole-model driver armed (the load log names any decline)")
            t |> equal(mirror_kv_dtype(), KVDtype.q8_0, "the tq4 ask armed the q8_0 mirror by the serving rule")
            t |> equal(kv_codec_for(tr.config, KVDtype.tq4), KVDtype.q8_0, "and a session's codec steps down the same way")
            with_dasllama_jobque_() {
                forced_feed_over(t, tr, "tq4 asked, q8_0 served: ", 40l, 8l, Q8KV_BAR_REL) $ {
                    served_every_call(t)
                }
            }
        }
        moe_gpu_drop_model()
        delete tr
    }
}

[test]
def test_gpu_resident_qwen2_codec_rows_up(t : T?) {
    t |> run("Qwen2.5-0.5B: the device steps over q8_0 rows the CPU prefill wrote") @(t : T?) {
        cell_codec_distance(t, CARRIER, 40l, KVDtype.q8_0, true, CODEC_DIST_REL, CODEC_PAIR_REL)
    }
    t |> run("Qwen3-0.6B: the device steps over tq4 rows the CPU prefill wrote") @(t : T?) {
        cell_codec_distance(t, CARRIER_TQ4, 40l, KVDtype.tq4, true, CODEC_DIST_REL, CODEC_PAIR_REL_TQ4)
    }
}

[test]
def test_gpu_resident_qwen2_codec_rows(t : T?) {
    t |> run("Qwen2.5-0.5B: the q8_0 mirror's stored rows against the CPU cache's, layer 0") @(t : T?) {
        cell_codec_stored_rows(t, CARRIER, KVDtype.q8_0, ROWS_BARS)
    }
    t |> run("Qwen3-0.6B: the tq4 mirror's stored rows against the CPU cache's, layer 0") @(t : T?) {
        cell_codec_stored_rows(t, CARRIER_TQ4, KVDtype.tq4, ROWS_BARS_TQ4)
    }
}
