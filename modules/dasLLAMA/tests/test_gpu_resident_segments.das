options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 - the umbrella fires each arch [init] registration (llama)
require dasllama/dasllama   // create_session, mirror_kv_dtype
require dasllama/dasllama_common   // nolint:STYLE030,LINT019 - eval_segments_, forward_prefill, forward
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, moe_gpu_resident_prefills
require ?vulkan dasllama/dasllama_vulkan_prefill   // nolint:STYLE030,LINT019 - vk_rdec_prefill_segment_windows, read in the vulkan half
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // resident_ready, load_resident, the ramps, rows_within_control, served_every_call

// The whole-model resident driver's SEGMENTED window (`DASLLAMA_GPU=1`): two host-cached sessions' chunks as ONE window
// chain, each in its own mirror region, against the same chunks as two one-session windows on the same driver - the
// driver against itself, no CPU chain. Llama-3.2-1B at Q8_0 (the ids form: the driver gathers the embedding) and at
// Q4_K_M (the x form: the host embeds). The continuations' rows - two 40-token openings, then tails of 23 and 31 - and
// then eight fed steps on each session, every row within the split bar of the one-session form's with the row one
// step off as the control; then both openings as one window from position zero the same way. The witnesses: the
// segmented-window counter up by one a segmented call, the driver's prefill count up by a segment, and every call
// served. The decline: under the server's prefill pin a host-cached chunk passes to the CPU rails, so the form declines
// the call whole and the engine runs each session alone (the pass counted once a session, the counter unmoved), every
// row still within the bar of the device-prefilled references. Never more live host-cached sessions than regions: a
// later prefill would take a live session's region, which the engine leaves to the scheduler's pin.
// Every cell skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without its file under
// models_dir().

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the fallback arms prefill through the one-session override; nothing here means the CPU
}

//! of the step's max |logit|: the regions rig's split bar - the window's GEMMs split k by the row count, so two row counts round apart
let private SPLIT_BAR_REL = 0.06
let private OPEN_ROWS = 40l
let private TAIL_A = 23l
let private TAIL_B = 31l
let private STEPS = 8l

//! the segmented windows the Vulkan driver counted so far; 0 with no vulkan module (the cells then skip on the tier)
def private segment_windows : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_prefill_segment_windows()
    } else {
        return 0l
    }
}

//! `s` after `opening` from position zero: the one-session window, the session's position past it
def private open_session(tr : Model; var s : Session; opening : array<int64>) {
    eval_(tr, s, opening)
}

//! the prefill's logits then each fed step's on `s` at its own position, the prefill already run
def private steps_after(tr : Model; var s : Session; toks : array<int64>) : array<array<float>> {
    var out : array<array<float>>
    out |> reserve(long_length(toks) + 1l)
    out |> emplace <| logits_of(s, tr.config.vocab_size)
    var one : array<int64>
    one |> resize(1)
    for (tok in toks) {
        one[0] = tok
        eval_(tr, s, one)
        out |> emplace <| logits_of(s, tr.config.vocab_size)
    }
    return <- out
}

//! the one-session form: `opening` then `tail` as two windows of one session, then the fed steps
def private solo_rows(tr : Model; opening, tail, toks : array<int64>) : array<array<float>> {
    var s <- create_session(tr, mirror_kv_dtype())
    open_session(tr, s, opening)
    eval_(tr, s, tail)
    var rows <- steps_after(tr, s, toks)
    delete s
    return <- rows
}

//! the two sessions' rows held to their solo forms: every row within the bar, the row one step off past it
def private hold_rows(t : T?; what : string; got, want : array<array<float>>) {
    rows_within_control(t, what, got, want, SPLIT_BAR_REL, 1l, 1.0, $(r) => r == 0l ? "the prefill's row" : "step {r}")
}

//! two sessions' tails as one window against their solo windows, then both openings as one window from zero
def private segments_cell(t : T?; tr : Model) {
    var open_a <- resident_prompt(OPEN_ROWS, 0l)
    var open_b <- resident_prompt(OPEN_ROWS, 7l)
    var tail_a <- ramp_ids(TAIL_A, 3l, 2000l, 53l, 4000l)
    var tail_b <- ramp_ids(TAIL_B, 11l, 2000l, 53l, 4000l)
    let toks <- resident_feed(STEPS)
    var want_a <- solo_rows(tr, open_a, tail_a, toks)
    var want_b <- solo_rows(tr, open_b, tail_b, toks)
    var sa <- create_session(tr, mirror_kv_dtype())
    var sb <- create_session(tr, mirror_kv_dtype())
    open_session(tr, sa, open_a)
    open_session(tr, sb, open_b)
    let sw0 = segment_windows()
    let pf0 = moe_gpu_resident_prefills()
    var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
    var chunks : array<array<int64>>
    chunks |> reserve(2l)
    chunks |> push_clone(tail_a)
    chunks |> push_clone(tail_b)
    eval_segments_(tr, sess, chunks)
    sess |> clear()
    t |> equal(segment_windows() - sw0, 1l, "the two tails rode one segmented window")
    t |> equal(moe_gpu_resident_prefills() - pf0, 2l, "the driver counted a prefill a segment")
    t |> equal(sa.n_past, OPEN_ROWS + TAIL_A, "session A's position moved by its tail")
    t |> equal(sb.n_past, OPEN_ROWS + TAIL_B, "session B's position moved by its tail")
    var got_a <- steps_after(tr, sa, toks)
    var got_b <- steps_after(tr, sb, toks)
    hold_rows(t, "session A, the tail as a segment", got_a, want_a)
    hold_rows(t, "session B, the tail as a segment", got_b, want_b)
    log_gen_texts(tr, "session A: solo window, then the tail as a segment", [for (r in want_a); argmax_of(r)], [for (r in got_a); argmax_of(r)])
    log_gen_texts(tr, "session B: solo window, then the tail as a segment", [for (r in want_b); argmax_of(r)], [for (r in got_b); argmax_of(r)])
    delete sa
    delete sb
    // the openings as one window from position zero
    var ca <- create_session(tr, mirror_kv_dtype())
    var cb <- create_session(tr, mirror_kv_dtype())
    var sess0 <- [unsafe(addr(ca)), unsafe(addr(cb))]
    var opens : array<array<int64>>
    opens |> reserve(2l)
    opens |> push_clone(open_a)
    opens |> push_clone(open_b)
    let sw1 = segment_windows()
    eval_segments_(tr, sess0, opens)
    sess0 |> clear()
    t |> equal(segment_windows() - sw1, 1l, "the two openings rode one segmented window from position zero")
    let g0 <- [ <- logits_of(ca, tr.config.vocab_size), <- logits_of(cb, tr.config.vocab_size)]
    delete ca   //! the two regions go free before the references take them: a live host-cached session's region is never taken
    delete cb
    var ra <- create_session(tr, mirror_kv_dtype())
    var rb <- create_session(tr, mirror_kv_dtype())
    open_session(tr, ra, open_a)
    open_session(tr, rb, open_b)
    let w0 <- [ <- logits_of(ra, tr.config.vocab_size), <- logits_of(rb, tr.config.vocab_size)]
    rows_within_control(t, "the openings as one window", g0, w0, SPLIT_BAR_REL, 1l, 1.0, $(r) => r == 0l ? "session A's last row (B's as the control)" : "session B's last row")
    served_every_call(t)
    delete ra
    delete rb
}

//! the decline: under the server's prefill pin (`set_resident_prefill_allowed(false)`) a host-cached session's chunk passes to
//! the CPU rails, so the segmented form declines the call whole and the engine runs each session alone - the pass counted once a
//! session, no segmented window - and the sessions' rows still land within the bar of the device-prefilled references
def private decline_cell(t : T?; tr : Model) {
    let toks <- resident_feed(STEPS)
    var opens : array<array<int64>>
    opens |> reserve(2l)
    var sess : array<Session?>
    sess |> reserve(2l)
    var sessions : array<Session>
    sessions |> reserve(2l)
    for (i in range64(2l)) {
        opens |> emplace <| resident_prompt(OPEN_ROWS + i * 5l, i * 13l)
        sessions |> emplace <| create_session(tr, mirror_kv_dtype())
    }
    for (i in range64(2l)) {
        sess |> push(unsafe(addr(sessions[i])))
    }
    let sw0 = segment_windows()
    let pinned = pass_count("pinned_off")
    set_resident_prefill_allowed(false)
    eval_segments_(tr, sess, opens)
    set_resident_prefill_allowed(true)
    sess |> clear()
    t |> equal(segment_windows() - sw0, 0l, "under the prefill pin the sessions rode no segmented window")
    t |> equal(pass_count("pinned_off") - pinned, 2l, "the engine ran each session alone, each passed to the CPU rails as pinned off")
    //! the steps on both sessions before any reference session takes a region: two live host-cached sessions a region each
    var got : array<array<array<float>>>
    got |> reserve(2l)
    for (i in range64(2l)) {
        got |> emplace <| steps_after(tr, sessions[i], toks)
    }
    delete sessions
    for (i in range64(2l)) {
        var ref <- create_session(tr, mirror_kv_dtype())
        open_session(tr, ref, opens[i])
        var inscope want <- steps_after(tr, ref, toks)
        hold_rows(t, "session {i} prefilled alone after the decline", got[i], want)
        delete ref
    }
    delete got
}

def private segments_cells(t : T?; file : string) {
    let path = path_join(models_dir(), file)
    return if (!resident_ready(t, path))
    var tr <- load_resident(path, 2l)
    return if (!resident_admitted(t, "the whole-model driver armed on {file} (the load log names any decline)", ""))
    t |> equal(tr.arch, "llama", "the file is a llama-family model")
    with_dasllama_jobque_() {
        segments_cell(t, tr)   // two sessions' tails as one window, then both openings as one
        decline_cell(t, tr)    // three sessions over two regions decline the window and still land
    }
    delete tr
}

[test]
def test_gpu_resident_segments_llama_1b_q8(t : T?) {
    segments_cells(t, "Llama-3.2-1B-Instruct-Q8_0.gguf")
}

[test]
def test_gpu_resident_segments_llama_1b_k(t : T?) {
    segments_cells(t, "Llama-3.2-1B-Instruct-Q4_K_M.gguf")
}
