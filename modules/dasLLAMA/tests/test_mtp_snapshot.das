options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common   // Session, mtp_state_snapshot / mtp_state_restore

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
    s.dn_pos = 17l
    mtp_state_snapshot(s)
    t |> equal(long_length(s.mtp_snap_state), ns, "the state snapshot holds every float")
    t |> equal(long_length(s.mtp_snap_conv), nc, "the conv snapshot holds every float")
    t |> equal(s.mtp_snap_pos, 17l, "the position rides along")
    s.dn_state[ns - 1l] = -9.0
    s.dn_pos = 18l
    mtp_state_restore(s)
    t |> equal(s.dn_state[ns - 1l], 1.5, "restore puts the snapshot back")
    t |> equal(s.dn_pos, 17l, "restore rewinds the position")
    delete s
}
