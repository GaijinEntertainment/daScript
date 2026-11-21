options gen2
options _dasllama_internal = true

module _segment_cells shared public

require dastest/testing_boost public
require dasllama/dasllama
require dasllama/dasllama_common   // nolint:STYLE030,LINT019 - eval_segments_, forward_prefill, forward
require daslib/jobque_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require ?vulkan dasllama/dasllama_vulkan_prefill   // nolint:STYLE030,LINT019 - vk_rdec_prefill_segment_windows, read in the vulkan half
require dasllama/dasllama_cuda_decode
require dasllama/dasllama_env
require _model_tier
require _resident_feed

// The segmented window's cells on a model the caller loaded (`test_gpu_resident_segments.das`,
// the qwen35moe cut's block in `test_gpu_resident_qwen_moe.das`): two host-cached sessions' chunks as
// ONE window chain against the same chunks as two one-session windows on the same driver, and on
// a recurrent model the checkpoint stops one segmented call captures.

let SEG_OPEN_ROWS = 40l
let SEG_TAIL_A = 23l
let SEG_TAIL_B = 31l
let SEG_WINDOW = 512l   //! the file's prompt window: two SEG_LONG_TAILs run past it, so the segmented call runs two windows, the second holding B's rows alone
let SEG_LONG_TAIL = 300l

def pin_segment_window() {
    set_pf_window_rows_pin(SEG_WINDOW)   //! before every load of the file: the plan and the window chain read it
}
let SEG_STEPS = 8l
//! the bar a segment's rows hold against its solo window: the split bar, the routed bar on a routed model - its windows' expert
//! tiles schedule other row counts than the solo window's, and the router's near-ties part the two (the 35B reads 0.56-0.88 of a
//! 9-12 max logit, 0.09 at its widest, the one-step-off control far past)
var g_seg_bar_rel = SPLIT_BAR_REL

//! the segmented windows the armed driver counted so far - the CUDA home's under DASLLAMA_CUDA=1, else the Vulkan driver's; 0 with
//! no vulkan module (the cells then skip on the tier)
def segment_windows : int64 {
    if (gpu_want_cuda()) return cud_rdec_prefill_segment_windows()
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_prefill_segment_windows()
    } else {
        return 0l
    }
}

//! which driver's counter `segment_windows` reads, named in every assert on it
def seg_counter : string => gpu_want_cuda() ? " [the CUDA home's counter, DASLLAMA_CUDA=1]" : " [the Vulkan driver's counter, DASLLAMA_CUDA unset]"

def seg_open_session(tr : Model; var s : Session; opening : array<int64>) {
    eval_(tr, s, opening)
}

def seg_solo_rows(tr : Model; opening, tail, toks : array<int64>) : array<array<float>> {
    var s <- create_session(tr, mirror_kv_dtype())
    seg_open_session(tr, s, opening)
    eval_(tr, s, tail)
    var rows <- steps_after(tr, s, toks)
    delete s
    return <- rows
}

//! the two sessions' rows held to their solo forms: every row within the bar, the row one step off past it
def seg_hold_rows(t : T?; what : string; got, want : array<array<float>>) {
    rows_within_control(t, what, got, want, g_seg_bar_rel, 1l, 1.0, $(r) => r == 0l ? "the prefill's row" : "step {r}")
}

//! the windows of a segmented call that hold both segments - the one the boundary between A and B falls in; 0 with no vulkan
//! module and the CUDA home not asked for
def shared_windows(ta : int64) : int64 {
    if (gpu_want_cuda()) return ta % pf_window_rows() != 0l ? 1l : 0l
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return ta % pf_window_rows() != 0l ? 1l : 0l
    } else {
        return 0l
    }
}

//! two host-cached sessions' tails of `ta` and `tb` rows after their openings as ONE segmented call, then `SEG_STEPS` fed steps
//! each, against the same tails as two one-session windows: a call past one window runs its segments across windows, the
//! first segment ending in an earlier window than the last
def tails_as_segments(t : T?; tr : Model; ta, tb : int64; what : string) {
    var open_a <- resident_prompt(SEG_OPEN_ROWS, 0l)
    var open_b <- resident_prompt(SEG_OPEN_ROWS, 7l)
    var tail_a <- ramp_ids(ta, 3l, 2000l, 53l, 4000l)
    var tail_b <- ramp_ids(tb, 11l, 2000l, 53l, 4000l)
    let toks <- resident_feed(SEG_STEPS)
    var want_a <- seg_solo_rows(tr, open_a, tail_a, toks)
    var want_b <- seg_solo_rows(tr, open_b, tail_b, toks)
    var sa <- create_session(tr, mirror_kv_dtype())
    var sb <- create_session(tr, mirror_kv_dtype())
    seg_open_session(tr, sa, open_a)
    seg_open_session(tr, sb, open_b)
    let sw0 = segment_windows()
    let pf0 = gpu_resident_prefills_()
    var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
    var chunks : array<array<int64>>
    chunks |> reserve(2l)
    chunks |> push_clone(tail_a)
    chunks |> push_clone(tail_b)
    eval_segments_(tr, sess, chunks)
    sess |> clear()
    t |> equal(segment_windows() - sw0, shared_windows(ta), "{what}: the two tails rode one segmented call, {shared_windows(ta)} window holding both segments at the {pf_window_rows()}-row window{seg_counter()}")
    t |> equal(gpu_resident_prefills_() - pf0, 2l, "{what}: the driver counted a prefill a segment")
    t |> equal(sa.n_past, SEG_OPEN_ROWS + ta, "{what}: session A's position moved by its tail")
    t |> equal(sb.n_past, SEG_OPEN_ROWS + tb, "{what}: session B's position moved by its tail")
    var got_a <- steps_after(tr, sa, toks)
    var got_b <- steps_after(tr, sb, toks)
    seg_hold_rows(t, "{what}: session A, the tail as a segment", got_a, want_a)
    seg_hold_rows(t, "{what}: session B, the tail as a segment", got_b, want_b)
    log_argmax_rows(tr, "{what}: session A: solo window, then the tail as a segment", got_a, want_a)
    log_argmax_rows(tr, "{what}: session B: solo window, then the tail as a segment", got_b, want_b)
    delete sa
    delete sb
    delete open_a
    delete open_b
}

def segments_cell(t : T?; tr : Model) {
    tails_as_segments(t, tr, SEG_TAIL_A, SEG_TAIL_B, "short tails")
    var open_a <- resident_prompt(SEG_OPEN_ROWS, 0l)
    var open_b <- resident_prompt(SEG_OPEN_ROWS, 7l)
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
    t |> equal(segment_windows() - sw1, 1l, "the two openings rode one segmented window from position zero{seg_counter()}")
    let g0 <- [ <- logits_of(ca, tr.config.vocab_size), <- logits_of(cb, tr.config.vocab_size)]
    delete ca   //! the two regions go free before the references take them: a live host-cached session's region is never taken
    delete cb
    var ra <- create_session(tr, mirror_kv_dtype())
    var rb <- create_session(tr, mirror_kv_dtype())
    seg_open_session(tr, ra, open_a)
    seg_open_session(tr, rb, open_b)
    let w0 <- [ <- logits_of(ra, tr.config.vocab_size), <- logits_of(rb, tr.config.vocab_size)]
    rows_within_control(t, "the openings as one window", g0, w0, g_seg_bar_rel, 1l, 1.0, $(r) => r == 0l ? "session A's last row (B's as the control)" : "session B's last row")
    log_argmax_rows(tr, "the openings as one window: the solo rows, then the segmented rows", g0, w0)
    served_every_call(t)
    delete ra
    delete rb
}


//! a fresh device-home session adopts `claim` at `at` - a checkpoint row's position - continues the tail past it and takes `SEG_STEPS`
//! fed steps, held to the solo form; its own parked claim comes back for the next adoption
def adopt_at_row(t : T?; tr : Model; claim, at : int64; opening, tail, toks : array<int64>; want : array<array<float>>; what : string) : int64 {
    var s <- create_device_session(tr, gpu_device_session_dtype())
    t |> success(gpu_device_kv_adopt(s, claim, at), "{what}: a fresh session adopts the claim at the checkpoint row ({at})")
    let from = at - long_length(opening)
    var rest <- [for (i in range64(long_length(tail) - from)); tail[from + i]]
    eval_(tr, s, rest)
    var got <- steps_after(tr, s, toks)
    seg_hold_rows(t, "{what}: the continuation off the checkpoint row", got, want)
    log_argmax_rows(tr, "{what}: solo window, then the continuation off the checkpoint row", got, want)
    let next = gpu_device_kv_park(s)
    delete s
    delete got
    delete rest
    return next
}

def private rows_are(rows : array<int64>; a, b : int64) : bool =>
    long_length(rows) == 2l && ((rows[0] == a && rows[1] == b) || (rows[0] == b && rows[1] == a))

//! two device-home sessions' tails with checkpoint stops inside them as ONE segmented call, session A's region already keeping
//! a checkpoint row at its opening, so the two segments' captures plan different rows of their regions: every stop's row adopts
//! and continues as the solo form does - a stop's capture lands
//! on its own segment's row, not the first free row
def captures_cell(t : T?; tr : Model) {
    var open_a <- resident_prompt(SEG_OPEN_ROWS, 0l)
    var open_b <- resident_prompt(SEG_OPEN_ROWS, 7l)
    var tail_a <- ramp_ids(SEG_TAIL_A, 3l, 2000l, 53l, 4000l)
    var tail_b <- ramp_ids(SEG_TAIL_B, 11l, 2000l, 53l, 4000l)
    let toks <- resident_feed(SEG_STEPS)
    var want_a <- seg_solo_rows(tr, open_a, tail_a, toks)
    var want_b <- seg_solo_rows(tr, open_b, tail_b, toks)
    var sa <- create_device_session(tr, gpu_device_session_dtype())
    var sb <- create_device_session(tr, gpu_device_session_dtype())
    eval_(tr, sa, open_a)
    eval_(tr, sb, open_b)
    t |> success(gpu_device_kv_checkpoint(sa), "captures: session A's region keeps a checkpoint row at its opening ({SEG_OPEN_ROWS})")
    let stop_a = SEG_OPEN_ROWS + 10l
    let stop_b0 = SEG_OPEN_ROWS + 8l
    let stop_b1 = SEG_OPEN_ROWS + 20l
    sa.dn_stops <- [stop_a]
    sb.dn_stops <- [stop_b0, stop_b1]
    let pf0 = gpu_resident_prefills_()
    var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
    var chunks : array<array<int64>>
    chunks |> reserve(2l)
    chunks |> push_clone(tail_a)
    chunks |> push_clone(tail_b)
    eval_segments_(tr, sess, chunks)
    sess |> clear()
    t |> equal(gpu_resident_prefills_() - pf0, 2l, "captures: the driver counted a prefill a segment")
    t |> equal(long_length(sa.dn_captures), 1l, "captures: session A's stop captured inside the segmented call")
    t |> equal(long_length(sb.dn_captures), 2l, "captures: session B's two stops captured inside the segmented call")
    sa.dn_captures |> resize(0)
    sb.dn_captures |> resize(0)
    let claim_a = gpu_device_kv_park(sa)
    let claim_b = gpu_device_kv_park(sb)
    var rows_a <- gpu_device_kv_checkpoints(claim_a)
    var rows_b <- gpu_device_kv_checkpoints(claim_b)
    t |> success(rows_are(rows_a, SEG_OPEN_ROWS, stop_a), "captures: region A keeps its opening's row and the stop's ({rows_a})")
    t |> success(rows_are(rows_b, stop_b0, stop_b1), "captures: region B keeps both stops' rows ({rows_b})")
    let claim_b2 = adopt_at_row(t, tr, claim_b, stop_b1, open_b, tail_b, toks, want_b, "captures: session B's second stop")   //! the later stop first: an adoption keeps the rows at or under its position
    let claim_a2 = adopt_at_row(t, tr, claim_a, stop_a, open_a, tail_a, toks, want_a, "captures: session A's stop")
    let claim_b3 = adopt_at_row(t, tr, claim_b2, stop_b0, open_b, tail_b, toks, want_b, "captures: session B's first stop")
    t |> success(claim_a2 != 0l && claim_b3 != 0l, "captures: every adopter parked its claim")
    delete sa
    delete sb
    delete rows_a
    delete rows_b
    delete want_a
    delete want_b
    delete open_a
    delete open_b
    delete tail_a
    delete tail_b
}


//! the routed chain's bar (`_moe_resident.das`' reading)
let MOE_SEG_BAR_REL = 0.20

//! a recurrent model's segmented cells on a load at two regions the
//! caller made, under the segment window (`pin_segment_window`): a short
//! tail, a tail under the conv taps, tails past one window, and the
//! stops one call captures, inside the caller's engine job queue; the bar the
//! routed one on a routed model
def recurrent_segments_cell(t : T?; tr : Model) {
    t |> success(tr.config.recr_mask != 0ul, "the file is a recurrent hybrid")
    t |> success(gpu_device_prefill_segments_(tr), "the driver's segmented window takes the recurrent model")
    let allowed = resident_prefill_allowed()
    set_resident_prefill_allowed(true)
    g_seg_bar_rel = tr.config.n_expert > 0l ? MOE_SEG_BAR_REL : SPLIT_BAR_REL
    segments_cell(t, tr)
    tails_as_segments(t, tr, 2l, SEG_TAIL_B, "a tail under the conv taps")   //! the ring's leading rows off the history segment A continues
    tails_as_segments(t, tr, SEG_LONG_TAIL, SEG_LONG_TAIL, "tails past one window")
    captures_cell(t, tr)
    g_seg_bar_rel = SPLIT_BAR_REL
    set_resident_prefill_allowed(allowed)
}
