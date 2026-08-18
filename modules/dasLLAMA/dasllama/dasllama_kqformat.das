options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_kqformat shared public

// The K-quant format taxonomy — ONE enum, ONE stride truth, ONE id space. This module requires
// nothing dasllama: everything format-shaped keys off it (convert reads codec strides, repack the
// layouts, layout/common the loader walks, gemm_schema the kernel side). Int format ids exist
// only at the IR/kernel-param boundary — cast there via kq_schema_id, never carry an int format
// across a module seam.

require dasllama/dasllama_lint public

//! Per-weight storage format under a q8-mode load with native planes: q8 = qblob/qscales;
//! k4/k5/k6/q40 = superblock plane pairs (mixed PER TENSOR); q51 = native Q5_1 per-32 planes.
enum KqFmt : uint8 {
    q8
    k4
    k5
    k6
    q40
    q51
}

//! The superblock-lattice formats (Q8_K-form activations, % 256 rows, repack + stamped kq
//! kernels). q51 deliberately fails this: it rides per-32 planes with Q8_0-form activations —
//! a `fmt != KqFmt.q8` test does not imply the kq lattice; branch on kq_sb where it does.
def kq_sb(f : KqFmt) : bool => f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6 || f == KqFmt.q40

//! The same predicate over the int id space the GPU drivers carry per stack/plane.
def kq_sb(fmt : int) : bool => fmt == int(KqFmt.k4) || fmt == int(KqFmt.k5) || fmt == int(KqFmt.k6) || fmt == int(KqFmt.q40)

//! KqFmt -> the kernel/IR format id space (4/5/6/40) — the ONE bridge between the enum and the
//! int ids the generated kernels take as runtime params. q8/q51 are not kq superblock
//! formats and panic.
def kq_schema_id(f : KqFmt) : int {
    if (f == KqFmt.k4) {
        return 4
    }
    if (f == KqFmt.k5) {
        return 5
    }
    if (f == KqFmt.k6) {
        return 6
    }
    if (f == KqFmt.q40) {
        return 40
    }
    panic("kq_schema_id: not a kq superblock format")
    return 0
}

// ===== plane stride truth (per 256-weight superblock row unless noted) =====

let K4_QSB = 128l   // Q4_K quant plane: 128 nibble bytes
let K4_SSB = 20l    // Q4_K scale plane: 16B disk block + 4B pad, decoded in place at repack
let K5_QSB = 160l   // Q5_K quant plane: 128 nibble bytes + 32 qh bytes
let K5_SSB = 20l    // Q5_K scale plane: same 20B row as k4
let K6_QSB = 192l   // Q6_K quant plane: 128 ql + 64 qh bytes
let K6_SSB = 18l    // Q6_K scale plane: 16 int8 sub-scales + f16 d, native
let Q40_QSB = 128l  // Q4_0 quant plane: 128 nibble bytes (k4 tiling)
let Q40_SSB = 16l   // Q4_0 scale plane: 8 x f16 d, verbatim
let Q51_QB = 20l    // q5_1 quant plane PER 32-BLOCK: 16 nibble bytes (k/k+16 pairing) + 4 qh
let Q51_SB = 4l     // q5_1 scale plane PER 32-BLOCK: f16 d + f16 m

//! Quant-plane bytes per stride unit — the typed stride surface. The stride unit is one
//! 256-weight superblock for the kq lattice and one 32-weight block for q51 (see kq_elems).
//! q8 panics (its planes are byte-per-elem, not stride-walked here).
def kq_qsb(f : KqFmt) : int64 {
    if (f == KqFmt.k4 || f == KqFmt.q40) {
        return K4_QSB
    }
    if (f == KqFmt.k5) {
        return K5_QSB
    }
    if (f == KqFmt.k6) {
        return K6_QSB
    }
    if (f == KqFmt.q51) {
        return Q51_QB
    }
    panic("kq_qsb: no plane stride for this format")
    return 0l
}

//! Scale-plane bytes per stride unit (see kq_qsb for the unit).
def kq_ssb(f : KqFmt) : int64 {
    if (f == KqFmt.k4 || f == KqFmt.k5) {
        return K4_SSB
    }
    if (f == KqFmt.k6) {
        return K6_SSB
    }
    if (f == KqFmt.q40) {
        return Q40_SSB
    }
    if (f == KqFmt.q51) {
        return Q51_SB
    }
    panic("kq_ssb: no plane stride for this format")
    return 0l
}

//! Weights per stride unit: the kq lattice strides per 256-weight superblock; q8/q51 per
//! 32-weight block.
def kq_elems(f : KqFmt) : int64 => kq_sb(f) ? 256l : 32l

//! The loader's stream/repack region tag for a format (stream_repack_one / push_repack codes).
def kq_stream_code(f : KqFmt) : int {
    if (f == KqFmt.q8) {
        return 0
    }
    if (f == KqFmt.q51) {
        return 2
    }
    return kq_schema_id(f)
}
