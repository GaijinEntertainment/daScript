options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen35)
require dasllama/dasllama   // create_session, moe_gpu_drop_model

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the hybrid
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, the resolved coopmat mode and the kernel census, where the module exists
require ?vulkan dasllama/dasllama_vulkan_prefill   // pf_f16_feed: the feed admission the driver acts on
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed

// The whole-model resident driver on a deltanet hybrid under DASLLAMA_GPU=1: the resident window
// chain prefills the prompt (the recurrent layers through the conv + chunked scan over their device
// state, the gated partial-rope attention over the mirror), then the resident decode steps every
// layer - the recurrent ones through the fused deltanet step - as one submit per token. The GPU
// chains and the CPU chain are not bit-identical (GPU exp / f32 accumulation order), so the gate is
// the forced-feed logits-tolerance form: the same fixed tokens fed to both arms, the logits within
// a bar of the CPU chain after the prefill and at every step, and the one-step-off control that
// must exceed it. Three fixtures: the Q8_0 file over four prompt lengths - one window, two windows
// (the conv tail and the state carry across the window seam), one past the window, and two tokens
// (both conv-tail edges: the last window shorter than the taps); the K-quant twin (Q4_K_M) over one
// and two windows; and the mixed twin (Q4_K_M with the deltanet out plane at Q8_0) over the same
// two, whose recurrent layers take a K-quant x feed and a Q8_0 o feed - the routing witness below
// holds that the x feed rode the coopmat tiles. The NextN-headed twin (the -MTP- file) holds the
// speculative carry the driver lands - the prefill's last row and every step's post-final-norm
// hidden as `mtp_h`, its watermark as `mtp_h_pos1` - one row at a time and through the N-row command,
// and the NextN draft through the head the driver homes against the CPU `forward_mtp` on one token,
// carry and row, then the round's drafts with the draft seat unset and put back.
// Every model cell skips interpreted, without its model file, or without the armed tier; the
// K-quant cells skip on the f32 mirrors (`DASLLAMA_VK_KV32=1`) too.

let private DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit| - the deltanet bar test_deltanet.das holds too
//! the K-quant bar on the f16 feed and mirror: CPU-vs-device rounding sits flat at 0.39-0.57 of a ~12.5 max logit at every step (the Q8 file 0.19-0.36), with no growth and the one-step-off control well past it
let private DN_LOGIT_BAR_REL_KQ_F16_FEED = 0.06
//! the K-quant bar on the quant feed (no coopmat tile serves the planes, the prefill's activations ride Q8_K): the mixed twin's two-window cell under `DASLLAMA_COOPMAT=sdot4` on the RTX 5060 Ti reads 0.084 of an 11.6 max logit at one step, argmax unchanged, the rest under 0.05
let private DN_LOGIT_BAR_REL_KQ_QUANT_FEED = 0.10
let private PF_WINDOW_ROWS = 512l     //! the engine's PF_WINDOW, asserted equal by the two-window cells: a prompt past it runs as sequential windows
let private RQ_NO_CENSUS = -1l        //! the census count with no vulkan module: the routing witness stands down

//! the plane formats a fixture's deltanet layers carry: all Q8_0, all K-quant, or K-quant qkv/z beside a Q8_0 out plane
enum HybridPlanes {
    q8
    kq
    kq_q8out
}

def private window_pin(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
    }
}

//! the coverage census's count of Q8_K requant dispatches so far; RQ_NO_CENSUS with no vulkan module
def private q8k_requants() : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_kernel_coverage_of("cls_q8k_rq_spv")
    } else {
        return RQ_NO_CENSUS
    }
}

//! the driver's own answer: does the prefill's f16 feed admit a K-quant plane on the armed coopmat mode
def private f16_feed_admits_kq() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return pf_f16_feed(int(KqFmt.k4))
    } else {
        return false
    }
}

// prefill `n` tokens + 8 fed steps on the resident driver, then the same on the all-CPU chain; the
// prefill logits and every step's within the deltanet bar, the one-step-off control past it.
def private hybrid_forced_feed(t : T?; n : int64; file : string; planes : HybridPlanes) {
    let kq = planes != HybridPlanes.q8
    let q8out = planes == HybridPlanes.kq_q8out
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return
    }
    let path = path_join(models_dir(), file)
    if (!model_available(t, path)) return
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return
    }
    if (kq && mirror_kv_dtype() == KVDtype.f32) {
        t |> skip("the K-quant bar is calibrated on the f16 mirror the tier arms; the f32 A/B mirror (DASLLAMA_VK_KV32=1) is no gate for it")
        return
    }
    var tr <- load_model_(path, QuantMode.q8)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the hybrid (the load log names any decline)")
    var nrec = 0l
    if (kq) {
        var native = 0l
        for (l in range64(tr.config.n_layers)) {
            if (!layer_is_recurrent(tr.config, l)) continue
            nrec++
            if (fmt_at(tr.dnqkv_fmt, l) != KqFmt.q8 && fmt_at(tr.dngate_fmt, l) != KqFmt.q8
                    && ((fmt_at(tr.dnout_fmt, l) == KqFmt.q8) == q8out)) {
                native++
            }
        }
        let out_form = q8out ? "the out plane Q8_0" : "the out plane too"
        t |> success(native > 0l, "the loader kept the deltanet qkv/z planes in their K-quant file formats, {out_form} ({native} recurrent layers)")
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]
        var toks <- [for (i in range64(8l)); 2000l + (i * 53l) % 4000l]
        let rq0 = q8k_requants()
        var prefill_q8k_rq = RQ_NO_CENSUS
        var fed <- resident_then_cpu_feed(t, tr, prompt, toks, $ {
            prefill_q8k_rq = rq0 == RQ_NO_CENSUS ? RQ_NO_CENSUS : q8k_requants() - rq0
        }) $ {}
        if (kq && prefill_q8k_rq != RQ_NO_CENSUS) {
            //! off the f16 feed the x route requants once per recurrent layer per window - the threshold both arms read
            to_log(LOG_INFO, "prefill ({n} tokens) [{tile_arm_tag()}]: {prefill_q8k_rq} Q8_K requants over {nrec} recurrent layers, f16 feed admits kq {f16_feed_admits_kq()}\n")
            if (f16_feed_admits_kq()) {
                t |> success(prefill_q8k_rq < nrec, "on the f16 feed the prefill never requantized the block input to Q8_K for the qkv/z pair ({prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers) [{tile_arm_tag()}]")
            } else {
                t |> success(prefill_q8k_rq >= nrec, "off the f16 feed the K-quant qkv/z pair reads the Q8_K form ({prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers) [{tile_arm_tag()}]")
            }
        }
        let rel = kq ? (f16_feed_admits_kq() ? DN_LOGIT_BAR_REL_KQ_F16_FEED : DN_LOGIT_BAR_REL_KQ_QUANT_FEED) : DN_LOGIT_BAR_REL
        feed_steps_within(t, tr, "", n, fed.gpu, fed.cpu, toks, rel)
        delete fed
        delete prompt
        delete toks
    }
    delete tr
}

[test]
def test_gpu_resident_hybrid_decode(t : T?) {
    t |> run("Qwen3.5-0.8B: resident prefill + decode with deltanet layers on device vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

[test]
def test_gpu_resident_hybrid_two_windows(t : T?) {
    t |> run("Qwen3.5-0.8B: the resident prefill across two windows (the conv tail and state carry the seam)") @(t : T?) {
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

// the conv-tail edge: a prompt one past the window (the last window would carry one row, fewer
// than the three conv taps - the chain keeps it at the taps by shortening the window before), and
// a prompt shorter than the taps (one window, the ring's leading rows zero)
[test]
def test_gpu_resident_hybrid_short_windows(t : T?) {
    t |> run("Qwen3.5-0.8B: a prompt one past the window - the last window still carries the conv taps") @(t : T?) {
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 1l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
    t |> run("Qwen3.5-0.8B: a two-token prompt - one window shorter than the conv taps") @(t : T?) {
        hybrid_forced_feed(t, 2l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

[test]
def test_gpu_resident_hybrid_kq(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M: K-quant deltanet planes on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M.gguf", HybridPlanes.kq)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M: the same across two windows") @(t : T?) {
        window_pin(t)
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q4_K_M.gguf", HybridPlanes.kq)
    }
}

[test]
def test_gpu_resident_hybrid_kq_q8out(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: K-quant qkv/z with a Q8_0 out plane on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", HybridPlanes.kq_q8out)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: the same across two windows") @(t : T?) {
        window_pin(t)
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", HybridPlanes.kq_q8out)
    }
}

let private MTP_CARRY_FILE = "Qwen3.5-0.8B-MTP-Q8_0.gguf"   //! the hybrid's NextN-headed twin: the driver lands the carry only on a model whose head reads it
let private CARRY_STEPS = 4l
let private CARRY_RIG_CTX = 4096l   //! the host sessions' context: the file's 262144 would size every session's K/V cache in gigabytes

//! the speculative carry a session holds: `mtp_h`, the post-final-norm hidden of its last row, and `mtp_h_pos1`, that row's position plus one
struct private Carry {
    h : array<float>
    pos1 : int64
}

def private carry_of(s : Session) : Carry => <- Carry(h <- [for (x in s.mtp_h); x], pos1 = s.mtp_h_pos1)

//! `prompt` then `toks` through a fresh session a row at a time, the carry kept after the prefill and after each step
def private carries_alone(tr : Model; prompt, toks : array<int64>) : array<Carry> {
    var s <- create_session(tr, mirror_kv_dtype())
    let n = long_length(prompt)
    var out : array<Carry>
    out |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    out |> emplace <| carry_of(s)
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out |> emplace <| carry_of(s)
    }
    delete s
    return <- out
}

//! every resident carry's watermark at its row's position plus one and its hidden within the deltanet bar of the CPU chain's,
//! with the CPU chain's previous carry (one step off) past the bar
def private carries_within(t : T?; what : string; nprompt : int64; gpu, cpu : array<Carry>) {
    for (st in range64(long_length(cpu))) {
        let tag = what + (st == 0l ? " prefill ({nprompt} tokens)" : " step {st - 1l}")
        t |> equal(gpu[st].pos1, nprompt + st, "{tag}: the resident carry's watermark is its row's position plus one")
        t |> equal(cpu[st].pos1, nprompt + st, "{tag}: the CPU chain's watermark")
        t |> equal(long_length(gpu[st].h), long_length(cpu[st].h), "{tag}: one hidden row a side")
        let bar = DN_LOGIT_BAR_REL * logits_maxabs(cpu[st].h)
        let diff = logits_maxdiff(gpu[st].h, cpu[st].h)
        to_log(LOG_INFO, "{tag}: carry maxdiff {diff} (bar {bar})\n")
        t |> success(diff <= bar, "{tag}: the resident carry within {bar} of the CPU chain's post-norm hidden (maxdiff {diff})")
        if (st > 0l) {
            let off = logits_maxdiff(gpu[st].h, cpu[st - 1l].h)
            to_log(LOG_INFO, "{tag}: one-step-off control maxdiff {off} (bar {bar})\n")
            t |> success(off > bar, "{tag} control: the bar discriminates the carry one step off (maxdiff {off} vs bar {bar})")
        }
    }
}

def private mtp_carry_ready(t : T?) : bool {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return false
    }
    if (!model_available(t, path_join(models_dir(), MTP_CARRY_FILE))) return false
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return false
    }
    return true
}

//! the -MTP- file at `regions` mirror regions on the resident route, the driver armed on it, the host sessions' context capped;
//! both levers go back where they stood
def private load_mtp_carrier(t : T?; regions : int64) : Model {
    let regions0 = gpu_resident_regions_()
    let route0 = gpu_want_resident()
    set_gpu_resident_regions_(regions)
    set_gpu_resident_route(true)
    var tr <- load_model_(path_join(models_dir(), MTP_CARRY_FILE), QuantMode.q8)
    set_gpu_resident_route(route0)
    set_gpu_resident_regions_(regions0)
    tr.config.seq_len = min(tr.config.seq_len, CARRY_RIG_CTX)
    t |> equal(tr.config.n_layer_nextn, 1l, "the -MTP- file carries its NextN head")
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the NextN-headed hybrid at {regions} regions (the load log names any decline)")
    return <- tr
}

def private served_every_call(t : T?) {
    var passes <- gpu_cpu_passes_()
    t |> equal(length(passes), 0, "the device served every call: {passes}")
    delete passes
}

[test]
def test_gpu_resident_hybrid_mtp_carry(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the resident prefill and each step land the post-norm hidden as the speculative carry, against the CPU chain's") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(40l)); 1000l + (i * 37l) % 5000l]
            var toks <- [for (i in range64(CARRY_STEPS)); 2000l + (i * 53l) % 4000l]
            let served0 = moe_gpu_resident_prefills()
            var gpu <- carries_alone(tr, prompt, toks)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the prompt")
            served_every_call(t)
            moe_gpu_drop_model()   //! every later step is the CPU chain
            var cpu <- carries_alone(tr, prompt, toks)
            carries_within(t, "one row:", long_length(prompt), gpu, cpu)
            delete gpu
            delete cpu
            delete prompt
            delete toks
        }
        delete tr
    }
}

[test]
def test_gpu_resident_hybrid_mtp_carry_batched(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: two sessions batched through the N-row command land each row's carry, against each session alone on the CPU chain") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 2l)
        with_job_que() {
            setup_dasllama_jobque_()
            var pa <- [for (i in range64(40l)); 1000l + (i * 37l) % 5000l]
            var pb <- [for (i in range64(57l)); 3000l + (i * 91l) % 6000l]
            var fa <- [for (i in range64(CARRY_STEPS)); 2000l + (i * 53l) % 4000l]
            var fb <- [for (i in range64(CARRY_STEPS)); 2500l + (i * 71l) % 4500l]
            var sa <- create_session(tr, mirror_kv_dtype())
            var sb <- create_session(tr, mirror_kv_dtype())
            eval(tr, sa, pa)   // `eval` advances `n_past`, the position a batched step reads
            eval(tr, sb, pb)
            var ga : array<Carry>
            var gb : array<Carry>
            ga |> emplace <| carry_of(sa)
            gb |> emplace <| carry_of(sb)
            var ws <- create_batch_workspace(tr)
            var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
            let steps0 = rdec_nrow_steps()
            for (st in range64(CARRY_STEPS)) {
                var toks <- [fa[st], fb[st]]
                eval_batch(tr, ws, sess, toks)
                delete toks
                ga |> emplace <| carry_of(sa)
                gb |> emplace <| carry_of(sb)
            }
            t |> equal(rdec_nrow_steps() - steps0, CARRY_STEPS, "every batched step went through the N-row command")
            served_every_call(t)
            sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete sa
            delete sb
            moe_gpu_drop_model()   //! every later step is the CPU chain
            var ca <- carries_alone(tr, pa, fa)
            var cb <- carries_alone(tr, pb, fb)
            carries_within(t, "batched row A:", long_length(pa), ga, ca)
            carries_within(t, "batched row B:", long_length(pb), gb, cb)
            delete ga
            delete gb
            delete ca
            delete cb
            delete pa
            delete pb
            delete fa
            delete fb
        }
        delete tr
    }
}

let private DRAFT_PROMPT = 40l
let private DRAFT_TOKEN = 2000l

//! a draft's two outputs: the logits and the head's own hidden `forward_mtp` leaves in the session
struct private Draft {
    logits : array<float>
    h : array<float>
}

//! `forward_mtp` of `tok` at head row `row` from the carry `h0`, through whichever seat the process has installed
def private draft_from(tr : Model; var s : Session; tok, row : int64; h0 : array<float>) : Draft {
    copy_floats(h0, 0l, s.mtp_h, 0l, long_length(h0))
    forward_mtp(tr, s, tok, row)
    return <- Draft(logits <- [for (x in s.logits); x], h <- [for (x in s.mtp_h); x])
}

//! `n` prompt tokens through the resident prefill with the speculative warm on, so the head's host rows the draft hydrates hold history
def private prefill_warm(tr : Model; var s : Session; prompt : array<int64>) {
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    forward_prefill(tr, s, prompt, long_length(prompt), 0l)
    set_mtp_spec(spec0)
    s.n_past = long_length(prompt)
}

//! the device draft's `what` within the deltanet bar of the CPU draft's, the CPU draft one row back past it
def private draft_within(t : T?; what : string; gpu, cpu, off : array<float>) {
    let bar = DN_LOGIT_BAR_REL * logits_maxabs(cpu)
    let diff = logits_maxdiff(gpu, cpu)
    let offd = logits_maxdiff(gpu, off)
    to_log(LOG_INFO, "draft {what}: maxdiff {diff}, one-step-off control {offd} (bar {bar})\n")
    t |> success(diff <= bar, "the resident draft's {what} within {bar} of the CPU forward_mtp's (maxdiff {diff})")
    t |> success(offd > bar, "control: the bar discriminates the CPU draft one row back ({what} maxdiff {offd} vs bar {bar})")
}

[test]
def test_gpu_resident_hybrid_mtp_draft(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: one NextN draft through the resident head against the CPU forward_mtp on the same token, carry and row") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            t |> equal(s.mtp_h_pos1, DRAFT_PROMPT, "the resident prefill landed the carry at the prompt's end")
            var h0 <- [for (x in s.mtp_h); x]
            let row = DRAFT_PROMPT - 1l   //! the head's row pos - 1: the round's draft at position pos
            let drafts0 = moe_gpu_resident_drafts()
            var gpu <- draft_from(tr, s, DRAFT_TOKEN, row, h0)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the resident head drafted")
            t |> equal(s.mtp_h_pos1, 0l, "the draft's hidden is the head's own: its watermark stays 0")
            let pick = moe_gpu_resident_draft_pick()
            let seat = unset_rdec_draft()
            var cpu <- draft_from(tr, s, DRAFT_TOKEN, row, h0)
            var off <- draft_from(tr, s, prompt[row], row - 1l, h0)   //! the draft at pos - 2, of the token the prompt holds at pos - 1
            reinstall_rdec_draft(seat)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the unset seat left both CPU drafts to forward_mtp")
            served_every_call(t)
            let ag = argmax_of(gpu.logits)
            let ac = argmax_of(cpu.logits)
            to_log(LOG_INFO, "draft (row {row}, token {DRAFT_TOKEN}): argmax cpu {ac} '{decode_(tr, [ac])}' ({cpu.logits[ac]}), resident {ag} '{decode_(tr, [ag])}' ({gpu.logits[ag]}), device pick {pick}\n")
            draft_within(t, "logits", gpu.logits, cpu.logits, off.logits)
            draft_within(t, "hidden", gpu.h, cpu.h, off.h)
            t |> equal(pick, ag, "the device pick is the argmax of the logits it landed")
            t |> equal(pick, ac, "the draft token equals the CPU draft's")
            delete gpu
            delete cpu
            delete off
            delete h0
            delete s
            delete prompt
        }
        delete tr
    }
    t |> run("Qwen3.5-0.8B-MTP: the speculative round drafts on the CPU with the draft seat unset, and on the device with it back") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            let drafts0 = moe_gpu_resident_drafts()
            let seat = unset_rdec_draft()
            var acc = 0l
            mtp_spec_eval(tr, s, DRAFT_TOKEN, acc)
            reinstall_rdec_draft(seat)
            t |> equal(s.mtp_drafted, 1l, "the first round drafted")
            t |> equal(moe_gpu_resident_drafts(), drafts0, "the first round's draft ran on the CPU: the unset seat left the head's counter")
            mtp_spec_eval(tr, s, argmax_of(s.logits), acc)
            set_mtp_spec(spec0)
            t |> equal(s.mtp_drafted, 2l, "the second round drafted")
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the second round's draft ran on the device: the seat back took it")
            delete s
            delete prompt
        }
        delete tr
    }
}

// model-free: the routing witness stands on the census answering by name - a name nothing seeded panics instead of reading a zero count
[test]
def test_kernel_census_by_name(t : T?) {
    t |> run("the kernel census refuses a name no kernel seeded") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            var tripped = false
            try {
                vk_kernel_coverage_of("no_such_kernel_spv")
            } recover {
                tripped = true
            }
            t |> success(tripped, "an unseeded census name panics, so a misspelt key cannot read as a zero count")
            t |> equal(vk_kernel_coverage_of("cls_q8k_rq_spv") >= 0l, true, "a seeded class kernel answers by its SPIR-V name, device or not")
        } else {
            t |> skip("no vulkan module in this build")
        }
    }
}
