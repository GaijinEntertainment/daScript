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
require daslib/strings_boost
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the hybrid
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, the resolved coopmat mode and the kernel census, where the module exists
require ?vulkan dasllama/dasllama_vulkan_prefill   // pf_f16_feed: the feed admission the driver acts on
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed
require _kv_codec_cells   // the K/V codec cells' rig: the gated carrier under a block-codec mirror

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
// the deltanet bar over one window and over two, and six speculative rounds against the CPU round from
// one prompt. The gates around the round: speculation off lands no carry, a NaN carry panics the draft
// by row, the pass arms (codec, unhomed, gap) hand the round to the CPU rails by name and the CPU verify
// completes over the recurrent state the CPU rails hold, a rewind passes the verify by name and the CPU
// rails then refuse it by name, and a head kept off the device passes the verify as verify_rows with
// the resident prefill declining the CPU verify's window as verify_window.
// Every model cell skips interpreted, without its model file, or without the armed tier; the
// K-quant cells and the codec pass arm skip on the f32 mirrors (`DASLLAMA_VK_KV32=1`) too.

let private DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit| - the deltanet bar test_deltanet.das holds too
//! the K-quant bar on the f16 feed and mirror: CPU-vs-device rounding sits flat at 0.39-0.57 of a ~12.5 max logit at every step (the Q8 file 0.19-0.36), with no growth and the one-step-off control well past it
let private DN_LOGIT_BAR_REL_KQ_F16_FEED = 0.06
//! the K-quant bar on the quant feed (no coopmat tile serves the planes, the prefill's activations ride Q8_K): the mixed twin's two-window cell under `DASLLAMA_COOPMAT=sdot4` on the RTX 5060 Ti reads 0.084 of an 11.6 max logit at one step, argmax unchanged, the rest under 0.05
let private DN_LOGIT_BAR_REL_KQ_QUANT_FEED = 0.10
//! the mixed twin's bar on the f16 feed: its CPU chain quantizes the activations twice a recurrent layer (Q8_K for qkv/z, Q8_0
//! for out), so the device and the CPU chain round on either side of the file's f32 chain - one step reads 0.91 of a 12.3 max
//! logit apart (the device 0.63 and the CPU chain 0.50 from the f32 row, the device's pick the f32 chain's), the rest 0.42-0.60
let private DN_LOGIT_BAR_REL_KQ_Q8OUT = 0.08
//! the device's summed distance from the file's f32 chain, as a ratio of the CPU q8 chain's own: reads a 0.91 ratio (3.42 against 3.77)
let private F32_DIST_REL = 1.15
//! the head's K row is the f16-stored projection of a trunk hidden row that already sits within DN_LOGIT_BAR_REL of the CPU chain's, so the row's own rounding stacks on the trunk's: twice the deltanet bar
let private HEAD_K_BAR_REL = 0.08
//! the CPU row one down misses a device row by at least this many bars, or the bar is too wide to tell the row from its neighbour
let private HEAD_K_CONTROL_BARS = 2.0

//! the plane formats a fixture's deltanet layers carry: all Q8_0, all K-quant, or K-quant qkv/z beside a Q8_0 out plane
enum HybridPlanes {
    q8
    kq
    kq_q8out
}

//! the driver's own answer: does the prefill's f16 feed admit a K-quant plane on the armed coopmat mode
def private f16_feed_admits_kq() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return pf_f16_feed(int(KqFmt.k4))
    } else {
        return false
    }
}

//! The mixed twin's same-or-better witness: the same tokens through the file's f32 chain on the CPU (`path` loaded again at
//! `QuantMode.fp32`, its device state dropped), and the device no further from it than `F32_DIST_REL` of the CPU q8 chain's
//! own distance, the f32 rows one step off past that bound. `tr` decodes the logged picks.
def private f32_chain_witness(t : T?; tr : Model; path : string; n : int64; prompt, toks : array<int64>; fed : FeedPair) {
    var f32_model <- load_model_(path, QuantMode.fp32)
    moe_gpu_drop_model()
    var f32_rows <- feed_alone(f32_model, mirror_kv_dtype(), prompt, toks)
    let what = "{n} tokens against the f32 chain [{tile_arm_tag()}]"
    let dist = chain_distances(t, tr, what, fed.gpu, fed.cpu, f32_rows, 0, "the f32 chain", "the CPU q8 chain")
    t |> success(dist.dev_ref <= F32_DIST_REL * dist.cpu_ref, "{what}: the device sits no further from the f32 chain ({dist.dev_ref}) than {F32_DIST_REL} of the CPU q8 chain's own distance ({dist.cpu_ref})")
    t |> success(dist.dev_ref_ctrl > F32_DIST_REL * dist.cpu_ref, "{what} control: the f32 rows one step off sit past that bound ({dist.dev_ref_ctrl})")
    delete f32_rows
    delete f32_model
}

// prefill `n` tokens + 8 fed steps on the resident driver, then the same on the all-CPU chain; the
// prefill logits and every step's within the deltanet bar, the one-step-off control past it.
def private hybrid_forced_feed(t : T?; n : int64; file : string; planes : HybridPlanes) {
    let kq = planes != HybridPlanes.q8
    let q8out = planes == HybridPlanes.kq_q8out
    let path = path_join(models_dir(), file)
    if (!resident_ready(t, path)) return
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
    with_dasllama_jobque_() {
        var prompt <- resident_prompt(n)
        var toks <- resident_feed(8l)
        let rq0 = census("cls_q8k_rq_spv")
        var prefill_q8k_rq = NO_CENSUS
        var fed <- resident_then_cpu_feed(t, tr, prompt, toks, $ {
            prefill_q8k_rq = rq0 == NO_CENSUS ? NO_CENSUS : census("cls_q8k_rq_spv") - rq0
        }) $ {}
        if (kq && prefill_q8k_rq != NO_CENSUS) {
            //! off the f16 feed the x route requants once per recurrent layer per window - the threshold both arms read
            to_log(LOG_INFO, "prefill ({n} tokens) [{tile_arm_tag()}]: {prefill_q8k_rq} Q8_K requants over {nrec} recurrent layers, f16 feed admits kq {f16_feed_admits_kq()}\n")
            if (f16_feed_admits_kq()) {
                t |> success(prefill_q8k_rq < nrec, "on the f16 feed the prefill never requantized the block input to Q8_K for the qkv/z pair ({prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers) [{tile_arm_tag()}]")
            } else {
                t |> success(prefill_q8k_rq >= nrec, "off the f16 feed the K-quant qkv/z pair reads the Q8_K form ({prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers) [{tile_arm_tag()}]")
            }
        }
        let f16_rel = q8out ? DN_LOGIT_BAR_REL_KQ_Q8OUT : DN_LOGIT_BAR_REL_KQ_F16_FEED
        let rel = kq ? (f16_feed_admits_kq() ? f16_rel : DN_LOGIT_BAR_REL_KQ_QUANT_FEED) : DN_LOGIT_BAR_REL
        feed_steps_within(t, tr, "", n, fed.gpu, fed.cpu, toks, rel)
        if (q8out) {
            f32_chain_witness(t, tr, path, n, prompt, toks, fed)
        }
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
        assert_pf_window(t)
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q4_K_M.gguf", HybridPlanes.kq)
    }
}

[test]
def test_gpu_resident_hybrid_kq_q8out(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: K-quant qkv/z with a Q8_0 out plane on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", HybridPlanes.kq_q8out)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: the same across two windows") @(t : T?) {
        assert_pf_window(t)
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", HybridPlanes.kq_q8out)
    }
}

//! the gated carrier under a block codec: the q8_0 mirror against the CPU chain on q8_0 sessions reads 0.21-0.31 of a
//! 10.7-12.3 max logit (the f16 mirror's own reading); under tq4 the device's summed distance from the f16 chain reads 1.08
//! of the CPU tq4 chain's own (9.2 against 8.5, the f16 rows one step off 9.2 times it) and the two tq4 chains 0.63 of it
//! apart (the CPU tq4 rows one step off 9.3 times it)
let private GATED_Q8KV_BAR_REL = 0.06
let private GATED_CODEC_DIST_REL = 1.15
let private GATED_CODEC_PAIR_REL = 0.8

[test]
def test_gpu_resident_hybrid_gated_block_codecs(t : T?) {
    let file = "Qwen3.5-0.8B-Q8_0.gguf"
    t |> run("Qwen3.5-0.8B: a q8_0 K/V mirror under the gated attention - resident prefill + decode vs the CPU chain on q8_0 sessions") @(t : T?) {
        cell_codec_forced_feed(t, file, 40l, KVDtype.q8_0, GATED_Q8KV_BAR_REL)
    }
    t |> run("Qwen3.5-0.8B: a tq4 K/V mirror under the gated attention - the un-rotation applies the head gate - against the CPU chain's tq4 and f16 rows") @(t : T?) {
        cell_codec_distance(t, file, 40l, KVDtype.tq4, false, GATED_CODEC_DIST_REL, GATED_CODEC_PAIR_REL)
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

//! `prompt` then `toks` through a fresh session a row at a time under the speculative gate `spec`, the carry kept after the
//! prefill and after each step; the gate goes back where it stood
[cold_path]
def private carries_alone(tr : Model; prompt, toks : array<int64>; spec : bool = true) : array<Carry> {
    let spec0 = get_mtp_spec()
    set_mtp_spec(spec)
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
    set_mtp_spec(spec0)
    return <- out
}

def private hidden_rows(cs : array<Carry>) : array<array<float>> {
    var out : array<array<float>>
    out |> reserve(long_length(cs))
    for (c in cs) {
        out |> emplace <| clone_to_move(c.h)
    }
    return <- out
}

//! every resident carry's watermark at its row's position plus one (the CPU chain's too), one hidden row a side, and the hidden
//! within the deltanet bar of the CPU chain's with the CPU chain's previous carry as the one-step-off control
def private carries_within(t : T?; what : string; nprompt : int64; gpu, cpu : array<Carry>) {
    for (st in range64(long_length(cpu))) {
        let tag = what + (st == 0l ? " prefill ({nprompt} tokens)" : " step {st - 1l}")
        t |> equal(gpu[st].pos1, nprompt + st, "{tag}: the resident carry's watermark is its row's position plus one")
        t |> equal(cpu[st].pos1, nprompt + st, "{tag}: the CPU chain's watermark")
        t |> equal(long_length(gpu[st].h), long_length(cpu[st].h), "{tag}: one hidden row a side")
    }
    var g <- hidden_rows(gpu)
    var c <- hidden_rows(cpu)
    rows_within_control(t, "{what} carry", g, c, DN_LOGIT_BAR_REL, -1l, 1.0, $(st) => (st == 0l ? "prefill ({nprompt} tokens)" : "step {st - 1l}"))
    delete g
    delete c
}

def private mtp_carry_ready(t : T?) : bool => resident_ready(t, path_join(models_dir(), MTP_CARRY_FILE))

//! the -MTP- file at `regions` mirror regions on the resident route, the driver armed on it, the host sessions' context capped;
//! under `head_off` the draft seat is unset for the load, so the plan keeps the NextN head off the device (the seat put back after);
//! every lever goes back where it stood
def private load_mtp_carrier(t : T?; regions : int64; head_off : bool = false) : Model {
    var seat : RdecDraftFn
    if (head_off) {
        seat = unset_rdec_draft()
    }
    var tr <- load_resident(path_join(models_dir(), MTP_CARRY_FILE), regions)
    if (head_off) {
        reinstall_rdec_draft(seat)
    }
    tr.config.seq_len = min(tr.config.seq_len, CARRY_RIG_CTX)
    t |> equal(tr.config.n_layer_nextn, 1l, "the -MTP- file carries its NextN head")
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the NextN-headed hybrid at {regions} regions (the load log names any decline)")
    t |> equal(mirror_kv_dtype(), resident_mirror_dtype(), "the mirror armed at the codec asked")
    return <- tr
}

//! `body` on the -MTP- carrier `load_mtp_carrier` loads at `regions`, inside the engine's job queue, the model deleted after; nothing
//! past the gate's skip
def private with_mtp_carrier(t : T?; regions : int64; head_off : bool; body : block<(var tr : Model) : void>) {
    if (!mtp_carry_ready(t)) return
    var tr <- load_mtp_carrier(t, regions, head_off)
    with_dasllama_jobque_() {
        invoke(body, tr)
    }
    delete tr
}

def private with_mtp_carrier(t : T?; regions : int64; body : block<(var tr : Model) : void>) {
    with_mtp_carrier(t, regions, false, body)
}

//! `body` on the one-region carrier and a fresh session holding `resident_prompt(DRAFT_PROMPT)` through `prefill_warm`, the
//! session and the prompt deleted after
def private with_warm_carrier(t : T?; body : block<(var tr : Model; prompt : array<int64>; var s : Session) : void>) {
    with_mtp_carrier(t, 1l) $(var tr) {
        var prompt <- resident_prompt(DRAFT_PROMPT)
        var s <- create_session(tr, mirror_kv_dtype())
        prefill_warm(tr, s, prompt)
        invoke(body, tr, prompt, s)
        delete s
        delete prompt
    }
}

//! the prefill's logits, then each fed step's, through a fresh session under the speculative gate `spec` (put back after)
[cold_path]
def private feed_under_spec(tr : Model; prompt, toks : array<int64>; spec : bool) : array<array<float>> {
    let spec0 = get_mtp_spec()
    set_mtp_spec(spec)
    var out <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
    set_mtp_spec(spec0)
    return <- out
}

[test]
def test_gpu_resident_hybrid_mtp_carry(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the resident prefill and each step land the post-norm hidden as the speculative carry, against the CPU chain's") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            var prompt <- resident_prompt(40l)
            var toks <- resident_feed(CARRY_STEPS)
            let served0 = moe_gpu_resident_prefills()
            var gpu <- carries_alone(tr, prompt, toks)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the prompt")
            served_every_call(t)
            moe_gpu_drop_model()
            var cpu <- carries_alone(tr, prompt, toks)
            carries_within(t, "one row:", long_length(prompt), gpu, cpu)
            delete gpu
            delete cpu
            delete prompt
            delete toks
        }
    }
}

[test]
def test_gpu_resident_hybrid_mtp_carry_batched(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: two sessions batched through the N-row command land each row's carry, against each session alone on the CPU chain") @(t : T?) {
        with_mtp_carrier(t, 2l) $(var tr) {
            var pa <- resident_prompt(40l)
            var pb <- [for (i in range64(57l)); 3000l + (i * 91l) % 6000l]
            var fa <- resident_feed(CARRY_STEPS)
            var fb <- [for (i in range64(CARRY_STEPS)); 2500l + (i * 71l) % 4500l]
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            var sa <- create_session(tr, mirror_kv_dtype())
            var sb <- create_session(tr, mirror_kv_dtype())
            eval(tr, sa, pa)   // `eval` advances `n_past`, the position a batched step reads
            eval(tr, sb, pb)
            var ga : array<Carry>
            var gb : array<Carry>
            ga |> emplace <| carry_of(sa)
            gb |> emplace <| carry_of(sb)
            let steps0 = rdec_nrow_steps()
            with_batch_rows(tr, sa, sb) $(var ws; var sess) {
                for (st in range64(CARRY_STEPS)) {
                    var toks <- [fa[st], fb[st]]
                    eval_batch(tr, ws, sess, toks)
                    delete toks
                    ga |> emplace <| carry_of(sa)
                    gb |> emplace <| carry_of(sb)
                }
            }
            t |> equal(rdec_nrow_steps() - steps0, CARRY_STEPS, "every batched step went through the N-row command")
            served_every_call(t)
            set_mtp_spec(spec0)
            delete sa
            delete sb
            moe_gpu_drop_model()
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
    }
}

[test]
def test_gpu_resident_hybrid_mtp_carry_gate(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: with speculation off the NextN-headed hybrid lands no carry - the watermark stays 0 after the resident prefill and every step - and its logits sit within the deltanet bar of the spec-on run's at every row, the one-step-off control past it") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            var prompt <- resident_prompt(40l)
            var toks <- resident_feed(CARRY_STEPS)
            let served0 = moe_gpu_resident_prefills()
            var off <- carries_alone(tr, prompt, toks, false)
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the prompt with speculation off")
            for (st in range64(long_length(off))) {
                let what = st == 0l ? "prefill" : "step {st - 1l}"
                t |> equal(off[st].pos1, 0l, "{what}: speculation off, the resident driver lands no carry - the watermark stays 0")
            }
            var on <- carries_alone(tr, prompt, toks, true)
            t |> equal(on[CARRY_STEPS].pos1, long_length(prompt) + CARRY_STEPS, "speculation on, the same driver lands the carry: the watermark at the last step's row plus one")
            var off_logits <- feed_under_spec(tr, prompt, toks, false)
            var on_logits <- feed_under_spec(tr, prompt, toks, true)
            served_every_call(t)
            feed_steps_within(t, tr, "carry gate: ", long_length(prompt), off_logits, on_logits, toks, DN_LOGIT_BAR_REL, "spec-off", "the spec-on run")
            delete off_logits
            delete on_logits
            delete off
            delete on
            delete prompt
            delete toks
        }
    }
}

let private DRAFT_PROMPT = 40l
let private DRAFT_TOKEN = 2000l
let private ROUNDS = 6l   //! the speculative rounds the device-vs-CPU cells run, one fed token each

//! `prompt` through the prefill at `start_pos` with the speculative warm on, so the head's host rows the draft hydrates hold history;
//! a continuation (`start_pos` past 0) on the hybrid passes to the CPU rails, which leave the recurrent state and the carry at its end
def private prefill_warm(tr : Model; var s : Session; prompt : array<int64>; start_pos : int64 = 0l) {
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    forward_prefill(tr, s, prompt, long_length(prompt), start_pos)
    set_mtp_spec(spec0)
    s.n_past = start_pos + long_length(prompt)
}

//! one device row `gpu` within the deltanet bar of the CPU row `cpu`, the CPU row `off` - one step or one row away - as the control past it
def private dn_row_within(t : T?; what : string; gpu, cpu, off : array<float>) {
    row_within_ctrl(t, what, gpu, cpu, off, DN_LOGIT_BAR_REL, "(the CPU row one away as the control)")
}

[test]
def test_gpu_resident_hybrid_mtp_draft(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: one NextN draft through the resident head against the CPU forward_mtp on the same token, carry and row") @(t : T?) {
        with_warm_carrier(t) $(var tr; prompt; var s) {
            t |> equal(s.mtp_h_pos1, DRAFT_PROMPT, "the resident prefill landed the carry at the prompt's end")
            var h0 <- [for (x in s.mtp_h); x]
            let row = DRAFT_PROMPT - 1l   // the head's row pos - 1: the round's draft at position pos
            let drafts0 = moe_gpu_resident_drafts()
            var gpu <- draft_from(tr, s, DRAFT_TOKEN, row, h0)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the resident head drafted")
            t |> equal(s.mtp_h_pos1, 0l, "the draft's hidden is the head's own: its watermark stays 0")
            let pick = moe_gpu_resident_draft_pick()
            let seat = unset_rdec_draft()
            var cpu <- draft_from(tr, s, DRAFT_TOKEN, row, h0)
            var one_row_back <- draft_from(tr, s, prompt[row], row - 1l, h0)
            reinstall_rdec_draft(seat)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the unset seat left both CPU drafts to forward_mtp")
            served_every_call(t)
            let ag = argmax_of(gpu.logits)
            let ac = argmax_of(cpu.logits)
            to_log(LOG_INFO, "draft (row {row}, token {DRAFT_TOKEN}): argmax cpu {ac} '{decode_(tr, [ac])}' ({cpu.logits[ac]}), resident {ag} '{decode_(tr, [ag])}' ({gpu.logits[ag]}), device pick {pick}\n")
            dn_row_within(t, "draft logits", gpu.logits, cpu.logits, one_row_back.logits)
            dn_row_within(t, "draft hidden", gpu.h, cpu.h, one_row_back.h)
            t |> equal(pick, ag, "the device pick is the argmax of the logits it landed")
            pick_agrees(t, tr, "draft (row {row}, token {DRAFT_TOKEN})", pick, cpu.logits, DN_LOGIT_BAR_REL)
            delete gpu
            delete cpu
            delete one_row_back
            delete h0
        }
    }
    t |> run("Qwen3.5-0.8B-MTP: the speculative round drafts on the CPU with the draft seat unset, and on the device with it back") @(t : T?) {
        with_warm_carrier(t) $(var tr; prompt; var s) {
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
        }
    }
}

[test]
def test_gpu_resident_hybrid_mtp_draft_upload(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: a device draft past the rows the device holds brings the host's rows up with it and counts the device's rows through its own; the pass after it brings the drafted row down, and the CPU draft over the same rows picks the same token") @(t : T?) {
        with_warm_carrier(t) $(var tr; prompt; var s) {
            let pos = DRAFT_PROMPT
            var h0 <- [for (x in s.mtp_h); x]
            declined_draft(tr, s, DRAFT_TOKEN, pos - 1l)
            declined_draft(tr, s, DRAFT_TOKEN, pos)
            t |> equal(moe_gpu_resident_head_rows(s), pos - 1l, "the device's rows stop below the CPU's two")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos + 1l, "the host holds every row through the CPU's second")
            var stale_row <- head_k_row_bytes(tr, s, pos + 1l)
            let drafts0 = moe_gpu_resident_drafts()
            var gpu <- draft_from(tr, s, DRAFT_TOKEN, pos + 1l, h0)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the device drafted row {pos + 1l}")
            let pick = moe_gpu_resident_draft_pick()
            t |> equal(moe_gpu_resident_head_rows(s), pos + 2l, "the device holds every row through the draft's: the two it uploaded and its own")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos + 1l, "the host's copy stops below the draft's row")
            declined_draft(tr, s, DRAFT_TOKEN, pos + 2l)
            t |> equal(moe_gpu_resident_head_host_rows(s), pos + 3l, "the pass brought the device's row down and the CPU wrote the next: the host holds every row through it")
            var landed_row <- head_k_row_bytes(tr, s, pos + 1l)
            t |> success(mismatch_exact(landed_row, stale_row) > 0, "the device's K row {pos + 1l} came down on the pass: the host row moved")
            let seat = unset_rdec_draft()
            var cpu <- draft_from(tr, s, DRAFT_TOKEN, pos + 1l, h0)
            reinstall_rdec_draft(seat)
            served_every_call(t)
            pick_agrees(t, tr, "draft over uploaded rows (row {pos + 1l}, token {DRAFT_TOKEN})", pick, cpu.logits, DN_LOGIT_BAR_REL)
            delete cpu
            delete gpu
            delete landed_row
            delete stale_row
            delete h0
        }
    }
}

//! a verify row against the one-row step of the same token, bit for bit: its logits and its post-norm hidden, the argmax both
//! sides read logged, and the step one row off (the other row's) as the control the compare can fail
def private verify_row_is_step(t : T?; tr : Model; row : int64; vlog, vhid : array<float>; step_log, step_h, off_log : array<float>) {
    let la = mismatch_exact(vlog, step_log)
    let ha = mismatch_exact(vhid, step_h)
    let ag = argmax_of(vlog)
    let ac = argmax_of(step_log)
    to_log(LOG_INFO, "verify row {row}: logits {la} apart (maxdiff {logits_maxdiff(vlog, step_log)}), hidden {ha} apart (maxdiff {logits_maxdiff(vhid, step_h)}); argmax verify {ag} '{decode_(tr, [ag])}', step {ac} '{decode_(tr, [ac])}'\n")
    t |> equal(la, 0, "verify row {row}'s logits are the one-row step's bit for bit ({la} elements apart)")
    t |> equal(ha, 0, "verify row {row}'s post-norm hidden is the one-row step's bit for bit ({ha} elements apart)")
    t |> success(mismatch_exact(vlog, off_log) > 0, "control: verify row {row} misses the other row's step logits")
}

let private VERIFY_ROWS = 2l   //! the CPU round's depth-1 verify: the committed token and its draft

//! a run of one-row steps: each step's logits and carry
struct private StepRows {
    logits : array<array<float>>
    h : array<array<float>>
}

//! `prompt` then `toks` through a fresh session a row at a time on the resident driver
[cold_path]
def private steps_alone(tr : Model; prompt, toks : array<int64>) : StepRows {
    var s <- create_session(tr, mirror_kv_dtype())
    prefill_warm(tr, s, prompt)
    var out : StepRows
    let n = long_length(prompt)
    out.logits |> reserve(long_length(toks))
    out.h |> reserve(long_length(toks))
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)   //! the carry lands only while the round is on: the steps are the round's rows, so they run under its knob
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out.logits |> emplace <| [for (x in s.logits); x]
        out.h |> emplace <| [for (x in s.mtp_h); x]
    }
    set_mtp_spec(spec0)
    delete s
    return <- out
}

//! a draft of `tok` at head row `row` from the session's carry through whichever seat serves, its logits answered; the carry
//! and its watermark are put back, so the round that follows drafts from the same state
[cold_path]
def private draft_ahead(tr : Model; var s : Session; tok, row : int64) : array<float> {
    var h0 <- [for (x in s.mtp_h); x]
    let pos1 = s.mtp_h_pos1
    forward_mtp(tr, s, tok, row)
    var out <- [for (x in s.logits); x]
    copy_floats(h0, 0l, s.mtp_h, 0l, long_length(h0))
    s.mtp_h_pos1 = pos1
    delete h0
    return <- out
}

//! a draft the seat declines (unset for the call): the CPU head at row `row` reads the head's host rows below it, so the
//! device's rows come down first - the pass a cell reads the device's head rows through; the carry is put back after
[cold_path]
def private declined_draft(tr : Model; var s : Session; tok, row : int64) {
    let seat = unset_rdec_draft()
    var logits <- draft_ahead(tr, s, tok, row)
    reinstall_rdec_draft(seat)
    delete logits
}

//! one speculative round's record: the draft the round verified, whether it accepted it, the position it left, the token the
//! caller's sample_ took after it (-1 where none was taken), and the CPU draft's logits where the reference kept them
struct private RoundRec {
    draft : int64
    hit : bool
    n_past : int64
    next : int64
    draft_logits : array<float>
}

//! one speculative round of `s` a token of `toks`: `ask` sets the caller's pick ask before every round; under `chain` the
//! token after the first is the one sample_ took after the previous round (a landed pick, or the logits row's argmax);
//! `draft_ref` keeps each round's draft logits - the head drafted once ahead of the round from the same carry - for the
//! tie-aware compare; the speculative gate goes back where it stood
[cold_path]
def private spec_rounds(tr : Model; var s : Session; toks : array<int64>; ask : bool = false; chain : bool = false; draft_ref : bool = false) : array<RoundRec> {
    var out : array<RoundRec>
    out |> reserve(long_length(toks))
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    var tok = toks[0]
    for (fed in toks) {
        if (!chain) {
            tok = fed
        }
        var rec : RoundRec
        if (draft_ref) {
            rec.draft_logits <- draft_ahead(tr, s, tok, s.n_past - 1l)
        }
        s.pick_asked = ask
        var acc = 0l
        rec.hit = mtp_spec_eval(tr, s, tok, acc)
        s.pick_asked = false
        rec.draft = s.mtp_vbatch[1]
        rec.n_past = s.n_past
        rec.next = chain ? sample_(s, SamplingParams()) : -1l
        tok = rec.next
        out |> emplace(rec)
    }
    set_mtp_spec(spec0)
    return <- out
}

//! `toks` through rounds on `s` with the draft seat unset: the CPU head drafts over the host's rows, which the declined seat
//! hydrates from the device first, and the device verifies
[cold_path]
def private declined_draft_rounds(tr : Model; var s : Session; toks : array<int64>) : array<RoundRec> {
    let seat = unset_rdec_draft()
    var out <- spec_rounds(tr, s, toks)
    reinstall_rdec_draft(seat)
    return <- out
}

[test]
def test_gpu_resident_hybrid_mtp_verify(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the round's two verify rows as same-slab rows through the resident seat, bit for bit the two one-row steps of the same tokens; the head's rows stay on the device and come down on a pass, the re-warm's row bit for bit the draft's") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            let c = tr.config
            var prompt <- resident_prompt(DRAFT_PROMPT)
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let pos = DRAFT_PROMPT
            t |> equal(moe_gpu_resident_head_host_rows(s), pos - 1l, "the prompt warm's rows came down with the chain: the host holds the head's rows below the prompt's last")
            var head_row0 <- head_k_row_bytes(tr, s, pos - 1l)
            var h0 <- [for (x in s.mtp_h); x]
            let drafts0 = moe_gpu_resident_drafts()
            forward_mtp(tr, s, DRAFT_TOKEN, pos - 1l)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the device drafted row {pos - 1l}")
            t |> equal(moe_gpu_resident_head_rows(s), pos, "the device holds the head's rows through the draft's")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos - 1l, "the host's copy stops below the draft's row: nothing came down")
            copy_floats(h0, 0l, s.mtp_h, 0l, c.dim)
            s.mtp_h_pos1 = pos
            declined_draft(tr, s, DRAFT_TOKEN, pos)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the unset seat left the draft to the CPU head")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos + 1l, "the pass brought the device's rows down and the CPU wrote row {pos}: the host holds every row through it")
            t |> equal(moe_gpu_resident_head_rows(s), pos, "the device's copy stops below the CPU's row")
            var draft_row <- head_k_row_bytes(tr, s, pos - 1l)
            t |> success(mismatch_exact(head_row0, draft_row) > 0, "the draft's K row {pos - 1l} came down on the pass: the host row moved")
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
            var vlog0 <- copy_span(s.mtp_logits_b, 0l, c.vocab_size)
            var vlog1 <- copy_span(s.mtp_logits_b, c.vocab_size, c.vocab_size)
            var vhid0 <- copy_span(s.mtp_hrows, 0l, c.dim)
            var vhid1 <- copy_span(s.mtp_hrows, c.dim, c.dim)
            t |> equal(moe_gpu_resident_verify_pick(0l), argmax_of(vlog0), "row 0's device pick is the argmax of the logits it landed")
            t |> equal(moe_gpu_resident_verify_pick(1l), argmax_of(vlog1), "row 1's device pick is the argmax of the logits it landed")
            t |> equal(s.n_past, pos + (hit ? VERIFY_ROWS : 1l), "the round committed the draft on an accept, the token alone on a reject")
            t |> equal(moe_gpu_resident_rows(s), s.n_past, "the region's rows end at the session's position ({hit ? "an accept keeps both verify rows" : "the reject's replay rewrote row pos"})")
            t |> equal(moe_gpu_resident_head_host_rows(s), pos - 1l, "the round's head rows stayed on the device: the host's copy stops below the draft's row again")
            declined_draft(tr, s, DRAFT_TOKEN, s.n_past - 1l)
            t |> equal(moe_gpu_resident_head_host_rows(s), s.n_past, "the pass brought the round's rows down through the CPU draft's")
            var warm_row <- head_k_row_bytes(tr, s, pos - 1l)
            t |> equal(mismatch_exact(warm_row, draft_row), 0, "the re-warm's K row {pos - 1l} - the store-only pass over the k heads - is the draft command's row bit for bit: the same [enorm(token) ; hnorm(carry)] input ({mismatch_exact(warm_row, draft_row)} bytes apart)")
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
    }
}

[test]
def test_gpu_resident_hybrid_mtp_verify_asked(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: six rounds under the caller's pick ask - the picks alone landed, the committed row's pick published for sample_ - token for token the rounds with the logits landing") @(t : T?) {
        with_mtp_carrier(t, 2l) $(var tr) {
            var prompt <- resident_prompt(DRAFT_PROMPT)
            var sa <- create_session(tr, mirror_kv_dtype())
            var sb <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, sa, prompt)
            prefill_warm(tr, sb, prompt)
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            let verifies0 = moe_gpu_resident_verifies()
            let drafts0 = moe_gpu_resident_drafts()
            var chain <- [for (_r in range64(ROUNDS)); DRAFT_TOKEN]   // the first token alone is fed: the rounds chain through sample_
            var asked <- spec_rounds(tr, sa, chain, true, true)
            var landed <- spec_rounds(tr, sb, chain, false, true)
            set_mtp_spec(spec0)
            t |> equal(moe_gpu_resident_verifies(), verifies0 + 2l * ROUNDS, "every round of both sessions verified on the device")
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 2l * ROUNDS, "every round of both sessions drafted on the device")
            served_every_call(t)
            for (r in range64(ROUNDS)) {
                to_log(LOG_INFO, "asked round {r}: draft {asked[r].draft} '{decode_(tr, [asked[r].draft])}' hit {asked[r].hit} at {asked[r].n_past} next {asked[r].next} '{decode_(tr, [asked[r].next])}'; landed round: draft {landed[r].draft} '{decode_(tr, [landed[r].draft])}' hit {landed[r].hit} at {landed[r].n_past} next {landed[r].next} '{decode_(tr, [landed[r].next])}'\n")
                t |> equal(asked[r].draft, landed[r].draft, "round {r}: the pick-asked round drafts the token the logits-landing round drafts")
                t |> equal(asked[r].hit, landed[r].hit, "round {r}: both rounds accepted or both rejected")
                t |> equal(asked[r].n_past, landed[r].n_past, "round {r}: both rounds left the session at one position")
                t |> equal(asked[r].next, landed[r].next, "round {r}: the pick the asked round published is the argmax of the logits the other round landed")
            }
            t |> success(mismatch_exact(sa.logits, sb.logits) > 0, "the asked session's logits row is not the round's: the rounds landed picks alone (its row misses the landing session's)")
            delete chain
            delete asked
            delete landed
            delete sa
            delete sb
            delete prompt
        }
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

[cold_path]
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
        with_mtp_carrier(t, 1l) $(var tr) {
            let c = tr.config
            var prompt <- resident_prompt(DRAFT_PROMPT)
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let pos = DRAFT_PROMPT
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            let verifies0 = moe_gpu_resident_verifies()
            let rollbacks0 = moe_gpu_resident_rollbacks()
            let hit = forced_reject_round(tr, s, DRAFT_TOKEN)
            t |> success(!hit, "the forced reject rejected the draft")
            t |> equal(moe_gpu_resident_verifies(), verifies0 + 1l, "the rejected round's verify ran on the device")
            t |> equal(moe_gpu_resident_rollbacks(), rollbacks0 + 1l, "the reject rolled back on the device")
            t |> equal(long_length(s.mtp_snap.conv), 0l, "the round took no snapshot: its buffers were never sized")
            t |> equal(s.n_past, pos + 1l, "the reject committed the token alone")
            t |> equal(moe_gpu_resident_rows(s), pos + 1l, "the rollback cut the region's rows to the token's")
            t |> equal(moe_gpu_resident_head_rows(s), pos, "the region holds {pos} head rows after the reject: row {pos} held the rejected draft's input, and the next draft rewrites it")
            t |> equal(s.dn_pos, pos + 1l, "the recurrent state's position sits at the token's row")
            t |> equal(s.mtp_h_pos1, pos + 1l, "the carry sits at the token's row")
            let d = s.mtp_vbatch[1]
            var vlog0 <- copy_span(s.mtp_logits_b, 0l, c.vocab_size)
            var vhid0 <- copy_span(s.mtp_hrows, 0l, c.dim)
            var kept_log <- [for (x in s.logits); x]
            var kept_h <- [for (x in s.mtp_h); x]
            t |> equal(mismatch_exact(kept_log, vlog0), 0, "the reject kept verify row 0's logits as the session's")
            t |> equal(mismatch_exact(kept_h, vhid0), 0, "the reject re-seeded the carry from verify row 0's hidden")
            let next = argmax_of(kept_log)
            var rolled <- dn_host(s)
            forward(tr, s, next, pos + 1l)
            s.n_past = pos + 2l   // a plain step moves no position; the round reads its position from here, and a stale one steps the recurrent state back
            var next_log <- [for (x in s.logits); x]
            var acc = 0l
            mtp_spec_eval(tr, s, argmax_of(s.logits), acc)
            set_mtp_spec(spec0)
            t |> equal(moe_gpu_resident_verifies(), verifies0 + 2l, "the round after the reject verified on the device again")
            t |> equal(moe_gpu_resident_rows(s), s.n_past, "the region's rows end at the session's position after the next round")
            served_every_call(t)
            delete s
            var one <- state_after_steps(tr, prompt, [DRAFT_TOKEN], next)
            var two <- state_after_steps(tr, prompt, [DRAFT_TOKEN, d], next)
            let state_off = mismatch_exact(rolled.state, one.dn.state)
            let conv_off = mismatch_exact(rolled.conv, one.dn.conv)
            let logits_off = mismatch_exact(next_log, one.next_logits)
            to_log(LOG_INFO, "rollback: state {state_off} apart, conv history {conv_off} apart, next step's logits {logits_off} apart (maxdiff {logits_maxdiff(next_log, one.next_logits)}); argmax rolled {argmax_of(next_log)} '{decode_(tr, [argmax_of(next_log)])}', one-row session {argmax_of(one.next_logits)} '{decode_(tr, [argmax_of(one.next_logits)])}'; the two-row control {mismatch_exact(two.dn.state, one.dn.state)} state elements apart\n")
            t |> equal(state_off, 0, "the rolled-back deltanet state is the one-row session's bit for bit ({state_off} elements apart)")
            t |> equal(conv_off, 0, "the rolled-back conv history is the one-row session's bit for bit ({conv_off} elements apart)")
            t |> equal(logits_off, 0, "the next plain step's logits are the one-row session's next step bit for bit ({logits_off} elements apart)")
            t |> success(mismatch_exact(two.dn.state, one.dn.state) > 0, "control: the state after the verify's two rows misses the one-row session's")
            t |> success(mismatch_exact(next_log, two.next_logits) > 0, "control: the next step misses the two-row session's next step")
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
    }
}

[test]
def test_gpu_resident_hybrid_mtp_verify_reject_asked(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: a rejected device verify under the caller's pick ask publishes verify row 0's pick - the one-row step's argmax - in place of its logits, the carry re-seeded from its hidden") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            let c = tr.config
            var prompt <- resident_prompt(DRAFT_PROMPT)
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let pos = DRAFT_PROMPT
            var before <- [for (x in s.logits); x]
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            let rollbacks0 = moe_gpu_resident_rollbacks()
            s.pick_asked = true
            let hit = forced_reject_round(tr, s, DRAFT_TOKEN)
            s.pick_asked = false
            set_mtp_spec(spec0)
            t |> success(!hit, "the forced reject rejected the draft")
            t |> equal(moe_gpu_resident_rollbacks(), rollbacks0 + 1l, "the reject rolled back on the device")
            served_every_call(t)
            t |> success(s.pick_ready, "the reject published a pick for the caller's sample_")
            t |> equal(s.pick_tok, moe_gpu_resident_verify_pick(0l), "the published pick is verify row 0's device pick")
            t |> equal(mismatch_exact(s.logits, before), 0, "the pick-asked round landed no logits: the session's row is the prefill's still")
            var vhid0 <- copy_span(s.mtp_hrows, 0l, c.dim)
            var kept_h <- [for (x in s.mtp_h); x]
            t |> equal(mismatch_exact(kept_h, vhid0), 0, "the reject re-seeded the carry from verify row 0's hidden, which lands under the pick ask")
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
    }
}

let private NAN_BITS = 0x7FC00000u

//! the NextN head's K row `row` of the session's cache as floats, whichever codec the session carries (a tq4 row stays in its rotated basis)
def private head_k_row_f32(tr : Model; var s : Session; row : int64) : array<float> {
    let c = tr.config
    let kvd = layer_kv_dim(c, c.n_layers)
    let kp = kv_layer_kbase(s, c.n_layers, c.seq_len)
    var out : array<float>
    out |> resize(kvd)
    kv_load_row(kp, s.kv_dtype_k, kvd, row, 0l, kvd, unsafe(addr(out[0])))
    return <- out
}

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

//! the head's K rows on `gpu` against the CPU warm's within `HEAD_K_BAR_REL` of each CPU row's max, the CPU row one down as the
//! control missing every device row by `HEAD_K_CONTROL_BARS` bars
def private head_rows_within(t : T?; what : string; gpu, cpu : array<array<float>>) {
    rows_within_control(t, what, gpu, cpu, HEAD_K_BAR_REL, 1l, HEAD_K_CONTROL_BARS, $(r) => "head K row {r}")
}

[test]
def test_gpu_resident_hybrid_mtp_xb_poison(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the host residual rows poisoned with NaN before a resident prefill leave the device draft bit for bit the clean prefill's - the head's prompt rows warm on the device, off no host row") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            var prompt <- resident_prompt(DRAFT_PROMPT)
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
            t |> equal(mismatch_exact(s.mtp_h, h0), 0, "the poisoned prefill lands the clean prefill's carry bit for bit")
            let drafts0 = moe_gpu_resident_drafts()
            var got <- draft_from(tr, s, DRAFT_TOKEN, row, h0)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the resident head drafted")
            served_every_call(t)
            let ag = argmax_of(got.logits)
            let aw = argmax_of(want.logits)
            to_log(LOG_INFO, "poisoned x_b draft: argmax {ag} '{decode_(tr, [ag])}' ({got.logits[ag]}), the clean prefill's {aw} '{decode_(tr, [aw])}' ({want.logits[aw]}); logits maxdiff {logits_maxdiff(got.logits, want.logits)}, hidden maxdiff {logits_maxdiff(got.h, want.h)}\n")
            t |> success(all_finite(got.logits), "every landed logit is finite")
            t |> equal(mismatch_exact(got.logits, want.logits), 0, "the poisoned prefill's draft logits are the clean prefill's bit for bit ({mismatch_exact(got.logits, want.logits)} elements apart)")
            t |> equal(mismatch_exact(got.h, want.h), 0, "the poisoned prefill's draft hidden is the clean prefill's bit for bit")
            t |> equal(moe_gpu_resident_draft_pick(), pick_clean, "the device pick is the clean prefill's")
            var one_row_back <- draft_from(tr, s, prompt[row], row - 1l, h0)
            t |> success(mismatch_exact(one_row_back.logits, want.logits) > 0, "control: the draft one row back misses the clean draft's logits")
            delete one_row_back
            delete got
            delete want
            delete h0
            delete s
            delete prompt
        }
    }
}

//! the head's K rows [0, nprompt - 1) the window chain warmed over an `nprompt`-token prompt, read back to the host, against the
//! CPU warm's rows on the same prompt (the model dropped): within the head-row bar, the CPU row one down as the control
def private head_warm_rows_cell(t : T?; nprompt : int64) {
    with_mtp_carrier(t, 1l) $(var tr) {
        var prompt <- resident_prompt(nprompt)
        let n = nprompt - 1l
        var s <- create_session(tr, mirror_kv_dtype())
        let served0 = moe_gpu_resident_prefills()
        prefill_warm(tr, s, prompt)
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {nprompt}-token prompt")
        t |> equal(moe_gpu_resident_head_rows(s), n, "the region holds {n} head rows after the prompt warm: one per prompt row but the last, whose next token is not known yet")
        served_every_call(t)
        var gpu <- head_k_rows_f32(tr, s, n)
        delete s
        moe_gpu_drop_model()
        var c <- create_session(tr, mirror_kv_dtype())
        prefill_warm(tr, c, prompt)
        var cpu <- head_k_rows_f32(tr, c, n)
        t |> equal(long_length(gpu), n, "{n} device head rows read back")
        head_rows_within(t, "head warm ({nprompt} tokens)", gpu, cpu)
        delete c
        delete gpu
        delete cpu
        delete prompt
    }
}

[test]
def test_gpu_resident_hybrid_mtp_head_warm(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the head's K rows the window chain warmed, read back to the host, against the CPU warm's rows on the same prompt within twice the deltanet bar, the row one down past it") @(t : T?) {
        head_warm_rows_cell(t, DRAFT_PROMPT)
    }
    t |> run("Qwen3.5-0.8B-MTP: the same over two windows - the second window's carry row is its row 0, the head's rope a row behind the trunk's - every head K row of a prompt one window plus 88 rows long against the CPU warm's") @(t : T?) {
        assert_pf_window(t)
        head_warm_rows_cell(t, PF_WINDOW_ROWS + 88l)
    }
}

//! the device's hydrated head rows against the CPU session's over the first `agreed_rows` rows - past a round whose draft a
//! near-tie flipped, the two sessions hold different tokens; `hydrated` is cut to those rows
def private agreed_rows_within(t : T?; tr : Model; var c : Session; var hydrated : array<array<float>>; agreed_rows : int64) {
    let nrows = long_length(hydrated)
    t |> success(agreed_rows >= DRAFT_PROMPT - 1l, "at least the prompt's head rows are compared ({agreed_rows} of {nrows})")
    var crows <- head_k_rows_f32(tr, c, agreed_rows)
    for (i in range64(agreed_rows, nrows)) {
        delete hydrated[i]
    }
    hydrated |> resize(agreed_rows)
    head_rows_within(t, "six rounds, hydrated ({agreed_rows} of {nrows} rows)", hydrated, crows)
    delete crows
}

[test]
def test_gpu_resident_hybrid_mtp_rounds(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: six speculative rounds on the resident driver against the CPU round from the same prompt - every round's draft the CPU round's or a near-tie of it, accept and position equal while the drafts agree; then two rounds whose draft the seat declines - the CPU head over the head's rows hydrated from the device - against the CPU round, the hydrated rows within twice the deltanet bar, the CPU round on poisoned head rows as the control") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            var prompt <- resident_prompt(DRAFT_PROMPT)
            var toks <- resident_feed(ROUNDS + 2l)
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
            var round7 <- declined_draft_rounds(tr, s, [toks[ROUNDS]])
            t |> equal(moe_gpu_resident_drafts(), drafts0 + ROUNDS, "the seventh round's draft ran on the CPU head: the unset seat left the counter")
            t |> equal(moe_gpu_resident_verifies(), verifies0 + ROUNDS + 1l, "... and its verify on the device")
            var hydrated_rows <- head_k_rows_f32(tr, s, nrows)
            var round8 <- declined_draft_rounds(tr, s, [toks[ROUNDS + 1l]])
            served_every_call(t)
            delete s
            moe_gpu_drop_model()
            var c <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, c, prompt)
            var cpu <- spec_rounds(tr, c, fed, false, false, true)
            var agreed = true   // once a near-tie flips a draft the two sessions part, and the rounds after it are no longer the same round
            var agreed_head_rows = nrows
            for (r in range64(ROUNDS)) {
                to_log(LOG_INFO, "round {r}: device draft {gpu[r].draft} '{decode_(tr, [gpu[r].draft])}' hit {gpu[r].hit} at {gpu[r].n_past}, CPU draft {cpu[r].draft} '{decode_(tr, [cpu[r].draft])}' hit {cpu[r].hit} at {cpu[r].n_past}\n")
                if (!agreed) {
                    to_log(LOG_INFO, "round {r}: an earlier near-tie parted the sessions - the accept and position are not compared\n")
                    continue
                }
                agreed = pick_agrees(t, tr, "round {r} draft", gpu[r].draft, cpu[r].draft_logits, DN_LOGIT_BAR_REL)
                if (agreed) {
                    t |> equal(gpu[r].hit, cpu[r].hit, "round {r}: both rounds accepted or both rejected")
                    t |> equal(gpu[r].n_past, cpu[r].n_past, "round {r}: both rounds left the session at one position")
                } else {
                    agreed_head_rows = (r == 0l ? DRAFT_PROMPT : cpu[r - 1l].n_past) - 1l
                }
            }
            if (agreed) {
                t |> equal(c.n_past - 1l, nrows, "the CPU rounds left the session where the device rounds did")
            }
            t |> equal(long_length(hydrated_rows), nrows, "{nrows} hydrated head rows read back")
            agreed_rows_within(t, tr, c, hydrated_rows, agreed_head_rows)
            var c7 <- spec_rounds(tr, c, [toks[ROUNDS]], false, false, true)
            to_log(LOG_INFO, "round 6 (declined draft): CPU head over hydrated rows drafts {round7[0].draft} '{decode_(tr, [round7[0].draft])}' hit {round7[0].hit} at {round7[0].n_past}, the CPU round {c7[0].draft} '{decode_(tr, [c7[0].draft])}' hit {c7[0].hit} at {c7[0].n_past}\n")
            if (agreed && pick_agrees(t, tr, "round 6 draft (the CPU head over the hydrated rows)", round7[0].draft, c7[0].draft_logits, DN_LOGIT_BAR_REL)) {
                t |> equal(round7[0].hit, c7[0].hit, "round 6: both rounds accepted or both rejected")
                t |> equal(round7[0].n_past, c7[0].n_past, "round 6: both rounds left the session at one position")
            }
            poison_head_rows(tr, c, c.n_past - 1l)
            var c8 <- spec_rounds(tr, c, [toks[ROUNDS + 1l]])
            var pl <- [for (x in c.mtp_logits); x]
            to_log(LOG_INFO, "control round: the draft over hydrated rows {round8[0].draft} '{decode_(tr, [round8[0].draft])}', the CPU draft over poisoned head rows {c8[0].draft} '{decode_(tr, [c8[0].draft])}' (its verify row 0 finite {all_finite(pl)})\n")
            t |> success(round8[0].draft != c8[0].draft || !all_finite(pl), "control: the CPU round over poisoned head rows drafts another token or lands non-finite rows")
            delete pl
            delete c8
            delete c7
            delete round8
            delete round7
            delete hydrated_rows
            delete cpu
            delete gpu
            delete c
            delete fed
            delete toks
            delete prompt
        }
    }
}

[test]
def test_gpu_resident_hybrid_mtp_nonfinite(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: a NaN carry through the resident head lands non-finite logits, and the draft panics naming the row instead of landing a pick") @(t : T?) {
        with_warm_carrier(t) $(var tr; prompt; var s) {
            let row = DRAFT_PROMPT - 1l
            let nan = unsafe(reinterpret<float>(NAN_BITS))
            for (i in range64(long_length(s.mtp_h))) {
                s.mtp_h[i] = nan
            }
            let drafts0 = moe_gpu_resident_drafts()
            let why = panic_text() $ {
                forward_mtp(tr, s, DRAFT_TOKEN, row)
            }
            to_log(LOG_INFO, "NaN carry draft at row {row}: {empty(why) ? "no panic" : why}\n")
            t |> success(contains(why, "landed non-finite logits") && contains(why, "resident draft") && contains(why, "row {row}"), "the draft panics on the non-finite row it landed, naming the row: '{why}'")
            t |> equal(moe_gpu_resident_drafts(), drafts0, "no draft was counted: the panic fired before the pick landed")
        }
    }
}

//! a session prefilled on the device with the speculative warm on, its position and the pass counters of `reason` and the
//! device counters read before a round; the round of `tok` then runs with the speculative gate on
struct private PassArm {
    reason : string
    passes0 : int64
    drafts0 : int64
    verifies0 : int64
}

def private pass_arm_begin(reason : string) : PassArm
    => PassArm(reason = reason, passes0 = pass_count(reason), drafts0 = moe_gpu_resident_drafts(), verifies0 = moe_gpu_resident_verifies())

//! one speculative round of `tok` on `s`, any panic's text answered (the round's outcome logged decoded)
[cold_path]
def private pass_arm_round(tr : Model; var s : Session; tok : int64) : string {
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    var hit = false
    let why = panic_text() $ {
        var acc = 0l
        hit = mtp_spec_eval(tr, s, tok, acc)
    }
    set_mtp_spec(spec0)
    let d = s.mtp_vbatch[1]
    to_log(LOG_INFO, "round of {tok} '{decode_(tr, [tok])}': draft {d} '{decode_(tr, [d])}' hit {hit} at {s.n_past}; {empty(why) ? "no panic" : why}\n")
    return why
}

//! the arm's counters: `arm.reason` passed at least `min_passes` calls, the device draft counter moved by `drafts`, the verify counter unmoved
def private pass_arm_counted(t : T?; arm : PassArm; min_passes, drafts : int64) {
    let n = pass_count(arm.reason) - arm.passes0
    t |> success(n >= min_passes, "{arm.reason}: at least {min_passes} call(s) passed to the CPU rails as {arm.reason} ({n} counted)")
    t |> equal(moe_gpu_resident_drafts() - arm.drafts0, drafts, "{arm.reason}: {drafts} device draft(s) - the draft seat {drafts > 0l ? "has no such arm and served" : "passed too"}")
    t |> equal(moe_gpu_resident_verifies() - arm.verifies0, 0l, "{arm.reason}: no device verify ran")
}

//! the arm's witnesses: the round completed on the CPU rails without a panic, and its counters
def private pass_arm_holds(t : T?; arm : PassArm; why : string; min_passes, drafts : int64) {
    t |> success(empty(why), "{arm.reason}: the round completed without a panic ({why})")
    pass_arm_counted(t, arm, min_passes, drafts)
}

let private GAP_ROWS = 5l   //! the rows the CPU rails serve past the region's, so the round's position is past the mirror's

[test]
def test_gpu_resident_hybrid_mtp_pass_arms(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: an f32 session under the f16 mirror - the prefill, the draft and the verify each pass to the CPU rails as codec, the round completing on the CPU") @(t : T?) {
        if (!mtp_carry_ready(t)) return
        if (mirror_kv_dtype() != KVDtype.f16) {
            t |> skip("the codec arm shapes an f32 session under the f16 mirror; under DASLLAMA_VK_KV32=1 the mirror is f32")
            return
        }
        var tr <- load_mtp_carrier(t, 1l)
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(DRAFT_PROMPT)
            var s <- create_session(tr, KVDtype.f32)
            let arm = pass_arm_begin("codec")
            prefill_warm(tr, s, prompt)
            let why = pass_arm_round(tr, s, DRAFT_TOKEN)
            pass_arm_holds(t, arm, why, 3l, 0l)
            delete s
            delete prompt
        }
    }
    t |> run("Qwen3.5-0.8B-MTP: a session the device never homed - its prompt read on the CPU rails under the server's prefill pin - drafts and verifies on the CPU rails as unhomed, the CPU verify over the recurrent state the CPU rails left at the prompt's end") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            var prompt <- resident_prompt(DRAFT_PROMPT)
            var s <- create_session(tr, mirror_kv_dtype())
            let pinned0 = pass_count("pinned_off")
            set_resident_prefill_allowed(false)
            prefill_warm(tr, s, prompt)
            set_resident_prefill_allowed(true)
            t |> success(pass_count("pinned_off") - pinned0 >= 1l, "the prompt passed to the CPU rails as pinned_off ({pass_count("pinned_off") - pinned0} counted)")
            t |> equal(moe_gpu_resident_rows(s), -1l, "the session holds no mirror region: the device never homed it")
            t |> equal(s.dn_pos, DRAFT_PROMPT, "the CPU rails left the recurrent state at the prompt's end")
            t |> equal(s.mtp_h_pos1, DRAFT_PROMPT, "... and stashed the carry there")
            let arm = pass_arm_begin("unhomed")
            let why = pass_arm_round(tr, s, DRAFT_TOKEN)
            pass_arm_holds(t, arm, why, 2l, 0l)
            delete s
            delete prompt
        }
    }
    t |> run("Qwen3.5-0.8B-MTP: a round five rows past the region's rows - rows the CPU rails served as a continuation the hybrid's resident prefill passes - the draft, which has no gap arm, serves on the device; the verify passes as gap and the CPU verify completes over the recurrent state the CPU rails left") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            var prompt <- resident_prompt(DRAFT_PROMPT)
            var more <- [for (i in range64(GAP_ROWS)); 3000l + (i * 61l) % 4000l]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            t |> equal(moe_gpu_resident_rows(s), DRAFT_PROMPT, "the region's rows end at the prompt")
            let cont0 = pass_count("continuation")
            prefill_warm(tr, s, more, DRAFT_PROMPT)
            t |> success(pass_count("continuation") - cont0 >= 1l, "the continuation passed to the CPU rails as continuation ({pass_count("continuation") - cont0} counted): a hybrid's resident prefill serves a prompt from its start alone")
            t |> equal(moe_gpu_resident_rows(s), DRAFT_PROMPT, "the region's rows still end at the prompt: the CPU rails served the {GAP_ROWS} past it")
            t |> equal(s.dn_pos, DRAFT_PROMPT + GAP_ROWS, "the recurrent state sits past the CPU-served rows")
            t |> equal(s.mtp_h_pos1, DRAFT_PROMPT + GAP_ROWS, "the CPU tail stashed the carry at the last CPU-served row")
            let arm = pass_arm_begin("gap")
            let why = pass_arm_round(tr, s, DRAFT_TOKEN)
            pass_arm_holds(t, arm, why, 1l, 1l)
            delete s
            delete more
            delete prompt
        }
    }
}

[test]
def test_gpu_resident_hybrid_mtp_rewind_refused(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: a round off the recurrent state's position - the draft, which has no rewind arm, serves on the device; the verify passes as rewind, and the CPU rails refuse the rewind by name: the recurrent state is forward-only on every rail") @(t : T?) {
        with_warm_carrier(t) $(var tr; prompt; var s) {
            t |> equal(s.dn_pos, DRAFT_PROMPT, "the recurrent state sits at the prompt's end")
            s.dn_pos = DRAFT_PROMPT - 1l
            let arm = pass_arm_begin("rewind")
            let why = pass_arm_round(tr, s, DRAFT_TOKEN)
            t |> success(contains(why, "deltanet state is forward-only") && contains(why, "position {DRAFT_PROMPT}") && contains(why, "session is at {DRAFT_PROMPT - 1l}"), "the CPU verify refused the rewind by name, both positions in the text: '{why}'")
            pass_arm_counted(t, arm, 1l, 1l)
        }
    }
}

[test]
def test_gpu_resident_hybrid_mtp_head_off(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: with the NextN head kept off the device (the draft seat unset for the load) the plan carries no verify rows, so a round's verify passes as verify_rows and the CPU verify's two-row window passes the resident prefill as verify_window - no panic, the verify counter unmoved, the driver still armed - and the CPU verify's row 0 sits within the deltanet bar of the CPU chain's step of the same token, the prefill's logits as the control") @(t : T?) {
        with_mtp_carrier(t, 1l, true) $(var tr) {
            t |> equal(rdec_verify_rows(), 0l, "the plan carries no verify rows: the head is off the device")
            var prompt <- resident_prompt(DRAFT_PROMPT)
            let pos = DRAFT_PROMPT
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            t |> equal(moe_gpu_resident_head_rows(s), 0l, "the chain warmed no head row: the head is off the device")
            let rows_arm = pass_arm_begin("verify_rows")
            let win0 = pass_count("verify_window")
            let why = pass_arm_round(tr, s, DRAFT_TOKEN)
            pass_arm_holds(t, rows_arm, why, 1l, 0l)
            t |> success(pass_count("verify_window") - win0 >= 1l, "the resident prefill declined the CPU verify's window as verify_window ({pass_count("verify_window") - win0} counted)")
            t |> success(moe_gpu_resident_active(), "the driver stays armed on the model after the CPU verify")
            var truth <- [for (x in s.mtp_logits); x]
            delete s
            moe_gpu_drop_model()
            var c <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, c, prompt)
            var pf <- [for (x in c.logits); x]
            forward(tr, c, DRAFT_TOKEN, pos)
            var step <- [for (x in c.logits); x]
            let at = argmax_of(truth)
            let ac = argmax_of(step)
            to_log(LOG_INFO, "head off: the CPU verify's row 0 argmax {at} '{decode_(tr, [at])}' ({truth[at]}), the CPU chain's step {ac} '{decode_(tr, [ac])}' ({step[ac]}); maxdiff {logits_maxdiff(truth, step)}\n")
            dn_row_within(t, "head off: the CPU verify's row 0 over the rows the decline brought down", truth, step, pf)
            delete step
            delete pf
            delete c
            delete truth
            delete prompt
        }
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

// model-free: a NaN in the CPU reference row reads past the bar its finite elements set, so no device row holds it
[test]
def test_row_bar_nan_reference(t : T?) {
    let rel = 0.04
    let cpu <- [1.0, -4.0, 2.0, 0.5]
    var gpu := cpu
    gpu[0] += 0.1
    var nan_cpu := cpu
    nan_cpu[2] = unsafe(reinterpret<float>(NAN_BITS))
    t |> equal(row_bar(cpu, rel), rel * 4.0, "the bar is rel of the row's largest |logit|")
    t |> success(logits_maxdiff(gpu, cpu) <= row_bar(cpu, rel), "a device row within the bar of a finite reference holds it")
    t |> equal(row_bar(nan_cpu, rel), rel * 4.0, "a NaN in the reference row scales no bar - the finite elements set it")
    t |> success(!(logits_maxdiff(gpu, nan_cpu) <= row_bar(nan_cpu, rel)), "a NaN in the reference row reads past the bar")
    t |> success(logits_maxdiff(gpu, nan_cpu) <= rel * logits_maxabs(nan_cpu), "control: the NaN-scaled bar is infinite and the same row holds it")
}

//! the asserts `rows_within_control` fails on a probe over the one device row `g` against the reference `c`, `off` its row one off
def private rows_bar_fails(g, c, off : array<float>) : int {
    var gr <- [ <- clone_to_move(g)]
    var cr <- [ <- clone_to_move(c), <- clone_to_move(off)]
    let fails = probe_fails() $(p) {
        rows_within_control(p, "probe", gr, cr, 0.04, 1l, 1.0) $(_r) => "row"
    }
    delete gr
    delete cr
    return fails
}

//! the asserts `tq4_within` fails on a probe over `g` against `c`, `off` its row one off
def private tq4_bar_fails(g, c, off : array<float>) : int {
    return probe_fails() $(p) {
        tq4_within(p, g, c, off, "probe")
    }
}

//! the resident rigs' row bars on synthetic rows, run on a probe handle: a NaN in the device row or the reference row reds
//! `rows_within_control` and `tq4_within`, the clean rows hold both (the control)
[test]
def test_row_bars_red_a_nan(t : T?) {
    let cpu <- [1.0, -4.0, 2.0, 0.5]
    let off <- [3.0, -1.0, -2.0, 4.5]
    var gpu := cpu
    gpu[0] += 0.05
    var nan_gpu := gpu
    nan_gpu[1] = unsafe(reinterpret<float>(NAN_BITS))
    var nan_cpu := cpu
    nan_cpu[2] = unsafe(reinterpret<float>(NAN_BITS))
    t |> equal(rows_bar_fails(gpu, cpu, off), 0, "control: rows_within_control holds a device row within the bar, its row one off past it")
    t |> success(rows_bar_fails(nan_gpu, cpu, off) > 0, "rows_within_control reds a NaN in the device row")
    t |> success(rows_bar_fails(gpu, nan_cpu, off) > 0, "rows_within_control reds a NaN in the reference row")
    t |> equal(tq4_bar_fails(gpu, cpu, off), 0, "control: tq4_within holds a device row nearer the reference than its share of the row one off")
    t |> success(tq4_bar_fails(nan_gpu, cpu, off) > 0, "tq4_within reds a NaN in the device row")
    t |> success(tq4_bar_fails(gpu, nan_cpu, off) > 0, "tq4_within reds a NaN in the reference row")
}

[test]
def test_gpu_resident_hybrid_mtp_block_codec(t : T?) {
    let pin = gpu_kv_dtype_pin()
    set_gpu_kv_dtype(KVDtype.q8_0)
    if (resident_mirror_dtype() != KVDtype.q8_0) {
        t |> skip("DASLLAMA_VK_KV32=1 arms the f32 mirror over a codec asked - the block-codec cells need it unset")
        restore_gpu_kv_dtype_(pin)
        return
    }
    t |> run("the NextN head over a q8_0 mirror: the draft, the verify, the warmed head rows, six rounds") @(t : T?) {
        test_gpu_resident_hybrid_mtp_draft(t)
        test_gpu_resident_hybrid_mtp_verify(t)
        test_gpu_resident_hybrid_mtp_head_warm(t)
        test_gpu_resident_hybrid_mtp_rounds(t)
    }
    set_gpu_kv_dtype(KVDtype.tq4)
    t |> run("the NextN head over a tq4 mirror: the verify and the forced reject against the driver's own one-row steps") @(t : T?) {
        test_gpu_resident_hybrid_mtp_verify(t)
        test_gpu_resident_hybrid_mtp_verify_reject(t)
    }
    restore_gpu_kv_dtype_(pin)
}
