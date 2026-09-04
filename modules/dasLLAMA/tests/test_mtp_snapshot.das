options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common   // Session, mtp_state_snapshot / mtp_state_restore, mtp_pos_*
require strings

// The speculative round's deltanet rollback: the snapshot sizes its two buffers to the session's
// recurrent state on the first call. A 27B hybrid (48 recurrent layers x 32 heads x 128 x 192) is
// 151 MB of state - past the 64 MB unreserved-growth guard - so the first snapshot must reserve
// before it grows, on a bare session, with no model file.

def private big_state(var a : array<float>; n : int64; fill : float) {
    a |> reserve(n)
    a |> resize(n)
    for (i in range64(n)) {
        a[i] = fill
    }
}

[test]
def test_first_snapshot_of_a_large_recurrent_state(t : T?) {
    var s : Session
    let ns = 48l * 32l * 128l * 192l   // 37748736 floats = 151 MB, the Qwen3.8-27B deltanet state
    let nc = 48l * 3l * 5120l
    big_state(s.dn_state, ns, 1.5)
    big_state(s.dn_conv_state, nc, 0.25)
    // a distinct value at an interior index of each of the copy's (up to eight) lane spans and one
    // conv element: the lane-parallel copy must restore every span, not just the last element
    let lanes = 8l
    for (li in range64(lanes)) {
        s.dn_state[li * ns / lanes + 1000l] = 3.0 + float(li)
    }
    s.dn_conv_state[nc / 2l] = 0.75
    s.dn_pos = 17l
    mtp_state_snapshot(s)
    t |> equal(long_length(s.mtp_snap_state), ns, "the state snapshot holds every float")
    t |> equal(long_length(s.mtp_snap_conv), nc, "the conv snapshot holds every float")
    t |> equal(s.mtp_snap_pos, 17l, "the position rides along")
    s.dn_state[ns - 1l] = -9.0
    for (li in range64(lanes)) {
        s.dn_state[li * ns / lanes + 1000l] = -9.0
    }
    s.dn_conv_state[nc / 2l] = -9.0
    s.dn_pos = 18l
    mtp_state_restore(s)
    t |> equal(s.dn_state[ns - 1l], 1.5, "restore puts the snapshot back")
    for (li in range64(lanes)) {
        t |> equal(s.dn_state[li * ns / lanes + 1000l], 3.0 + float(li), "restore covers lane {li}'s span, not only the last element")
    }
    t |> equal(s.dn_conv_state[nc / 2l], 0.75, "restore covers the conv state too")
    t |> equal(s.dn_pos, 17l, "restore rewinds the position")
    delete s
}

// The per-position accept telemetry both round drivers feed: hit[i] / try[i] over the rounds
// since the last reset, one entry per drafted depth, silent past the deepest draft.
[test]
def test_per_position_accept_telemetry(t : T?) {
    mtp_pos_reset()
    mtp_pos_note(3l, 2l)
    mtp_pos_note(2l, 0l)
    mtp_pos_note(1l, 1l)
    let rep = mtp_pos_report()
    t |> success(find(rep, "p1 66.7% (2/3)") >= 0, "p1 is two hits in three tries: {rep}")
    t |> success(find(rep, "p2 50.0% (1/2)") >= 0, "p2 is one hit in two tries: {rep}")
    t |> success(find(rep, "p3 0.0% (0/1)") >= 0, "p3 is no hit in one try: {rep}")
    t |> success(find(rep, "p4") < 0, "a depth never drafted stays out of the report: {rep}")
    mtp_pos_reset()
    let clean = mtp_pos_report()
    t |> success(find(clean, "p1") < 0, "reset clears every counter: {clean}")
}
