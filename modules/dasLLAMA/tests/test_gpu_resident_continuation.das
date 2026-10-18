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

// A prompt continued on the whole-model driver: its second half as a continuation window from the first half's position, and the
// same second half from a checkpoint a fresh session adopts after the first session stepped past it (the recurrent state and, on a
// hyper-connection model, the n-gram conv ring ride the checkpoint row; the pre-step's id window comes from the tokens) - each
// against the whole prompt through one window, the prompt one token short as the control. The hybrid carrier (deltanet and
// attention, no routed experts) reads the window chain's continuation alone; the hyper-connection carrier adds the side input.

//! the continuation's bar: the same device chain cut at a different place, the one-token-off control its teeth; a routed carrier reads every compare at the wide bar - the pool's fill and the meet's split change run to run, and a near-tie expert flips on the rounding
let private CONT_BAR_REL = 0.05
let private ROUTED_WIDE_BAR_REL = 0.5
let private HALF = 48l   //! past the hot pool's 32-row window floor on a routed carrier
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

//! the three last rows against the whole prompt's: continued in one session, continued from an adopted checkpoint, and the control;
//! `routed` reads the whole-prompt compare at the wide bar and skips its control, the two continuations against each other tight
[cold_path]
def private continuation_cell(t : T?; tr : Model; routed : bool) {
    let half = HALF
    var prompt <- resident_prompt(2l * half)
    var first <- [for (i in range64(half)); prompt[i]]
    var rest <- [for (i in range64(half, 2l * half)); prompt[i]]
    var short <- [for (i in range64(2l * half - 1l)); prompt[i]]   //! the control: the prompt one token short
    let served0 = moe_gpu_resident_prefills()
    var sw <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sw, prompt, 2l * half, 0l)
    var whole <- [for (x in sw.logits); x]
    delete sw
    var sx <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sx, short, 2l * half - 1l, 0l)
    var control <- [for (x in sx.logits); x]
    delete sx
    let bar = row_bar(whole, CONT_BAR_REL)
    let whole_bar = row_bar(whole, routed ? ROUTED_WIDE_BAR_REL : CONT_BAR_REL)
    if (!routed) {
        control_past(t, "the whole prompt's last row", whole, control, bar)
    }
    var sc <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sc, first, half, 0l)
    sc.n_past = half
    forward_prefill(tr, sc, rest, half, half)
    t |> equal(moe_gpu_resident_prefills(), served0 + 4l, "the window chain served every prefill: the whole prompt, the control, the first half and its continuation (none passed to the CPU rails)")
    served_every_call(t)
    var cont <- [for (x in sc.logits); x]
    delete sc
    row_within(t, "the continuation from the first half's position against the whole prompt", cont, whole, whole_bar)
    var sa <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sa, first, half, 0l)
    sa.n_past = half
    t |> success(gpu_device_kv_checkpoint(sa), "the region kept a checkpoint at the first half's end")
    forward(tr, sa, prompt[half], half)   //! one step past the checkpoint: the region's state (and ring) move on
    let claim = gpu_device_kv_park(sa)
    t |> success(claim != 0l, "the stepped session parked its claim")
    var sb <- create_session(tr, mirror_kv_dtype())
    t |> success(gpu_device_kv_adopt(sb, claim, half), "a fresh session adopted the claim at the checkpoint, behind the step")
    if (!empty(sb.ple_prev)) {
        ple_window_seed(sb.ple_prev, prompt, half)
    }
    sb.n_past = half
    forward_prefill(tr, sb, rest, half, half)
    served_every_call(t)
    var adopted <- [for (x in sb.logits); x]
    row_within(t, "the continuation from the adopted checkpoint against the whole prompt", adopted, whole, whole_bar)
    row_within(t, "the continuation from the adopted checkpoint against the one continued in place (the same windows on the same state)", adopted, cont, whole_bar)
    delete sb
    delete sa
    delete adopted
    delete cont
    delete control
    delete whole
    delete short
    delete rest
    delete first
    delete prompt
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
            continuation_cell(t, tr, false)
        }
        delete tr
    }
}

[test]
def test_gpu_resident_continuation_hc(t : T?) {
    t |> run("Qwen3.8-Flash-Next: a prompt continued from its position on the device, and from an adopted checkpoint, against one window") @(t : T?) {
        let path = path_join(models_dir(), FN_TRUNK)
        if (!continuation_ready(t, path)) return
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(tr.config.hyper_conn && has_ple_ngram(tr.config), "the carrier is the hyper-connection model with the n-gram side input the continuation hands off")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on it (the load log names any decline)")
        with_dasllama_jobque_() {
            continuation_cell(t, tr, true)
        }
        delete tr
    }
}
