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

// ===== grp<mr> weight repacks (one per format; run once per tensor at load) =====

//! mr-generalized Q8 interleave repack — the q8q8_repack_type(mr) layout for any mr (mr=4
//! reproduces the old hand arm64-laneq interleave byte-for-byte). Group bytes biased by wbias
//! (bias128 stamps bake w^0x80); tail rows stay row-major AND unbiased.
def repack_q8q8_grp(var wp : int8?; var sp : float?; n, d, mr : int64; wbias : int64 = 0l; kgroup : int64 = 4l) {
    // a zero here means the caller read layout numbers a lane context cannot see — name the
    // failure instead of a bare div0 (2026-07-28)
    if (mr <= 0l || kgroup <= 0l) {
        panic("repack_q8q8_grp: mr={mr} kgroup={kgroup} - layout numbers unreadable (pool-lane context globals?)")
    }
    let nb = n / 32l
    let ng = d / mr
    let ncg = n / kgroup
    var tw : array<int8>
    var ts : array<float>
    tw |> resize(d * n)
    ts |> resize(d * nb)
    unsafe {
        var twp = addr(tw[0])
        var tsp = addr(ts[0])
        memcpy(twp, wp, d * n)
        memcpy(tsp, sp, d * nb * 4l)
        for (g in range64(ng)) {
            let gbase = g * mr * n
            for (kg in range64(ncg)) {
                let dst = gbase + kg * kgroup * mr
                for (r in range64(mr)) {
                    let src = (g * mr + r) * n + kg * kgroup
                    for (j in range64(kgroup)) {
                        wp[dst + r * kgroup + j] = twp[src + j]
                    }
                }
            }
            let sgbase = g * mr * nb
            for (bi in range64(nb)) {
                for (r in range64(mr)) {
                    sp[sgbase + bi * mr + r] = tsp[(g * mr + r) * nb + bi]
                }
            }
        }
        if (wbias != 0l) {
            // bias128: one pass over the GROUP region only (tails stay plain s8);
            // int8 truncation makes +128 the byte xor 0x80
            for (i in range64(ng * mr * n)) {
                wp[i] = int8(int(wp[i]) + int(wbias))
            }
        }
    }
    delete tw
    delete ts
}

//! mr-generalized MXFP4 plane repack (mr=4 reproduces the old hand arm64-laneq mx interleave
//! byte-for-byte). Nibble layout maps byte-for-byte onto the grp<mr> Q8 order (see
//! expand_mx4_region_q8); tail rows (d % mr) stay row-major.
def repack_mx4_grp(var np : uint8?; var ep : uint8?; n, d, mr : int64) {
    let nrow = n / 2l    // nibble bytes per row
    let nbb = n / 32l    // blocks per row
    let ng = d / mr
    var tn : array<uint8>
    var te : array<uint8>
    tn |> resize(d * nrow)
    te |> resize(d * nbb)
    unsafe {
        var tnp = addr(tn[0])
        var tep = addr(te[0])
        memcpy(tnp, np, d * nrow)
        memcpy(tep, ep, d * nbb)
        for (g in range64(ng)) {
            let gnb = g * mr * nrow
            for (bi in range64(nbb)) {
                let dst = gnb + bi * 16l * mr
                for (j in range64(4l)) {
                    for (r in range64(mr)) {
                        let src = (g * mr + r) * nrow + bi * 16l + j * 4l
                        let dj = dst + j * 4l * mr + r * 4l
                        np[dj + 0l] = tnp[src + 0l]
                        np[dj + 1l] = tnp[src + 1l]
                        np[dj + 2l] = tnp[src + 2l]
                        np[dj + 3l] = tnp[src + 3l]
                    }
                }
            }
            let geb = g * mr * nbb
            for (bi in range64(nbb)) {
                for (r in range64(mr)) {
                    ep[geb + bi * mr + r] = tep[(g * mr + r) * nbb + bi]
                }
            }
        }
    }
    delete tn
    delete te
}

//! mr-generalized q51 plane repack: per (block-group, block) at 20·mr bytes — nibbles
//! dword-interleaved like repack_mx4_grp, then qh row-major (mr × 4B); scale plane mr × 4B
//! f16 (d, m) per block. grp1 == disk order; tail rows (d % mr) stay disk-order.
def repack_q51_grp(var qp : uint8?; var sp : uint8?; n, d, mr : int64) {
    let nb = n / 32l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * nb * 20l)
    ts |> resize(d * nb * 4l)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, qp, d * nb * 20l)
        memcpy(tsp, sp, d * nb * 4l)
        for (g in range64(ng)) {
            let gqb = g * mr * nb * 20l
            let gsb = g * mr * nb * 4l
            for (bi in range64(nb)) {
                let dst = gqb + bi * 20l * mr
                for (r in range64(mr)) {
                    let src = ((g * mr + r) * nb + bi) * 20l
                    for (j in range64(4l)) {   // nibble dwords, mx4 order
                        let dj = dst + j * 4l * mr + r * 4l
                        qp[dj + 0l] = tqp[src + j * 4l + 0l]
                        qp[dj + 1l] = tqp[src + j * 4l + 1l]
                        qp[dj + 2l] = tqp[src + j * 4l + 2l]
                        qp[dj + 3l] = tqp[src + j * 4l + 3l]
                    }
                    let dh = dst + 16l * mr + r * 4l       // qh tail section
                    qp[dh + 0l] = tqp[src + 16l]
                    qp[dh + 1l] = tqp[src + 17l]
                    qp[dh + 2l] = tqp[src + 18l]
                    qp[dh + 3l] = tqp[src + 19l]
                    let ds = gsb + bi * 4l * mr + r * 4l
                    let ss = ((g * mr + r) * nb + bi) * 4l
                    sp[ds + 0l] = tsp[ss + 0l]
                    sp[ds + 1l] = tsp[ss + 1l]
                    sp[ds + 2l] = tsp[ss + 2l]
                    sp[ds + 3l] = tsp[ss + 3l]
                }
            }
        }
    }
    delete tq
    delete ts
}

//! mr-generalized K-quant plane repack, one function per format. k4/k5 (kq v2) also DECODE the
//! 6-bit scale packing into the u8 sc/mn planes — the padded-disk 20B rows and the decoded
//! 20B/row grp regions share one stride. Tail rows (d % mr) stay disk-order.
def repack_k4_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 128l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 128l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 128l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            for (t in range64(4l)) {
                                let k = blk * 32l + j * 4l + t
                                let lo = k45_nib(tqp + sq, k)
                                let hi = k45_nib(tqp + sq, k + 16l)
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = uint8(lo | (hi << 4u))
                            }
                        }
                        let sm = k4_sc_mn(tsp + ss + 4l, blk)
                        ks[ds + 4l * mr + blk * mr + r] = uint8(sm.x)
                        ks[ds + 12l * mr + blk * mr + r] = uint8(sm.y)
                    }
                    ks[ds + 2l * r] = tsp[ss]              // f16 d
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]       // f16 dmin
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                }
            }
        }
    }
    delete tq
    delete ts
}

def repack_k5_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 160l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 160l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 160l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            var hb = 0u
                            for (t in range64(4l)) {
                                let k = blk * 32l + j * 4l + t
                                let lo = k45_nib(tqp + sq, k)
                                let hi = k45_nib(tqp + sq, k + 16l)
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = uint8(lo | (hi << 4u))
                                hb |= k5_hbit(tqp + sq + 128l, k) << uint(t)
                                hb |= k5_hbit(tqp + sq + 128l, k + 16l) << uint(4l + t)
                            }
                            kq[dq + 128l * mr + (blk * 4l + j) * mr + r] = uint8(hb)
                        }
                        let sm = k4_sc_mn(tsp + ss + 4l, blk)
                        ks[ds + 4l * mr + blk * mr + r] = uint8(sm.x)
                        ks[ds + 12l * mr + blk * mr + r] = uint8(sm.y)
                    }
                    ks[ds + 2l * r] = tsp[ss]              // f16 d
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]       // f16 dmin
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                }
            }
        }
    }
    delete tq
    delete ts
}

def repack_k6_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 192l
    let srow = nsb * 18l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 192l * mr
                let ds = g * mr * srow + sbi * 18l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 192l
                    let ss = (g * mr + r) * srow + sbi * 18l
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
                    for (h in range64(2l)) {   // qh bytes verbatim, row-interleaved by 4-byte columns
                        for (jj in range64(8l)) {
                            for (t in range64(4l)) {
                                kq[dq + 128l * mr + ((h * 8l + jj) * mr + r) * 4l + t] = tqp[sq + 128l + h * 32l + jj * 4l + t]
                            }
                        }
                    }
                    for (idx in range64(16l)) {
                        ks[ds + idx * mr + r] = tsp[ss + idx]
                    }
                    ks[ds + 16l * mr + 2l * r] = tsp[ss + 16l]
                    ks[ds + 16l * mr + 2l * r + 1l] = tsp[ss + 17l]
                }
            }
        }
    }
    delete tq
    delete ts
}

def repack_q40_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    // q40 grp layout: quants land byte-for-byte in the k4 tiling — the disk bytes already carry
    // the k / k+16 nibble pairing, so byte t of (blk, j) is disk byte blk*16 + j*4 + t (a pure
    // gather); scales interleave per block: [blk 0..7][mr x f16 d]
    let nsb = n / 256l
    let qrow = nsb * 128l
    let srow = nsb * 16l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 128l * mr
                let ds = g * mr * srow + sbi * 16l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 128l
                    let ss = (g * mr + r) * srow + sbi * 16l
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            for (t in range64(4l)) {
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = tqp[sq + blk * 16l + j * 4l + t]
                            }
                        }
                        ks[ds + blk * 2l * mr + 2l * r] = tsp[ss + blk * 2l]        // f16 d
                        ks[ds + blk * 2l * mr + 2l * r + 1l] = tsp[ss + blk * 2l + 1l]
                    }
                }
            }
        }
    }
    delete tq
    delete ts
}

//! iq4xs grp layout: the q40 quant gather (the disk k/k+16 pairing IS the k4 tiling) and the k4
//! scale-row interleave over the DECODED 20B row — [mr x f16 d][mr x f16 0][8 x mr int8 sc][8 x mr 0].
def repack_iq4xs_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 128l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 128l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 128l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            for (t in range64(4l)) {
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = tqp[sq + blk * 16l + j * 4l + t]
                            }
                        }
                        ks[ds + 4l * mr + blk * mr + r] = tsp[ss + 4l + blk]
                        ks[ds + 12l * mr + blk * mr + r] = uint8(0)
                    }
                    ks[ds + 2l * r] = tsp[ss]
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = uint8(0)
                    ks[ds + 2l * mr + 2l * r + 1l] = uint8(0)
                }
            }
        }
    }
    delete tq
    delete ts
}

//! k3 grp layout: the qs bytes as 16 four-byte columns ([h 0..1][8 l-groups]) x mr rows, then the
//! hmask as 8 columns x mr rows at 64*mr — the 2-bit lanes and mask bits stay in place (the
//! kernels shift per block); scales interleave as k6's row ([16 x mr int8][mr x f16 d]).
def repack_k3_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 96l
    let srow = nsb * 18l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 96l * mr
                let ds = g * mr * srow + sbi * 18l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 96l
                    let ss = (g * mr + r) * srow + sbi * 18l
                    for (c in range64(16l)) {
                        for (t in range64(4l)) {
                            kq[dq + (c * mr + r) * 4l + t] = tqp[sq + c * 4l + t]
                        }
                    }
                    for (c in range64(8l)) {
                        for (t in range64(4l)) {
                            kq[dq + 64l * mr + (c * mr + r) * 4l + t] = tqp[sq + 64l + c * 4l + t]
                        }
                    }
                    for (idx in range64(16l)) {
                        ks[ds + idx * mr + r] = tsp[ss + idx]
                    }
                    ks[ds + 16l * mr + 2l * r] = tsp[ss + 16l]
                    ks[ds + 16l * mr + 2l * r + 1l] = tsp[ss + 17l]
                }
            }
        }
    }
    delete tq
    delete ts
}

//! iq3s grp layout: the whole 104B row as 26 four-byte columns x mr rows — qs, qh and sign
//! bytes stay in place per column (the kernels compose per block); scales interleave as k4's
//! row ([d f16 x mr][pad x mr][8 sc x mr][8 pad x mr]).
def repack_iq3s_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 104l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 104l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 104l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (c in range64(26l)) {
                        for (t in range64(4l)) {
                            kq[dq + (c * mr + r) * 4l + t] = tqp[sq + c * 4l + t]
                        }
                    }
                    ks[ds + 2l * r] = tsp[ss]
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                    for (b in range64(8l)) {
                        ks[ds + 4l * mr + b * mr + r] = tsp[ss + 4l + b]
                        ks[ds + 12l * mr + b * mr + r] = tsp[ss + 12l + b]
                    }
                }
            }
        }
    }
    delete tq
    delete ts
}

//! k2 grp layout: the qs bytes as 16 four-byte columns x mr rows (the 2-bit lanes stay in
//! place - the kernels shift per group); scale row interleaves FIELD-MAJOR as
//! [16 sc/min bytes x mr][mr x f16 d][mr x f16 dmin] (contiguous d rows for the emitter's f16 vector loads).
def repack_k2_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 64l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 64l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 64l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (c in range64(16l)) {
                        for (t in range64(4l)) {
                            kq[dq + (c * mr + r) * 4l + t] = tqp[sq + c * 4l + t]
                        }
                    }
                    for (idx in range64(16l)) {
                        ks[ds + idx * mr + r] = tsp[ss + 4l + idx]
                    }
                    ks[ds + 16l * mr + 2l * r] = tsp[ss]
                    ks[ds + 16l * mr + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 18l * mr + 2l * r] = tsp[ss + 2l]
                    ks[ds + 18l * mr + 2l * r + 1l] = tsp[ss + 3l]
                }
            }
        }
    }
    delete tq
    delete ts
}

//! iq2s grp layout: the whole 72B row as 18 four-byte columns x mr rows - idx, sign and qh
//! bytes stay in place per column; scales interleave as k4's row with 16 strips
//! ([d8 f16 x mr][pad x mr][16 strips x mr]).
def repack_iq2s_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 72l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 72l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 72l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (c in range64(18l)) {
                        for (t in range64(4l)) {
                            kq[dq + (c * mr + r) * 4l + t] = tqp[sq + c * 4l + t]
                        }
                    }
                    ks[ds + 2l * r] = tsp[ss]
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                    for (g16 in range64(16l)) {
                        ks[ds + 4l * mr + g16 * mr + r] = tsp[ss + 4l + g16]
                    }
                }
            }
        }
    }
    delete tq
    delete ts
}

//! iq2xs grp layout: the whole 64B row as 16 four-byte columns x mr rows - the u16 qs words
//! stay in place per column (each word's two bytes share a column); scales interleave as k4's
//! row with 16 strips ([d8 f16 x mr][pad x mr][16 strips x mr]).
def repack_iq2xs_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 64l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 64l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 64l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (c in range64(16l)) {
                        for (t in range64(4l)) {
                            kq[dq + (c * mr + r) * 4l + t] = tqp[sq + c * 4l + t]
                        }
                    }
                    ks[ds + 2l * r] = tsp[ss]
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                    for (g16 in range64(16l)) {
                        ks[ds + 4l * mr + g16 * mr + r] = tsp[ss + 4l + g16]
                    }
                }
            }
        }
    }
    delete tq
    delete ts
}

//! iq2xxs grp layout: the whole 64B row as 16 four-byte columns x mr rows - the u16 qs words
//! stay in place per column (each word's two bytes share a column); scales interleave as k4's
//! row with 16 strips ([d8 f16 x mr][pad x mr][16 strips x mr]).
def repack_iq2xxs_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 64l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 64l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 64l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (c in range64(16l)) {
                        for (t in range64(4l)) {
                            kq[dq + (c * mr + r) * 4l + t] = tqp[sq + c * 4l + t]
                        }
                    }
                    ks[ds + 2l * r] = tsp[ss]
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                    for (g16 in range64(16l)) {
                        ks[ds + 4l * mr + g16 * mr + r] = tsp[ss + 4l + g16]
                    }
                }
            }
        }
    }
    delete tq
    delete ts
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
    let smask = fixed_array<uint>(0x00000000u, 0x000000FFu, 0x0000FF00u, 0x0000FFFFu,
                                  0x00FF0000u, 0x00FF00FFu, 0x00FFFF00u, 0x00FFFFFFu,
                                  0xFF000000u, 0xFF0000FFu, 0xFF00FF00u, 0xFF00FFFFu,
                                  0xFFFF0000u, 0xFFFF00FFu, 0xFFFFFF00u, 0xFFFFFFFFu)
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
                        let m1 = smask[sgn & 15]
                        let m2 = smask[(sgn >> 4) & 15]
                        let w1 = (grid[i1] ^ m1) + (m1 & 0x01010101u)
                        let w2 = (grid[i2] ^ m2) + (m2 & 0x01010101u)
                        // element e = l*8 + half*4 + t: e < 16 lands in the lo panel half, else hi
                        let e1 = l * 8l
                        let e2 = l * 8l + 4l
                        let d1 = (e1 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e1 % 16l) / 4l) * mr + r) * 4l
                        let d2 = (e2 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e2 % 16l) / 4l) * mr + r) * 4l
                        var wp1 = reinterpret<uint?>(dst + (ob + d1))
                        var wp2 = reinterpret<uint?>(dst + (ob + d2))
                        wp1[0] = w1
                        wp2[0] = w2
                    }
                }
            }
        }
    }
}

//! iq3xxs grp layout: the whole 96B row as 24 four-byte columns x mr rows — qs and aux move
//! together; the scale row keeps the iq3s interleave (d pairs, pads, strips, pads).
def repack_iq3xxs_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 96l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(d * qrow)
    ts |> resize(d * srow)
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        memcpy(tqp, kq, d * qrow)
        memcpy(tsp, ks, d * srow)
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 96l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 96l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (c in range64(24l)) {
                        for (t in range64(4l)) {
                            kq[dq + (c * mr + r) * 4l + t] = tqp[sq + c * 4l + t]
                        }
                    }
                    ks[ds + 2l * r] = tsp[ss]
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                    for (b in range64(8l)) {
                        ks[ds + 4l * mr + b * mr + r] = tsp[ss + 4l + b]
                        ks[ds + 12l * mr + b * mr + r] = tsp[ss + 12l + b]
                    }
                }
            }
        }
    }
    delete tq
    delete ts
}

//! iq3xxs panel unpack: the halved-grid gather + ksigns sign apply, packed grp planes -> the
//! byte-expanded signed panel (the iq3s panel form — the shared fmt-33/34 tile reads it).
def unpack_iq3xxs_panel_grp(kqg : uint8 const?; var dst : uint8?; mr, nsb : int64) {
    let grid = iq3xxs_grid()
    let ksg = ksigns_iq2xs()
    let smask = fixed_array<uint>(0x00000000u, 0x000000FFu, 0x0000FF00u, 0x0000FFFFu,
                                  0x00FF0000u, 0x00FF00FFu, 0x00FFFF00u, 0x00FFFFFFu,
                                  0xFF000000u, 0xFF0000FFu, 0xFF00FF00u, 0xFF00FFFFu,
                                  0xFFFF0000u, 0xFFFF00FFu, 0xFFFFFF00u, 0xFFFFFFFFu)
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
                        let m1 = smask[sgn & 15]
                        let m2 = smask[(sgn >> 4) & 15]
                        let w1 = (grid[i1] ^ m1) + (m1 & 0x01010101u)
                        let w2 = (grid[i2] ^ m2) + (m2 & 0x01010101u)
                        // element e = l*8 + half*4 + t: e < 16 lands in the lo panel half, else hi
                        let e1 = l * 8l
                        let e2 = l * 8l + 4l
                        let d1 = (e1 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e1 % 16l) / 4l) * mr + r) * 4l
                        let d2 = (e2 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e2 % 16l) / 4l) * mr + r) * 4l
                        var wp1 = reinterpret<uint?>(dst + (ob + d1))
                        var wp2 = reinterpret<uint?>(dst + (ob + d2))
                        wp1[0] = w1
                        wp2[0] = w2
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
    let smask = fixed_array<uint>(0x00000000u, 0x000000FFu, 0x0000FF00u, 0x0000FFFFu,
                                  0x00FF0000u, 0x00FF00FFu, 0x00FFFF00u, 0x00FFFFFFu,
                                  0xFF000000u, 0xFF0000FFu, 0xFF00FF00u, 0xFF00FFFFu,
                                  0xFFFF0000u, 0xFFFF00FFu, 0xFFFFFF00u, 0xFFFFFFFFu)
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
                        let m1 = smask[sgn & 15]
                        let m2 = smask[(sgn >> 4) & 15]
                        let w1 = (grid[ix] ^ m1) + (m1 & 0x01010101u)
                        let w2 = (grid[ix + 1] ^ m2) + (m2 & 0x01010101u)
                        // element e = l*8 + half*4 + t: e < 16 lands in the lo panel half, else hi
                        let e1 = l * 8l
                        let e2 = l * 8l + 4l
                        let d1 = (e1 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e1 % 16l) / 4l) * mr + r) * 4l
                        let d2 = (e2 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e2 % 16l) / 4l) * mr + r) * 4l
                        var wp1 = reinterpret<uint?>(dst + (ob + d1))
                        var wp2 = reinterpret<uint?>(dst + (ob + d2))
                        wp1[0] = w1
                        wp2[0] = w2
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
    let smask = fixed_array<uint>(0x00000000u, 0x000000FFu, 0x0000FF00u, 0x0000FFFFu,
                                  0x00FF0000u, 0x00FF00FFu, 0x00FFFF00u, 0x00FFFFFFu,
                                  0xFF000000u, 0xFF0000FFu, 0xFF00FF00u, 0xFF00FFFFu,
                                  0xFFFF0000u, 0xFFFF00FFu, 0xFFFFFF00u, 0xFFFFFFFFu)
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
                        let m1 = smask[sgn & 15]
                        let m2 = smask[(sgn >> 4) & 15]
                        let w1 = (grid[ix] ^ m1) + (m1 & 0x01010101u)
                        let w2 = (grid[ix + 1] ^ m2) + (m2 & 0x01010101u)
                        // element e = l*8 + half*4 + t: e < 16 lands in the lo panel half, else hi
                        let e1 = l * 8l
                        let e2 = l * 8l + 4l
                        let d1 = (e1 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e1 % 16l) / 4l) * mr + r) * 4l
                        let d2 = (e2 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e2 % 16l) / 4l) * mr + r) * 4l
                        var wp1 = reinterpret<uint?>(dst + (ob + d1))
                        var wp2 = reinterpret<uint?>(dst + (ob + d2))
                        wp1[0] = w1
                        wp2[0] = w2
                    }
                }
            }
        }
    }
}

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
                let g2 = uint64((blk % 4l) * 2l)
                let hbase = qb + 128l * mr + (blk / 4l) * 32l * mr   // k6 high plane (k5 ignores)
                for (j in range64(4l)) {
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
                            let h0w = reinterpret<uint64 const?>(kqg + (hbase + (j * mr + r4) * 4l))
                            let h1w = reinterpret<uint64 const?>(kqg + (hbase + ((j + 4l) * mr + r4) * 4l))
                            for (w in range64(2l)) {
                                let n = nw[w]
                                low[w] = (n & LO4) | (((h0w[w] >> g2) & TOP2) << 4ul)
                                hiw[w] = ((n >> 4ul) & LO4) | (((h1w[w] >> g2) & TOP2) << 4ul)
                            }
                        }
                        r4 += 4l
                    }
                }
            }
        }
    }
}
