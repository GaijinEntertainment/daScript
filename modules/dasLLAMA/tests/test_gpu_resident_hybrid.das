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
require dasllama/dasllama_kv_codec   // kv_row_to_f32: the head's K rows as floats, whichever codec the session carries
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
// carry and row, then the round's drafts with the draft seat unset and put back, and the round's
// same-slab verify rows bit for bit against one-row steps of the same tokens, accepted and rejected,
// and the head's prompt warm on the window chain: the host residual rows poisoned before a resident
// prefill leave the draft bit for bit, the head's warmed K rows against the CPU warm's within twice
// the deltanet bar, and six speculative rounds against the CPU round from one prompt.
// Every model cell skips interpreted, without its model file, or without the armed tier; the
// K-quant cells skip on the f32 mirrors (`DASLLAMA_VK_KV32=1`) too.

let private DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit| - the deltanet bar test_deltanet.das holds too
//! the K-quant bar on the f16 feed and mirror: CPU-vs-device rounding sits flat at 0.39-0.57 of a ~12.5 max logit at every step (the Q8 file 0.19-0.36), with no growth and the one-step-off control well past it
let private DN_LOGIT_BAR_REL_KQ_F16_FEED = 0.06
//! the K-quant bar on the quant feed (no coopmat tile serves the planes, the prefill's activations ride Q8_K): the mixed twin's two-window cell under `DASLLAMA_COOPMAT=sdot4` on the RTX 5060 Ti reads 0.084 of an 11.6 max logit at one step, argmax unchanged, the rest under 0.05
let private DN_LOGIT_BAR_REL_KQ_QUANT_FEED = 0.10
//! the head's K row is the f16-stored projection of a trunk hidden row that already sits within DN_LOGIT_BAR_REL of the CPU chain's, so the row's own rounding stacks on the trunk's: twice the deltanet bar (the pod reads 0.05 of the row's max at the widest, under either mirror codec)
let private HEAD_K_BAR_REL = 0.08
//! the CPU row one down misses a device row by at least this many bars, or the bar is too wide to tell the row from its neighbour
let private HEAD_K_CONTROL_BARS = 2.0
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

[test]
def test_gpu_resident_hybrid_mtp_draft_upload(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: a device draft past the rows the device holds brings the host's rows up with it and counts the device's rows through its own; the pass after it brings the drafted row down, and the CPU draft over the same rows picks the same token") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let pos = DRAFT_PROMPT
            var h0 <- [for (x in s.mtp_h); x]
            declined_draft(tr, s, DRAFT_TOKEN, pos - 1l)   //! the CPU rewrites the prompt's last head row on the host ...
            declined_draft(tr, s, DRAFT_TOKEN, pos)        //! ... and writes the one past it: two host rows the device does not hold
            t |> equal(moe_gpu_resident_head_rows(s), pos - 1l, "the device's rows stop below the CPU's two")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos + 1l, "the host holds every row through the CPU's second")
            var stale_row <- head_k_row(tr, s, pos + 1l)
            let drafts0 = moe_gpu_resident_drafts()
            var gpu <- draft_from(tr, s, DRAFT_TOKEN, pos + 1l, h0)   //! the device draft two rows past the device's: the host's two come up first
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the device drafted row {pos + 1l}")
            let pick = moe_gpu_resident_draft_pick()
            t |> equal(moe_gpu_resident_head_rows(s), pos + 2l, "the device holds every row through the draft's: the two it uploaded and its own")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos + 1l, "the host's copy stops below the draft's row")
            declined_draft(tr, s, DRAFT_TOKEN, pos + 2l)   //! the pass: the device's row pos + 1 comes down before the CPU reads it
            t |> equal(moe_gpu_resident_head_host_rows(s), pos + 3l, "the pass brought the device's row down and the CPU wrote the next: the host holds every row through it")
            var landed_row <- head_k_row(tr, s, pos + 1l)
            t |> success(apart(landed_row, stale_row) > 0l, "the device's K row {pos + 1l} came down on the pass: the host row moved")
            let seat = unset_rdec_draft()
            var cpu <- draft_from(tr, s, DRAFT_TOKEN, pos + 1l, h0)   //! the CPU draft over the host's rows, the hydrated one among them
            reinstall_rdec_draft(seat)
            served_every_call(t)
            let ac = argmax_of(cpu.logits)
            to_log(LOG_INFO, "draft over uploaded rows (row {pos + 1l}, token {DRAFT_TOKEN}): device pick {pick} '{decode_(tr, [pick])}', the CPU draft's argmax {ac} '{decode_(tr, [ac])}'\n")
            t |> equal(pick, ac, "the device draft over the uploaded rows picks the token the CPU draft over the host's rows picks")
            delete cpu
            delete gpu
            delete landed_row
            delete stale_row
            delete h0
            delete s
            delete prompt
        }
        delete tr
    }
}

//! row `row` (`n` wide) of the rows `a`
def private row_of(a : array<float>; row, n : int64) : array<float> => <- [for (i in range64(n)); a[row * n + i]]

//! the elements two rows hold apart, bit for bit (a NaN differs from everything)
def private apart(a, b : array<auto(TT)>) : int64 {
    var bad = 0l
    for (x, y in a, b) {
        if (x != y) {
            bad++
        }
    }
    return bad
}

//! the host bytes of the NextN head's K row `row`
def private head_k_row(tr : Model; var s : Session; row : int64) : array<uint8> {
    let c = tr.config
    let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, c.n_layers))
    let kp = kv_layer_kbase(s, c.n_layers, c.seq_len)
    return <- [for (i in range64(rowb)); unsafe(kp[row * rowb + i])]
}

//! a verify row against the one-row step of the same token, bit for bit: its logits and its post-norm hidden, the argmax both
//! sides read logged, and the step one row off (the other row's) as the control the compare can fail
def private verify_row_is_step(t : T?; tr : Model; row : int64; vlog, vhid : array<float>; step_log, step_h, off_log : array<float>) {
    let la = apart(vlog, step_log)
    let ha = apart(vhid, step_h)
    let ag = argmax_of(vlog)
    let ac = argmax_of(step_log)
    to_log(LOG_INFO, "verify row {row}: logits {la} apart (maxdiff {logits_maxdiff(vlog, step_log)}), hidden {ha} apart (maxdiff {logits_maxdiff(vhid, step_h)}); argmax verify {ag} '{decode_(tr, [ag])}', step {ac} '{decode_(tr, [ac])}'\n")
    t |> equal(la, 0l, "verify row {row}'s logits are the one-row step's bit for bit ({la} elements apart)")
    t |> equal(ha, 0l, "verify row {row}'s post-norm hidden is the one-row step's bit for bit ({ha} elements apart)")
    t |> success(apart(vlog, off_log) > 0l, "control: verify row {row} misses the other row's step logits")
}

let private VERIFY_ROWS = 2l   //! the CPU round's depth-1 verify: the committed token and its draft

//! a run of one-row steps: each step's logits and carry
struct private StepRows {
    logits : array<array<float>>
    h : array<array<float>>
}

//! `prompt` then `toks` through a fresh session a row at a time on the resident driver
def private steps_alone(tr : Model; prompt, toks : array<int64>) : StepRows {
    var s <- create_session(tr, mirror_kv_dtype())
    prefill_warm(tr, s, prompt)
    var out : StepRows
    let n = long_length(prompt)
    out.logits |> reserve(long_length(toks))
    out.h |> reserve(long_length(toks))
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out.logits |> emplace <| [for (x in s.logits); x]
        out.h |> emplace <| [for (x in s.mtp_h); x]
    }
    delete s
    return <- out
}

//! a draft the seat declines (unset for the call): the CPU head at row `row` reads the head's host rows below it, so the
//! device's rows come down first - the pass a cell reads the device's head rows through; the carry is put back after
def private declined_draft(tr : Model; var s : Session; tok, row : int64) {
    var h0 <- [for (x in s.mtp_h); x]
    let pos1 = s.mtp_h_pos1
    let seat = unset_rdec_draft()
    forward_mtp(tr, s, tok, row)
    reinstall_rdec_draft(seat)
    copy_floats(h0, 0l, s.mtp_h, 0l, long_length(h0))
    s.mtp_h_pos1 = pos1
    delete h0
}

//! one round's record under a pick ask beside one under the logits landing: the draft, the accept, the position and the
//! token the caller's sample_ takes next
struct private AskedRound {
    draft : int64
    hit : bool
    n_past : int64
    next : int64
}

//! `ROUNDS` rounds on `s` from `tok`, the next token taken through sample_ each round (a landed pick, or the logits row's
//! argmax); `ask` sets the pick ask before every round
def private asked_rounds(tr : Model; var s : Session; tok0 : int64; ask : bool) : array<AskedRound> {
    var out : array<AskedRound>
    out |> reserve(ROUNDS)
    var tok = tok0
    for (_r in range64(ROUNDS)) {
        s.pick_asked = ask
        var acc = 0l
        let hit = mtp_spec_eval(tr, s, tok, acc)
        s.pick_asked = false
        let next = sample_(s, SamplingParams())
        out |> push(AskedRound(draft = s.mtp_vbatch[1], hit = hit, n_past = s.n_past, next = next))
        tok = next
    }
    return <- out
}

[test]
def test_gpu_resident_hybrid_mtp_verify(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the round's two verify rows as same-slab rows through the resident seat, bit for bit the two one-row steps of the same tokens; the head's rows stay on the device and come down on a pass, the re-warm's row bit for bit the draft's") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            let c = tr.config
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let pos = DRAFT_PROMPT
            t |> equal(moe_gpu_resident_head_host_rows(s), pos - 1l, "the prompt warm's rows came down with the chain: the host holds the head's rows below the prompt's last")
            var head_row0 <- head_k_row(tr, s, pos - 1l)
            var h0 <- [for (x in s.mtp_h); x]
            let drafts0 = moe_gpu_resident_drafts()
            forward_mtp(tr, s, DRAFT_TOKEN, pos - 1l)   //! the device draft the round will repeat: its head row pos - 1, on the device alone
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the device drafted row {pos - 1l}")
            t |> equal(moe_gpu_resident_head_rows(s), pos, "the device holds the head's rows through the draft's")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos - 1l, "the host's copy stops below the draft's row: nothing came down")
            copy_floats(h0, 0l, s.mtp_h, 0l, c.dim)
            s.mtp_h_pos1 = pos
            declined_draft(tr, s, DRAFT_TOKEN, pos)   //! the pass: the CPU head at row pos reads the rows below, so row pos - 1 comes down
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the unset seat left the draft to the CPU head")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos + 1l, "the pass brought the device's rows down and the CPU wrote row {pos}: the host holds every row through it")
            t |> equal(moe_gpu_resident_head_rows(s), pos, "the device's copy stops below the CPU's row")
            var draft_row <- head_k_row(tr, s, pos - 1l)
            t |> success(apart(head_row0, draft_row) > 0l, "the draft's K row {pos - 1l} came down on the pass: the host row moved")
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            let verifies0 = moe_gpu_resident_verifies()
            var acc = 0l
            let hit = mtp_spec_eval(tr, s, DRAFT_TOKEN, acc)
            set_mtp_spec(spec0)
            t |> equal(moe_gpu_resident_verifies(), verifies0 + 1l, "the round's verify ran on the device")
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 2l, "the round's draft ran on the device")
            served_every_call(t)
            let d = s.mtp_vbatch[1]
            t |> equal(long_length(s.mtp_logits_b), VERIFY_ROWS * c.vocab_size, "every verify row's logits landed: no pick was asked")
            t |> equal(long_length(s.mtp_hrows), VERIFY_ROWS * c.dim, "every verify row's hidden landed")
            t |> success(!s.pick_ready, "no pick was asked, so none was published")
            var vlog0 <- row_of(s.mtp_logits_b, 0l, c.vocab_size)
            var vlog1 <- row_of(s.mtp_logits_b, 1l, c.vocab_size)
            var vhid0 <- row_of(s.mtp_hrows, 0l, c.dim)
            var vhid1 <- row_of(s.mtp_hrows, 1l, c.dim)
            t |> equal(moe_gpu_resident_verify_pick(0l), argmax_of(vlog0), "row 0's device pick is the argmax of the logits it landed")
            t |> equal(moe_gpu_resident_verify_pick(1l), argmax_of(vlog1), "row 1's device pick is the argmax of the logits it landed")
            t |> equal(s.n_past, pos + (hit ? VERIFY_ROWS : 1l), "the round committed the draft on an accept, the token alone on a reject")
            t |> equal(moe_gpu_resident_rows(s), s.n_past, "the region's rows end at the session's position ({hit ? "an accept keeps both verify rows" : "the reject's replay rewrote row pos"})")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos - 1l, "the round's head rows stayed on the device: the host's copy stops below the draft's row again")
            declined_draft(tr, s, DRAFT_TOKEN, s.n_past - 1l)   //! the pass after the round: the re-warm's row pos - 1 comes down
            t |> equal(moe_gpu_resident_head_host_rows(s), s.n_past, "the pass brought the round's rows down through the CPU draft's")
            var warm_row <- head_k_row(tr, s, pos - 1l)
            t |> equal(apart(warm_row, draft_row), 0l, "the re-warm's K row {pos - 1l} - the store-only pass over the k heads - is the draft command's row bit for bit: the same [enorm(token) ; hnorm(carry)] input ({apart(warm_row, draft_row)} bytes apart)")
            delete s
            var toks <- [DRAFT_TOKEN, d]
            var steps <- steps_alone(tr, prompt, toks)
            verify_row_is_step(t, tr, 0l, vlog0, vhid0, steps.logits[0], steps.h[0], steps.logits[1])
            verify_row_is_step(t, tr, 1l, vlog1, vhid1, steps.logits[1], steps.h[1], steps.logits[0])
            delete steps
            delete toks
            delete vlog0
            delete vlog1
            delete vhid0
            delete vhid1
            delete warm_row
            delete draft_row
            delete head_row0
            delete h0
            delete prompt
        }
        delete tr
    }
}

[test]
def test_gpu_resident_hybrid_mtp_verify_asked(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: six rounds under the caller's pick ask - the picks alone landed, the committed row's pick published for sample_ - token for token the rounds with the logits landing") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 2l)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            var sa <- create_session(tr, mirror_kv_dtype())
            var sb <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, sa, prompt)
            prefill_warm(tr, sb, prompt)
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            let verifies0 = moe_gpu_resident_verifies()
            let drafts0 = moe_gpu_resident_drafts()
            var asked <- asked_rounds(tr, sa, DRAFT_TOKEN, true)
            var landed <- asked_rounds(tr, sb, DRAFT_TOKEN, false)
            set_mtp_spec(spec0)
            t |> equal(moe_gpu_resident_verifies(), verifies0 + 2l * ROUNDS, "every round of both sessions verified on the device")
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 2l * ROUNDS, "every round of both sessions drafted on the device")
            served_every_call(t)
            for (r in range64(ROUNDS)) {
                to_log(LOG_INFO, "asked round {r}: draft {asked[r].draft} '{decode_(tr, [asked[r].draft])}' hit {asked[r].hit} at {asked[r].n_past} next {asked[r].next} '{decode_(tr, [asked[r].next])}'; landed round: draft {landed[r].draft} hit {landed[r].hit} at {landed[r].n_past} next {landed[r].next}\n")
                t |> equal(asked[r].draft, landed[r].draft, "round {r}: the pick-asked round drafts the token the logits-landing round drafts")
                t |> equal(asked[r].hit, landed[r].hit, "round {r}: both rounds accepted or both rejected")
                t |> equal(asked[r].n_past, landed[r].n_past, "round {r}: both rounds left the session at one position")
                t |> equal(asked[r].next, landed[r].next, "round {r}: the pick the asked round published is the argmax of the logits the other round landed")
            }
            t |> success(apart(sa.logits, sb.logits) > 0l, "the asked session's logits row is not the round's: the rounds landed picks alone (its row misses the landing session's)")
            delete asked
            delete landed
            delete sa
            delete sb
            delete prompt
        }
        delete tr
    }
}

//! the session's deltanet state and conv history as the host holds them once the device's copies come home
struct private DnHost {
    state : array<float>
    conv : array<float>
}

def private dn_host(var s : Session) : DnHost {
    dn_flush_all(s)
    var state <- [for (x in s.dn_state); x]
    var conv <- [for (x in s.dn_conv_state); x]
    return <- DnHost(state <- state, conv <- conv)
}

//! a fresh session stepped a row at a time: its deltanet state after `toks`, and the logits of one more step of `next`
struct private StateRun {
    dn : DnHost
    next_logits : array<float>
}

def private state_after_steps(tr : Model; prompt, toks : array<int64>; next : int64) : StateRun {
    var s <- create_session(tr, mirror_kv_dtype())
    prefill_warm(tr, s, prompt)
    let n = long_length(prompt)
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
    }
    var dn <- dn_host(s)
    forward(tr, s, next, n + long_length(toks))
    var next_logits <- [for (x in s.logits); x]
    delete s
    return <- StateRun(dn <- dn, next_logits <- next_logits)
}

[test]
def test_gpu_resident_hybrid_mtp_verify_reject(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: a rejected device verify rolls the region back on the device - no snapshot and no replayed step; the live deltanet slots and the next step bit for bit a one-row session's, the rows, head rows, dn_pos and carry at the token's row, the next round on the device again") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            let c = tr.config
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let pos = DRAFT_PROMPT
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            set_mtp_force_reject_every(1l)   //! every draft rejected: the rollback seat takes the reject
            let verifies0 = moe_gpu_resident_verifies()
            let rollbacks0 = moe_gpu_resident_rollbacks()
            var acc = 0l
            let hit = mtp_spec_eval(tr, s, DRAFT_TOKEN, acc)
            set_mtp_force_reject_every(0l)
            t |> success(!hit, "the forced reject rejected the draft")
            t |> equal(moe_gpu_resident_verifies(), verifies0 + 1l, "the rejected round's verify ran on the device")
            t |> equal(moe_gpu_resident_rollbacks(), rollbacks0 + 1l, "the reject rolled back on the device")
            t |> equal(long_length(s.mtp_snap_conv), 0l, "the round took no snapshot: its buffers were never sized")
            t |> equal(s.n_past, pos + 1l, "the reject committed the token alone")
            t |> equal(moe_gpu_resident_rows(s), pos + 1l, "the rollback cut the region's rows to the token's")
            t |> equal(moe_gpu_resident_head_rows(s), pos, "the region holds {pos} head rows after the reject: row {pos} held the rejected draft's input, and the next draft rewrites it")
            t |> equal(s.dn_pos, pos + 1l, "the recurrent state's position sits at the token's row")
            t |> equal(s.mtp_h_pos1, pos + 1l, "the carry sits at the token's row")
            let d = s.mtp_vbatch[1]
            var vlog0 <- row_of(s.mtp_logits_b, 0l, c.vocab_size)
            var vhid0 <- row_of(s.mtp_hrows, 0l, c.dim)
            var kept_log <- [for (x in s.logits); x]
            var kept_h <- [for (x in s.mtp_h); x]
            t |> equal(apart(kept_log, vlog0), 0l, "the reject kept verify row 0's logits as the session's")
            t |> equal(apart(kept_h, vhid0), 0l, "the reject re-seeded the carry from verify row 0's hidden")
            let next = argmax_of(kept_log)
            var rolled <- dn_host(s)   //! the live slots after the rollback, read back to the host
            forward(tr, s, next, pos + 1l)
            s.n_past = pos + 2l   //! a plain step moves no position; the round reads its position from here, and a stale one steps the recurrent state back
            var next_log <- [for (x in s.logits); x]
            mtp_spec_eval(tr, s, argmax_of(s.logits), acc)
            set_mtp_spec(spec0)
            t |> equal(moe_gpu_resident_verifies(), verifies0 + 2l, "the round after the reject verified on the device again")
            t |> equal(moe_gpu_resident_rows(s), s.n_past, "the region's rows end at the session's position after the next round")
            served_every_call(t)
            delete s
            var one <- state_after_steps(tr, prompt, [DRAFT_TOKEN], next)
            var two <- state_after_steps(tr, prompt, [DRAFT_TOKEN, d], next)   //! the control: the verify's two rows stepped - the state the rollback undid
            let sa = apart(rolled.state, one.dn.state)
            let ca = apart(rolled.conv, one.dn.conv)
            let la = apart(next_log, one.next_logits)
            to_log(LOG_INFO, "rollback: state {sa} apart, conv history {ca} apart, next step's logits {la} apart (maxdiff {logits_maxdiff(next_log, one.next_logits)}); argmax rolled {argmax_of(next_log)} '{decode_(tr, [argmax_of(next_log)])}', one-row session {argmax_of(one.next_logits)} '{decode_(tr, [argmax_of(one.next_logits)])}'; the two-row control {apart(two.dn.state, one.dn.state)} state elements apart\n")
            t |> equal(sa, 0l, "the rolled-back deltanet state is the one-row session's bit for bit ({sa} elements apart)")
            t |> equal(ca, 0l, "the rolled-back conv history is the one-row session's bit for bit ({ca} elements apart)")
            t |> equal(la, 0l, "the next plain step's logits are the one-row session's next step bit for bit ({la} elements apart)")
            t |> success(apart(two.dn.state, one.dn.state) > 0l, "control: the state after the verify's two rows misses the one-row session's")
            t |> success(apart(next_log, two.next_logits) > 0l, "control: the next step misses the two-row session's next step")
            delete two
            delete one
            delete rolled
            delete next_log
            delete kept_h
            delete kept_log
            delete vhid0
            delete vlog0
            delete prompt
        }
        delete tr
    }
}

[test]
def test_gpu_resident_hybrid_mtp_verify_reject_asked(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: a rejected device verify under the caller's pick ask publishes verify row 0's pick - the one-row step's argmax - in place of its logits, the carry re-seeded from its hidden") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            let c = tr.config
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let pos = DRAFT_PROMPT
            var before <- [for (x in s.logits); x]
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            set_mtp_force_reject_every(1l)
            let rollbacks0 = moe_gpu_resident_rollbacks()
            s.pick_asked = true
            var acc = 0l
            let hit = mtp_spec_eval(tr, s, DRAFT_TOKEN, acc)
            s.pick_asked = false
            set_mtp_force_reject_every(0l)
            set_mtp_spec(spec0)
            t |> success(!hit, "the forced reject rejected the draft")
            t |> equal(moe_gpu_resident_rollbacks(), rollbacks0 + 1l, "the reject rolled back on the device")
            served_every_call(t)
            t |> success(s.pick_ready, "the reject published a pick for the caller's sample_")
            t |> equal(s.pick_tok, moe_gpu_resident_verify_pick(0l), "the published pick is verify row 0's device pick")
            t |> equal(apart(s.logits, before), 0l, "the pick-asked round landed no logits: the session's row is the prefill's still")
            var vhid0 <- row_of(s.mtp_hrows, 0l, c.dim)
            var kept_h <- [for (x in s.mtp_h); x]
            t |> equal(apart(kept_h, vhid0), 0l, "the reject re-seeded the carry from verify row 0's hidden, which lands under the pick ask")
            t |> equal(s.mtp_h_pos1, pos + 1l, "the carry sits at the token's row")
            let next = sample_(s, SamplingParams())
            t |> equal(next, s.pick_tok, "sample_ returns the published pick")
            t |> success(!s.pick_ready, "... and consumes it")
            delete s
            var steps <- steps_alone(tr, prompt, [DRAFT_TOKEN])
            let ac = argmax_of(steps.logits[0])
            to_log(LOG_INFO, "pick-asked reject: published {next} '{decode_(tr, [next])}', the one-row step's argmax {ac} '{decode_(tr, [ac])}'\n")
            t |> equal(next, ac, "the published pick is the one-row step's argmax over the same token")
            delete steps
            delete kept_h
            delete vhid0
            delete before
            delete prompt
        }
        delete tr
    }
}

let private NAN_BITS = 0x7FC00000u
let private ROUNDS = 6l   //! the speculative rounds the device-vs-CPU cell runs, one fed token each

//! the NextN head's K row `row` of the session's cache as floats, whichever codec the session carries
def private head_k_row_f32(tr : Model; var s : Session; row : int64) : array<float> {
    let c = tr.config
    let kvd = layer_kv_dim(c, c.n_layers)
    let kp = kv_layer_kbase(s, c.n_layers, c.seq_len)
    var out : array<float>
    out |> resize(kvd)
    unsafe {
        if (s.kv_dtype_k == KVDtype.f32) {
            kv_row_to_f32(addr(out[0]), reinterpret<float const?>(kp), row * kvd, kvd)
        } else {
            kv_row_to_f32(addr(out[0]), reinterpret<uint16 const?>(kp), row * kvd, kvd)
        }
    }
    return <- out
}

//! the head's K rows [0, n) as floats
def private head_k_rows_f32(tr : Model; var s : Session; n : int64) : array<array<float>> {
    var out : array<array<float>>
    out |> reserve(n)
    for (r in range64(n)) {
        out |> emplace <| head_k_row_f32(tr, s, r)
    }
    return <- out
}

//! every byte of the head's host K and V rows [0, n) set, so any row the CPU head reads is NaN
def private poison_head_rows(tr : Model; var s : Session; n : int64) {
    let c = tr.config
    let kvd = layer_kv_dim(c, c.n_layers)
    let kb = kv_row_bytes(s.kv_dtype_k, kvd) * n
    let vb = kv_row_bytes(s.kv_dtype_v, kvd) * n
    var kp = kv_layer_kbase(s, c.n_layers, c.seq_len)
    var vp = kv_layer_vbase(s, c.n_layers, c.seq_len)
    unsafe {
        for (i in range64(kb)) {
            kp[i] = uint8(0xFF)
        }
        for (i in range64(vb)) {
            vp[i] = uint8(0xFF)
        }
    }
}

//! the session's residual rows sized for `n` positions and every float NaN before the prefill sizes them (the size holds, the fill stays)
def private poison_residual_rows(var s : Session; n : int64) {
    let nan = unsafe(reinterpret<float>(NAN_BITS))
    s.x_b |> reserve(n)
    s.x_b |> resize(n)
    for (i in range64(n)) {
        s.x_b[i] = nan
    }
}

def private all_finite(a : array<float>) : bool {
    for (x in a) {
        if (!(x == x) || x > 3.4e38 || x < -3.4e38) {   // nolint:LINT007 — the NaN test: a NaN is the one float that is not equal to itself
            return false
        }
    }
    return true
}

//! the head's rows [0, n) on `gpu` against `cpu` within `HEAD_K_BAR_REL` of each CPU row's max; the CPU row one down misses every device row by `HEAD_K_CONTROL_BARS` bars, the tightest such miss logged beside the widest
def private head_rows_within(t : T?; what : string; gpu, cpu : array<array<float>>; n : int64) {
    var worst = 0.0
    var worst_row = -1l
    var worst_bar = 0.0
    var tight_off = 0.0
    var tight_bar = 0.0
    var tight_row = -1l
    for (r in range64(n)) {
        let bar = HEAD_K_BAR_REL * logits_maxabs(cpu[r])
        let diff = logits_maxdiff(gpu[r], cpu[r])
        if (diff > worst) {
            worst = diff
            worst_row = r
            worst_bar = bar
        }
        t |> success(diff <= bar, "{what}: the device's head K row {r} is within {bar} of the CPU warm's row (maxdiff {diff})")
        if (r + 1l < n) {
            let off = logits_maxdiff(gpu[r], cpu[r + 1l])
            if (tight_row < 0l || off * tight_bar < tight_off * bar) {
                tight_off = off
                tight_bar = bar
                tight_row = r
            }
            t |> success(off >= HEAD_K_CONTROL_BARS * bar, "{what} control: the CPU row one down misses device row {r} by {HEAD_K_CONTROL_BARS} bars (maxdiff {off}, bar {bar})")
        }
    }
    to_log(LOG_INFO, "{what}: {n} head rows, the widest miss {worst} at row {worst_row} (bar {worst_bar}); the tightest one-down control {tight_off} at row {tight_row} (bar {tight_bar}, {tight_bar > 0.0 ? tight_off / tight_bar : 0.0} bars)\n")
}

[test]
def test_gpu_resident_hybrid_mtp_xb_poison(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the host residual rows poisoned with NaN before a resident prefill leave the device draft bit for bit the clean prefill's - the head's prompt rows warm on the device, off no host row") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            let row = DRAFT_PROMPT - 1l
            var clean <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, clean, prompt)
            t |> equal(moe_gpu_resident_head_rows(clean), DRAFT_PROMPT - 1l, "the region holds {DRAFT_PROMPT - 1l} head rows after the prompt warm: one per prompt row but the last, whose next token is not known yet")
            var h0 <- [for (x in clean.mtp_h); x]
            var want <- draft_from(tr, clean, DRAFT_TOKEN, row, h0)
            let pick_clean = moe_gpu_resident_draft_pick()
            delete clean
            var s <- create_session(tr, mirror_kv_dtype())
            poison_residual_rows(s, DRAFT_PROMPT * tr.config.dim)
            prefill_warm(tr, s, prompt)
            t |> equal(apart(s.mtp_h, h0), 0l, "the poisoned prefill lands the clean prefill's carry bit for bit")
            let drafts0 = moe_gpu_resident_drafts()
            var got <- draft_from(tr, s, DRAFT_TOKEN, row, h0)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the resident head drafted")
            served_every_call(t)
            let ag = argmax_of(got.logits)
            to_log(LOG_INFO, "poisoned x_b draft: argmax {ag} '{decode_(tr, [ag])}' ({got.logits[ag]}), the clean prefill's {argmax_of(want.logits)} ({want.logits[argmax_of(want.logits)]})\n")
            t |> success(all_finite(got.logits), "every landed logit is finite")
            t |> equal(apart(got.logits, want.logits), 0l, "the poisoned prefill's draft logits are the clean prefill's bit for bit ({apart(got.logits, want.logits)} elements apart)")
            t |> equal(apart(got.h, want.h), 0l, "the poisoned prefill's draft hidden is the clean prefill's bit for bit")
            t |> equal(moe_gpu_resident_draft_pick(), pick_clean, "the device pick is the clean prefill's")
            var off <- draft_from(tr, s, prompt[row], row - 1l, h0)   //! the control: the draft one row back differs
            t |> success(apart(off.logits, want.logits) > 0l, "control: the draft one row back misses the clean draft's logits")
            delete off
            delete got
            delete want
            delete h0
            delete s
            delete prompt
        }
        delete tr
    }
}

[test]
def test_gpu_resident_hybrid_mtp_head_warm(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the head's K rows the window chain warmed, read back to the host, against the CPU warm's rows on the same prompt within the deltanet bar, the row one down past it") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            let n = DRAFT_PROMPT - 1l   //! the head's rows below the prompt's last: the draft writes that one
            var s <- create_session(tr, mirror_kv_dtype())
            let served0 = moe_gpu_resident_prefills()
            prefill_warm(tr, s, prompt)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the prompt")
            t |> equal(moe_gpu_resident_head_rows(s), n, "the region holds {n} head rows after the prompt warm: one per prompt row but the last, whose next token is not known yet")
            served_every_call(t)
            var gpu <- head_k_rows_f32(tr, s, n)
            delete s
            moe_gpu_drop_model()   //! the CPU warm from here
            var c <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, c, prompt)
            var cpu <- head_k_rows_f32(tr, c, n)
            head_rows_within(t, "head warm", gpu, cpu, n)
            delete c
            delete gpu
            delete cpu
            delete prompt
        }
        delete tr
    }
}

//! one speculative round's record: the draft the round verified, whether it accepted it, and the position it left
struct private RoundRec {
    draft : int64
    hit : bool
    n_past : int64
}

//! `ROUNDS` speculative rounds on `s` from the prompt's end, one fed token each
def private spec_rounds(tr : Model; var s : Session; toks : array<int64>) : array<RoundRec> {
    var out : array<RoundRec>
    out |> reserve(long_length(toks))
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    for (tok in toks) {
        var acc = 0l
        let hit = mtp_spec_eval(tr, s, tok, acc)
        out |> push(RoundRec(draft = s.mtp_vbatch[1], hit = hit, n_past = s.n_past))
    }
    set_mtp_spec(spec0)
    return <- out
}

//! `toks` through `ROUNDS`-style rounds on `s` with the draft seat unset: the CPU head drafts over the host's rows, which the
//! declined seat hydrates from the device first, and the device verifies
def private declined_draft_rounds(tr : Model; var s : Session; toks : array<int64>) : array<RoundRec> {
    let seat = unset_rdec_draft()
    var out <- spec_rounds(tr, s, toks)
    reinstall_rdec_draft(seat)
    return <- out
}

[test]
def test_gpu_resident_hybrid_mtp_rounds(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: six speculative rounds on the resident driver against the CPU round from the same prompt - every round's draft, accept and position equal; then two rounds whose draft the seat declines - the CPU head over the head's rows hydrated from the device - against the CPU round, the hydrated rows within the deltanet bar, the CPU round on poisoned head rows as the control") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        var tr <- load_mtp_carrier(t, 1l)
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(DRAFT_PROMPT)); 1000l + (i * 37l) % 5000l]
            var toks <- [for (i in range64(ROUNDS + 2l)); 2000l + (i * 53l) % 4000l]
            var fed <- [for (i in range64(ROUNDS)); toks[i]]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let drafts0 = moe_gpu_resident_drafts()
            let verifies0 = moe_gpu_resident_verifies()
            var gpu <- spec_rounds(tr, s, fed)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + ROUNDS, "every round drafted on the device")
            t |> equal(moe_gpu_resident_verifies(), verifies0 + ROUNDS, "every round verified on the device")
            served_every_call(t)
            let nrows = s.n_past - 1l
            t |> equal(moe_gpu_resident_head_rows(s), nrows, "the region holds {nrows} head rows after six rounds, the session's position {s.n_past} less one: head row i needs the token at i + 1, the last committed token sits at {s.n_past - 1l}, so rows 0 .. {s.n_past - 2l} exist - a reject cuts the count to pos + a, an accept counts pos + 1, both the position less one")
            t |> success(moe_gpu_resident_head_host_rows(s) < nrows, "the rounds' head rows stayed on the device: the host's copy stops short of them ({moe_gpu_resident_head_host_rows(s)} of {nrows} rows)")
            var g7 <- declined_draft_rounds(tr, s, [toks[ROUNDS]])   //! the seventh round: the CPU head over the rows the pass hydrates
            t |> equal(moe_gpu_resident_drafts(), drafts0 + ROUNDS, "the seventh round's draft ran on the CPU head: the unset seat left the counter")
            t |> equal(moe_gpu_resident_verifies(), verifies0 + ROUNDS + 1l, "... and its verify on the device")
            var grows <- head_k_rows_f32(tr, s, nrows)   //! the rows below the seventh draft's row, hydrated on its pass
            var g8 <- declined_draft_rounds(tr, s, [toks[ROUNDS + 1l]])   //! the eighth: the control's twin
            served_every_call(t)
            delete s
            moe_gpu_drop_model()   //! the CPU round from here
            var c <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, c, prompt)
            var cpu <- spec_rounds(tr, c, fed)
            for (r in range64(ROUNDS)) {
                to_log(LOG_INFO, "round {r}: device draft {gpu[r].draft} '{decode_(tr, [gpu[r].draft])}' hit {gpu[r].hit} at {gpu[r].n_past}, CPU draft {cpu[r].draft} '{decode_(tr, [cpu[r].draft])}' hit {cpu[r].hit} at {cpu[r].n_past}\n")
                t |> equal(gpu[r].draft, cpu[r].draft, "round {r}: the device draft is the CPU round's")
                t |> equal(gpu[r].hit, cpu[r].hit, "round {r}: both rounds accepted or both rejected")
                t |> equal(gpu[r].n_past, cpu[r].n_past, "round {r}: both rounds left the session at one position")
            }
            t |> equal(c.n_past - 1l, nrows, "the CPU rounds left the session where the device rounds did")
            var crows <- head_k_rows_f32(tr, c, nrows)
            head_rows_within(t, "six rounds, hydrated", grows, crows, nrows)
            var c7 <- spec_rounds(tr, c, [toks[ROUNDS]])
            to_log(LOG_INFO, "round 6 (declined draft): CPU head over hydrated rows drafts {g7[0].draft} '{decode_(tr, [g7[0].draft])}' hit {g7[0].hit} at {g7[0].n_past}, the CPU round {c7[0].draft} '{decode_(tr, [c7[0].draft])}' hit {c7[0].hit} at {c7[0].n_past}\n")
            t |> equal(g7[0].draft, c7[0].draft, "round 6: the CPU head over the hydrated rows drafts what the CPU round drafts")
            t |> equal(g7[0].hit, c7[0].hit, "round 6: both rounds accepted or both rejected")
            t |> equal(g7[0].n_past, c7[0].n_past, "round 6: both rounds left the session at one position")
            poison_head_rows(tr, c, c.n_past - 1l)   //! the control: the CPU head over NaN rows cannot draft what the hydrated rows did
            var c8 <- spec_rounds(tr, c, [toks[ROUNDS + 1l]])
            var pl <- [for (x in c.mtp_logits); x]
            to_log(LOG_INFO, "control round: the draft over hydrated rows {g8[0].draft}, the CPU draft over poisoned head rows {c8[0].draft} (its verify row 0 finite {all_finite(pl)})\n")
            t |> success(g8[0].draft != c8[0].draft || !all_finite(pl), "control: the CPU round over poisoned head rows drafts another token or lands non-finite rows")
            delete pl
            delete c8
            delete c7
            delete g8
            delete g7
            delete crows
            delete grows
            delete cpu
            delete gpu
            delete c
            delete fed
            delete toks
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
