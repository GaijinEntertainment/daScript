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
//! k4/k5/k6/q40/iq4xs/k3/iq3s = superblock plane pairs (mixed PER TENSOR); q51 = native Q5_1 per-32
//! planes. New members append — the int value is the device stack tag and the image plane id.
enum KqFmt : uint8 {
    q8
    k4
    k5
    k6
    q40
    q51
    iq4xs
    k3
    iq3s
}

//! The superblock-lattice formats (Q8_K-form activations, % 256 rows, repack + stamped kq
//! kernels). q51 deliberately fails this: it rides per-32 planes with Q8_0-form activations —
//! a `fmt != KqFmt.q8` test does not imply the kq lattice; branch on kq_sb where it does.
def kq_sb(f : KqFmt) : bool => f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6 || f == KqFmt.q40 || f == KqFmt.iq4xs || f == KqFmt.k3 || f == KqFmt.iq3s

//! The same predicate over the int id space the GPU drivers carry per stack/plane.
def kq_sb(fmt : int) : bool => fmt == int(KqFmt.k4) || fmt == int(KqFmt.k5) || fmt == int(KqFmt.k6) || fmt == int(KqFmt.q40) || fmt == int(KqFmt.iq4xs) || fmt == int(KqFmt.k3) || fmt == int(KqFmt.iq3s)

//! KqFmt -> the kernel/IR format id space — the ONE bridge between the enum and the int ids the
//! generated kernels take as runtime params. Mnemonic ids: a K-quant is its bit width (3/4/5/6),
//! Q4_0 is 40, an i-quant is bit width x 10 + a variant digit (IQ4_XS = 44, IQ3_S = 33). q8/q51 panic.
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
    if (f == KqFmt.iq4xs) {
        return 44
    }
    if (f == KqFmt.k3) {
        return 3
    }
    if (f == KqFmt.iq3s) {
        return 33
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
let IQ4XS_QSB = 128l   // IQ4_XS quant plane: 128 nibble bytes, the disk k/k+16 pairing (q40 tiling)
let IQ4XS_SSB = 20l    // IQ4_XS scale plane: f16 d, 2 pad, 8 x int8 (ls - 32), 8 pad — the k4 decoded-row shape
let K3_QSB = 96l    // Q3_K quant plane: 64 qs bytes (four 2-bit lanes each) + 32 hmask bytes, verbatim disk order
let K3_SSB = 18l    // Q3_K scale plane: 16 x int8 (6-bit scale - 32, decoded at transcode) + f16 d — the k6 row
let IQ3S_QSB = 104l   // IQ3_S quant plane: [64 grid-index qs][8 qh ninth bits][32 sign bytes], verbatim disk order
let IQ3S_SSB = 20l    // IQ3_S scale plane: f16 d, 2 pad, 8 x int8 (1 + 2s), 8 pad — the iq4xs/k4 row shape

//! The IQ4_NL / IQ4_XS nibble codebook: weight = scale x IQ4NL_LUT[nibble] (ggml's kvalues_iq4nl).
//! Main-context use only (tests, oracles, the emitter's constant bake): a kernel that can run on a
//! forked worker reads a module global as zeros - kernels take `iq4nl_lut()` as a local instead.
let IQ4NL_LUT = fixed_array<int8>(-127, -104, -83, -65, -49, -35, -22, -10, 1, 13, 25, 38, 53, 69, 89, 113)

//! The same codebook as a per-call local (16 bytes on the caller's frame) - the form every
//! worker-run kernel reads.
def iq4nl_lut() : int8[16] => fixed_array<int8>(-127, -104, -83, -65, -49, -35, -22, -10, 1, 13, 25, 38, 53, 69, 89, 113)

//! The IQ3_S grid (ggml's iq3s_grid): entry qs | (qh bit << 8) packs FOUR weight magnitudes as
//! bytes; the per-element sign rides the block's own sign bytes. Per-call local (2 KB on the
//! caller's frame) - the form every worker-run kernel reads; IQ3S_GRID is the main-context copy.
def iq3s_grid() : uint[512] {
    return fixed_array<uint>(
        0x01010101u, 0x01010103u, 0x01010105u, 0x0101010bu, 0x0101010fu, 0x01010301u, 0x01010303u, 0x01010305u,
        0x01010309u, 0x0101030du, 0x01010501u, 0x01010503u, 0x0101050bu, 0x01010707u, 0x01010901u, 0x01010905u,
        0x0101090bu, 0x0101090fu, 0x01010b03u, 0x01010b07u, 0x01010d01u, 0x01010d05u, 0x01010f03u, 0x01010f09u,
        0x01010f0fu, 0x01030101u, 0x01030103u, 0x01030105u, 0x01030109u, 0x01030301u, 0x01030303u, 0x0103030bu,
        0x01030501u, 0x01030507u, 0x0103050fu, 0x01030703u, 0x0103070bu, 0x01030909u, 0x01030d03u, 0x01030d0bu,
        0x01030f05u, 0x01050101u, 0x01050103u, 0x0105010bu, 0x0105010fu, 0x01050301u, 0x01050307u, 0x0105030du,
        0x01050503u, 0x0105050bu, 0x01050701u, 0x01050709u, 0x01050905u, 0x0105090bu, 0x0105090fu, 0x01050b03u,
        0x01050b07u, 0x01050f01u, 0x01050f07u, 0x01070107u, 0x01070303u, 0x0107030bu, 0x01070501u, 0x01070505u,
        0x01070703u, 0x01070707u, 0x0107070du, 0x01070909u, 0x01070b01u, 0x01070b05u, 0x01070d0fu, 0x01070f03u,
        0x01070f0bu, 0x01090101u, 0x01090307u, 0x0109030fu, 0x01090503u, 0x01090509u, 0x01090705u, 0x01090901u,
        0x01090907u, 0x01090b03u, 0x01090f01u, 0x010b0105u, 0x010b0109u, 0x010b0501u, 0x010b0505u, 0x010b050du,
        0x010b0707u, 0x010b0903u, 0x010b090bu, 0x010b090fu, 0x010b0d0du, 0x010b0f07u, 0x010d010du, 0x010d0303u,
        0x010d0307u, 0x010d0703u, 0x010d0b05u, 0x010d0f03u, 0x010f0101u, 0x010f0105u, 0x010f0109u, 0x010f0501u,
        0x010f0505u, 0x010f050du, 0x010f0707u, 0x010f0b01u, 0x010f0b09u, 0x03010101u, 0x03010103u, 0x03010105u,
        0x03010109u, 0x03010301u, 0x03010303u, 0x03010307u, 0x0301030bu, 0x0301030fu, 0x03010501u, 0x03010505u,
        0x03010703u, 0x03010709u, 0x0301070du, 0x03010b09u, 0x03010b0du, 0x03010d03u, 0x03010f05u, 0x03030101u,
        0x03030103u, 0x03030107u, 0x0303010du, 0x03030301u, 0x03030309u, 0x03030503u, 0x03030701u, 0x03030707u,
        0x03030903u, 0x03030b01u, 0x03030b05u, 0x03030f01u, 0x03030f0du, 0x03050101u, 0x03050305u, 0x0305030bu,
        0x0305030fu, 0x03050501u, 0x03050509u, 0x03050705u, 0x03050901u, 0x03050907u, 0x03050b0bu, 0x03050d01u,
        0x03050f05u, 0x03070103u, 0x03070109u, 0x0307010fu, 0x03070301u, 0x03070307u, 0x03070503u, 0x0307050fu,
        0x03070701u, 0x03070709u, 0x03070903u, 0x03070d05u, 0x03070f01u, 0x03090107u, 0x0309010bu, 0x03090305u,
        0x03090309u, 0x03090703u, 0x03090707u, 0x03090905u, 0x0309090du, 0x03090b01u, 0x03090b09u, 0x030b0103u,
        0x030b0301u, 0x030b0307u, 0x030b0503u, 0x030b0701u, 0x030b0705u, 0x030b0b03u, 0x030d0501u, 0x030d0509u,
        0x030d050fu, 0x030d0909u, 0x030d090du, 0x030f0103u, 0x030f0107u, 0x030f0301u, 0x030f0305u, 0x030f0503u,
        0x030f070bu, 0x030f0903u, 0x030f0d05u, 0x030f0f01u, 0x05010101u, 0x05010103u, 0x05010107u, 0x0501010bu,
        0x0501010fu, 0x05010301u, 0x05010305u, 0x05010309u, 0x0501030du, 0x05010503u, 0x05010507u, 0x0501050fu,
        0x05010701u, 0x05010705u, 0x05010903u, 0x05010907u, 0x0501090bu, 0x05010b01u, 0x05010b05u, 0x05010d0fu,
        0x05010f01u, 0x05010f07u, 0x05010f0bu, 0x05030101u, 0x05030105u, 0x05030301u, 0x05030307u, 0x0503030fu,
        0x05030505u, 0x0503050bu, 0x05030703u, 0x05030709u, 0x05030905u, 0x05030b03u, 0x05050103u, 0x05050109u,
        0x0505010fu, 0x05050503u, 0x05050507u, 0x05050701u, 0x0505070fu, 0x05050903u, 0x05050b07u, 0x05050b0fu,
        0x05050f03u, 0x05050f09u, 0x05070101u, 0x05070105u, 0x0507010bu, 0x05070303u, 0x05070505u, 0x05070509u,
        0x05070703u, 0x05070707u, 0x05070905u, 0x05070b01u, 0x05070d0du, 0x05090103u, 0x0509010fu, 0x05090501u,
        0x05090507u, 0x05090705u, 0x0509070bu, 0x05090903u, 0x05090f05u, 0x05090f0bu, 0x050b0109u, 0x050b0303u,
        0x050b0505u, 0x050b070fu, 0x050b0901u, 0x050b0b07u, 0x050b0f01u, 0x050d0101u, 0x050d0105u, 0x050d010fu,
        0x050d0503u, 0x050d0b0bu, 0x050d0d03u, 0x050f010bu, 0x050f0303u, 0x050f050du, 0x050f0701u, 0x050f0907u,
        0x050f0b01u, 0x07010105u, 0x07010303u, 0x07010307u, 0x0701030bu, 0x0701030fu, 0x07010505u, 0x07010703u,
        0x07010707u, 0x0701070bu, 0x07010905u, 0x07010909u, 0x0701090fu, 0x07010b03u, 0x07010d07u, 0x07010f03u,
        0x07030103u, 0x07030107u, 0x0703010bu, 0x07030309u, 0x07030503u, 0x07030507u, 0x07030901u, 0x07030d01u,
        0x07030f05u, 0x07030f0du, 0x07050101u, 0x07050305u, 0x07050501u, 0x07050705u, 0x07050709u, 0x07050b01u,
        0x07070103u, 0x07070301u, 0x07070309u, 0x07070503u, 0x07070507u, 0x0707050fu, 0x07070701u, 0x07070903u,
        0x07070907u, 0x0707090fu, 0x07070b0bu, 0x07070f07u, 0x07090107u, 0x07090303u, 0x0709030du, 0x07090505u,
        0x07090703u, 0x07090b05u, 0x07090d01u, 0x07090d09u, 0x070b0103u, 0x070b0301u, 0x070b0305u, 0x070b050bu,
        0x070b0705u, 0x070b0909u, 0x070b0b0du, 0x070b0f07u, 0x070d030du, 0x070d0903u, 0x070f0103u, 0x070f0107u,
        0x070f0501u, 0x070f0505u, 0x070f070bu, 0x09010101u, 0x09010109u, 0x09010305u, 0x09010501u, 0x09010509u,
        0x0901050fu, 0x09010705u, 0x09010903u, 0x09010b01u, 0x09010f01u, 0x09030105u, 0x0903010fu, 0x09030303u,
        0x09030307u, 0x09030505u, 0x09030701u, 0x0903070bu, 0x09030907u, 0x09030b03u, 0x09030b0bu, 0x09050103u,
        0x09050107u, 0x09050301u, 0x0905030bu, 0x09050503u, 0x09050707u, 0x09050901u, 0x09050b0fu, 0x09050d05u,
        0x09050f01u, 0x09070109u, 0x09070303u, 0x09070307u, 0x09070501u, 0x09070505u, 0x09070703u, 0x0907070bu,
        0x09090101u, 0x09090105u, 0x09090509u, 0x0909070fu, 0x09090901u, 0x09090f03u, 0x090b010bu, 0x090b010fu,
        0x090b0503u, 0x090b0d05u, 0x090d0307u, 0x090d0709u, 0x090d0d01u, 0x090f0301u, 0x090f030bu, 0x090f0701u,
        0x090f0907u, 0x090f0b03u, 0x0b010105u, 0x0b010301u, 0x0b010309u, 0x0b010505u, 0x0b010901u, 0x0b010909u,
        0x0b01090fu, 0x0b010b05u, 0x0b010d0du, 0x0b010f09u, 0x0b030103u, 0x0b030107u, 0x0b03010bu, 0x0b030305u,
        0x0b030503u, 0x0b030705u, 0x0b030f05u, 0x0b050101u, 0x0b050303u, 0x0b050507u, 0x0b050701u, 0x0b05070du,
        0x0b050b07u, 0x0b070105u, 0x0b07010fu, 0x0b070301u, 0x0b07050fu, 0x0b070909u, 0x0b070b03u, 0x0b070d0bu,
        0x0b070f07u, 0x0b090103u, 0x0b090109u, 0x0b090501u, 0x0b090705u, 0x0b09090du, 0x0b0b0305u, 0x0b0b050du,
        0x0b0b0b03u, 0x0b0b0b07u, 0x0b0d0905u, 0x0b0f0105u, 0x0b0f0109u, 0x0b0f0505u, 0x0d010303u, 0x0d010307u,
        0x0d01030bu, 0x0d010703u, 0x0d010707u, 0x0d010d01u, 0x0d030101u, 0x0d030501u, 0x0d03050fu, 0x0d030d09u,
        0x0d050305u, 0x0d050709u, 0x0d050905u, 0x0d050b0bu, 0x0d050d05u, 0x0d050f01u, 0x0d070101u, 0x0d070309u,
        0x0d070503u, 0x0d070901u, 0x0d09050bu, 0x0d090907u, 0x0d090d05u, 0x0d0b0101u, 0x0d0b0107u, 0x0d0b0709u,
        0x0d0b0d01u, 0x0d0d010bu, 0x0d0d0901u, 0x0d0f0303u, 0x0d0f0307u, 0x0f010101u, 0x0f010109u, 0x0f01010fu,
        0x0f010501u, 0x0f010505u, 0x0f01070du, 0x0f010901u, 0x0f010b09u, 0x0f010d05u, 0x0f030105u, 0x0f030303u,
        0x0f030509u, 0x0f030907u, 0x0f03090bu, 0x0f050103u, 0x0f050109u, 0x0f050301u, 0x0f05030du, 0x0f050503u,
        0x0f050701u, 0x0f050b03u, 0x0f070105u, 0x0f070705u, 0x0f07070bu, 0x0f070b07u, 0x0f090103u, 0x0f09010bu,
        0x0f090307u, 0x0f090501u, 0x0f090b01u, 0x0f0b0505u, 0x0f0b0905u, 0x0f0d0105u, 0x0f0d0703u, 0x0f0f0101u)
}

//! Main-context use only (tests, oracles, the emitter's constant bake) - a forked worker reads
//! a module global as zeros; kernels take `iq3s_grid()` instead.
let IQ3S_GRID = iq3s_grid()

//! Quant-plane bytes per stride unit — the typed stride surface. The stride unit is one
//! 256-weight superblock for the kq lattice and one 32-weight block for q51 (see kq_elems).
//! q8 panics (its planes are byte-per-elem, not stride-walked here).
def kq_qsb(f : KqFmt) : int64 {
    if (f == KqFmt.k4 || f == KqFmt.q40 || f == KqFmt.iq4xs) {
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
    if (f == KqFmt.k3) {
        return K3_QSB
    }
    if (f == KqFmt.iq3s) {
        return IQ3S_QSB
    }
    panic("kq_qsb: no plane stride for this format")
    return 0l
}

//! Scale-plane bytes per stride unit (see kq_qsb for the unit).
def kq_ssb(f : KqFmt) : int64 {
    if (f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.iq4xs || f == KqFmt.iq3s) {
        return K4_SSB
    }
    if (f == KqFmt.k6 || f == KqFmt.k3) {
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
