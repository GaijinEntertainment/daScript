options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_repack shared public

require dasllama/dasllama_kqformat   // iq3s_grid - the panel unpack gathers through the codebook
require dasllama/dasllama_lint public
require dasllama/dasllama_math_default  // k4_sc_mn — the kq 6-bit sc/mn decoder (stays there: hot in the kq dots, auto-inline is same-module-only)

// Disk-order -> compute-order kernel data repacking, ALL weight formats, platform-neutral.
// Each repack rearranges one 2D tensor IN PLACE into the grp<mr> interleave the stamped CPU
// row cores read; GPU backends never repack (they gather device planes in dasllama_layout).
// Number sources stay with their owners: dasllama_math_gen's `_gen` twins feed stamped
// companion numbers, dasllama_math's `_bake` selectors feed cross-box override numbers —
// this module is the pure transform, parameterized by (mr, wbias, kgroup).

// ===== disk-order bit extractors (the K-quant on-disk packing) =====

//! low 4 bits of weight k off a disk-order k4/k5 nibble row (128 bytes per superblock)
def k45_nib(rowq : uint8 const?; k : int64) : uint {
    let js = k / 64l
    let rem = k % 64l
    let b = uint(unsafe(rowq[js * 32l + rem % 32l]))
    return rem < 32l ? b & 15u : b >> 4u
}

//! the 5th bit of weight k off a disk-order k5 qh row (32 bytes per superblock)
def k5_hbit(rowh : uint8 const?; k : int64) : uint {
    let js = int(k / 64l)
    let rem = k % 64l
    return (uint(unsafe(rowh[rem % 32l])) >> uint(2 * js + (rem < 32l ? 0 : 1))) & 1u
}

//! low 4 bits of weight k off a disk-order k6 ql row (128 bytes per superblock)
def k6_nib(rowq : uint8 const?; k : int64) : uint {
    let h = k / 128l
    let g = (k % 128l) / 32l
    let b = uint(unsafe(rowq[h * 64l + (g % 2l) * 32l + k % 32l]))
    return g < 2l ? b & 15u : b >> 4u
}

struct private RepackField {
    src : int64     //!< byte offset of the field in the unit's disk-order scale row
    width : int64   //!< bytes per element
    count : int64   //!< elements
}

//! Runs `blk` over a copy of the two planes: the interleave rewrites every byte of a group region
//! in place, so rows read from the copy while their slots are written.
def private repack_in_place(var kq : uint8?; var ks : uint8?; qbytes, sbytes : int64; blk : block<(tqp : uint8 const?; tsp : uint8 const?) : void>) {
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> reserve(qbytes)
    tq |> resize(qbytes)
    ts |> reserve(sbytes)
    ts |> resize(sbytes)
    unsafe {
        memcpy(addr(tq[0]), kq, qbytes)
        memcpy(addr(ts[0]), ks, sbytes)
        invoke(blk, addr<uint8 const?>(tq[0]), addr<uint8 const?>(ts[0]))
    }
    delete tq
    delete ts
}

//! The quant-plane interleave of a unit made of `colw`-byte columns: column c of unit u in row r
//! lands at [g][u][c][r][colw] - the form the column kernels read, mr rows' columns adjacent.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#grp-repack-interleaves")]
def private repack_columns(var dst : uint8?; src : uint8 const?; nunits, ubytes, colw, d, mr : int64) {
    let row = nunits * ubytes
    let cols = ubytes / colw
    unsafe {
        for (g in range64(d / mr)) {
            for (u in range64(nunits)) {
                let db = g * mr * row + u * ubytes * mr
                for (r in range64(mr)) {
                    let sb = (g * mr + r) * row + u * ubytes
                    if (colw == 4l) {
                        var d4 = reinterpret<uint?>(dst + db)
                        let s4 = reinterpret<uint const?>(src + sb)
                        for (c in range64(cols)) {
                            d4[c * mr + r] = s4[c]
                        }
                    } elif (colw == 8l) {
                        var d8 = reinterpret<uint64?>(dst + db)
                        let s8 = reinterpret<uint64 const?>(src + sb)
                        for (c in range64(cols)) {
                            d8[c * mr + r] = s8[c]
                        }
                    } else {
                        for (c in range64(cols)) {
                            for (t in range64(colw)) {
                                dst[db + (c * mr + r) * colw + t] = src[sb + c * colw + t]
                            }
                        }
                    }
                }
            }
        }
    }
}

//! The scale-plane interleave, field-major: each field of the unit's disk-order row lands as
//! [g][u][field][i][r][width], the fields in destination order with their source offsets.
def private repack_fields(var dst : uint8?; src : uint8 const?; nunits, ssb, d, mr : int64; fields : array<RepackField>) {
    let srow = nunits * ssb
    unsafe {
        for (g in range64(d / mr)) {
            for (u in range64(nunits)) {
                var fb = g * mr * srow + u * ssb * mr
                for (f in fields) {
                    for (r in range64(mr)) {
                        let sb = (g * mr + r) * srow + u * ssb + f.src
                        if (f.width == 1l) {
                            for (i in range64(f.count)) {
                                dst[fb + i * mr + r] = src[sb + i]
                            }
                        } elif (f.width == 2l) {
                            var d2 = reinterpret<uint16?>(dst + fb)
                            let s2 = reinterpret<uint16 const?>(src + sb)
                            for (i in range64(f.count)) {
                                d2[i * mr + r] = s2[i]
                            }
                        } else {
                            for (i in range64(f.count)) {
                                for (b in range64(f.width)) {
                                    dst[fb + (i * mr + r) * f.width + b] = src[sb + i * f.width + b]
                                }
                            }
                        }
                    }
                    fb += f.width * f.count * mr
                }
            }
        }
    }
}

//! the k4 row: [f16 d x mr][f16 dmin (or pad) x mr][8 sub-scales x mr][8 mins (or pad) x mr]
def private k4_row_fields() : array<RepackField> => [RepackField(src = 0l, width = 2l, count = 1l), RepackField(src = 2l, width = 2l, count = 1l), RepackField(src = 4l, width = 1l, count = 16l)]

// the k6 row: [16 int8 sub-scales x mr][f16 d x mr] - k6 and k3
def private k6_row_fields() : array<RepackField> => [RepackField(src = 0l, width = 1l, count = 16l), RepackField(src = 16l, width = 2l, count = 1l)]

def private verbatim_unit_field(width, count : int64) : array<RepackField> => [RepackField(src = 0l, width = width, count = count)]

//! One weight tensor's planes ([d rows x n cols] at the pointers) into the grp<mr> interleave the
//! format's kernels read, in place - the format's `repack_grp` overload through the stamp.
def repack_kq_grp(fmt : KqFmt; var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    kq_fmt_stamp(fmt) $(F) {
        repack_grp(kq, ks, n, d, mr, F)
    }
}

def repack_grp(var _kq : uint8?; var _ks : uint8?; _n, _d, _mr : int64; _f : KqTag_q8) {
    panic("q8 planes repack through repack_q8q8_grp (int8 quants, f32 scales, the bias and k-group axes)")
}

//! mr-generalized Q8 interleave repack — the q8q8_repack_type(mr) layout for any mr (mr=4
//! reproduces the old hand arm64-laneq interleave byte-for-byte). Group bytes biased by wbias
//! (bias128 stamps bake w^0x80); tail rows stay row-major AND unbiased.
def repack_q8q8_grp(var wp : int8?; var sp : float?; n, d, mr : int64; wbias : int64 = 0l; kgroup : int64 = 4l) {
    // a zero here means the caller read layout numbers a lane context cannot see — name the
    // failure instead of a bare div0
    if (mr <= 0l || kgroup <= 0l) {
        panic("repack_q8q8_grp: mr={mr} kgroup={kgroup} - layout numbers unreadable (pool-lane context globals?)")
    }
    let nb = n / 32l
    unsafe {
        var wb = reinterpret<uint8?>(wp)
        var sb = reinterpret<uint8?>(sp)
        repack_in_place(wb, sb, d * n, d * nb * 4l) $(twp, tsp) {
            repack_columns(wb, twp, 1l, n, kgroup, d, mr)
            repack_fields(sb, tsp, nb, 4l, d, mr, verbatim_unit_field(4l, 1l))
        }
        if (wbias != 0l) {
            // bias128: one pass over the GROUP region only (tails stay plain s8);
            // int8 truncation makes +128 the byte xor 0x80
            for (i in range64((d / mr) * mr * n)) {
                wp[i] = int8(int(wp[i]) + int(wbias))
            }
        }
    }
}

//! mr-generalized MXFP4 plane repack (mr=4 reproduces the old hand arm64-laneq mx interleave
//! byte-for-byte): the nibble bytes as four-byte columns per block, mapping byte-for-byte onto
//! the grp<mr> Q8 order (see expand_mx4_region_q8); the e8m0 scale one byte per block.
def repack_grp(var np : uint8?; var ep : uint8?; n, d, mr : int64; _f : KqTag_mx4) {
    let nbb = n / 32l
    repack_in_place(np, ep, d * nbb * 16l, d * nbb) $(tnp, tep) {
        repack_columns(np, tnp, nbb, 16l, 4l, d, mr)
        repack_fields(ep, tep, nbb, 1l, d, mr, verbatim_unit_field(1l, 1l))
    }
}

//! mr-generalized q51 plane repack: per block the 20 quant bytes as five four-byte columns (the
//! nibble dwords in mx4 order, then the qh dword), the 4-byte f16 (d, m) scale per block.
//! grp1 == disk order.
def repack_grp(var qp : uint8?; var sp : uint8?; n, d, mr : int64; _f : KqTag_q51) {
    let nb = n / 32l
    repack_in_place(qp, sp, d * nb * Q51_QB, d * nb * Q51_SB) $(tqp, tsp) {
        repack_columns(qp, tqp, nb, Q51_QB, 4l, d, mr)
        repack_fields(sp, tsp, nb, Q51_SB, d, mr, verbatim_unit_field(Q51_SB, 1l))
    }
}

//! the k4/k5 scale row: d and dmin verbatim, then the 12 packed 6-bit sc/mn bytes DECODED into the two 8-strip fields
def private repack_k45_scales(var ks : uint8?; tsp : uint8 const?; nsb, d, mr : int64) {
    repack_fields(ks, tsp, nsb, K4_SSB, d, mr, [RepackField(src = 0l, width = 2l, count = 1l), RepackField(src = 2l, width = 2l, count = 1l)])
    let srow = nsb * K4_SSB
    unsafe {
        for (g in range64(d / mr)) {
            for (sbi in range64(nsb)) {
                let ds = g * mr * srow + sbi * K4_SSB * mr
                for (r in range64(mr)) {
                    let ss = (g * mr + r) * srow + sbi * K4_SSB
                    for (blk in range64(8l)) {
                        let sm = k4_sc_mn(tsp + ss + 4l, blk)
                        ks[ds + 4l * mr + blk * mr + r] = uint8(sm.x)
                        ks[ds + 12l * mr + blk * mr + r] = uint8(sm.y)
                    }
                }
            }
        }
    }
}

// the k4/k5 nibble re-pairing: the disk k/k+32 pairs become the kernels' k/k+16 pairs, four-byte
// columns of (blk, j); k5 also packs the two high bits per column into its qh plane
def private repack_k45_quants(var kq : uint8?; tqp : uint8 const?; nsb, d, mr : int64; qsb : int64) {
    let qrow = nsb * qsb
    unsafe {
        for (g in range64(d / mr)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * qsb * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * qsb
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            var hb = 0u
                            for (t in range64(4l)) {
                                let k = blk * 32l + j * 4l + t
                                let lo = k45_nib(tqp + sq, k)
                                let hi = k45_nib(tqp + sq, k + 16l)
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = uint8(lo | (hi << 4u))
                                if (qsb == K5_QSB) {
                                    hb |= k5_hbit(tqp + sq + 128l, k) << uint(t)
                                    hb |= k5_hbit(tqp + sq + 128l, k + 16l) << uint(4l + t)
                                }
                            }
                            if (qsb == K5_QSB) {
                                kq[dq + 128l * mr + (blk * 4l + j) * mr + r] = uint8(hb)
                            }
                        }
                    }
                }
            }
        }
    }
}

//! mr-generalized K-quant plane repack, one overload per format. k4/k5 (kq v2) also DECODE the
//! 6-bit scale packing into the u8 sc/mn planes — the padded-disk 20B rows and the decoded
//! 20B/row grp regions share one stride.
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_k4) {
    let nsb = n / 256l
    repack_in_place(kq, ks, d * nsb * K4_QSB, d * nsb * K4_SSB) $(tqp, tsp) {
        repack_k45_quants(kq, tqp, nsb, d, mr, K4_QSB)
        repack_k45_scales(ks, tsp, nsb, d, mr)
    }
}

def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_k5) {
    let nsb = n / 256l
    repack_in_place(kq, ks, d * nsb * K5_QSB, d * nsb * K5_SSB) $(tqp, tsp) {
        repack_k45_quants(kq, tqp, nsb, d, mr, K5_QSB)
        repack_k45_scales(ks, tsp, nsb, d, mr)
    }
}

//! k6: the disk ql nibbles re-paired into four-byte columns of (blk, j), the qh two-bit tops
//! re-packed per sub-block into the high plane; the k6 scale row verbatim.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#kq-subblock-planes")]
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_k6) {
    let nsb = n / 256l
    repack_in_place(kq, ks, d * nsb * K6_QSB, d * nsb * K6_SSB) $(tqp, tsp) {
        repack_k6_quants(kq, tqp, nsb, d, mr)
        repack_fields(ks, tsp, nsb, K6_SSB, d, mr, k6_row_fields())
    }
}

//! a hand loop takes its operands as arguments: a block body reloads its captures through the frame after every byte store, measurable on a plane-sized loop
def private repack_k6_quants(var kq : uint8?; tqp : uint8 const?; nsb, d, mr : int64) {
    let qrow = nsb * K6_QSB
    unsafe {
        for (g in range64(d / mr)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * K6_QSB * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * K6_QSB
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            for (t in range64(4l)) {
                                let k = blk * 32l + j * 4l + t
                                let lo = k6_nib(tqp + sq, k)
                                let hi = k6_nib(tqp + sq, k + 16l)
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = uint8(lo | (hi << 4u))
                            }
                        }
                    }
                    for (blk in range64(8l)) {   // qh re-packed per sub-block
                        for (half in range64(2l)) {
                            for (t in range64(4l)) {
                                var qhbyte = 0u
                                for (j in range64(4l)) {
                                    let l = (blk / 4l) * 32l + half * 16l + j * 4l + t
                                    qhbyte |= ((uint(tqp[sq + 128l + l]) >> uint(2l * (blk % 4l))) & 3u) << uint(2l * j)
                                }
                                kq[dq + 128l * mr + ((blk * 2l + half) * mr + r) * 4l + t] = uint8(qhbyte)
                            }
                        }
                    }
                }
            }
        }
    }
}

//! q40: the disk bytes already carry the k / k+16 nibble pairing, so the quants are a pure
//! column gather; scales interleave per block: [blk 0..7][mr x f16 d].
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_q40) {
    let nsb = n / 256l
    repack_in_place(kq, ks, d * nsb * Q40_QSB, d * nsb * Q40_SSB) $(tqp, tsp) {
        repack_columns(kq, tqp, nsb, Q40_QSB, 4l, d, mr)
        repack_fields(ks, tsp, nsb, Q40_SSB, d, mr, verbatim_unit_field(2l, 8l))
    }
}

//! iq4nl: q40's plane shapes byte for byte.
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_iq4nl) {
    repack_grp(kq, ks, n, d, mr, KqTag_q40())
}

//! iq4xs: the q40 quant gather (the disk k/k+16 pairing IS the k4 tiling) and the k4 row over
//! the DECODED 20B scale row (its pad bytes are zero on the plane, so verbatim lands them).
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_iq4xs) {
    let nsb = n / 256l
    repack_in_place(kq, ks, d * nsb * IQ4XS_QSB, d * nsb * IQ4XS_SSB) $(tqp, tsp) {
        repack_columns(kq, tqp, nsb, IQ4XS_QSB, 4l, d, mr)
        repack_fields(ks, tsp, nsb, IQ4XS_SSB, d, mr, k4_row_fields())
    }
}

//! k3: qs and hmask re-packed per sub-block into four-byte columns (the 2-bit lanes and the
//! high bits gathered per column); scales interleave as k6's row ([16 x mr int8][mr x f16 d]).
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#kq-subblock-planes")]
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_k3) {
    let nsb = n / 256l
    repack_in_place(kq, ks, d * nsb * K3_QSB, d * nsb * K3_SSB) $(tqp, tsp) {
        repack_k3_quants(kq, tqp, nsb, d, mr)
        repack_fields(ks, tsp, nsb, K3_SSB, d, mr, k6_row_fields())
    }
}

def private repack_k3_quants(var kq : uint8?; tqp : uint8 const?; nsb, d, mr : int64) {
    let qrow = nsb * K3_QSB
    unsafe {
        for (g in range64(d / mr)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * K3_QSB * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * K3_QSB
                    for (blk in range64(8l)) {   // qs and hmask re-packed per sub-block
                        for (half in range64(2l)) {
                            for (t in range64(4l)) {
                                var qbyte = 0u
                                for (j in range64(4l)) {
                                    let l = (blk / 4l) * 32l + half * 16l + j * 4l + t
                                    qbyte |= ((uint(tqp[sq + l]) >> uint(2l * (blk % 4l))) & 3u) << uint(2l * j)
                                }
                                kq[dq + ((blk * 2l + half) * mr + r) * 4l + t] = uint8(qbyte)
                            }
                        }
                        for (t in range64(4l)) {
                            var hbyte = 0u
                            for (sidx in range64(8l)) {
                                hbyte |= ((uint(tqp[sq + 64l + sidx * 4l + t]) >> uint(blk)) & 1u) << uint(sidx)
                            }
                            kq[dq + 64l * mr + (blk * mr + r) * 4l + t] = uint8(hbyte)
                        }
                    }
                }
            }
        }
    }
}

// the column-form formats: the whole quant row as four-byte columns (qs, qh, sign, aux and grid
// bytes stay in place per column - the kernels compose per block), the k4 row for the scales
def private repack_column_form(var kq : uint8?; var ks : uint8?; n, d, mr, qsb, ssb : int64) {
    let nsb = n / 256l
    repack_in_place(kq, ks, d * nsb * qsb, d * nsb * ssb) $(tqp, tsp) {
        repack_columns(kq, tqp, nsb, qsb, 4l, d, mr)
        repack_fields(ks, tsp, nsb, ssb, d, mr, k4_row_fields())
    }
}

//! iq3s: 26 four-byte columns of [qs 64][qh 8][signs 32], the k4 scale row.
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_iq3s) {
    repack_column_form(kq, ks, n, d, mr, IQ3S_QSB, IQ3S_SSB)
}

//! k2: the qs bytes as 16 four-byte columns (the 2-bit lanes stay in place - the kernels shift
//! per group); the scale row interleaves FIELD-MAJOR as [16 sc/min bytes x mr][mr x f16 d]
//! [mr x f16 dmin] (contiguous d rows for the emitter's f16 vector loads).
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_k2) {
    let nsb = n / 256l
    repack_in_place(kq, ks, d * nsb * K2_QSB, d * nsb * K2_SSB) $(tqp, tsp) {
        repack_columns(kq, tqp, nsb, K2_QSB, 4l, d, mr)
        repack_fields(ks, tsp, nsb, K2_SSB, d, mr, [RepackField(src = 4l, width = 1l, count = 16l), RepackField(src = 0l, width = 2l, count = 1l), RepackField(src = 2l, width = 2l, count = 1l)])
    }
}

//! iq2s: 18 four-byte columns of [idx 32][signs 32][qh 8], the k4 row with 16 strips.
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_iq2s) {
    repack_column_form(kq, ks, n, d, mr, IQ2S_QSB, IQ2S_SSB)
}

//! iq2xs: 16 four-byte columns of the u16 qs words (a word's two bytes share a column), the
//! k4 row with 16 strips.
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_iq2xs) {
    repack_column_form(kq, ks, n, d, mr, IQ2XS_QSB, IQ2XS_SSB)
}

//! iq2xxs: 16 four-byte columns of [grid bytes][aux32] per block, the k4 row with 16 strips.
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_iq2xxs) {
    repack_column_form(kq, ks, n, d, mr, IQ2XXS_QSB, IQ2XXS_SSB)
}

// ===== grp<mr> panel unpack (the byte-expanded tile form the kq stamps load) =====

//! Unpack ONE group's packed grp<mr> k5/k6 quant panel into the BYTE-EXPANDED tile form (wlo/whi
//! kqBytes stamps load verbatim). SWAR over uint64 lanes: k5 deposit uses an add-carry test
//! ((x+0x7F)&0x80) to fold the high bit; k6 tops sit at a uniform shift per block.
[hint(unsafe_range_check, noalias = kqg, noalias = dst)]
//! iq3s panel unpack: the grid gather + sign apply, packed grp planes -> the byte-expanded
//! signed panel. One grid word = 4 elements = ONE i32 store; the sign nibble expands through
//! the mask table and negates byte-wise (magnitudes are odd 1..15 - no cross-byte carry).
def unpack_iq3s_panel_grp(kqg : uint8 const?; var dst : uint8?; mr, nsb : int64) {
    let grid = iq3s_grid()
    let smask = sign_masks_iq()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 104l * mr
            let ob = sbi * 256l * mr
            for (r in range64(mr)) {
                for (blk in range64(8l)) {
                    let hp = 64l + blk
                    let qh = int(kqg[qb + ((hp / 4l) * mr + r) * 4l + hp % 4l])
                    for (l in range64(4l)) {
                        let sp = 72l + blk * 4l + l
                        let sgn = int(kqg[qb + ((sp / 4l) * mr + r) * 4l + sp % 4l])
                        let q1p = blk * 8l + 2l * l
                        let q2p = q1p + 1l
                        let i1 = int(kqg[qb + ((q1p / 4l) * mr + r) * 4l + q1p % 4l]) | ((qh << int(8l - 2l * l)) & 256)
                        let i2 = int(kqg[qb + ((q2p / 4l) * mr + r) * 4l + q2p % 4l]) | ((qh << int(7l - 2l * l)) & 256)
                        store_grid_panel_pair(dst, ob, blk, l, r, mr, grid[i1], grid[i2], sgn, smask)
                    }
                }
            }
        }
    }
}

//! iq3xxs: 24 four-byte columns of [qs][aux] per block, the k4 scale row (d halved, strips, pads).
def repack_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64; _f : KqTag_iq3xxs) {
    repack_column_form(kq, ks, n, d, mr, IQ3XXS_QSB, IQ3XXS_SSB)
}

//! iq3xxs panel unpack: the halved-grid gather + ksigns sign apply, packed grp planes -> the
//! byte-expanded signed panel (the iq3s panel form — the shared fmt-33/34 tile reads it).
def unpack_iq3xxs_panel_grp(kqg : uint8 const?; var dst : uint8?; mr, nsb : int64) {
    let grid = iq3xxs_grid()
    let ksg = ksigns_iq2xs()
    let smask = sign_masks_iq()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 96l * mr
            let ob = sbi * 256l * mr
            for (r in range64(mr)) {
                for (blk in range64(8l)) {
                    var aux = 0u
                    for (t in range64(4l)) {
                        let ap = 64l + blk * 4l + t
                        aux |= uint(kqg[qb + ((ap / 4l) * mr + r) * 4l + ap % 4l]) << uint(8l * t)
                    }
                    for (l in range64(4l)) {
                        let sgn = int(ksg[int((aux >> uint(7l * l)) & 127u)])
                        let q1p = blk * 8l + 2l * l
                        let q2p = q1p + 1l
                        let i1 = int(kqg[qb + ((q1p / 4l) * mr + r) * 4l + q1p % 4l])
                        let i2 = int(kqg[qb + ((q2p / 4l) * mr + r) * 4l + q2p % 4l])
                        store_grid_panel_pair(dst, ob, blk, l, r, mr, grid[i1], grid[i2], sgn, smask)
                    }
                }
            }
        }
    }
}

//! iq2s panel unpack: the 10-bit qs|qh gather off the u64 grid (TWO words per index - eight
//! magnitudes {8, 25, 43}) + the block's own sign bytes, packed grp planes -> the shared
//! byte-expanded signed panel (the fmt-33/34 panel form; d is pre-eighth-ed at transcode).
def unpack_iq2s_panel_grp(kqg : uint8 const?; var dst : uint8?; mr, nsb : int64) {
    let grid = iq2s_grid2()
    let smask = sign_masks_iq()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 72l * mr
            let ob = sbi * 256l * mr
            for (r in range64(mr)) {
                for (blk in range64(8l)) {
                    let hp = 64l + blk
                    let qh = int(kqg[qb + ((hp / 4l) * mr + r) * 4l + hp % 4l])
                    for (l in range64(4l)) {
                        let sp = 32l + blk * 4l + l
                        let sgn = int(kqg[qb + ((sp / 4l) * mr + r) * 4l + sp % 4l])
                        let ip = blk * 4l + l
                        let ix = (int(kqg[qb + ((ip / 4l) * mr + r) * 4l + ip % 4l]) | ((qh << int(8l - 2l * l)) & 0x300)) * 2
                        store_grid_panel_pair(dst, ob, blk, l, r, mr, grid[ix], grid[ix + 1], sgn, smask)
                    }
                }
            }
        }
    }
}

//! iq2xs panel unpack: the u16 qs word's 9-bit index doubles into the u64 grid's word pair,
//! its high 7 bits pick the KSIGNS sign byte - packed grp planes -> the shared signed panel.
def unpack_iq2xs_panel_grp(kqg : uint8 const?; var dst : uint8?; mr, nsb : int64) {
    let grid = iq2xs_grid2()
    let ksgn = ksigns_iq2xs()
    let smask = sign_masks_iq()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 64l * mr
            let ob = sbi * 256l * mr
            for (r in range64(mr)) {
                for (blk in range64(8l)) {
                    for (l in range64(4l)) {
                        let lp = (blk * 4l + l) * 2l
                        let w16 = uint(kqg[qb + ((lp / 4l) * mr + r) * 4l + lp % 4l]) | (uint(kqg[qb + ((lp / 4l) * mr + r) * 4l + lp % 4l + 1l]) << 8u)
                        let sgn = int(ksgn[int(w16 >> 9u)])
                        let ix = int(w16 & 511u) * 2
                        store_grid_panel_pair(dst, ob, blk, l, r, mr, grid[ix], grid[ix + 1], sgn, smask)
                    }
                }
            }
        }
    }
}

//! iq2xxs panel unpack: block b's grid BYTES (column 2b) each double into the u64 grid's word
//! pair, its aux32 (column 2b+1) picks the KSIGNS sign bytes - packed grp planes -> the
//! shared signed panel (the per-32 scale rides the strip row).
def unpack_iq2xxs_panel_grp(kqg : uint8 const?; var dst : uint8?; mr, nsb : int64) {
    let grid = iq2xxs_grid2()
    let ksgn = ksigns_iq2xs()
    let smask = sign_masks_iq()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 64l * mr
            let ob = sbi * 256l * mr
            for (r in range64(mr)) {
                for (blk in range64(8l)) {
                    let ac = (2l * blk + 1l) * mr + r
                    let aux = uint(kqg[qb + ac * 4l]) | (uint(kqg[qb + ac * 4l + 1l]) << 8u) | (uint(kqg[qb + ac * 4l + 2l]) << 16u) | (uint(kqg[qb + ac * 4l + 3l]) << 24u)
                    let gc = (2l * blk) * mr + r
                    for (l in range64(4l)) {
                        let sgn = int(ksgn[int((aux >> uint(7l * l)) & 127u)])
                        let ix = int(kqg[qb + gc * 4l + l]) * 2
                        store_grid_panel_pair(dst, ob, blk, l, r, mr, grid[ix], grid[ix + 1], sgn, smask)
                    }
                }
            }
        }
    }
}

[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#kq-subblock-planes")]
def unpack_kq_panel_grp(fmt : int64; kqg : uint8 const?; var dst : uint8?; mr, nsb : int64) {
    if (fmt == 33l) {
        unpack_iq3s_panel_grp(kqg, dst, mr, nsb)
        return
    }
    if (fmt == 34l) {
        unpack_iq3xxs_panel_grp(kqg, dst, mr, nsb)
        return
    }
    if (fmt == 23l) {
        unpack_iq2s_panel_grp(kqg, dst, mr, nsb)
        return
    }
    if (fmt == 25l) {
        unpack_iq2xxs_panel_grp(kqg, dst, mr, nsb)
        return
    }
    if (fmt == 24l) {
        unpack_iq2xs_panel_grp(kqg, dst, mr, nsb)
        return
    }
    let LO4 = 0x0F0F0F0F0F0F0F0Ful
    let SEL = 0x0804020108040201ul   // LE lane t (0..3, twice) selects high-bit t
    let C7F = 0x7F7F7F7F7F7F7F7Ful
    let C80 = 0x8080808080808080ul
    let TOP2 = 0x0303030303030303ul
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * (fmt == 5l ? 160l : 192l) * mr
            let ob = sbi * 256l * mr
            for (blk in range64(8l)) {
                let hbase = qb + 128l * mr + blk * 2l * mr * 4l   // k6 high plane (k5 ignores)
                for (j in range64(4l)) {
                    let sj = uint64(2l * j)
                    let gcol = ((blk * 4l + j) * mr) * 4l
                    var r4 = 0l
                    while (r4 < mr) {
                        let np = kqg + (qb + gcol + r4 * 4l)
                        let nw = reinterpret<uint64 const?>(np)
                        var lop = dst + (ob + gcol + r4 * 4l)
                        var hip = dst + (ob + 128l * mr + gcol + r4 * 4l)
                        var low = reinterpret<uint64?>(lop)
                        var hiw = reinterpret<uint64?>(hip)
                        if (fmt == 5l) {
                            let hp = kqg + (qb + 128l * mr + (blk * 4l + j) * mr + r4)
                            let hb = reinterpret<uint const?>(hp)[0]   // 4 rows' high-bit bytes
                            for (w in range64(2l)) {
                                let n = nw[w]
                                let b0 = uint64((hb >> uint(16l * w)) & 0xFFu) * 0x01010101ul
                                let b1 = uint64((hb >> uint(16l * w + 8l)) & 0xFFu) * 0x01010101ul
                                let hbw = b0 | (b1 << 32ul)
                                let dl = (((hbw & SEL) + C7F) & C80) >> 3ul
                                let dh = ((((hbw >> 4ul) & SEL) + C7F) & C80) >> 3ul
                                low[w] = (n & LO4) | dl
                                hiw[w] = ((n >> 4ul) & LO4) | dh
                            }
                        } else {
                            let h0w = reinterpret<uint64 const?>(kqg + (hbase + r4 * 4l))
                            let h1w = reinterpret<uint64 const?>(kqg + (hbase + (mr + r4) * 4l))
                            for (w in range64(2l)) {
                                let n = nw[w]
                                low[w] = (n & LO4) | (((h0w[w] >> sj) & TOP2) << 4ul)
                                hiw[w] = ((n >> 4ul) & LO4) | (((h1w[w] >> sj) & TOP2) << 4ul)
                            }
                        }
                        r4 += 4l
                    }
                }
            }
        }
    }
}
