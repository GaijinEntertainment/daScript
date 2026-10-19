options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer
require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_env
require _model_tier
require _resident_feed
require _resident_regions

// A prompt continued on the whole-model driver: its second half as a continuation window from the first half's position, and the
// same second half from a checkpoint a fresh session adopts after the first session stepped past it (the recurrent state and, on a
// hyper-connection model, the n-gram conv ring ride the checkpoint row; the pre-step's id window comes from the tokens) - each
// against the whole prompt through one window, the prompt one token short as the control. The hybrid carrier (deltanet and
// attention, no routed experts) reads the window chain's continuation alone through the regions' `continue_at`; the
// hyper-connection carrier adds the side input: its logits at the wide bar (the pool's fill and the meet's split flip a near-tie
// expert run to run), its n-gram conv ring after the continuation row for row against the CPU chain's, a zeroed ring the
// control. Skips: no file, no armed tier, DASLLAMA_VK_KV32=1 (the f32 mirror passes every f16 session as codec).

let private CONT_BAR_REL = 0.05         //! the same device chain cut at a different place, the one-token-off control its teeth
let private ROUTED_WIDE_BAR_REL = 0.5   //! a routed carrier's device run against itself reads up to 20%: the bar holds the routing flips, the ring compare carries the teeth
let private RING_BAR_REL = 0.02         //! the device's n-gram ring against the CPU chain's: the window's rows and the side rows rounded once each
let private HALF = 48l                  //! past the hot pool's 32-row window floor on a routed carrier
let private HYBRID = "Qwen3.5-0.8B-Q4_K_M.gguf"
let private FN_TRUNK = "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf"

def private continuation_ready(t : T?; path : string) : bool {
    if (!resident_ready(t, path)) return false
    if (g_env_vulkan.vk_kv32 != 0l) {
        t |> skip("DASLLAMA_VK_KV32=1: the f32 mirror passes every f16 session as codec")
        return false
    }
    return true
}

//! the second half from a checkpoint a fresh session adopts: the first half prefilled, a checkpoint kept, one step past it, the
//! claim parked and adopted behind the step, the id window seeded from the tokens, the continuation served; the adopted row back
def private adopted_row(t : T?; tr : Model; prompt, first, rest : array<int64>) : array<float> {
    var sa <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sa, first, HALF, 0l)
    sa.n_past = HALF
    t |> success(gpu_device_kv_checkpoint(sa), "the region kept a checkpoint at the first half's end")
    forward(tr, sa, prompt[HALF], HALF)
    let claim = gpu_device_kv_park(sa)
    t |> success(claim != 0l, "the stepped session parked its claim")
    var sb <- create_session(tr, mirror_kv_dtype())
    t |> success(gpu_device_kv_adopt(sb, claim, HALF), "a fresh session adopted the claim at the checkpoint, behind the step")
    if (!empty(sb.ple_prev)) {
        ple_window_seed(sb.ple_prev, prompt, HALF)
    }
    sb.n_past = HALF
    forward_prefill(tr, sb, rest, HALF, HALF)
    served_every_call(t)
    var adopted <- [for (x in sb.logits); x]
    delete sb
    delete sa
    return <- adopted
}

[test]
def test_gpu_resident_continuation_hybrid(t : T?) {
    t |> run("Qwen3.5-0.8B: a prompt continued from its position on the device, and from an adopted checkpoint, against one window") @(t : T?) {
        let path = path_join(models_dir(), HYBRID)
        if (!continuation_ready(t, path)) return
        var tr <- load_resident(path)
        t |> success(tr.config.recr_mask != 0ul, "the carrier is the hybrid with the recurrent state a continuation carries")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on it (the load log names any decline)")
        with_dasllama_jobque_() {
            continue_at(t, tr, 2l * HALF, HALF, CONT_BAR_REL)
            var prompt <- resident_prompt(2l * HALF)
            var first <- [for (i in range64(HALF)); prompt[i]]
            var rest <- [for (i in range64(HALF, 2l * HALF)); prompt[i]]
            var short_logits <- short_control(tr, prompt, 2l * HALF)
            var sw <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sw, prompt, 2l * HALF, 0l)
            var whole <- [for (x in sw.logits); x]
            delete sw
            var adopted <- adopted_row(t, tr, prompt, first, rest)
            let bar = row_bar(whole, CONT_BAR_REL)
            control_past(t, "the whole prompt's last row", whole, short_logits, bar)
            row_within_say(t, tr, "the continuation from the adopted checkpoint against the whole prompt", adopted, whole, bar)
            delete adopted
            delete whole
            delete short_logits
            delete rest
            delete first
            delete prompt
        }
        delete tr
    }
}

[test]
def test_gpu_resident_continuation_hc(t : T?) {
    t |> run("Qwen3.8-Flash-Next: a prompt continued from its position on the device, and from an adopted checkpoint, against one window; the n-gram ring against the CPU chain's") @(t : T?) {
        let path = path_join(models_dir(), FN_TRUNK)
        if (!continuation_ready(t, path)) return
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(tr.config.hyper_conn && has_ple_ngram(tr.config), "the carrier is the hyper-connection model with the n-gram side input the continuation hands off")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on it (the load log names any decline)")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(2l * HALF)
            var first <- [for (i in range64(HALF)); prompt[i]]
            var rest <- [for (i in range64(HALF, 2l * HALF)); prompt[i]]
            let served0 = moe_gpu_resident_prefills()
            var sw <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sw, prompt, 2l * HALF, 0l)
            var whole <- [for (x in sw.logits); x]
            delete sw
            var sc <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sc, first, HALF, 0l)
            sc.n_past = HALF
            forward_prefill(tr, sc, rest, HALF, HALF)
            t |> equal(moe_gpu_resident_prefills(), served0 + 3l, "the window chain served every prefill: the whole prompt, the first half and its continuation (none passed to the CPU rails)")
            served_every_call(t)
            var cont <- [for (x in sc.logits); x]
            var into : DnSnapshot
            var snap <- gpu_device_kv_snapshot(sc, 2l * HALF, into)
            t |> equal(snap.pos, 2l * HALF, "the device's snapshot stands at the continuation's end")
            var ring := snap.ple_hist
            delete snap
            delete sc
            var adopted <- adopted_row(t, tr, prompt, first, rest)
            let wide = row_bar(whole, ROUTED_WIDE_BAR_REL)
            row_within_say(t, tr, "the continuation from the first half's position against the whole prompt", cont, whole, wide)
            row_within_say(t, tr, "the continuation from the adopted checkpoint against the whole prompt", adopted, whole, wide)
            row_within_say(t, tr, "the continuation from the adopted checkpoint against the one continued in place", adopted, cont, wide)
            //! the ring's teeth: the CPU chain's ring over the same prompt, the device dropped so the chain runs on the host
            moe_gpu_drop_model()
            var sh <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sh, prompt, 2l * HALF, 0l)
            t |> equal(long_length(ring), long_length(sh.ple_conv_hist), "the device's ring and the host's hold the same rows")
            let ring_bar = row_bar(sh.ple_conv_hist, RING_BAR_REL)
            row_within(t, "the n-gram conv ring after the continuation against the CPU chain's", ring, sh.ple_conv_hist, ring_bar)
            var zeroed : array<float>
            zeroed |> resize(long_length(ring))
            control_past(t, "the n-gram conv ring", ring, zeroed, ring_bar)
            delete zeroed
            delete sh
            delete adopted
            delete ring
            delete cont
            delete whole
            delete rest
            delete first
            delete prompt
        }
        delete tr
    }
}
