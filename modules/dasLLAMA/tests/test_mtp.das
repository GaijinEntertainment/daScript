options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — engine + arch registrations (incl. forward_mtp / generate_mtp_greedy)

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-correctness test — prefills on the CPU by design
}
require dasllama/dasllama_math   // parallel_argmax (engine-level, not re-exported by the facade)
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_gguf   // set_mtp_head_path - the split-head fixture names its head explicitly
require dasllama/dasllama_kv_dtype   // kv_row_bytes / kv_layer_kbase: the head's K row the seam writes
require daslib/jobque_boost
require daslib/linq
require daslib/fio
require daslib/strings_boost
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require _resident_feed   // head_k_row_bytes

// MTP/NextN self-speculative decode gates, on the qwen35-family -MTP- GGUFs (the files ship one
// extra trunk-shaped draft block at layer index n_layers plus nextn.{eh_proj,enorm,hnorm,...}).
// Three layers of gate, mirroring how the feature was proven in:
//   1. loader: nextn extras load, and with spec OFF the trunk continues the fixture EXACTLY like
//      the plain (non-MTP) file — trunk equality was proven with the simple_ids reference decode on both;
//   2. output-invariance: generate_mtp_greedy() == generate() token-for-token — greedy self-spec
//      changes wall time, never tokens (accept path reads batch-kernel logits, so this holds only
//      under the PINNED bit-exact kernels, same reasoning as test_parity's parity_gen_ok);
//   3. rollback: a poisoned 2-row verify batch + mtp_state_restore replays plain decode exactly
//      (the deltanet conv+S state is the only thing a rejected draft leaves behind — KV rows get
//      overwritten by construction).
// Beside them the round's seams on the 0.8B: the continuation prefill's seam row under a fake seam
// seat (served, declined on a planar model, declined on a blob), and the CPU round's fail-closed exit
// under a fake prefill driver that serves the verify window (the verify-window flag read as it runs).
// The GGUFs are gitignored multi-GB files, so each case skips cleanly when its model is absent.

// Qwen3.5-0.8B word-counting fixture: "Count from one to twenty: one, two, three," and its greedy
// 40-token continuation (" four, five, ... twenty.\nCount from one to twenty"). Frozen from
// the simple_ids reference decode on the PLAIN Qwen3.5-0.8B-Q8_0 file; the same run on the -MTP-
// file produced the identical PROMPT+GEN ids, so this single fixture gates the loader AND trunk
// equality of the MTP conversion. Confident list momentum — near-tie-free under the pinned kernels.
let MTP_PROMPT <- [2427l, 494l, 799l, 310l, 16570l, 25l, 799l, 11l, 1330l, 11l, 2250l, 11l]
let MTP_WANT <- [2943l, 11l, 4097l, 11l, 4590l, 11l, 7840l, 11l, 7810l, 11l, 11292l, 11l, 5600l, 11l,
                 42744l, 11l, 28279l, 11l, 58614l, 11l, 60163l, 11l, 35442l, 11l, 56091l, 11l, 78592l,
                 11l, 59942l, 11l, 90600l, 11l, 16570l, 13l, 198l, 2427l, 494l, 799l, 310l, 16570l]

// Pin the bit-exact kernel modes around `body` and restore the previous modes — classic prefill
// attention (not flash), scalar libm activations (not exp4), per-token recurrent deltanet prefill
// (not chunked). Token-exact gates must not ride near-tie kernel variance; see parity_gen_ok.
def private with_pinned_kernels(body : block) {
    let prev_attn = get_prefill_attn_mode()
    set_prefill_attn_mode(AttnPrefillMode.classic)
    let prev_act = get_act_vec()
    set_act_vec(false)
    let prev_dn = get_deltanet_prefill_mode()
    set_deltanet_prefill_mode(DnPrefillMode.recurrent)
    invoke(body)
    set_prefill_attn_mode(prev_attn)
    set_act_vec(prev_act)
    set_deltanet_prefill_mode(prev_dn)
}

// Plain greedy reference over one fresh session (generate()'s echo contract: prompt[1..] then the
// continuation).
def private gen_plain(tr : Model; prompt : array<int64>; total : int) : array<int64> {
    var out : array<int64>
    with_dasllama_jobque() {
        var s = make_run_state(tr.config)
        out <- generate(tr, s, prompt, total)
        delete s
    }
    return <- out
}

// Depth-1 self-spec greedy over one fresh session; accept telemetry lands in nd/na.
def private gen_spec(tr : Model; prompt : array<int64>; total : int; var nd, na : int64&) : array<int64> {
    var out : array<int64>
    with_dasllama_jobque() {
        var s = make_run_state(tr.config)
        out <- generate_mtp_greedy(tr, s, prompt, int64(total), nd, na)
        delete s
    }
    return <- out
}

// Spec-off pinned generate-and-compare (parity_gen_ok's shape). Helpers like this stay plain
// functions on purpose: they JIT-compile, so the deep forward chain runs on the native stack —
// calling the engine from the [test] run-lambda's interpreted frame overflows its 16K das stack.
def private specoff_gen_ok(tr : Model; prompt, want : array<int64>) : bool {
    var ok = false
    with_dasllama_jobque() {
        var s = make_run_state(tr.config)
        let ids <- generate(tr, s, prompt, length(prompt) + length(want))
        let echo = length(prompt) - 1
        let got <- [for (i in range(length(want))); ids[echo + i]; where echo + i < length(ids)]
        ok = ids_equal(got, want)
        delete s
    }
    return ok
}

//! the manual reject over the CPU restore: a poisoned 2-row verify (the draft a fixed garbage token) advances the deltanet
//! state past a wrong row, and the restore plus one plain forward must leave it as if the batch never ran; answers the tokens
//! decoded after it (first = the prefill argmax) and fills `want` with the plain-decode slice they must equal. Free of T? like
//! specoff_gen_ok: a T? param keeps the helper off the JIT, and the engine chain overflows the interpreter stack
def private reject_rollback_ids(tr : Model; n_check : int64; var want : array<int64>) : array<int64> {
    var got : array<int64>
    with_dasllama_jobque() {
        let np = long_length(MTP_PROMPT)
        // plain-decode reference (echo convention: ids[np-1] is the first generated token)
        var s1 = make_run_state(tr.config)
        let ref_ids <- generate(tr, s1, MTP_PROMPT, int(np + n_check + 1l))
        delete s1
        delete want
        want <- [for (i in range64(n_check + 1l)); ref_ids[np - 1l + i]]
        var s = make_run_state(tr.config)
        forward_prefill(tr, s, MTP_PROMPT, np, 0l)
        let tok = parallel_argmax(s.logits, tr.config.vocab_size)
        got |> push(tok)
        mtp_state_snapshot(s)
        var vbatch <- [tok, 42l]   // 42 = arbitrary garbage draft
        forward_prefill(tr, s, vbatch, 2l, np)
        mtp_state_restore(s)
        forward(tr, s, tok, np)   // re-advance through the accepted token alone
        var pos = np + 1l
        for (_i in range64(n_check)) {
            let next = parallel_argmax(s.logits, tr.config.vocab_size)
            got |> push(next)
            forward(tr, s, next, pos)
            pos++
        }
        delete s
        delete vbatch
    }
    return <- got
}

//! a forced-reject round's outcome: the tokens decoded after it (first = the prefill argmax), the plain-decode slice they must
//! equal, and whether the round accepted
struct private RejectRound {
    got : array<int64>
    want : array<int64>
    hit : bool
}

//! mtp_spec_eval with every draft rejected, then n_check plain steps that must equal plain decode on either rail; free of T? like reject_rollback_ids
[cold_path]
def private reject_round_ids(tr : Model; n_check : int64) : RejectRound {
    var out : RejectRound
    with_dasllama_jobque() {
        let np = long_length(MTP_PROMPT)
        var s1 = make_run_state(tr.config)
        let ref_ids <- generate(tr, s1, MTP_PROMPT, int(np + n_check + 1l))
        delete s1
        out.want <- [for (i in range64(n_check + 1l)); ref_ids[np - 1l + i]]
        let spec0 = get_mtp_spec()
        set_mtp_spec(true)   // the prompt's head warm and the round both key off it, as generate_mtp_greedy arms it
        var s = make_run_state(tr.config)
        forward_prefill(tr, s, MTP_PROMPT, np, 0l)
        s.n_past = np
        let tok = parallel_argmax(s.logits, tr.config.vocab_size)
        out.got |> push(tok)
        let reject0 = get_mtp_force_reject_every()
        set_mtp_force_reject_every(1l)
        var acc = 0l
        out.hit = mtp_spec_eval(tr, s, tok, acc)
        set_mtp_force_reject_every(reject0)
        set_mtp_spec(spec0)
        var pos = np + 1l
        for (_i in range64(n_check)) {
            let next = parallel_argmax(s.logits, tr.config.vocab_size)
            out.got |> push(next)
            forward(tr, s, next, pos)
            pos++
        }
        delete s
    }
    return <- out
}

// Run the plain-vs-spec invariance pair on a loaded model and assert token-for-token equality
// plus sane accept telemetry. `tag` distinguishes the arms in the report.
def private invariance_ok(t : T?; tr : Model; prompt : array<int64>; n_gen : int; tag : string) {
    let total = length(prompt) + n_gen
    let ref_ids <- gen_plain(tr, prompt, total)
    var nd = 0l
    var na = 0l
    let spec_ids <- gen_spec(tr, prompt, total, nd, na)
    t |> success(ids_equal(ref_ids, spec_ids), "{tag}: spec decode token-for-token vs plain")
    t |> success(nd > 0l, "{tag}: the draft head actually drafted ({nd} drafts)")
}

[test]
def test_mtp_loader(t : T?) {
    t |> run("MTP/NextN loader (Qwen3.5-0.8B-MTP Q8): nextn extras + spec-off trunk parity") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_gguf(path, QuantMode.q8)
        // block_count INCLUDES the draft block: n_layers stays the trunk count, nextn rides extra
        t |> equal(tr.config.n_layer_nextn, 1l)
        t |> equal(tr.config.n_layers, 24l)
        t |> success(tr.mtp_ehproj_off >= 0l && tr.mtp_enorm_off >= 0l && tr.mtp_hnorm_off >= 0l,
                     "nextn eh_proj/enorm/hnorm loaded")
        t |> success(tr.mtp_headnorm_off >= 0l, "nextn shared_head_norm loaded (qwen35 MTP files ship it)")
        tr.config.seq_len = 2048l
        // spec OFF: the -MTP- file's trunk must continue the fixture exactly like the plain file
        with_pinned_kernels() {
            t |> success(specoff_gen_ok(tr, MTP_PROMPT, MTP_WANT), "spec-off token-for-token vs the plain-file oracle")
        }
        delete tr
    }
}

[test]
def test_mtp_output_invariance(t : T?) {
    t |> run("MTP self-spec greedy decode is output-invariant (Qwen3.5-0.8B-MTP Q8)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_model_(path, QuantMode.q8)   // load_model: the prose arm encodes in-test
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            // the counting fixture is accept-heavy: exercises the 2-row batched accept path + backfill
            t |> invariance_ok(tr, MTP_PROMPT, 48, "counting")
            // prose is reject-heavier: exercises rollback + single-token re-advance mid-loop
            let prose <- encode(tr, "Once upon a time, there was a", true, false)
            t |> invariance_ok(tr, prose, 40, "prose")
        }
        delete tr
    }
}

[test]
def test_mtp_q4_0(t : T?) {
    t |> run("MTP self-spec greedy decode rides the q4_0 requant tier (Qwen3.5-0.8B-MTP Q8 -> q4_0)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_model_(path, QuantMode.q4_0)
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            t |> invariance_ok(tr, MTP_PROMPT, 48, "q4_0 counting")
        }
        delete tr
    }
}

[test]
def test_mtp_reject_rollback(t : T?) {
    t |> run("MTP rollback: poisoned 2-row verify + restore replays plain decode exactly, and a forced-reject round through the engine's seam does too") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_model_(path, QuantMode.q8)   // the engine entry with the tokenizer, for the decoded streams below; load_gguf leaves it out
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            var want : array<int64>
            let got <- reject_rollback_ids(tr, 6l, want)
            to_log(LOG_INFO, "manual reject: plain decode '{decode_(tr, want)}' {want}, post-rollback '{decode_(tr, got)}' {got}\n")
            t |> success(ids_equal(got, want), "post-rollback decode token-for-token vs plain")
            let rollbacks0 = moe_gpu_resident_rollbacks()
            let verifies0 = moe_gpu_resident_verifies()
            let round <- reject_round_ids(tr, 6l)
            t |> success(!round.hit, "the forced reject rejected the round's draft")
            to_log(LOG_INFO, "forced-reject round: plain decode '{decode_(tr, round.want)}' {round.want}, post-reject '{decode_(tr, round.got)}' {round.got}\n")
            t |> success(ids_equal(round.got, round.want), "forced-reject round: post-reject decode token-for-token vs plain")
            let served = moe_gpu_resident_verifies() - verifies0
            let rolled = moe_gpu_resident_rollbacks() - rollbacks0
            to_log(LOG_INFO, "forced-reject round: {served} device verify served, {rolled} device rollback counted (resident driver active {moe_gpu_resident_active()})\n")
            t |> equal(rolled, served, "the device rolled the reject back exactly when it served the verify ({served} served, {rolled} rolled back; an f32 session under an f16 mirror passes both as codec)")
        }
        delete tr
    }
}

let private SEAM_PROMPT = 20l   //! the first window's rows; the continuation window starts here
var private g_probe_seam_calls = 0l
var private g_probe_seam_serves = false
var private g_probe_seam_tok = -1l
var private g_probe_seam_pos = -1l
var private g_probe_window_active = false
var private g_probe_prefill_calls = 0l

[unused_argument(t, s, token, pos)]
def private probe_decode(t : Model; var s : Session; token, pos : int64) : bool => false

[unused_argument(t)]
def private probe_owner(t : Model) : bool => true

//! the seam probe: records the token and position it was asked for, and serves or declines as `g_probe_seam_serves` says
[unused_argument(t, s)]
def private probe_seam(t : Model; var s : Session; token, pos : int64) : bool {
    g_probe_seam_calls++
    g_probe_seam_tok = token
    g_probe_seam_pos = pos
    return g_probe_seam_serves
}

//! the prefill probe: claims the whole stack and its logits without computing either, and reads the session's verify window as it runs
[unused_argument(t, npos, start_pos)]
def private probe_prefill(t : Model; var s : Session; npos, start_pos : int64) : bool {
    g_probe_prefill_calls++
    g_probe_window_active = mtp_verify_window_active(s)
    prefill_override_logits_done()
    return true
}

//! the embed probe: claims the prompt's embed rows without gathering them, so a planar model flagged a blob reads no blob plane before the prefill probe claims the window
[unused_argument(t, s, npos, start_pos)]
def private probe_embed(t : Model; s : Session; npos, start_pos : int64) : bool => true

let private PROBE_SEAM = "mtp_seam_probe"   //! a fake decode override whose owner claims every model and whose seam seat is the probe
let private PROBE_PREFILL = "mtp_prefill_probe"   //! a fake prefill override that serves every window without computing it

//! every byte of the head's K row `row` set to 0xFF: a row the CPU seam rewrites moves off it, a row nothing wrote keeps it
def private poison_head_k_row(tr : Model; var s : Session; row : int64) {
    let c = tr.config
    let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, c.n_layers))
    var kp = kv_layer_kbase(s, c.n_layers, c.seq_len)
    for (i in range64(rowb)) {
        unsafe(kp[row * rowb + i]) = uint8(0xFF)
    }
}

//! a continuation window of `tokens` at `SEAM_PROMPT` on a session prefilled over the fixture's first `SEAM_PROMPT` rows with
//! speculation on, the head's seam row poisoned first, under the seam probe (`serves`) and, under `blob`, the model flagged a
//! blob with the prefill probe claiming the window: true when the seam row kept its poison (the CPU seam never ran)
[cold_path]
def private seam_row_kept(var tr : Model; serves, blob : bool) : bool {
    var kept = false
    with_dasllama_jobque() {
        let spec0 = get_mtp_spec()
        set_mtp_spec(true)
        var s = make_run_state(tr.config)
        var first <- [for (i in range64(SEAM_PROMPT)); MTP_PROMPT[i % long_length(MTP_PROMPT)]]
        var next <- [for (i in range64(4l)); MTP_PROMPT[(i + 3l) % long_length(MTP_PROMPT)]]
        forward_prefill(tr, s, first, SEAM_PROMPT, 0l)
        s.n_past = SEAM_PROMPT
        poison_head_k_row(tr, s, SEAM_PROMPT - 1l)
        register_decode_override(PROBE_SEAM, @@probe_decode)
        register_mtp_seat_owner(PROBE_SEAM, @@probe_owner)
        register_mtp_seam_override(PROBE_SEAM, @@probe_seam)
        register_prefill_override(PROBE_PREFILL, @@probe_prefill)
        let decode0 = active_decode_override()
        let prefill0 = active_prefill_override()
        select_decode_override(PROBE_SEAM)
        let embed_had = embed_gpu_gate_installed()
        let embed0 = embed_gpu_gate()
        if (blob) {
            select_prefill_override(PROBE_PREFILL)
            register_embed_gpu_gate(@@probe_embed)
            tr.metal_blob = true
        }
        g_probe_seam_serves = serves
        g_probe_seam_calls = 0l
        forward_prefill(tr, s, next, long_length(next), SEAM_PROMPT)
        tr.metal_blob = false
        if (embed_had) {
            register_embed_gpu_gate(embed0)
        } else {
            unregister_embed_gpu_gate()
        }
        select_prefill_override(prefill0)
        select_decode_override(decode0)
        set_mtp_spec(spec0)
        var row <- head_k_row_bytes(tr, s, SEAM_PROMPT - 1l)
        kept = all(row, $(b) => b == uint8(0xFF))
        delete row
        delete s
        delete first
        delete next
    }
    return kept
}

[test]
def test_mtp_prefill_seam(t : T?) {
    t |> run("the continuation prefill's seam row (Qwen3.5-0.8B-MTP Q8): a seam seat that serves skips the CPU seam, a declined seat on a planar model falls to it, and a declined seat on a blob model warns and writes nothing") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_gguf(path, QuantMode.q8)
        tr.config.seq_len = 2048l
        t |> success(seam_row_kept(tr, true, false), "a seam seat that serves: the CPU seam never wrote the head's row {SEAM_PROMPT - 1l} (its poison stands)")
        t |> equal(g_probe_seam_calls, 1l, "the seat was asked once")
        t |> equal(g_probe_seam_tok, MTP_PROMPT[3], "... for the window's first token")
        t |> equal(g_probe_seam_pos, SEAM_PROMPT, "... at the window's start")
        t |> success(!seam_row_kept(tr, false, false), "a declined seat on a planar model: the CPU seam wrote the head's row {SEAM_PROMPT - 1l} (the control - its poison moved)")
        t |> equal(g_probe_seam_calls, 1l, "the seat was asked once and declined")
        t |> success(seam_row_kept(tr, false, true), "a declined seat on a blob model (the prefill probe claiming the window): the seam row is left to the driver - nothing wrote it, the warning names the position")
        t |> equal(g_probe_prefill_calls, 1l, "the prefill probe claimed the window")
        delete tr
    }
}

[test]
def test_mtp_verify_window(t : T?) {
    t |> run("the CPU round's fail-closed exit (Qwen3.5-0.8B-MTP Q8): a prefill driver that serves the speculative verify window - the verify-window flag up while it runs, down outside the round - panics naming the driver") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_gguf(path, QuantMode.q8)
        tr.config.seq_len = 2048l
        moe_gpu_drop_model()   // the CPU round is the cell's subject: an armed driver whose mirror matches the session's codec would serve the verify itself, and no window opens
        let why = verify_window_panic(tr)
        to_log(LOG_INFO, "served verify window: {empty(why) ? "no panic" : why}\n")
        t |> success(contains(why, "served a speculative verify window") && contains(why, PROBE_PREFILL), "the CPU round panics naming the prefill driver that served its verify window: '{why}'")
        t |> success(g_probe_window_active, "the session's verify window was up while the driver served it")
        t |> success(!g_probe_window_after, "... and is down on the session outside the round")
        t |> success(!g_probe_window_outside, "... and was down for the prompt's own prefill under the same driver")
        delete tr
    }
}

var private g_probe_window_outside = false
var private g_probe_window_after = false

//! the fixture prefilled on the CPU with speculation on, then one round under the prefill probe claiming every window: the
//! panic's text, "" where none fired; the probe's window reading for the prompt's own prefill lands in `g_probe_window_outside`,
//! the session's window after the round in `g_probe_window_after`
[cold_path]
def private verify_window_panic(tr : Model) : string {
    var why = ""
    with_dasllama_jobque() {
        let np = long_length(MTP_PROMPT)
        let spec0 = get_mtp_spec()
        set_mtp_spec(true)
        var s = make_run_state(tr.config)
        forward_prefill(tr, s, MTP_PROMPT, np, 0l)
        s.n_past = np
        let tok = parallel_argmax(s.logits, tr.config.vocab_size)
        register_prefill_override(PROBE_PREFILL, @@probe_prefill)
        let prefill0 = active_prefill_override()
        select_prefill_override(PROBE_PREFILL)
        var s2 = make_run_state(tr.config)
        forward_prefill(tr, s2, MTP_PROMPT, np, 0l)   // the prompt through the probe: the flag must read down here
        g_probe_window_outside = g_probe_window_active
        delete s2
        why = panic_text() $ {
            var acc = 0l
            mtp_spec_eval(tr, s, tok, acc)
        }
        g_probe_window_after = mtp_verify_window_active(s)
        select_prefill_override(prefill0)
        set_mtp_spec(spec0)
        delete s
    }
    return why
}

// Qwen3.6-27B-MTP Q4_K_M — the 64-trunk-layer carrier: its draft block sits at layer index 64, the
// exact index where the pre-fix per-layer masks wrapped (mask >> 64 == mask >> 0 on x86) — this
// arm regresses that whole bug class (layer_is_recurrent / layer_is_sliding / wv_offs guards)
// besides being the model the feature is FOR (1.39x decode when frozen). Large tier (~16 GB):
// DASLLAMA_PARITY_FULL=1.
[test]
def test_mtp_invariance_27b(t : T?) {
    t |> run("MTP self-spec output-invariance (Qwen3.6-27B-MTP Q4_K_M, draft block at layer 64)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.6-27B-MTP-Q4_K_M.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_gguf(path, QuantMode.q8)
        t |> equal(tr.config.n_layer_nextn, 1l)
        t |> equal(tr.config.n_layers, 64l)
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            t |> invariance_ok(tr, MTP_PROMPT, 48, "counting-27b")
        }
        delete tr
    }
}

// GLM-4.5-Air Q4_K_M — the only NON-RECURRENT MTP model we have, so this is the sole arm covering
// mtp_spec_eval's `recr_mask == 0` reject branch (a rejected draft consumes the verify's own row-0
// logits + h0 instead of paying a second full forward; every qwen35 MTP file is a deltanet hybrid
// and takes the restore-and-re-advance branch instead). The code fixture's own reject count rides
// the kernels' summation order (it has read zero), so the arm reaches the branch through the
// engine's seam - every third draft rejected whatever the verify said - and holds the stream to
// plain decode there too. It is also the first arch combining
// attn_qkv_bias with a NextN block — the shape where the qkv/out bias arrays were sized by the
// TRUNK layer count, so blk.<n_layers>.attn_{q,k,v}.bias never loaded and the draft read past the
// end of t.bq. And glm4moe ships nextn.embed_tokens/shared_head_head as copies of the trunk's own
// tables, which the loader shape-checks rather than disabling the head over.
// Large tier (~73 GB, 2 shards): DASLLAMA_PARITY_FULL=1.
[test]
def test_mtp_invariance_glm(t : T?) {
    t |> run("MTP self-spec output-invariance (GLM-4.5-Air Q4_K_M, non-recurrent reject path)") @(t : T?) {
        let path = path_join(models_dir(), "glm45air/Q4_K_M/GLM-4.5-Air-Q4_K_M-00001-of-00002.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_model_(path, QuantMode.q8)   // load_model: this arm encodes its prompt in-test
        t |> equal(tr.config.n_layer_nextn, 1l)    // head ENABLED, not disabled over the table copies
        t |> equal(tr.config.n_layers, 46l)        // block_count 47 - nextn 1
        // the two properties that make this arm load-bearing — asserted so a future file losing
        // either tells us the coverage moved instead of silently passing
        t |> equal(tr.config.recr_mask, 0ul)
        t |> success(tr.config.attn_qkv_bias, "glm4moe carries qkv bias (the layers_ld sizing case)")
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            // A CODE prompt on purpose. Strict token-identity is only meaningful on a NEAR-TIE-FREE
            // fixture: on K-quant planes the accept path reads BATCH-kernel logits (mm_gemm/mm_b_f)
            // while plain decode reads GEMV (mm_kq), and those differ in reduction order — a
            // legitimate FP difference that `with_pinned_kernels` does NOT cover (it pins attention
            // mode, activations and deltanet, not the matmul family). At Q4_K that delta is far
            // below the model's own quantization error, so on tie-dense PROSE the two paths pick
            // differently at ties and identity fails for no defect (measured: prose diverges every
            // run, this fixture is bit-identical at 95% accept). Code is strongly determined ⇒ no
            // ties to break ⇒ identity is a real assertion. Same reasoning as the 27B arm's
            // deliberately near-tie-free counting fixture.
            // FOLLOW-UP: a margin-gated arm can cover prose — assert that any divergence sits below
            // the quantization noise floor; mtp_verify_logits already computes the margin.
            let code <- encode(tr, "def fibonacci(n):", true, false)
            let total = length(code) + 40
            var nd = 0l
            var na = 0l
            let ref_ids <- gen_plain(tr, code, total)
            let spec_ids <- gen_spec(tr, code, total, nd, na)
            t |> success(ids_equal(ref_ids, spec_ids), "code-glm: spec decode token-for-token vs plain")
            t |> success(nd > 0l, "code-glm: the draft head actually drafted ({nd} drafts, {nd - na} rejected on its own)")
            // the reject branch on demand: every third draft rejected, the stream still plain decode's
            set_mtp_force_reject_every(3l)
            var nd_forced = 0l
            var na_forced = 0l
            let forced_ids <- gen_spec(tr, code, total, nd_forced, na_forced)
            set_mtp_force_reject_every(0l)
            t |> success(ids_equal(ref_ids, forced_ids), "code-glm forced: spec decode through the reject branch token-for-token vs plain")
            t |> success(nd_forced >= 3l && nd_forced - na_forced >= nd_forced / 3l,
                         "code-glm forced: {nd_forced - na_forced} of {nd_forced} drafts rejected - reject branch covered")
        }
        delete tr
    }
}

// Qwen3.8-Flash-Next UD-IQ4_XS + its split `shared` Q8_0 head (the MTP/ sibling folder, so the head is
// named explicitly): the hyper-connection carrier - the draft head takes the trunk's WIDE residual,
// norms its streams itself, concatenates per stream into eh_proj, runs one hc-mixed attention + MoE
// block and collapses through its own head mixer before the borrowed classifier. The counting fixture
// (every Qwen-vocab carrier continues it identically) holds spec decode to plain decode token for
// token. Large tier (94 GB, 3 shards + 2.6 GB head): DASLLAMA_PARITY_FULL=1.
[test]
def test_mtp_invariance_flashnext(t : T?) {
    t |> run("MTP self-spec output-invariance (Qwen3.8-Flash-Next + split shared head, the wide-residual carry)") @(t : T?) {
        return if (!jit_only(t))
        let path = path_join(models_dir(), "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf")
        let head = path_join(models_dir(), "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf")
        if (!model_available(t, path) || !model_available(t, head)) return
        set_mtp_head_path(head)
        var tr <- load_gguf(path, QuantMode.q8)
        clear_mtp_head_path()
        t |> equal(tr.config.n_layer_nextn, 1l)
        t |> equal(tr.config.n_layers, 48l)
        t |> success(tr.config.hyper_conn, "the carrier is the hyper-connection arch")
        t |> success(tr.mtp_hc_head_norm_off >= 0l && tr.mtp_hc_head_down_off >= 0l && tr.mtp_hc_head_up_off >= 0l,
                     "the draft head's own head mixer loaded")
        tr.config.seq_len = 2048l
        let count <- [16l, 11l, 220l, 17l, 11l, 220l, 18l, 11l, 220l, 19l, 11l, 220l, 20l, 11l, 220l, 21l, 11l, 220l, 22l, 11l]
        with_pinned_kernels() {
            t |> invariance_ok(tr, count, 40, "counting-flashnext")
            // the reject branch on demand: the n-gram window and the recurrent state both roll back past a rejected draft
            let total = length(count) + 40
            let ref_ids <- gen_plain(tr, count, total)
            set_mtp_force_reject_every(3l)
            var nd_forced = 0l
            var na_forced = 0l
            let forced_ids <- gen_spec(tr, count, total, nd_forced, na_forced)
            set_mtp_force_reject_every(0l)
            t |> success(ids_equal(ref_ids, forced_ids), "counting-flashnext forced: spec decode through the reject branch token-for-token vs plain")
            t |> success(nd_forced >= 3l && nd_forced - na_forced >= nd_forced / 3l,
                         "counting-flashnext forced: {nd_forced - na_forced} of {nd_forced} drafts rejected - reject branch covered")
        }
        delete tr
    }
}
