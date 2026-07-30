options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options persistent_heap   // the layout arm loads a multi-GB model; explicit delete below

require dastest/testing_boost public
require _model_tier
require dasllama/dasllama_gguf
require dasllama/dasllama_convert
require dasllama/dasllama_math   // pin_kernel_backend — the layout arm needs disk-order planes
require dasllama/dasllama_math_default   // dot_k4q8/k5/k6/q40q8 — the kernel arms gate them vs the plane dequant
require dasllama/dasllama_math_gen   // kq_grp_row_dot — the stage-4 repack oracle
require dasllama/dasllama_repack     // repack_k4/k5/k6/q40_grp + repack_q51_grp
require dasllama/dasllama_gemm_schema   // kq_qsb/kq_ssb — the fmt-id plane strides
require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration for load_gguf
require math

// K-quant (Q4_K / Q6_K) superblock dequant gates. The synthetic arms hand-pack one superblock
// with the ggml bit layout written in the PACK direction (independent of the reader's unpack),
// so a layout misread fails loudly with no model files needed. The dequant arithmetic itself is
// proven bit-exact against gguf-py's dequantize (harness/q4k_dequant_probe.das); the real-file
// arm cross-checks against the Q8_0 twin of the same weights at known quantization-error levels.

// synthetic quant patterns: every nibble/5-bit/6-bit value, position-dependent so swaps can't cancel
def private q4_pat(k : int) : int => (k * 7 + 3) % 16
def private q5_pat(k : int) : int => (k * 3 + 7) % 32
def private q6_pat(k : int) : int => (k * 5 + 11) % 64

// The shared synthetic scale/min sets: 6-bit values > 15 exercise the split high-bit arms of
// get_scale_min_k4 (j >= 4); d/dmin are f16-exact so expected-value math uses the f32 value.
let SYNTH_SC <- [63, 21, 37, 5, 48, 17, 60, 33]
let SYNTH_MN <- [1, 62, 19, 44, 7, 58, 25, 36]
let SYNTH_D = 0.5
let SYNTH_DMIN = 0.25
let SYNTH_SCQ <- [3, -5, 60, -60, 17, 127, -128, 1, 0, 33, -77, 90, -2, 45, -101, 7]

// Pack the 4-byte header + 12 scale/min bytes shared by the Q4_K/Q5_K synthetic superblocks.
def private pack_kq_header(var blkb : array<uint8>) {
    let dbits = f32_to_f16(SYNTH_D)
    let mbits = f32_to_f16(SYNTH_DMIN)
    blkb[0] = uint8(dbits & 0xFF)
    blkb[1] = uint8(dbits >> 8u)
    blkb[2] = uint8(mbits & 0xFF)
    blkb[3] = uint8(mbits >> 8u)
    for (j in range(4)) {
        blkb[4 + j] = uint8((SYNTH_SC[j] & 63) | (((SYNTH_SC[j + 4] >> 4) & 3) << 6))
        blkb[8 + j] = uint8((SYNTH_MN[j] & 63) | (((SYNTH_MN[j + 4] >> 4) & 3) << 6))
        blkb[12 + j] = uint8((SYNTH_SC[j + 4] & 15) | ((SYNTH_MN[j + 4] & 15) << 4))
    }
}

def private build_q4k_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(144)
    pack_kq_header(blkb)
    // chunk js: 32 bytes carry weights 64js..+31 in low nibbles, 64js+32..+31 in high
    for (js in range(4)) {
        for (l in range(32)) {
            blkb[16 + 32 * js + l] = uint8(q4_pat(64 * js + l) | (q4_pat(64 * js + 32 + l) << 4))
        }
    }
    return <- blkb
}

def private build_q5k_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(176)
    pack_kq_header(blkb)
    // qh[l] bit 2js = high bit of weight 64js+l, bit 2js+1 = high bit of weight 64js+32+l
    for (js in range(4)) {
        for (l in range(32)) {
            blkb[48 + 32 * js + l] = uint8((q5_pat(64 * js + l) & 15) | ((q5_pat(64 * js + 32 + l) & 15) << 4))
            var h = int(blkb[16 + l])
            h |= (q5_pat(64 * js + l) >> 4) << (2 * js)
            h |= (q5_pat(64 * js + 32 + l) >> 4) << (2 * js + 1)
            blkb[16 + l] = uint8(h)
        }
    }
    return <- blkb
}

def private build_q6k_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(210)
    for (half in range(2)) {
        let base = 128 * half
        for (l in range(32)) {
            blkb[64 * half + l] = uint8((q6_pat(base + l) & 15) | ((q6_pat(base + 64 + l) & 15) << 4))
            blkb[64 * half + 32 + l] = uint8((q6_pat(base + 32 + l) & 15) | ((q6_pat(base + 96 + l) & 15) << 4))
            blkb[128 + 32 * half + l] = uint8((q6_pat(base + l) >> 4) | ((q6_pat(base + 32 + l) >> 4) << 2)
                | ((q6_pat(base + 64 + l) >> 4) << 4) | ((q6_pat(base + 96 + l) >> 4) << 6))
        }
    }
    for (i in range(16)) {
        blkb[192 + i] = uint8(SYNTH_SCQ[i] & 255)
    }
    let dbits = f32_to_f16(0.25)
    blkb[208] = uint8(dbits & 0xFF)
    blkb[209] = uint8(dbits >> 8u)
    return <- blkb
}

// loop-generated test labels: "k4"/"k5"/"k6" for the K-quants, "q40" for fmt 40 — matches the
// kernel/repack function names (dot_q40q8, repack_q40_grp) so failures grep 1:1
def private kq_tag(fmt : int) : string => fmt == 40 ? "q40" : "k{fmt}"

// q40 synthetic superblock: 8 x 18B q4_0 disk blocks, per-block f16-exact d = (blk+1)/16 and
// the shared nibble pattern — weight k = d(k/32) * (q4_pat(k) - 8)
def private q40_d(blk : int) : float => float(blk + 1) * 0.0625

def private build_q40_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(144)
    for (blk in range(8)) {
        let dbits = f32_to_f16(q40_d(blk))
        blkb[blk * 18] = uint8(dbits & 0xFF)
        blkb[blk * 18 + 1] = uint8(dbits >> 8u)
        for (j in range(16)) {   // byte j: weight blk*32+j in the low nibble, +16 in the high
            blkb[blk * 18 + 2 + j] = uint8(q4_pat(blk * 32 + j) | (q4_pat(blk * 32 + 16 + j) << 4))
        }
    }
    return <- blkb
}

// q51 synthetic 32-block index `blk`: f16-exact d/m varied per block, elem k = q5_pat(k + blk)
// (shifted per block so an inter-block offset bug can't cancel); disk layout [d][m][qh][qs].
def private q51_d(blk : int) : float => float(blk % 7 + 1) * 0.0625
def private q51_m(blk : int) : float => float(blk % 5) * 0.25 - 0.5

def private build_q51_block(blk : int) : array<uint8> {
    var b : array<uint8>
    b |> resize(24)
    let dbits = f32_to_f16(q51_d(blk))
    let mbits = f32_to_f16(q51_m(blk))
    b[0] = uint8(dbits & 0xFF)
    b[1] = uint8(dbits >> 8u)
    b[2] = uint8(mbits & 0xFF)
    b[3] = uint8(mbits >> 8u)
    var qh = 0u
    for (k in range(32)) {   // qh bit k = the 5th bit of element k
        if ((q5_pat(k + blk) & 16) != 0) {
            qh |= 1u << uint(k)
        }
    }
    for (i in range(4)) {
        b[4 + i] = uint8(qh >> uint(i * 8))
    }
    for (j in range(16)) {   // byte j: element blk32+j low nibble, +16 high nibble
        b[8 + j] = uint8((q5_pat(j + blk) & 15) | ((q5_pat(j + 16 + blk) & 15) << 4))
    }
    return <- b
}

[test]
def test_q4k_synthetic(t : T?) {
    t |> run("Q4_K superblock unpacks the 6-bit scale/min packing") @(t : T?) {
        let blkb <- build_q4k_block()
        var dst : array<float>
        dst |> resize(256)
        dequant_q4k_superblock(blkb, 0l, dst, 0l)
        for (k in range(256)) {
            let sub = k / 32
            let expected = SYNTH_D * float(SYNTH_SC[sub]) * float(q4_pat(k)) - SYNTH_DMIN * float(SYNTH_MN[sub])
            t |> success(dst[k] == expected, "Q4_K element must match hand-packed value exactly")
        }
    }
    t |> run("Q5_K superblock unpacks the split high-bit plane") @(t : T?) {
        let blkb <- build_q5k_block()
        var dst : array<float>
        dst |> resize(256)
        dequant_q5k_superblock(blkb, 0l, dst, 0l)
        for (k in range(256)) {
            let sub = k / 32
            let expected = SYNTH_D * float(SYNTH_SC[sub]) * float(q5_pat(k)) - SYNTH_DMIN * float(SYNTH_MN[sub])
            t |> success(dst[k] == expected, "Q5_K element must match hand-packed value exactly")
        }
    }
    t |> run("Q6_K superblock unpacks the ql/qh split and signed sub-scales") @(t : T?) {
        let blkb <- build_q6k_block()
        var dst : array<float>
        dst |> resize(256)
        dequant_q6k_superblock(blkb, 0l, dst, 0l)
        for (k in range(256)) {
            let expected = 0.25 * float(SYNTH_SCQ[k / 16]) * float(q6_pat(k) - 32)
            t |> success(dst[k] == expected, "Q6_K element must match hand-packed value exactly")
        }
    }
}

[test]
def test_kq_transcode_planes(t : T?) {
    // The transcoded planes ARE the kernel-facing format, and they are verbatim disk splits for
    // all three formats (kq v2) — every plane dequant must equal the FILE dequant bit-exactly.
    t |> run("k4 planes reproduce the Q4_K dequant bit-exactly") @(t : T?) {
        let blkb <- build_q4k_block()
        var kq : array<uint8>
        var ks : array<uint8>
        kq |> resize(128)
        ks |> resize(20)
        transcode_q4k_superblock(blkb, 0l, kq, 0l, ks, 0l)
        var want : array<float>
        want |> resize(256)
        dequant_q4k_superblock(blkb, 0l, want, 0l)
        var dst : array<float>
        dst |> resize(256)
        dequant_k4_plane_superblock(kq, 0l, ks, 0l, dst, 0l)
        for (k in range(256)) {
            t |> success(dst[k] == want[k], "k4 plane element must equal the file dequant bit-exactly")
        }
    }
    t |> run("k5 planes reproduce the Q5_K dequant bit-exactly") @(t : T?) {
        let blkb <- build_q5k_block()
        var kq : array<uint8>
        var ks : array<uint8>
        kq |> resize(160)
        ks |> resize(20)
        transcode_q5k_superblock(blkb, 0l, kq, 0l, ks, 0l)
        var want : array<float>
        want |> resize(256)
        dequant_q5k_superblock(blkb, 0l, want, 0l)
        var dst : array<float>
        dst |> resize(256)
        dequant_k5_plane_superblock(kq, 0l, ks, 0l, dst, 0l)
        for (k in range(256)) {
            t |> success(dst[k] == want[k], "k5 plane element must equal the file dequant bit-exactly")
        }
    }
    t |> run("k6 planes reproduce the Q6_K dequant bit-exactly") @(t : T?) {
        let blkb <- build_q6k_block()
        var kq : array<uint8>
        var ks : array<uint8>
        kq |> resize(192)
        ks |> resize(18)
        transcode_q6k_superblock(blkb, 0l, kq, 0l, ks, 0l)
        var want : array<float>
        want |> resize(256)
        dequant_q6k_superblock(blkb, 0l, want, 0l)
        var dst : array<float>
        dst |> resize(256)
        dequant_k6_plane_superblock(kq, 0l, ks, 0l, dst, 0l)
        for (k in range(256)) {
            t |> success(dst[k] == want[k], "k6 plane element must equal the file dequant bit-exactly")
        }
    }
    t |> run("q40 planes unpack the hand-packed Q4_0 blocks exactly") @(t : T?) {
        let blkb <- build_q40_block()
        var kq : array<uint8>
        var ks : array<uint8>
        kq |> resize(128)
        ks |> resize(16)
        transcode_q40_superblock(blkb, 0l, kq, 0l, ks, 0l)
        var dst : array<float>
        dst |> resize(256)
        dequant_q40_plane_superblock(kq, 0l, ks, 0l, dst, 0l)
        for (k in range(256)) {
            let expected = q40_d(k / 32) * float(q4_pat(k) - 8)
            t |> success(dst[k] == expected, "q40 plane element must match hand-packed value exactly")
        }
    }
    t |> run("q51 planes unpack the hand-packed Q5_1 blocks exactly") @(t : T?) {
        for (blk in range(4)) {
            let blkb <- build_q51_block(blk)
            var kq : array<uint8>
            var ks : array<uint8>
            kq |> resize(20)
            ks |> resize(4)
            transcode_q51_block(blkb, 0l, kq, 0l, ks, 0l)
            var dst : array<float>
            dst |> resize(32)
            dequant_q51_plane_block(kq, 0l, ks, 0l, dst, 0l)
            for (k in range(32)) {
                let expected = q51_d(blk) * float(q5_pat(k + blk)) + q51_m(blk)
                t |> success(dst[k] == expected, "q51 plane element must match hand-packed value exactly")
            }
        }
    }
}

// XOR the quant payload of a synthetic superblock with an LCG byte stream — every quant bit
// pattern is valid, so the second superblock of a kernel test carries different data (an
// inter-superblock offset bug can't cancel).
def private perturb_quants(var blkb : array<uint8>; qoff, qlen : int) {
    var st = 0x12345u
    for (i in range(qlen)) {
        st = st * 1664525u + 1013904223u
        blkb[qoff + i] = uint8(uint(blkb[qoff + i]) ^ (st >> 16u))
    }
}

// ===== q51 kernel gates (per-32 planes, Q8_0-form activations — NOT the kq superblock lattice) =====

// Fill `rows` q51 plane rows of n elements; row r block b packs build_q51_block(r*31 + b) so no
// two blocks anywhere share data (offset bugs can't cancel).
def private q51_fill_planes(var kq : array<uint8>; var ks : array<uint8>; rows, n : int64) {
    let nb = n / 32l
    kq |> resize(rows * nb * 20l)
    ks |> resize(rows * nb * 4l)
    for (r in range64(rows)) {
        for (b in range64(nb)) {
            let blkb <- build_q51_block(int(r * 31l + b))
            transcode_q51_block(blkb, 0l, kq, (r * nb + b) * 20l, ks, (r * nb + b) * 4l)
        }
    }
}

// smooth per-row-shifted activation rows -> the Q8_0-form image (per-32 scales, no block sums)
def private q51_fill_acts(var xq : array<int8>; var xs : array<float>; rows, n : int64) {
    var src : array<float>
    src |> resize(n)
    xq |> resize(rows * n)
    xs |> resize(rows * (n / 32l))
    for (r in range64(rows)) {
        for (i in range64(n)) {
            src[i] = 1.7 * sin(float(i + r * 37l) * 0.23) + 0.3 * cos(float(i) * 0.071)
        }
        quantize_q8_0_into(src, n, xq, xs, r * n, r * (n / 32l))
    }
    delete src
}

def private q51_dot_gate(t : T?) {
    let n = 704l   // the real 26B-A4B expert row: % 32, NOT % 256 — why the tier exists
    let nb = n / 32l
    var kq : array<uint8>
    var ks : array<uint8>
    q51_fill_planes(kq, ks, 1l, n)
    var w : array<float>
    w |> resize(n)
    for (b in range64(nb)) {
        dequant_q51_plane_block(kq, b * 20l, ks, b * 4l, w, b * 32l)
    }
    var xq : array<int8>
    var xs : array<float>
    q51_fill_acts(xq, xs, 1l, n)
    var ref = 0.0lf
    for (i in range64(n)) {
        ref += double(w[i]) * double(xq[i]) * double(xs[i / 32l])
    }
    let got = dot_q51q8_scalar(unsafe(addr(kq[0])), unsafe(addr(ks[0])), unsafe(addr(xq[0])), unsafe(addr(xs[0])), n)
    let denom = max(abs(float(ref)), 1.0e-6)
    t |> success(abs(got - float(ref)) / denom < 1.0e-4, "dot_q51q8_scalar must match the fp64 plane-dequant reference")
}

def private q51_groupn_gate(t : T?) {
    let n = 704l
    let nb = n / 32l
    let d = 8l
    let nreg = 3l
    var kq : array<uint8>
    var ks : array<uint8>
    q51_fill_planes(kq, ks, nreg * d, n)
    var xq : array<int8>
    var xs : array<float>
    q51_fill_acts(xq, xs, nreg, n)
    var offs : array<int64>
    offs |> reserve(nreg * 2l)
    for (r in range64(nreg)) {   // (weight element off, activation element off) pairs
        offs |> push(r * d * n)
        offs |> push(r * n)
    }
    var y : array<float>
    y |> resize(nreg * d)
    matmul_q51q8_groupn(y, kq, ks, offs, nreg, xq, xs, n, d)
    unsafe {
        for (r in range64(nreg)) {
            for (row in range64(d)) {
                let want = dot_q51q8_scalar(addr(kq[(r * d + row) * nb * 20l]), addr(ks[(r * d + row) * nb * 4l]),
                    addr(xq[r * n]), addr(xs[r * (n / 32l)]), n)
                t |> success(y[r * d + row] == want, "q51 groupn row must bit-match the scalar row dot")
            }
        }
    }
}

def private q51_batch_gate(t : T?) {
    let n = 704l
    let nb = n / 32l
    let d = 8l
    let ntok = 5l
    let nreg = 2l
    var kq : array<uint8>
    var ks : array<uint8>
    q51_fill_planes(kq, ks, nreg * d, n)
    var xq : array<int8>
    var xs : array<float>
    q51_fill_acts(xq, xs, ntok, n)
    // batch off region 1's rows — the non-zero element woff pins the wrapper's /32*20 plane math
    var y : array<float>
    y |> resize(ntok * d)
    matmul_q51q8_batch(y, kq, ks, d * n, xq, xs, n, d, ntok)
    unsafe {
        for (tk in range64(ntok)) {
            for (row in range64(d)) {
                let want = dot_q51q8_scalar(addr(kq[(d + row) * nb * 20l]), addr(ks[(d + row) * nb * 4l]),
                    addr(xq[tk * n]), addr(xs[tk * (n / 32l)]), n)
                t |> success(y[tk * d + row] == want, "q51 batch cell must bit-match the scalar row dot")
            }
        }
    }
    // batch groupn: (woff, row0, cnt) triples — tokens 0..2 hit region 0, 2..5 region 1
    let offs <- [0l, 0l, 2l, d * n, 2l, 3l]
    var yg : array<float>
    yg |> resize(ntok * d)
    matmul_q51q8_batch_groupn(yg, kq, ks, offs, nreg, xq, xs, n, d)
    unsafe {
        for (r in range64(nreg)) {
            let row0 = offs[r * 3l + 1l]
            let cnt = offs[r * 3l + 2l]
            for (tk in range64(cnt)) {
                for (row in range64(d)) {
                    let want = dot_q51q8_scalar(addr(kq[(r * d + row) * nb * 20l]), addr(ks[(r * d + row) * nb * 4l]),
                        addr(xq[(row0 + tk) * n]), addr(xs[(row0 + tk) * (n / 32l)]), n)
                    t |> success(yg[(row0 + tk) * d + row] == want, "q51 batch-groupn cell must bit-match the scalar row dot")
                }
            }
        }
    }
}

// the grp<mr> q51 layout (the emitter family's plane): repack round-trip, the grp scalar walk,
// the grp expand twins, and the untouched disk-order tail rows — all bit-exact vs disk
def private q51_grp_gate(t : T?) {
    let n = 704l
    let nb = n / 32l
    let d = 26l   // 3 grp8 (6 grp4) groups + a 2-row disk-order tail
    var kq : array<uint8>
    var ks : array<uint8>
    q51_fill_planes(kq, ks, d, n)
    var xq : array<int8>
    var xs : array<float>
    q51_fill_acts(xq, xs, 1l, n)
    var xbs : array<int>
    xbs |> resize(int(nb))
    for (bi in range64(nb)) {
        var s = 0
        for (j in range64(32l)) {
            s += int(xq[bi * 32l + j])
        }
        xbs[bi] = s
    }
    var yref : array<float>
    yref |> resize(int(d))
    unsafe {
        for (r in range64(d)) {
            yref[r] = dot_q51q8_scalar(addr(kq[0]) + r * nb * 20l, addr(ks[0]) + r * nb * 4l, addr(xq[0]), addr(xs[0]), n)
        }
        var qe : array<int8>
        qe |> resize(int(n))
        var qe2 : array<int8>
        qe2 |> resize(int(n))
        var sc : array<uint8>
        sc |> resize(int(nb * 4l))
        for (mr in fixed_array(4l, 8l)) {
            var gq := kq
            var gs := ks
            repack_q51_grp(addr(gq[0]), addr(gs[0]), n, d, mr)
            let grpRows = (d / mr) * mr
            for (r in range64(d)) {
                if (r < grpRows) {
                    let gbase = (r / mr) * mr * nb
                    let got = q51_grp_row_dot(addr(gq[0]) + gbase * 20l, addr(gs[0]) + gbase * 4l, r % mr, mr,
                                              addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
                    t |> success(got == yref[r], "q51_grp_row_dot must bit-match the disk scalar dot")
                    expand_q51_grp_row(addr(gq[0]) + gbase * 20l, r % mr, mr, addr(qe[0]), nb)
                    gather_q51_grp_scales(addr(gs[0]) + gbase * 4l, r % mr, mr, addr(sc[0]), nb)
                    expand_q51_row(addr(kq[0]) + r * nb * 20l, addr(qe2[0]), nb)
                    let ge = dot_q51e(addr(qe[0]), addr(sc[0]), addr(xq[0]), addr(xs[0]), n)
                    let de = dot_q51e(addr(qe2[0]), addr(ks[0]) + r * nb * 4l, addr(xq[0]), addr(xs[0]), n)
                    t |> success(ge == de, "q51 grp expand twins must bit-match the disk expand")
                } else {
                    let got = dot_q51q8_scalar(addr(gq[0]) + r * nb * 20l, addr(gs[0]) + r * nb * 4l, addr(xq[0]), addr(xs[0]), n)
                    t |> success(got == yref[r], "q51 grp tail rows must stay disk-order")
                }
            }
            delete gq
            delete gs
        }
        delete qe
        delete qe2
        delete sc
    }
}

[test]
def test_q51_kernels(t : T?) {
    t |> run("dot_q51q8_scalar matches the fp64 plane-dequant reference at n=704") @(t : T?) {
        q51_dot_gate(t)
    }
    t |> run("q51 groupn bit-matches scalar row dots") @(t : T?) {
        q51_groupn_gate(t)
    }
    t |> run("q51 grp<mr> planes bit-match disk (repack + grp walk + expand twins + tails)") @(t : T?) {
        q51_grp_gate(t)
    }
    t |> run("q51 batch + batch-groupn bit-match scalar row dots") @(t : T?) {
        q51_batch_gate(t)
    }
}

// Kernel-vs-reference gate core: transcode nsb distinct superblocks of `fmt` (per-superblock
// LCG over the quant plane so an inter-superblock offset bug can't cancel), quantize a smooth
// activation with the bs quantizer, and compare the integer-domain kernel dot against the fp64
// elementwise dot over the plane dequant (the kernels' declared oracle).
def private kq_dot_gate(t : T?; fmt : int; n : int64 = 512l) {
    let nsb = n / 256l
    let blk1 <- fmt == 4 ? build_q4k_block() : (fmt == 5 ? build_q5k_block() : (fmt == 40 ? build_q40_block() : build_q6k_block()))
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(nsb * qsb)
    ks |> resize(nsb * ssb)
    for (s in range64(nsb)) {
        if (fmt == 4) {
            transcode_q4k_superblock(blk1, 0l, kq, s * qsb, ks, s * ssb)
        } elif (fmt == 5) {
            transcode_q5k_superblock(blk1, 0l, kq, s * qsb, ks, s * ssb)
        } elif (fmt == 40) {
            transcode_q40_superblock(blk1, 0l, kq, s * qsb, ks, s * ssb)
        } else {
            transcode_q6k_superblock(blk1, 0l, kq, s * qsb, ks, s * ssb)
        }
    }
    var st = 0x12345u
    for (i in range64(nsb * qsb)) {
        st = st * 1664525u + 1013904223u
        kq[i] = uint8(uint(kq[i]) ^ (st >> 16u))
    }
    // plane dequant = the reference weights
    var w : array<float>
    w |> resize(n)
    for (s in range64(nsb)) {
        if (fmt == 4) {
            dequant_k4_plane_superblock(kq, s * qsb, ks, s * ssb, w, s * 256l)
        } elif (fmt == 5) {
            dequant_k5_plane_superblock(kq, s * qsb, ks, s * ssb, w, s * 256l)
        } elif (fmt == 40) {
            dequant_q40_plane_superblock(kq, s * qsb, ks, s * ssb, w, s * 256l)
        } else {
            dequant_k6_plane_superblock(kq, s * qsb, ks, s * ssb, w, s * 256l)
        }
    }
    // activation: smooth signal -> bs quantizer (quants + scales + per-16 sums)
    var src : array<float>
    src |> resize(n)
    for (i in range64(n)) {
        src[i] = 1.7 * sin(float(i) * 0.23) + 0.3 * cos(float(i) * 0.071)
    }
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(n)
    xs |> resize(n / 32l)
    xbs |> resize(n / 16l)
    quantize_q8_k_into(src, n, xq, xs, xbs, 0l, 0l, 0l)   // kq v2: Q8_K-form activations, all formats
    // fp64 elementwise reference over the SAME quantized activation (Q8_K form: per-256 scale)
    let sdiv = 256l
    var ref = 0.0lf
    for (i in range64(n)) {
        ref += double(w[i]) * double(xq[i]) * double(xs[i / sdiv])
    }
    var got = 0.0
    unsafe {
        if (fmt == 4) {
            got = dot_k4q8(addr(kq[0]), addr(ks[0]), addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
        } elif (fmt == 5) {
            got = dot_k5q8(addr(kq[0]), addr(ks[0]), addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
        } elif (fmt == 40) {
            got = dot_q40q8(addr(kq[0]), addr(ks[0]), addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
        } else {
            got = dot_k6q8(addr(kq[0]), addr(ks[0]), addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
        }
    }
    let denom = max(abs(float(ref)), 1.0e-6)
    t |> success(abs(got - float(ref)) / denom < 1.0e-4, "kernel dot must match the fp64 plane-dequant reference")
}

[test]
def test_kq_dots(t : T?) {
    t |> run("dot_k4q8 matches the fp64 plane-dequant reference") @(t : T?) {
        kq_dot_gate(t, 4)
    }
    t |> run("dot_k5q8 matches the fp64 plane-dequant reference") @(t : T?) {
        kq_dot_gate(t, 5)
    }
    t |> run("dot_k6q8 matches the fp64 plane-dequant reference") @(t : T?) {
        kq_dot_gate(t, 6)
    }
    t |> run("dot_q40q8 matches the fp64 plane-dequant reference") @(t : T?) {
        kq_dot_gate(t, 40)
    }
    // 122B-class row widths: n=3072 (12 superblocks — qwen35moe-122B gate/up/cls) and n=1024
    // (its k5 down_exps). Every earlier kq model kept n at 512..2560.
    for (fmt in [4, 5, 6, 40]) {
        for (nn in [1024l, 3072l]) {
            t |> run("dot_{kq_tag(fmt)}q8 matches the fp64 reference at n={int(nn)}") @(t : T?) {
                kq_dot_gate(t, fmt, nn)
            }
        }
    }
}

// Portable multi-row GEMV gate: matmul_kq (the disk-order kq_gemv_kernel rail — what a
// kq-tagged model runs when the backend has no kq stamps) must bit-match the per-row disk
// dots. Guards the k*_rows_kernel PER-ROW plane strides, which the single-superblock dot
// gate above never exercises (a k4 scale-row stride miss shipped when K4_SSB went 32 -> 20:
// row 0 dotted clean, every later row read the wrong scale rows).
def private kq_gemv_rows_gate(t : T?; fmt : int; n : int64 = 512l) {
    let d = 8l
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let blk1 <- fmt == 4 ? build_q4k_block() : (fmt == 5 ? build_q5k_block() : (fmt == 40 ? build_q40_block() : build_q6k_block()))
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(int(d * nsb * qsb))
    ks |> resize(int(d * nsb * ssb))
    for (r in range64(d)) {
        for (s in range64(nsb)) {
            if (fmt == 4) {
                transcode_q4k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 5) {
                transcode_q5k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 40) {
                transcode_q40_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } else {
                transcode_q6k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            }
        }
        // per-row LCG over the quant payload only (scale planes stay valid f16/int8)
        var st = 0x9E3779Bu + uint(r) * 0x85EBCA6Bu
        for (i in range64(nsb * qsb)) {
            st = st * 1664525u + 1013904223u
            kq[r * nsb * qsb + i] = uint8(uint(kq[r * nsb * qsb + i]) ^ (st >> 16u))
        }
    }
    var src : array<float>
    src |> resize(n)
    for (i in range64(n)) {
        src[i] = 1.1 * sin(float(i) * 0.31) + 0.5 * cos(float(i) * 0.043)
    }
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(n)
    xs |> resize(n / 32l)
    xbs |> resize(n / 16l)
    quantize_q8_k_into(src, n, xq, xs, xbs, 0l, 0l, 0l)   // kq v2: Q8_K-form activations, all formats
    var y : array<float>
    y |> resize(int(d))
    matmul_kq(fmt, y, kq, ks, 0l, xq, xs, xbs, n, d)
    unsafe {
        var bad = 0
        for (r in range64(d)) {
            let kqr = addr(kq[r * nsb * qsb])
            let ksr = addr(ks[r * nsb * ssb])
            var want = 0.0
            if (fmt == 4) {
                want = dot_k4q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
            } elif (fmt == 5) {
                want = dot_k5q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
            } elif (fmt == 40) {
                want = dot_q40q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
            } else {
                want = dot_k6q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
            }
            if (y[r] != want) {
                bad++
            }
        }
        t |> success(bad == 0, "portable multi-row GEMV bit-matches the per-row disk dots")
    }
    delete kq
    delete ks
    delete src
    delete xq
    delete xs
    delete xbs
    delete y
}

[test]
def test_kq_gemv_rows(t : T?) {
    t |> run("portable k4 GEMV rows bit-match per-row disk dots") @(t : T?) {
        kq_gemv_rows_gate(t, 4)
    }
    t |> run("portable k5 GEMV rows bit-match per-row disk dots") @(t : T?) {
        kq_gemv_rows_gate(t, 5)
    }
    t |> run("portable k6 GEMV rows bit-match per-row disk dots") @(t : T?) {
        kq_gemv_rows_gate(t, 6)
    }
    t |> run("portable q40 GEMV rows bit-match per-row disk dots") @(t : T?) {
        kq_gemv_rows_gate(t, 40)
    }
    for (fmt in [4, 5, 6, 40]) {
        for (nn in [1024l, 3072l]) {
            t |> run("portable {kq_tag(fmt)} GEMV rows bit-match per-row disk dots at n={int(nn)}") @(t : T?) {
                kq_gemv_rows_gate(t, fmt, nn)
            }
        }
    }
}

// Stage-4 repack oracle core: build a d-row disk plane of `fmt` (transcoded synthetic
// superblocks, per-row LCG-perturbed quant payloads — every quant bit pattern is valid),
// capture the portable disk dots + row dequants, repack in place at `mr`, and require the
// grp<mr> scalar reference (kq_grp_row_dot — the stamped cores' decline body) and the grp row
// dequant to reproduce them BIT-EXACTLY (same block order, exact integer sub-sums — only the
// plane layout moved).
def private kq_repack_gate(t : T?; fmt : int; mr : int64; n : int64 = 512l) {
    let d = 32l
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let blk1 <- fmt == 4 ? build_q4k_block() : (fmt == 5 ? build_q5k_block() : (fmt == 40 ? build_q40_block() : build_q6k_block()))
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(int(d * nsb * qsb))
    ks |> resize(int(d * nsb * ssb))
    for (r in range64(d)) {
        for (s in range64(nsb)) {
            if (fmt == 4) {
                transcode_q4k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 5) {
                transcode_q5k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 40) {
                transcode_q40_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } else {
                transcode_q6k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            }
        }
        // per-row LCG over the quant payload only (scale planes stay valid f16/int8)
        var st = 0x9E3779Bu + uint(r) * 0x85EBCA6Bu
        for (i in range64(nsb * qsb)) {
            st = st * 1664525u + 1013904223u
            kq[r * nsb * qsb + i] = uint8(uint(kq[r * nsb * qsb + i]) ^ (st >> 16u))
        }
    }
    var src : array<float>
    src |> resize(n)
    for (i in range64(n)) {
        src[i] = 1.3 * sin(float(i) * 0.19) - 0.4 * cos(float(i) * 0.057)
    }
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(n)
    xs |> resize(n / 32l)
    xbs |> resize(n / 16l)
    quantize_q8_k_into(src, n, xq, xs, xbs, 0l, 0l, 0l)   // kq v2: Q8_K-form activations, all formats
    var want : array<float>
    var wrow : array<float>
    want |> resize(int(d))
    wrow |> resize(int(d * n))
    unsafe {
        for (r in range64(d)) {
            let kqr = addr(kq[r * nsb * qsb])
            let ksr = addr(ks[r * nsb * ssb])
            if (fmt == 4) {
                want[r] = dot_k4q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
            } elif (fmt == 5) {
                want[r] = dot_k5q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
            } elif (fmt == 40) {
                want[r] = dot_q40q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
            } else {
                want[r] = dot_k6q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
            }
            for (s in range64(nsb)) {
                if (fmt == 4) {
                    dequant_k4_plane_superblock(kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb, wrow, r * n + s * 256l)
                } elif (fmt == 5) {
                    dequant_k5_plane_superblock(kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb, wrow, r * n + s * 256l)
                } elif (fmt == 40) {
                    dequant_q40_plane_superblock(kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb, wrow, r * n + s * 256l)
                } else {
                    dequant_k6_plane_superblock(kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb, wrow, r * n + s * 256l)
                }
            }
        }
        if (fmt == 4) {
            repack_k4_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } elif (fmt == 5) {
            repack_k5_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } elif (fmt == 40) {
            repack_q40_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } else {
            repack_k6_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        }
        var dotbad = 0
        var deqbad = 0
        var row : array<float>
        row |> resize(n)
        for (r in range64(d)) {
            let g = r / mr
            let kqg = addr(kq[g * mr * nsb * qsb])
            let ksg = addr(ks[g * mr * nsb * ssb])
            let got = (fmt == 40
                ? q40_grp_row_dot(kqg, ksg, r % mr, mr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
                : kq_grp_row_dot(int64(fmt), kqg, ksg, r % mr, mr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n))
            if (got != want[r]) {
                dotbad++
            }
            dequant_kq_row_grp(int64(fmt), kqg, ksg, r % mr, mr, n, addr(row[0]))
            for (i in range64(n)) {
                if (row[i] != wrow[r * n + i]) {
                    deqbad++
                }
            }
        }
        t |> success(dotbad == 0, "grp<mr> row dots bit-match the disk dots after repack")
        t |> success(deqbad == 0, "grp<mr> row dequant bit-matches the disk row dequant after repack")
        delete row
    }
    delete kq
    delete ks
    delete want
    delete wrow
    delete src
    delete xq
    delete xs
    delete xbs
}

[test]
def test_kq_repack_grp(t : T?) {
    for (fmt in [4, 5, 6, 40]) {
        for (mr in [4l, 8l, 16l]) {
            t |> run("repack_{kq_tag(fmt)}_grp mr={mr} preserves dots and row dequants") @(t : T?) {
                kq_repack_gate(t, fmt, mr)
            }
        }
        for (nn in [1024l, 3072l]) {
            t |> run("repack_{kq_tag(fmt)}_grp mr=8 preserves dots and row dequants at n={int(nn)}") @(t : T?) {
                kq_repack_gate(t, fmt, 8l, nn)
            }
        }
    }
}

// Tile-vs-GEMV gate: the kq 4-token tile must be BIT-exact vs 4 per-token GEMVs over the same
// repacked planes (the tile's per-token fold order IS the GEMV's). Repack at the layout
// companion's mr — the same value the stubs/kernels read, so this holds on the reference
// bodies (interp) AND the stamped kernels (-jit) alike.
def private kq_tile_gate(t : T?; fmt : int; n : int64 = 512l) {
    let d = 32l
    let ntok = 7l   // 4-token tile + a 3-token gemv tail
    let nsb = n / 256l
    let mr = kq_layout_of(fmt)   // the format's OWN layout companion (per-format tune families)
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let blk1 <- fmt == 4 ? build_q4k_block() : (fmt == 5 ? build_q5k_block() : (fmt == 40 ? build_q40_block() : build_q6k_block()))
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(int(d * nsb * qsb))
    ks |> resize(int(d * nsb * ssb))
    for (r in range64(d)) {
        for (s in range64(nsb)) {
            if (fmt == 4) {
                transcode_q4k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 5) {
                transcode_q5k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 40) {
                transcode_q40_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } else {
                transcode_q6k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            }
        }
        var st = 0xDEADBEFu + uint(r) * 0x9E3779Bu
        for (i in range64(nsb * qsb)) {
            st = st * 1664525u + 1013904223u
            kq[r * nsb * qsb + i] = uint8(uint(kq[r * nsb * qsb + i]) ^ (st >> 16u))
        }
    }
    unsafe {
        if (fmt == 4) {
            repack_k4_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } elif (fmt == 5) {
            repack_k5_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } elif (fmt == 40) {
            repack_q40_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } else {
            repack_k6_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        }
    }
    // ntok-row activation image, bs-quantized per row
    var src : array<float>
    src |> resize(int(ntok * n))
    for (i in range64(ntok * n)) {
        src[i] = 0.9 * sin(float(i) * 0.31) + 0.5 * cos(float(i) * 0.043)
    }
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(int(ntok * n))
    xs |> resize(int(ntok * n / 32l))
    xbs |> resize(int(ntok * n / 16l))
    unsafe {
        for (p in range64(ntok)) {
            quantize_q8_k_into_ptr(addr(src[p * n]), n, addr(xq[0]), addr(xs[0]), addr(xbs[0]), p * n, p * (n / 256l), p * (n / 16l))
        }
    }
    var ytile : array<float>
    var ygemv : array<float>
    ytile |> resize(int(ntok * d))
    ygemv |> resize(int(ntok * d))
    // k5/k6 tiles read the byte-expanded panel the batch cell unpacks per (group, token-block)
    // — the gate covers unpack_kq_panel_grp too (tile-over-panel vs gemv-over-packed planes);
    // k4/q40 tiles read the packed planes directly
    let packed = fmt == 4 || fmt == 40
    var panel : array<uint8>
    if (!packed) {
        panel |> resize(int(mr * nsb * 256l))
    }
    unsafe {
        let ng = d / mr
        for (g in range64(ng)) {
            let kqg = addr(kq[g * mr * nsb * qsb])
            let ksg = addr(ks[g * mr * nsb * ssb])
            if (!packed) {
                unpack_kq_panel_grp(int64(fmt), kqg, addr(panel[0]), mr, nsb)
            }
            var tk = 0l
            while (tk + 4l <= ntok) {   // whole tiles
                if (fmt == 4) {
                    k4q8_tile_gen(addr(ytile[0]), kqg, ksg, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, d, g, tk)
                } elif (fmt == 5) {
                    k5q8_tile_gen(addr(ytile[0]), addr(panel[0]), ksg, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, d, g, tk)
                } elif (fmt == 40) {
                    q40q8_tile_gen(addr(ytile[0]), kqg, ksg, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, d, g, tk)
                } else {
                    k6q8_tile_gen(addr(ytile[0]), addr(panel[0]), ksg, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, d, g, tk)
                }
                tk += 4l
            }
            while (tk < ntok) {   // token tail via the rows core (the batch cell's shape)
                if (fmt == 4) {
                    k4q8_gemv_gen(addr(ytile[tk * d]), addr(kq[0]), addr(ks[0]), addr(xq[tk * n]), addr(xs[tk * (n / 256l)]), addr(xbs[tk * (n / 16l)]), n, g * mr, (g + 1l) * mr)
                } elif (fmt == 5) {
                    k5q8_gemv_gen(addr(ytile[tk * d]), addr(kq[0]), addr(ks[0]), addr(xq[tk * n]), addr(xs[tk * (n / 256l)]), addr(xbs[tk * (n / 16l)]), n, g * mr, (g + 1l) * mr)
                } elif (fmt == 40) {
                    q40q8_gemv_gen(addr(ytile[tk * d]), addr(kq[0]), addr(ks[0]), addr(xq[tk * n]), addr(xs[tk * (n / 256l)]), addr(xbs[tk * (n / 16l)]), n, g * mr, (g + 1l) * mr)
                } else {
                    k6q8_gemv_gen(addr(ytile[tk * d]), addr(kq[0]), addr(ks[0]), addr(xq[tk * n]), addr(xs[tk * (n / 256l)]), addr(xbs[tk * (n / 16l)]), n, g * mr, (g + 1l) * mr)
                }
                tk++
            }
        }
        for (p in range64(ntok)) {   // reference: one full GEMV per token
            if (fmt == 4) {
                k4q8_gemv_gen(addr(ygemv[p * d]), addr(kq[0]), addr(ks[0]), addr(xq[p * n]), addr(xs[p * (n / 256l)]), addr(xbs[p * (n / 16l)]), n, 0l, d)
            } elif (fmt == 5) {
                k5q8_gemv_gen(addr(ygemv[p * d]), addr(kq[0]), addr(ks[0]), addr(xq[p * n]), addr(xs[p * (n / 256l)]), addr(xbs[p * (n / 16l)]), n, 0l, d)
            } elif (fmt == 40) {
                q40q8_gemv_gen(addr(ygemv[p * d]), addr(kq[0]), addr(ks[0]), addr(xq[p * n]), addr(xs[p * (n / 256l)]), addr(xbs[p * (n / 16l)]), n, 0l, d)
            } else {
                k6q8_gemv_gen(addr(ygemv[p * d]), addr(kq[0]), addr(ks[0]), addr(xq[p * n]), addr(xs[p * (n / 256l)]), addr(xbs[p * (n / 16l)]), n, 0l, d)
            }
        }
    }
    var bad = 0
    for (i in range64(ntok * d)) {
        if (ytile[i] != ygemv[i]) {
            bad++
        }
    }
    t |> success(bad == 0, "kq tile bit-matches per-token GEMVs")
    delete kq
    delete ks
    delete panel
    delete src
    delete xq
    delete xs
    delete xbs
    delete ytile
    delete ygemv
}

[test]
def test_kq_tile(t : T?) {
    for (fmt in [4, 5, 6, 40]) {
        t |> run("{kq_tag(fmt)} 4-token tile bit-matches per-token GEMVs") @(t : T?) {
            kq_tile_gate(t, fmt)
        }
        for (nn in [1024l, 3072l]) {
            t |> run("{kq_tag(fmt)} 4-token tile bit-matches per-token GEMVs at n={int(nn)}") @(t : T?) {
                kq_tile_gate(t, fmt, nn)
            }
        }
    }
}

// Groupn gate: the region-list kq GEMV (the MoE expert dispatch) must be BIT-exact vs per-row
// disk dots — disk-order (the portable matmul_kq_groupn) AND repacked (kq_groupn_gen over
// per-region grp slices, the per-expert-slice repack shape). Regions carry distinct LCG
// payloads; both offs forms run — the shared activation image (gate/up, xoff 0) and
// per-region activation rows (down, xoff = r*n).
def private kq_groupn_gate(t : T?; fmt : int; n : int64 = 512l; d : int64 = 32l; nreg : int64 = 3l) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let blk1 <- fmt == 4 ? build_q4k_block() : (fmt == 5 ? build_q5k_block() : (fmt == 40 ? build_q40_block() : build_q6k_block()))
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(int(nreg * d * nsb * qsb))
    ks |> resize(int(nreg * d * nsb * ssb))
    for (r in range64(nreg * d)) {
        for (s in range64(nsb)) {
            if (fmt == 4) {
                transcode_q4k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 5) {
                transcode_q5k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 40) {
                transcode_q40_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } else {
                transcode_q6k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            }
        }
        var st = 0x51ED270Bu + uint(r) * 0x85EBCA6Bu
        for (i in range64(nsb * qsb)) {
            st = st * 1664525u + 1013904223u
            kq[r * nsb * qsb + i] = uint8(uint(kq[r * nsb * qsb + i]) ^ (st >> 16u))
        }
    }
    // nreg activation rows, bs-quantized per row
    var src : array<float>
    src |> resize(int(nreg * n))
    for (i in range64(nreg * n)) {
        src[i] = 1.1 * sin(float(i) * 0.23) - 0.6 * cos(float(i) * 0.071)
    }
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(int(nreg * n))
    xs |> resize(int(nreg * n / 32l))
    xbs |> resize(int(nreg * n / 16l))
    unsafe {
        for (p in range64(nreg)) {
            quantize_q8_k_into_ptr(addr(src[p * n]), n, addr(xq[0]), addr(xs[0]), addr(xbs[0]), p * n, p * (n / 256l), p * (n / 16l))
        }
    }
    var offs_sh : array<int64>
    var offs_pr : array<int64>
    offs_sh |> reserve(int(nreg * 2l))
    offs_pr |> reserve(int(nreg * 2l))
    for (r in range64(nreg)) {
        offs_sh |> push(r * d * n)
        offs_sh |> push(0l)
        offs_pr |> push(r * d * n)
        offs_pr |> push(r * n)
    }
    // references: per-row disk dots for both activation bindings
    var want_sh : array<float>
    var want_pr : array<float>
    want_sh |> resize(int(nreg * d))
    want_pr |> resize(int(nreg * d))
    unsafe {
        for (r in range64(nreg)) {
            for (row in range64(d)) {
                let sb = (r * d + row) * nsb
                let kqr = addr(kq[sb * qsb])
                let ksr = addr(ks[sb * ssb])
                if (fmt == 4) {
                    want_sh[r * d + row] = dot_k4q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
                    want_pr[r * d + row] = dot_k4q8(kqr, ksr, addr(xq[r * n]), addr(xs[r * (n / 256l)]), addr(xbs[r * (n / 16l)]), n)
                } elif (fmt == 5) {
                    want_sh[r * d + row] = dot_k5q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
                    want_pr[r * d + row] = dot_k5q8(kqr, ksr, addr(xq[r * n]), addr(xs[r * (n / 256l)]), addr(xbs[r * (n / 16l)]), n)
                } elif (fmt == 40) {
                    want_sh[r * d + row] = dot_q40q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
                    want_pr[r * d + row] = dot_q40q8(kqr, ksr, addr(xq[r * n]), addr(xs[r * (n / 256l)]), addr(xbs[r * (n / 16l)]), n)
                } else {
                    want_sh[r * d + row] = dot_k6q8(kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n)
                    want_pr[r * d + row] = dot_k6q8(kqr, ksr, addr(xq[r * n]), addr(xs[r * (n / 256l)]), addr(xbs[r * (n / 16l)]), n)
                }
            }
        }
    }
    var y : array<float>
    y |> resize(int(nreg * d))
    var bad_dsh = 0
    var bad_dpr = 0
    matmul_kq_groupn(fmt, y, kq, ks, offs_sh, nreg, xq, xs, xbs, n, d)
    for (i in range64(nreg * d)) {
        if (y[i] != want_sh[i]) {
            bad_dsh++
        }
    }
    matmul_kq_groupn(fmt, y, kq, ks, offs_pr, nreg, xq, xs, xbs, n, d)
    for (i in range64(nreg * d)) {
        if (y[i] != want_pr[i]) {
            bad_dpr++
        }
    }
    t |> success(bad_dsh == 0, "disk groupn bit-matches per-row dots (shared image)")
    t |> success(bad_dpr == 0, "disk groupn bit-matches per-row dots (per-region rows)")
    // per-region grp repack (the per-expert-slice shape), then the gen driver over the slices.
    // The grp reference is the ROWS CORE itself (one full-region call per region): under -jit
    // that's the stamped kernel, whose float fold order differs from the portable dots at the
    // last bit — the groupn driver's job is the region walk, so THAT must be bit-exact vs the
    // per-region core calls (interp and stamped alike). Rows-core-vs-disk exactness for the
    // das reference body is kq_repack_gate's gate.
    let mr = kq_layout_of(fmt)   // the format's OWN layout companion (per-format tune families)
    var bad_gsh = 0
    var bad_gpr = 0
    unsafe {
        for (r in range64(nreg)) {
            if (fmt == 4) {
                repack_k4_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
            } elif (fmt == 5) {
                repack_k5_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
            } elif (fmt == 40) {
                repack_q40_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
            } else {
                repack_k6_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
            }
        }
        for (r in range64(nreg)) {   // grp references: one direct rows-core call per region, both bindings
            let kqr = addr(kq[r * d * nsb * qsb])
            let ksr = addr(ks[r * d * nsb * ssb])
            if (fmt == 4) {
                k4q8_gemv_gen(addr(want_sh[r * d]), kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, 0l, d)
                k4q8_gemv_gen(addr(want_pr[r * d]), kqr, ksr, addr(xq[r * n]), addr(xs[r * (n / 256l)]), addr(xbs[r * (n / 16l)]), n, 0l, d)
            } elif (fmt == 5) {
                k5q8_gemv_gen(addr(want_sh[r * d]), kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, 0l, d)
                k5q8_gemv_gen(addr(want_pr[r * d]), kqr, ksr, addr(xq[r * n]), addr(xs[r * (n / 256l)]), addr(xbs[r * (n / 16l)]), n, 0l, d)
            } elif (fmt == 40) {
                q40q8_gemv_gen(addr(want_sh[r * d]), kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, 0l, d)
                q40q8_gemv_gen(addr(want_pr[r * d]), kqr, ksr, addr(xq[r * n]), addr(xs[r * (n / 256l)]), addr(xbs[r * (n / 16l)]), n, 0l, d)
            } else {
                k6q8_gemv_gen(addr(want_sh[r * d]), kqr, ksr, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, 0l, d)
                k6q8_gemv_gen(addr(want_pr[r * d]), kqr, ksr, addr(xq[r * n]), addr(xs[r * (n / 256l)]), addr(xbs[r * (n / 16l)]), n, 0l, d)
            }
        }
        kq_groupn_gen(fmt, addr(y[0]), addr(kq[0]), addr(ks[0]), addr(offs_sh[0]), nreg, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, d)
        for (i in range64(nreg * d)) {
            if (y[i] != want_sh[i]) {
                bad_gsh++
            }
        }
        kq_groupn_gen(fmt, addr(y[0]), addr(kq[0]), addr(ks[0]), addr(offs_pr[0]), nreg, addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, d)
        for (i in range64(nreg * d)) {
            if (y[i] != want_pr[i]) {
                bad_gpr++
            }
        }
    }
    t |> success(bad_gsh == 0, "grp groupn bit-matches per-region rows-core calls (shared image)")
    t |> success(bad_gpr == 0, "grp groupn bit-matches per-region rows-core calls (per-region rows)")
    delete kq
    delete ks
    delete src
    delete xq
    delete xs
    delete xbs
    delete offs_sh
    delete offs_pr
    delete want_sh
    delete want_pr
    delete y
}

[test]
def test_kq_groupn(t : T?) {
    for (fmt in [4, 5, 6, 40]) {
        t |> run("{kq_tag(fmt)} region-list GEMV bit-matches per-row dots (disk + grp slices)") @(t : T?) {
            kq_groupn_gate(t, fmt)
        }
        for (nn in [1024l, 3072l]) {
            t |> run("{kq_tag(fmt)} region-list GEMV bit-matches per-row dots at n={int(nn)}") @(t : T?) {
                kq_groupn_gate(t, fmt, nn)
            }
        }
    }
    // the qwen35moe-122B expert slice shapes: gate/up k4 n=3072 d=1024, down k5 n=1024 d=3072,
    // top-8 regions — the small-d arms above never walk >4 mr-groups per region
    t |> run("k4 region-list GEMV at the 122B gate/up slice shape (n=3072 d=1024 nreg=8)") @(t : T?) {
        kq_groupn_gate(t, 4, 3072l, 1024l, 8l)
    }
    t |> run("k5 region-list GEMV at the 122B down slice shape (n=1024 d=3072 nreg=8)") @(t : T?) {
        kq_groupn_gate(t, 5, 1024l, 3072l, 8l)
    }
}

// Batch-groupn gate: the fused MoE prefill's kq batch GEMM (matmul_kq_batch_groupn — triple offs
// (woff, row0, cnt), token-major y over the gathered image) must bit-match per-(region, token)
// rows-core calls over the same repacked planes. This drives kq_batch_cell_gen's TB/tile/tail
// walk over bucket-shaped token runs — the rail no per-region gate above reaches.
def private kq_batch_groupn_gate(t : T?; fmt : int; n : int64 = 512l; d : int64 = 32l) {
    let prev_backend = active_kernel_backend()
    // matmul_kq_batch_groupn dispatches through the active backend and only the gen tier carries
    // the slot; a failed pin (interp, no avx2, declined stamp) keeps the previous backend un-pinned
    pin_kernel_backend(get_architecture_name() == "arm64" ? "arm64-gen" : "x64-gen")
    if (!kernel_backend_has_kq_batch_groupn()) {
        t |> skip("no gen backend registered (kq_batch_groupn is JIT-only)")
        return
    }
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let mr = kq_layout_of(fmt)
    // bucket mix: a 1-row run (pure gemv tail), whole tiles, tile+tail, and a TB-spanning run
    var cnts <- [1l, 4l, 6l, 13l]
    let nreg = long_length(cnts)
    var nk = 0l
    for (c in cnts) {
        nk += c
    }
    let blk1 <- fmt == 4 ? build_q4k_block() : (fmt == 5 ? build_q5k_block() : (fmt == 40 ? build_q40_block() : build_q6k_block()))
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(int(nreg * d * nsb * qsb))
    ks |> resize(int(nreg * d * nsb * ssb))
    for (r in range64(nreg * d)) {
        for (s in range64(nsb)) {
            if (fmt == 4) {
                transcode_q4k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 5) {
                transcode_q5k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 40) {
                transcode_q40_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } else {
                transcode_q6k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            }
        }
        var st = 0x7F4A7C15u + uint(r) * 0x85EBCA6Bu
        for (i in range64(nsb * qsb)) {
            st = st * 1664525u + 1013904223u
            kq[r * nsb * qsb + i] = uint8(uint(kq[r * nsb * qsb + i]) ^ (st >> 16u))
        }
    }
    unsafe {
        for (r in range64(nreg)) {   // per-region grp repack — the per-expert-slice shape
            if (fmt == 4) {
                repack_k4_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
            } elif (fmt == 5) {
                repack_k5_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
            } elif (fmt == 40) {
                repack_q40_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
            } else {
                repack_k6_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
            }
        }
    }
    // gathered activation image: nk rows, Q8_K-quantized per row
    var src : array<float>
    src |> resize(int(nk * n))
    for (i in range64(nk * n)) {
        src[i] = 0.8 * sin(float(i) * 0.29) - 0.4 * cos(float(i) * 0.057)
    }
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(int(nk * n))
    xs |> resize(int(nk * n / 256l))
    xbs |> resize(int(nk * n / 16l))
    unsafe {
        for (p in range64(nk)) {
            quantize_q8_k_into_ptr(addr(src[p * n]), n, addr(xq[0]), addr(xs[0]), addr(xbs[0]), p * n, p * (n / 256l), p * (n / 16l))
        }
    }
    // region triples + per-(region, token) rows-core references
    var offs : array<int64>
    offs |> reserve(nreg * 3l)
    var want : array<float>
    var y : array<float>
    want |> resize(int(nk * d))
    y |> resize(int(nk * d))
    var row0 = 0l
    unsafe {
        for (r in range64(nreg)) {
            offs |> push(r * d * n)   // nolint:STYLE012 — triple stream, not a collectable comprehension
            offs |> push(row0)
            offs |> push(cnts[r])
            let kqr = addr(kq[r * d * nsb * qsb])
            let ksr = addr(ks[r * d * nsb * ssb])
            for (tk in range64(row0, row0 + cnts[r])) {
                if (fmt == 4) {
                    k4q8_gemv_gen(addr(want[tk * d]), kqr, ksr, addr(xq[tk * n]), addr(xs[tk * (n / 256l)]), addr(xbs[tk * (n / 16l)]), n, 0l, d)
                } elif (fmt == 5) {
                    k5q8_gemv_gen(addr(want[tk * d]), kqr, ksr, addr(xq[tk * n]), addr(xs[tk * (n / 256l)]), addr(xbs[tk * (n / 16l)]), n, 0l, d)
                } elif (fmt == 40) {
                    q40q8_gemv_gen(addr(want[tk * d]), kqr, ksr, addr(xq[tk * n]), addr(xs[tk * (n / 256l)]), addr(xbs[tk * (n / 16l)]), n, 0l, d)
                } else {
                    k6q8_gemv_gen(addr(want[tk * d]), kqr, ksr, addr(xq[tk * n]), addr(xs[tk * (n / 256l)]), addr(xbs[tk * (n / 16l)]), n, 0l, d)
                }
            }
            row0 += cnts[r]
        }
    }
    matmul_kq_batch_groupn(fmt, y, kq, ks, offs, nreg, xq, xs, xbs, n, d)
    var bad = 0
    for (i in range64(nk * d)) {
        if (y[i] != want[i]) {
            bad++
        }
    }
    t |> success(bad == 0, "kq batch groupn bit-matches per-(region, token) rows-core calls")
    clear_kernel_backend_pin()   // restore without leaving a pin that wasn't there on entry
    select_kernel_backend(prev_backend)
    delete cnts
    delete kq
    delete ks
    delete src
    delete xq
    delete xs
    delete xbs
    delete offs
    delete want
    delete y
}

[test]
def test_kq_batch_groupn(t : T?) {
    for (fmt in [4, 5, 6, 40]) {
        t |> run("{kq_tag(fmt)} batch groupn bit-matches per-token rows-core calls") @(t : T?) {
            kq_batch_groupn_gate(t, fmt)
        }
    }
    // the qwen35moe-122B fused-prefill slice shapes
    t |> run("k4 batch groupn at the 122B gate/up slice shape (n=3072 d=1024)") @(t : T?) {
        kq_batch_groupn_gate(t, 4, 3072l, 1024l)
    }
    t |> run("k5 batch groupn at the 122B down slice shape (n=1024 d=3072)") @(t : T?) {
        kq_batch_groupn_gate(t, 5, 1024l, 3072l)
    }
}

[test]
def test_quantize_bs(t : T?) {
    t |> run("bs quantizer matches quantize_q8_0 byte-for-byte plus exact per-16 sums") @(t : T?) {
        let n = 4096l
        var src : array<float>
        src |> resize(n)
        for (i in range64(n)) {
            src[i] = 2.0 * sin(float(i) * 0.37) - 0.11 * float(i % 13l)
        }
        let want <- quantize_q8_0(src, n)
        var qdst : array<int8>
        var sdst : array<float>
        var bdst : array<int>
        qdst |> resize(n)
        sdst |> resize(n / 32l)
        bdst |> resize(n / 16l)
        quantize_q8_0_bs_into(src, n, qdst, sdst, bdst, 0l, 0l, 0l)
        var qbad = 0
        var sbad = 0
        var bbad = 0
        for (j in range64(n)) {
            if (qdst[j] != want.q[j]) {
                qbad++
            }
        }
        for (b in range64(n / 32l)) {
            if (sdst[b] != want.scales[b]) {
                sbad++
            }
        }
        for (h in range64(n / 16l)) {
            var bs = 0
            for (i in range64(16l)) {
                bs += int(want.q[h * 16l + i])
            }
            if (bdst[h] != bs) {
                bbad++
            }
        }
        t |> success(qbad == 0, "quants byte-identical to quantize_q8_0")
        t |> success(sbad == 0, "scales bit-identical to quantize_q8_0")
        t |> success(bbad == 0, "per-16 sums exact")
    }
}

// Dequant `n` elements from a Model kq plane starting at plane-local element offset `eloff`.
def private dequant_plane_prefix(tr : Model; fmt : KqFmt; eloff, n : int64; var dst : array<float>) {
    for (blk in range64(n / 256l)) {
        let sb = (eloff + blk * 256l) / 256l
        if (fmt == KqFmt.k4) {
            dequant_k4_plane_superblock(tr.k4q, sb * K4_QSB, tr.k4s, sb * K4_SSB, dst, blk * 256l)
        } elif (fmt == KqFmt.k5) {
            dequant_k5_plane_superblock(tr.k5q, sb * K5_QSB, tr.k5s, sb * K5_SSB, dst, blk * 256l)
        } elif (fmt == KqFmt.k6) {
            dequant_k6_plane_superblock(tr.k6q, sb * K6_QSB, tr.k6s, sb * K6_SSB, dst, blk * 256l)
        } elif (fmt == KqFmt.q40) {
            dequant_q40_plane_superblock(tr.q40q, sb * Q40_QSB, tr.q40s, sb * Q40_SSB, dst, blk * 256l)
        } else {
            panic("dequant_plane_prefix: not a kq format")
        }
    }
}

[test]
def test_kq_load_layout(t : T?) {
    // End-to-end stage-2 oracle: a kquant_native load must place every tagged tensor at its
    // cursor-assigned plane offset. Plane-dequant of tensor prefixes vs the file dequant —
    // exact for k6 (verbatim planes), quant-noise-free but f16-pair-rounded for k4 (rel RMS
    // bound catches any misplaced superblock loudly; the synthetic arms above pin the bit level).
    t |> run("kquant_native load places tensors at their cursor offsets (Qwen3-4B Q4_K_M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "Qwen3-4B-Instruct-2507-Q4_K_M.gguf")
        if (!model_available(t, path)) {
            return
        }
        // this arm is the stage-2 CURSOR-PLACEMENT oracle over disk-order planes — pin the
        // portable backend so a kq-capable gen backend can't repack them at load (stage 4)
        pin_kernel_backend("portable")
        let prev_kq = get_kquant_native()
        set_kquant_native(true)
        var tr <- load_gguf(path, QuantMode.q8)
        set_kquant_native(prev_kq)
        clear_kernel_backend_pin()
        t |> success(!tr.kq_repacked, "portable-pinned load keeps the kq planes disk-order")
        t |> success(tr.kquant_native, "load detected native K-quant tensors")
        t |> success(cls_kq(tr), "tied Q6_K classifier takes the kq plane path")
        t |> success(tr.wq_fmt[0] == KqFmt.k4, "blk.0.attn_q tagged k4")
        t |> success(tr.wv_fmt[0] == KqFmt.k6, "blk.0.attn_v tagged k6")
        let n = 262144l
        var have : array<float>
        have |> resize(n)
        // deep-layer k4 tensor: catches cursor drift accumulated across every earlier tensor
        let l = tr.config.n_layers - 1l
        dequant_plane_prefix(tr, tr.wq_fmt[l], tr.wq_offs[l], n, have)
        var gt = 0
        var fdq <- read_prefix(path, "blk.{l}.attn_q.weight", n, gt)
        let r4 = rel_rms(have, fdq)
        t |> success(r4 < 1.0e-3, "deep-layer k4 plane matches the file dequant (f16-pair rounding only)")
        delete fdq
        // k6 (verbatim planes): the tied classifier row block — must be bit-exact
        var fdq6 <- read_prefix(path, "token_embd.weight", n, gt)
        dequant_plane_prefix(tr, tr.emb_fmt, tr.wcls_off, n, have)
        var bad = 0
        for (i in range64(n)) {
            if (have[i] != fdq6[i]) {
                bad++
            }
        }
        t |> success(bad == 0, "k6 classifier plane bit-matches the file dequant")
        delete fdq6
        delete have
        delete tr
    }
    // the q40 tier's end-to-end placement oracle: the QAT gemma file is 265 x Q4_0 + F32
    // norms/router + one Q6_K embd — its q40 planes are VERBATIM disk splits, so every
    // plane-dequant prefix must bit-match the file dequant (the k6 exactness class).
    t |> run("q40 load places Q4_0 tensors at their cursor offsets (gemma-4-26B QAT q4_0)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "gemma-4-26B_q4_0-it.gguf")
        if (!model_available(t, path)) {
            return
        }
        pin_kernel_backend("portable")   // disk-order planes (the placement oracle)
        let prev_kq = get_kquant_native()
        let prev_q40 = get_kq_q40_native()
        set_kquant_native(true)
        set_kq_q40_native(true)
        var tr <- load_gguf(path, QuantMode.q8)
        set_kquant_native(prev_kq)
        set_kq_q40_native(prev_q40)
        clear_kernel_backend_pin()
        t |> success(!tr.kq_repacked, "portable-pinned load keeps the q40 planes disk-order")
        t |> success(tr.kquant_native, "load detected native Q4_0 tensors")
        t |> success(tr.wq_fmt[0] == KqFmt.q40, "blk.0.attn_q tagged q40")
        let n = 262144l
        var have : array<float>
        have |> resize(n)
        var gt = 0
        // deep-layer q40 tensor: catches cursor drift accumulated across every earlier tensor
        let l = tr.config.n_layers - 1l
        dequant_plane_prefix(tr, tr.wq_fmt[l], tr.wq_offs[l], n, have)
        var fdq <- read_prefix(path, "blk.{l}.attn_q.weight", n, gt)
        t |> equal(gt, GGML_TYPE_Q4_0)
        var bad = 0
        for (i in range64(n)) {
            if (have[i] != fdq[i]) {
                bad++
            }
        }
        t |> success(bad == 0, "deep-layer q40 plane bit-matches the file dequant (verbatim split)")
        delete fdq
        delete have
        delete tr
    }
}

[test]
def test_kq_fused_qkv_slices(t : T?) {
    // Fused-projection (Phi3) gate-lift oracle: attn_qkv / ffn_up split at load with dim-multiple
    // src_offs into the kq planes — a slice landing off by one superblock shows up as noise vs
    // the file dequant. The K slice (src_off = dim*qd) is the non-trivial placement.
    t |> run("phi3 fused attn_qkv K-quant slices land at their plane offsets (Phi-3.5 Q4_K_M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "Phi-3.5-mini-instruct-Q4_K_M.gguf")
        if (!model_available(t, path)) {
            return
        }
        pin_kernel_backend("portable")   // disk-order planes (the placement oracle, like test_kq_load_layout)
        let prev_kq = get_kquant_native()
        set_kquant_native(true)
        var tr <- load_gguf(path, QuantMode.q8)
        set_kquant_native(prev_kq)
        clear_kernel_backend_pin()
        t |> success(tr.kquant_native, "load detected native K-quant tensors")
        t |> success(tr.wq_fmt[0] == KqFmt.k5 && tr.wk_fmt[0] == KqFmt.k5 && tr.wv_fmt[0] == KqFmt.k5,
            "fused attn_qkv slices share the Q5_K tag")
        t |> success(tr.w1_fmt[0] == KqFmt.k4 && tr.w3_fmt[0] == KqFmt.k4, "fused ffn_up gate/up slices tagged k4")
        t |> success(tr.wcls_fmt == KqFmt.k6 && !cls_kq(tr), "untied Q6_K classifier tagged (no tied-cls path)")
        let dim = tr.config.dim
        let qd = layer_qd(tr.config, 0l)
        let n = 262144l
        var have : array<float>
        have |> resize(n)
        var gt = 0
        // q slice (src_off 0): the trivial placement
        dequant_plane_prefix(tr, tr.wq_fmt[0], tr.wq_offs[0], n, have)
        var fq <- read_prefix(path, "blk.0.attn_qkv.weight", dim * qd + n, gt)
        var want : array<float>
        want |> resize(n)
        for (i in range64(n)) {
            want[i] = fq[i]
        }
        t |> success(rel_rms(have, want) < 1.0e-3, "q slice matches the file dequant (f16-pair rounding only)")
        // k slice (src_off = dim*qd): the fused-split placement this arm exists for
        dequant_plane_prefix(tr, tr.wk_fmt[0], tr.wk_offs[0], n, have)
        for (i in range64(n)) {
            want[i] = fq[dim * qd + i]
        }
        t |> success(rel_rms(have, want) < 1.0e-3, "k slice at src_off dim*qd matches the file dequant")
        delete fq
        delete want
        delete have
        delete tr
    }
}

[test]
def test_kq_ple_table(t : T?) {
    // E-series PLE token table (family c): gather-only tensor rides native kq planes — the tag
    // must take (a silent q8 fallback also passes parity, so pin it here) and whole-row plane
    // dequant must match the file dequant at its offset.
    t |> run("E-series PLE token table takes the kq plane path (gemma-4-E2B Q4_K_M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
        if (!model_available(t, path)) {
            return
        }
        pin_kernel_backend("portable")   // disk-order planes (the placement oracle)
        let prev_kq = get_kquant_native()
        set_kquant_native(true)
        var tr <- load_gguf(path, QuantMode.q8)
        set_kquant_native(prev_kq)
        clear_kernel_backend_pin()
        t |> success(tr.kquant_native, "load detected native K-quant tensors")
        t |> success(tr.ple_emb_fmt == KqFmt.k5, "per_layer_token_embd tagged k5")
        t |> success(tr.emb_fmt == KqFmt.k4 && cls_kq(tr), "tied k4 classifier takes the kq plane path")
        // whole-row placement: rows 0..R of the table vs the file dequant (f16-pair rounding only)
        let all = tr.config.n_layers * tr.config.n_embd_per_layer
        let n = all * 16l
        var have : array<float>
        have |> resize(n)
        dequant_plane_prefix(tr, tr.ple_emb_fmt, tr.ple_emb_off, n, have)
        var gt = 0
        var fdq <- read_prefix(path, "per_layer_token_embd.weight", n, gt)
        t |> success(rel_rms(have, fdq) < 1.0e-3, "PLE table plane rows match the file dequant")
        delete fdq
        delete have
        delete tr
    }
}

// Dequant the first `n` elements of `name` into fp32; reports the on-disk type via `gt`.
def private read_prefix(path, name : string; n : int64; var gt : int&) : array<float> {
    var dst : array<float>
    dst |> resize(n)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("cannot open {path}")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var m <- parse_gguf_meta(bytes)
        gt = gguf_tensor_type(m, name)
        gguf_read_tensor_f32(m, bytes, name, dst, 0l, n)
        delete m
    }
    fclose(f)
    return <- dst
}

// Relative RMS of (a - b) against RMS(b), double accumulation.
def private rel_rms(a, b : array<float>) : float {
    var num = 0.0lf
    var den = 0.0lf
    for (x, y in a, b) {
        let dxy = double(x) - double(y)
        num += dxy * dxy
        den += double(y) * double(y)
    }
    return float(sqrt(num / den))
}

