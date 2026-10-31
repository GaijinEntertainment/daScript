options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gemm_schema shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_kqformat public
require daslib/enum_trait   // the format taxonomy + typed stride truth these int-id forms delegate to

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
def q8q8_repack_type(mr : int; wbias : int = 0; kgroup : int = 4) : Q8RepackType =>
    Q8RepackType(interleave = mr, f32_scales = true, wbias = wbias, kgroup = kgroup)

//! The mr of the REFERENCE tier's layout (q8q8_repack_type(4), the old hand arm64-laneq layout
//! the reference stub bodies read). Non-reference mr runs the generated grp<mr> kernel/layout pair,
//! stamped from one manifest entry so both decline in lockstep (M4).
def GEMM_REFERENCE_MR : int => 4

//! GPU-tier device-plane shape of the Q8 rail: 32-weight blocks (the alias of KQ_B32_ELEMS), 32B
//! quants + one f16 scale halfword per block. Every q8 plane sizing (gathers, arena/stack math)
//! derives from these.
let Q8_BLOCK_ELEMS = KQ_B32_ELEMS
let Q8_BLOCK_BYTES = Q8_BLOCK_ELEMS + 2l   //! a block's bytes: its quants and the f16 scale
let Q8_QPB = 32l   // quant bytes per block
let Q8_SPB = 2l    // scale bytes per block (f16 halfword)

//! kq superblock element count (the alias of KQ_SB_ELEMS), and the DEVICE scale-row stride the
//! gathers decode a kq format's scales to (kq_ssb below is the DISK/plane stride, per format);
//! kq_dev_ssb is the per-format form.
let KQ_SUPERBLOCK_ELEMS = KQ_SB_ELEMS
let KQ_DEV_SSB = 20l

//! The codebook formats whose device scale row is two words: word 0 = f16 d with the format's
//! native high bits above it, word 1 = eight 4-bit fields.
def kq_two_word_row(f : KqFmt) : bool => f == KqFmt.iq4xs || f == KqFmt.iq3s || f == KqFmt.iq3xxs || f == KqFmt.iq2xxs || f == KqFmt.iq3s4

//! The device scale-row stride of a kq format: 8 B for the two-word-row formats, KQ_DEV_SSB for the rest.
def kq_dev_ssb(f : KqFmt) : int64 => kq_two_word_row(f) ? 8l : KQ_DEV_SSB

//! kq_dev_ssb over the tier's int(KqFmt) stack-format space.
def kq_dev_ssb_i(fmt : int) : int64 {
    for (f in type<KqFmt>) {
        return kq_dev_ssb(f) if (int(f) == fmt)
    }
    return KQ_DEV_SSB
}

//! the device layout's block strides of a format - the quant bytes and the scale bytes a block, and the block's elements: q8 a
//! 32-wide block of int8 under one f16 scale, a per-32 or mx4 format its CPU planes verbatim, a superblock format its quants
//! under the device's scale row
def dev_block_bytes(f : KqFmt) : tuple<qsb : int64; ssb : int64; belems : int64> {
    return (qsb = 32l, ssb = 2l, belems = 32l) if (f == KqFmt.q8)
    return (qsb = kq_qsb(f), ssb = kq_dev_ssb(f), belems = 256l) if (kq_sb(f))
    return (qsb = kq_qsb(f), ssb = kq_ssb(f), belems = 32l)
}

//! the device-layout bytes of `rows` rows of `n` elements of format `f`: the quant plane and the scale plane
def dev_rows_bytes(f : KqFmt; n, rows : int64) : tuple<q : int64; s : int64> {
    let b = dev_block_bytes(f)
    let blocks = n / b.belems * rows
    return (q = blocks * b.qsb, s = blocks * b.ssb)
}

//! q5_1 per-32-block plane strides: 16B nibbles + 4B qh quants, f16 d + f16 m scales.
let Q51_QPB = 20l
let Q51_SPB = 4l

//! Quant-plane bytes per stride unit for a kernel/IR format id (`kq_schema_id`'s space).
def kq_qsb(fmt : int) : int64 => kq_qsb(kq_fmt_of_id(fmt, "kq_qsb"))

//! Scale-plane bytes per stride unit for a kernel/IR format id (see kq_qsb).
def kq_ssb(fmt : int) : int64 => kq_ssb(kq_fmt_of_id(fmt, "kq_ssb"))

//! True for the kq ids whose tile reads the packed quant plane directly; the others (k5/k6 and
//! the grid formats) read a byte-expanded panel unpacked once per (group, token-block).
def kq_reads_packed_planes(fmt : int64) : bool =>
    fmt == 4l || fmt == 40l || fmt == 44l || fmt == 3l || fmt == 45l || fmt == 2l || fmt == 35l || fmt == 49l

//! the kq ids whose block body decodes a codebook grid (iq3s, iq3xxs, iq2s, iq2xs, iq2xxs)
def kq_is_grid(fmt : int) : bool => fmt == 33 || fmt == 34 || fmt == 23 || fmt == 24 || fmt == 25

//! the kq ids whose nibbles pass a 16-entry codebook (iq4xs, iq4nl, iq3s4, nv4)
def kq_is_lut(fmt : int) : bool => fmt == 44 || fmt == 45 || fmt == 35 || fmt == 49

//! the kq ids whose tile reads nibble planes (k4, k5, k6, q40 and the codebook formats)
def panel_is_nibble(fmt : int) : bool => fmt == 4 || fmt == 5 || fmt == 6 || fmt == 40 || kq_is_lut(fmt)

//! True for the kq ids `dasllama_gemm_gen` carries a block body for: their generators and every
//! companion stamp a perm together. Any other member's stubs decline on every rail, the layout
//! companion included, so its planes stay at the reference interleave the reference bodies read.
def kq_has_emitter(fmt : int) : bool => panel_is_nibble(fmt) || kq_is_grid(fmt) || fmt == 3 || fmt == 2

//! eight +-1 bytes for sign byte `sb`, lane k negative when bit k is set
def pm1_of(sb : int) : uint64 {
    var v = 0ul
    for (k in range(8)) {
        v |= uint64(((sb >> k) & 1) != 0 ? 0xFF : 0x01) << uint64(8 * k)
    }
    return v
}

//! the grid's symbol alphabet: the halved iq3xxs magnitudes, iq3s's odd magnitudes, the iq2 family's three
def vbmi_alphabet(kq : int) : array<int> {
    if (kq == 34) return <- [2, 6, 10, 14, 18, 22, 26, 31]
    if (kq == 33) return <- [1, 3, 5, 7, 9, 11, 13, 15]
    return <- [8, 25, 43]
}

//! a grid word's low `nbytes` weight bytes as packed symbols (weight t at bits t*bits); a byte outside the alphabet is a broken table
def vbmi_pack_word(word : uint; alpha : array<int>; bits, nbytes : int) : int {
    var code = 0
    for (t in range(nbytes)) {
        let wb = int((word >> uint(8 * t)) & 0xFFu)
        let si = alpha |> find_index(wb)
        if (si < 0) {
            panic("vbmi lattice: grid byte {wb} is outside the alphabet")
        }
        code |= si << (bits * t)
    }
    return code
}
