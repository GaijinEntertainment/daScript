options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen4exp)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the model
require dasllama/dasllama_env   // g_env_vulkan.vk_kv32: the f32 mirror passes the f16 sessions as codec
require dasllama/dasllama_moe   // moe_pick_tape: the CPU arm routes as the device arm did
require dasllama/dasllama_gguf
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // feed_logits, feed_steps_within

// The whole-model resident driver on the hyper-connection carrier (Qwen3.8-Flash-Next UD-IQ4_XS, three shards, the large tier,
// DASLLAMA_GPU=1): the wide residual's mixer seams, the n-gram side input and the deltanet sigmoid out-gate on the device, the
// routed experts summed on the host between the segments of the window chain and of the split token command. The forced-feed
// logits-tolerance form of test_gpu_resident_hybrid: the same fixed tokens through the armed driver - the prompt through the
// window chain, every step through the token command - and through the all-CPU chain, the CPU chain routing as the device
// arm did (the decode pick tape): 512 experts top-10 over 4 mixed streams flip a near-tie on kernel-order noise alone, and
// one flipped expert moves the whole row past any bar. Every row's logits within the bar, the one-step-off control past it.
// The NextN cells (the draft, the speculative round) also need the split head shard beside the trunk
// (Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf) and skip without it.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm prefills on the CPU by design
}

//! the routed block's bar (test_gpu_resident_moe's reading); with the routing pinned the arms read 0.37-0.68 against
//! bars of 1.7-2.6, the control 5.2-12.0 (PERF_LEDGER.md, the hot expert pool's entry)
let private LOGIT_BAR_REL = 0.20
let private HC_PROMPT = 48l   //! past the pool's 32-row window floor: the prompt's routed rows split between the device and the host
//! the head's wide carry with the routing pinned: the arms read 0.06 against a max of 22 (0.3%), the draft one row back 3.5 (16%)
//! (PERF_LEDGER.md, the NextN entry: the draft cell's readings)
let private CARRY_BAR_REL = 0.05
//! the verify rows against the split command's one-row steps on the same lanes (N-column stamps round apart, the two pools split picks apart; the regions files' split bar): rows read 0.05-0.07 of the max logit, carry rows 0.01-0.08, the one-row-off controls 10 bars and more
let private SPLIT_BAR_REL = 0.12
let private SPLIT_CARRY_BAR_REL = 0.10
//! ... and against the CPU chain's one-row steps, that rounding compounding with the rails' own (the regions files' rows-form wide bar): rows read 0.16-0.25 of the max logit (row 1 inherits row 0's), carry rows 0.07-0.17
//! (both PERF_LEDGER.md, the NextN entry: the verify cell's readings)
let private WIDE_BAR_REL = 0.35
let private WIDE_CARRY_BAR_REL = 0.25
let private FN_TRUNK = "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf"
let private FN_HEAD = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf"
let private DRAFT_TOKEN = 3000l

def private hc_ready(t : T?) : bool {
    if (!resident_ready(t, path_join(models_dir(), FN_TRUNK)) || !model_available(t, path_join(models_dir(), FN_HEAD))) return false
    if (g_env_vulkan.vk_kv32 != 0l) {
        t |> skip("DASLLAMA_VK_KV32=1: the f32 mirror passes every f16 session as codec")
        return false
    }
    return true
}

struct private Round {
    draft : int64
    dev0, dev1 : array<float>   //! the verify's two logits rows off the device
    devh0, devh1 : array<float>   //! ... and its two wide carry rows
    h0 : array<float>   //! the carry the round drafted from (the prompt's last wide residual)
}

//! one speculative round on the device over `prompt`: the draft, the two-row verify, the accept or the rollback - the counters say
//! every seat served, the accept follows row 0's argmax, and the pick tape records the head's select then the rows' selects a layer
[cold_path]
def private hc_round(t : T?; tr : Model; prompt : array<int64>) : Round {
    var s <- create_session(tr, mirror_kv_dtype())
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    forward_prefill(tr, s, prompt, long_length(prompt), 0l)
    s.n_past = long_length(prompt)
    var r <- Round(h0 <- [for (x in s.mtp_h); x])
    let drafts0 = moe_gpu_resident_drafts()
    let verifies0 = moe_gpu_resident_verifies()
    let rollbacks0 = moe_gpu_resident_rollbacks()
    moe_pick_tape(MoePickTape.record)
    let acc = mtp_spec_round(tr, s, DRAFT_TOKEN)
    moe_pick_tape(MoePickTape.off)
    set_mtp_spec(spec0)
    t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the round's draft ran on the device")
    t |> equal(moe_gpu_resident_verifies(), verifies0 + 1l, "the round's verify ran on the device")
    served_every_call(t)
    r.draft = s.mtp_vbatch[1]
    let vocab = tr.config.vocab_size
    let hd = hc_dim(tr.config)
    t |> success(long_length(s.mtp_logits_b) >= 2l * vocab && long_length(s.mtp_hrows) >= 2l * hd, "the verify landed two logits rows and two wide carry rows")
    r.dev0 <- [for (i in range64(vocab)); s.mtp_logits_b[i]]
    r.dev1 <- [for (i in range64(vocab)); s.mtp_logits_b[vocab + i]]
    r.devh0 <- [for (i in range64(hd)); s.mtp_hrows[i]]
    r.devh1 <- [for (i in range64(hd)); s.mtp_hrows[hd + i]]
    t |> equal(moe_gpu_resident_verify_rows(), 2l, "the device verify landed two rows: the token and the draft")
    let a0 = argmax_of(r.dev0)
    let a1 = argmax_of(r.dev1)
    to_log(LOG_INFO, "hc round: draft {r.draft} '{decode_(tr, [r.draft])}', verify row 0 argmax {a0} '{decode_(tr, [a0])}' ({a0 == r.draft ? "accepts" : "rejects"} the draft), row 1 argmax {a1} '{decode_(tr, [a1])}', accepted {acc}, rollbacks {moe_gpu_resident_rollbacks() - rollbacks0}, position {s.n_past}\n")
    t |> equal(acc, a0 == r.draft ? 1l : 0l, "the round accepted the draft exactly where row 0's argmax is the draft")
    t |> equal(moe_gpu_resident_rollbacks() - rollbacks0, a0 == r.draft ? 0l : 1l, "a reject rolled the region back on the device, an accept did not")
    t |> equal(s.n_past, HC_PROMPT + 1l + acc, "the position moved by the token and the accepted draft")
    delete s
    return <- r
}

struct private LaneSteps {
    l0, l1 : array<float>   //! the two tokens' logits, a one-row step each
    h0, h1 : array<float>   //! ... and the wide carry after each
    seen : int64   //! the selects the two lanes replayed
}

//! the round's two tokens as one-row steps on its pick lanes (row 0's lane, then row 1's): `carry_split` reads the split step's landed
//! carry (`mtp_h`), else the CPU chain's wide residual
[cold_path]
def private lane_steps(tr : Model; var s : Session; tok0, tok1, k : int64; carry_split : bool) : LaneSteps {
    var r : LaneSteps
    moe_pick_tape_lane(1l, 2l, k)
    forward(tr, s, tok0, HC_PROMPT)
    r.seen = moe_pick_tape_stats().seen
    r.l0 <- [for (x in s.logits); x]
    r.h0 <- [for (x in (carry_split ? s.mtp_h : s.hc_res)); x]
    moe_pick_tape_lane(2l, 2l, k)
    forward(tr, s, tok1, HC_PROMPT + 1l)
    r.seen += moe_pick_tape_stats().seen
    moe_pick_tape(MoePickTape.off)
    r.l1 <- [for (x in s.logits); x]
    r.h1 <- [for (x in (carry_split ? s.mtp_h : s.hc_res)); x]
    let a0 = argmax_of(r.l0)
    let a1 = argmax_of(r.l1)
    to_log(LOG_INFO, "hc lane steps ({carry_split ? "the split command" : "the CPU chain"}): row 0 argmax {a0} '{decode_(tr, [a0])}', row 1 argmax {a1} '{decode_(tr, [a1])}'\n")
    return <- r
}

//! `rows_within_control` over rows handed as literals, the arrays freed after the compare
def private within(t : T?; name : string; var g : array<array<float>>; var c : array<array<float>>; bar : float; tail : string) {
    rows_within_control(t, name, g, c, bar, 1l, 1.0, $(_r) => tail)
    delete g
    delete c
}

//! one speculative round on the device (the draft, the two-row verify, the accept or the rollback): its two rows and wide carry rows
//! against the split command's one-row steps and the CPU's on the round's pick lanes, the other row as each row's control
[test]
def test_gpu_resident_hc_mtp_verify(t : T?) {
    t |> run("Qwen3.8-Flash-Next + its split head: one speculative round with the draft and the two-row verify on the device against the CPU chain over the same tokens") @(t : T?) {
        if (!hc_ready(t)) return
        set_mtp_head_path(path_join(models_dir(), FN_HEAD))
        var tr <- load_model_(path_join(models_dir(), FN_TRUNK), QuantMode.q8)
        clear_mtp_head_path()
        t |> success(!mtp_head_path_pin().set, "the head pin is cleared for the process after the load")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the trunk and its head (the load log names any decline)")
        with_dasllama_jobque_() {
            let tape_before = moe_pick_tape_mode()
            var prompt <- resident_prompt(HC_PROMPT)
            var r <- hc_round(t, tr, prompt)
            let heat = g_env_gpu.gpu_heat ?? HOT_SLOTS_AUTO
            let k = tr.config.n_expert_used
            let nl = tr.config.n_layers
            var sd <- create_session(tr, mirror_kv_dtype())
            let spec1 = get_mtp_spec()
            set_mtp_spec(true)   // the split steps land the carry
            forward_prefill(tr, sd, prompt, long_length(prompt), 0l)
            sd.n_past = long_length(prompt)
            var ds <- lane_steps(tr, sd, DRAFT_TOKEN, r.draft, k, true)
            set_mtp_spec(spec1)
            delete sd
            t |> equal(ds.seen, 2l * nl, "the two split steps replayed the rows' lanes, a select a routed layer each")
            within(t, "hc verify row 0 against the split command's one-row step, picks alike (DASLLAMA_GPU_HEAT {heat})", [ <- clone_to_move(r.dev0)], [ <- clone_to_move(ds.l0), <- clone_to_move(ds.l1)], SPLIT_BAR_REL, "(the split command's row 1 as the control)")
            within(t, "hc verify row 1 against the split command's one-row step, picks alike (DASLLAMA_GPU_HEAT {heat})", [ <- clone_to_move(r.dev1)], [ <- clone_to_move(ds.l1), <- clone_to_move(ds.l0)], SPLIT_BAR_REL, "(the split command's row 0 as the control)")
            within(t, "hc verify carry row 0 against the split command's one-row step, picks alike (DASLLAMA_GPU_HEAT {heat})", [ <- clone_to_move(r.devh0)], [ <- clone_to_move(ds.h0), <- clone_to_move(ds.h1)], SPLIT_CARRY_BAR_REL, "(the split command's carry row 1 as the control)")
            within(t, "hc verify carry row 1 against the split command's one-row step, picks alike (DASLLAMA_GPU_HEAT {heat})", [ <- clone_to_move(r.devh1)], [ <- clone_to_move(ds.h1), <- clone_to_move(ds.h0)], SPLIT_CARRY_BAR_REL, "(the split command's carry row 0 as the control)")
            delete ds
            moe_gpu_drop_model()
            var sc <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, sc, prompt, long_length(prompt), 0l)
            sc.n_past = long_length(prompt)
            moe_pick_tape_lane(0l, 1l, k)
            var cpu_draft <- draft_from(tr, sc, DRAFT_TOKEN, HC_PROMPT - 1l, r.h0)
            let tape_d = moe_pick_tape_stats()
            var cs <- lane_steps(tr, sc, DRAFT_TOKEN, r.draft, k, false)
            t |> equal(tape_d.seen + cs.seen, 1l + 2l * nl, "the CPU draft and the two token steps replayed the round's selects, a lane each")
            let cpu_a = argmax_of(cpu_draft.logits)
            to_log(LOG_INFO, "hc round: the CPU draft's argmax {cpu_a} '{decode_(tr, [cpu_a])}' against the device's draft {r.draft} '{decode_(tr, [r.draft])}'; tape: {tape_d.seen + cs.seen} selects replayed, {tape_d.miss} of the CPU draft's own picks off the device head's\n")
            within(t, "hc verify row 1 logits against the CPU's one-row step", [ <- clone_to_move(r.dev1)], [ <- clone_to_move(cs.l1), <- clone_to_move(cs.l0)], WIDE_BAR_REL, "(the CPU's row 0 as the control)")
            within(t, "hc verify row 0 logits against the CPU's one-row step", [ <- clone_to_move(r.dev0)], [ <- clone_to_move(cs.l0), <- clone_to_move(cs.l1)], WIDE_BAR_REL, "(the CPU's row 1 as the control)")
            within(t, "hc verify wide carry row 0 against the CPU's one-row step", [ <- clone_to_move(r.devh0)], [ <- clone_to_move(cs.h0), <- clone_to_move(cs.h1)], WIDE_CARRY_BAR_REL, "(the CPU's carry row 1 as the control)")
            within(t, "hc verify wide carry row 1 against the CPU's one-row step", [ <- clone_to_move(r.devh1)], [ <- clone_to_move(cs.h1), <- clone_to_move(cs.h0)], WIDE_CARRY_BAR_REL, "(the CPU's carry row 0 as the control)")
            delete cs
            delete cpu_draft
            delete sc
            delete r
            delete prompt
            t |> equal(int(moe_pick_tape_mode().mode), int(tape_before.mode), "the pick tape is back at the mode the cell found")
        }
        delete tr
    }
}

//! the NextN head on the hyper-connection chain: one device draft against the CPU forward_mtp over the same carry and the head rows the chain warmed (hydrated to the host), the CPU draft one row back as the control
[test]
def test_gpu_resident_hc_mtp_draft(t : T?) {
    t |> run("Qwen3.8-Flash-Next + its split head: one NextN draft through the resident head on the chain against the CPU forward_mtp on the same token, wide carry and row") @(t : T?) {
        if (!hc_ready(t)) return
        set_mtp_head_path(path_join(models_dir(), FN_HEAD))
        var tr <- load_model_(path_join(models_dir(), FN_TRUNK), QuantMode.q8)
        clear_mtp_head_path()
        t |> success(!mtp_head_path_pin().set, "the head pin is cleared for the process after the load")
        t |> equal(tr.config.n_layer_nextn, 1l, "the split head attached")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the trunk and its head (the load log names any decline)")
        with_dasllama_jobque_() {
            let tape_before = moe_pick_tape_mode()
            var prompt <- resident_prompt(HC_PROMPT)
            var s <- create_session(tr, mirror_kv_dtype())
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            forward_prefill(tr, s, prompt, long_length(prompt), 0l)
            s.n_past = long_length(prompt)
            set_mtp_spec(spec0)
            t |> equal(s.mtp_h_pos1, HC_PROMPT, "the window chain landed the wide carry at the prompt's end")
            var h0 <- [for (x in s.mtp_h); x]
            t |> equal(long_length(h0), hc_dim(tr.config), "the carry is the wide residual")
            let row = HC_PROMPT - 1l
            let drafts0 = moe_gpu_resident_drafts()
            moe_pick_tape(MoePickTape.record)   // the head's one select: the CPU head routes as the device head did
            var gpu <- draft_from(tr, s, DRAFT_TOKEN, row, h0)
            moe_pick_tape(MoePickTape.off)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the resident head drafted on the chain")
            t |> equal(s.mtp_h_pos1, 0l, "the draft's carry is the head's own: its watermark stays 0")
            let pick = moe_gpu_resident_draft_pick()
            let seat = unset_rdec_draft()
            moe_pick_tape(MoePickTape.replay)
            var cpu <- draft_from(tr, s, DRAFT_TOKEN, row, h0)
            let tape = moe_pick_tape_stats()
            moe_pick_tape(MoePickTape.off)
            var one_row_back <- draft_from(tr, s, prompt[row], row - 1l, h0)
            reinstall_rdec_draft(seat)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the unset seat left both CPU drafts to forward_mtp")
            served_every_call(t)
            let ag = argmax_of(gpu.logits)
            let ac = argmax_of(cpu.logits)
            to_log(LOG_INFO, "hc draft (row {row}, token {DRAFT_TOKEN}): argmax cpu {ac} ({cpu.logits[ac]}), resident {ag} ({gpu.logits[ag]}), device pick {pick}; tape: {tape.seen} selects replayed, {tape.miss} of the CPU head's own picks off the device's\n")
            t |> equal(tape.seen, 1l, "the CPU head replayed the device head's one select")
            within(t, "hc draft logits", [ <- clone_to_move(gpu.logits)], [ <- clone_to_move(cpu.logits), <- clone_to_move(one_row_back.logits)], LOGIT_BAR_REL, "(the CPU draft one row back as the control)")
            within(t, "hc draft wide carry", [ <- clone_to_move(gpu.h)], [ <- clone_to_move(cpu.h), <- clone_to_move(one_row_back.h)], CARRY_BAR_REL, "(the CPU draft one row back as the control)")
            t |> equal(pick, ag, "the device pick is the argmax of the logits it landed")
            pick_agrees(t, tr, "hc draft (row {row}, token {DRAFT_TOKEN})", pick, cpu.logits, LOGIT_BAR_REL)
            t |> equal(int(moe_pick_tape_mode().mode), int(tape_before.mode), "the pick tape is back at the mode the cell found")
            delete gpu
            delete cpu
            delete one_row_back
            delete h0
            delete s
            delete prompt
        }
        delete tr
    }
}

[test]
def test_gpu_resident_hc(t : T?) {
    t |> run("Qwen3.8-Flash-Next: the window chain and the split token command (the mixers, the side input and the sigmoid out-gate on device, the routed experts on the host) vs the CPU chain") @(t : T?) {
        let path = path_join(models_dir(), FN_TRUNK)
        if (!resident_ready(t, path)) return
        if (g_env_vulkan.vk_kv32 != 0l) {
            t |> skip("DASLLAMA_VK_KV32=1: the f32 mirror passes every f16 session as codec")
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(tr.config.hyper_conn && tr.config.n_expert > 0l && tr.config.dn_z_sigmoid, "the carrier is the hyper-connection MoE with the sigmoid out-gate the chain exists for")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on it (the load log names any decline)")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(HC_PROMPT)
            var toks <- resident_feed(6l)
            let served0 = moe_gpu_resident_prefills()
            var sg <- create_session(tr, mirror_kv_dtype())
            moe_pick_tape(MoePickTape.record)
            var gpu <- feed_logits(tr, sg, prompt, toks)
            moe_pick_tape(MoePickTape.off)
            t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the window chain served the prompt (its hyper-connection form)")
            let hot = gpu_resident_hot_stats()
            let slots = gpu_resident_hot_slots()
            to_log(LOG_INFO, "hot expert pool: {gpu_resident_hot_slots()} slots a layer, {hot.uploads} experts uploaded, {hot.hits} of {hot.picks} decode picks and {hot.window_hits} of {hot.window_rows} window slot rows served on the device\n")
            if (slots > 0l) {
                let knob = "gpu_resident_hot_slots() = {slots}, DASLLAMA_GPU_HEAT = {g_env_gpu.gpu_heat ?? HOT_SLOTS_AUTO} (-1 = unset, the plan's own count)"
                t |> equal(hot.picks, long_length(toks) * tr.config.n_layers * tr.config.n_expert_used, "every routed layer took the pool, and every decode pick went through it ({knob})")
                t |> success(hot.uploads > 0l && hot.hits > 0l && hot.hits < hot.picks, "the prompt's picks filled the pool, the device served some of the steps' picks and the host the rest - both shares of the sum under the bar below ({knob}: {hot.hits} hits of {hot.picks} picks, {hot.uploads} uploads)")
                t |> equal(hot.window_rows, long_length(prompt) * tr.config.n_layers * tr.config.n_expert_used, "the tier's expert chain took every routed layer's window ({knob})")
                t |> success(hot.window_hits > 0l && hot.window_hits < hot.window_rows, "the device served some of the window's slot rows through the expert tiles and the host the rest ({knob}: {hot.window_hits} hits of {hot.window_rows} rows)")
            }
            delete sg
            moe_gpu_drop_model()
            var sc <- create_session(tr, mirror_kv_dtype())
            moe_pick_tape(MoePickTape.replay)   // the compare reads the seams, not the near-ties
            var cpu <- feed_logits(tr, sc, prompt, toks)
            let tape = moe_pick_tape_stats()
            moe_pick_tape(MoePickTape.off)
            delete sc
            t |> equal(tape.seen, (long_length(prompt) + long_length(toks)) * tr.config.n_layers, "every CPU select replayed one taped select (the arms route in the same order, the prompt's rows first)")
            to_log(LOG_INFO, "pick tape: the CPU chain's own decode top-k left {tape.miss} picks off the device arm's tape ({tape.seen} selects replayed)\n")
            feed_steps_within(t, tr, "", long_length(prompt), gpu, cpu, toks, LOGIT_BAR_REL, "the split command")
            delete gpu
            delete cpu
            delete prompt
            delete toks
        }
        delete tr
    }
}
