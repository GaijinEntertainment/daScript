options gen2
options indenting = 4
options _dasllama_internal = true

// An oracle in the block drafter's seat: a round's drafts are a known sequence's next ids, so every verify's rows are
// forced tokens the probe compares against its own walk.

module _forced_drafter shared public

require math
require dasllama/dasllama_common   // the block drafter's seams

var g_forced_seq : array<int64>   //! the forced sequence: g_forced_seq[i] is the id at position g_forced_base + i
var g_forced_base = 0l            //! the position of the sequence's first id
var g_forced_drafted = 0l         //! the drafts the forced drafter handed the round in flight

[unused_argument(t)]
def private forced_attached(t : Model) : bool => true

[unused_argument(s, l, rows, pos0, window)]
def private forced_captured(var s : Session; l, rows, pos0 : int64; window : bool) {}

[unused_argument(s, img, rows, pos0)]
def private forced_captured_rows(var s : Session; img : array<float>; rows, pos0 : int64) {}

//! the round's drafts are the forced sequence's next `k` ids after the anchor at `pos`
[unused_argument(t, tok)]
def private forced_draft(t : Model; var s : Session; tok, pos, k : int64) : int64 {
    let at = pos - g_forced_base
    let n = clamp(long_length(g_forced_seq) - 1l - at, 0l, k)
    for (j in range64(1l, n + 1l)) {
        s.mtp_vbatch[j] = g_forced_seq[at + j]
    }
    g_forced_drafted = n
    return n
}

[unused_argument(s)]
def private forced_land(var s : Session) {}

[unused_argument(s, pos0, rows)]
def private forced_commit(var s : Session; pos0, rows : int64) {}

//! the forced drafter registered in the block drafter's seat, capturing `layers`, `drafts` a round at the most
def attach_forced_drafter(layers : array<int64>; drafts : int64) {
    var ls := layers
    register_mtp_block_drafter(MtpBlockSeams(attached = @@forced_attached, captured = @@forced_captured, captured_rows = @@forced_captured_rows,
        draft = @@forced_draft, land = @@forced_land, commit = @@forced_commit, layers <- ls, drafts = drafts))
}
