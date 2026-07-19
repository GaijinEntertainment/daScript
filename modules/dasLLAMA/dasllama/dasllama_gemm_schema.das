options gen2
options _comment_hygiene = true

module dasllama_gemm_schema shared public

// The one place the Q8 interleaved-weight layout is defined (gemm_generator_plan.md part 3):
// the [llvm_code] tile generator derives its strides from q8q8_repack_type at emit time, and
// the load-time repack implements the same descriptor — one source, two callers, no desync.
// Pure das, no dependencies: required by both the llvm-jit-context generator module
// (dasllama_gemm_gen) and the runtime kernel module (dasllama_math_gen).

//! Layout descriptor for an mr-row register-tile repack.
struct Q8RepackType {
    //! rows interleaved per dword group (= the tile's mr)
    interleave : int
    //! scales pre-converted to f32 at repack time and interleaved per group (the wscale_f16
    //! rail converts the BUILT plane to raw binary16 in place afterwards, order-preserving —
    //! the *_s16 kernel twins read that halfword view of this same layout)
    f32_scales : bool
    //! additive bias baked into every GROUP-row weight byte (`w ^ 0x80` when 128 — the
    //! vpdpbusd bias128 perms; 0 = plain s8). Row-major tail rows always stay unbiased.
    wbias : int
    //! k-bytes per row per interleave group: 4 = dword-group layout ([kg4][r][4b]); 8 = smmla
    //! row-pair layout ([kg8][r][8b] — a row PAIR's 8 k-bytes form one 16-byte MMA operand).
    //! Tail rows stay row-major at any kgroup.
    kgroup : int
}

//! The repack layout an mr-row tile perm requires: mr-row block interleave with f32 group
//! scales, group bytes biased by `wbias`, `kgroup` k-bytes per row per group. `needs_repack`
//! and every kernel stride derive from this.
def q8q8_repack_type(mr : int; wbias : int = 0; kgroup : int = 4) : Q8RepackType {
    return Q8RepackType(interleave = mr, f32_scales = true, wbias = wbias, kgroup = kgroup)
}

//! The mr of the REFERENCE tier's layout (q8q8_repack_type(4), the old hand arm64-laneq layout
//! the reference stub bodies read). Non-reference mr runs the generated grp<mr> kernel/layout pair,
//! stamped from one manifest entry so both decline in lockstep (M4).
let GEMM_REFERENCE_MR = 4

//! Quant-plane bytes per 256-weight superblock per row for a kq format id (4/5/6 = Q4_K/Q5_K/
//! Q6_K, 40 = Q4_0). The ONE stride source for every fmt-branched kq walker/kernel/repack —
//! an unknown id panics instead of silently walking another format's stride.
def kq_qsb(fmt : int) : int64 {
    if (fmt == 4 || fmt == 40) return 128l
    if (fmt == 5) return 160l
    if (fmt == 6) return 192l
    panic("kq_qsb: unknown kq format id {fmt}")
    return 0l
}

//! Scale-plane bytes per 256-weight superblock per row (see kq_qsb): k4/k5 20 (16B disk block
//! + 4B pad, decoded in place at repack), k6 18 (native), q40 16 (8 x f16 d, verbatim).
def kq_ssb(fmt : int) : int64 {
    if (fmt == 4 || fmt == 5) return 20l
    if (fmt == 6) return 18l
    if (fmt == 40) return 16l
    panic("kq_ssb: unknown kq format id {fmt}")
    return 0l
}
