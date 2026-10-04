// The K-quant and q8 planes the CPU kernel cells feed their kernels - the disk superblocks (the synthetic ones whose
// every element a gate's formula knows, and the hash-patterned ones), the per-row salted plane regions, the Q8_K and
// Q8_0 activation rows - and the fp64 q8 references that hold them, with no fixture root beside them, so a model-free
// file requires this and never `_model_tier`.

options gen2
options _dasllama_internal = true

module _kq_fixtures public

require dasllama/dasllama_lint public
require dasllama/dasllama_convert   // kq_transcode_superblock, requant_rows_q8k_bs
require dasllama/dasllama_gemm_schema   // kq_qsb / kq_ssb by format id
require dasllama/dasllama_kqformat
require math
require _kq_dot public   // dot_kq on the run-time format - the reference the kq cells hold the kernels to
require _compares   // opaque: the activation fill's taint seam
require _vk_kq_fixtures public

//! one Q4_K superblock in the GGUF's disk layout: two f16 scales, 12 bytes of 6-bit sub-scales, 128 bytes of nibbles
def q4k_disk_superblock() : array<uint8> {
    var b : array<uint8>
    b |> resize(144l)
    put_f16(b, 0l, 0.375)
    put_f16(b, 2l, 0.125)
    for (i in range64(12l)) {
        b[4l + i] = uint8((i * 37l + 13l) % 256l)
    }
    for (i in range64(128l)) {
        b[16l + i] = uint8((i * 29l + 7l) % 256l)
    }
    return <- b
}

//! one Q6_K superblock in the GGUF's disk layout, hash-patterned: 192 bytes of ql/qh, 16 signed sub-scales, f16 d = 0.25
def build_q6k_disk() : array<uint8> {
    var b : array<uint8>
    b |> resize(210l)
    for (i in range64(192l)) {
        b[i] = uint8((i * 23l + 5l) % 256l)
    }
    for (i in range64(16l)) {
        b[192l + i] = uint8((i * 41l + 11l) % 256l)
    }
    put_f16(b, 208l, 0.25)
    return <- b
}

//! eight q4_0 blocks in the GGUF's disk layout, hash-patterned nibbles, f16 d = (block + 1) / 16
def build_q40_disk() : array<uint8> {
    var b : array<uint8>
    b |> resize(144l)
    for (blk in range64(8l)) {
        put_f16(b, blk * 18l, 0.0625 * float(blk + 1l))
        for (j in range64(16l)) {
            b[blk * 18l + 2l + j] = uint8(((blk * 16l + j) * 19l + 3l) % 256l)
        }
    }
    return <- b
}

//! `rows` rows of `n` elements of format `fmt` as the GGUF transcoder writes them: disk superblock `blk` everywhere, each
//! row's quant bytes salted by an LCG walk off `seed` + row x `step`
def kq_region_of(fmt : int; blk : array<uint8>; var kq : array<uint8>; var ks : array<uint8>; rows, n : int64; seed, step : uint) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let kf = kq_fmt_of_id(fmt, "kq_region_of")
    kq |> resize(rows * nsb * qsb)
    ks |> resize(rows * nsb * ssb)
    for (r in range64(rows)) {
        for (s in range64(nsb)) {
            kq_transcode_superblock(kf, blk, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
        }
        var st = seed + uint(r) * step
        for (i in range64(nsb * qsb)) {
            st = st * 1664525u + 1013904223u
            kq[r * nsb * qsb + i] = uint8(uint(kq[r * nsb * qsb + i]) ^ (st >> 16u))
        }
    }
}

//! `kq_region_of` for format `fmt` (4, 6 or 40) over its hash-patterned superblock, rows salted at a step of 0x85EBCA6B
def build_kq_region(fmt : int; var kq : array<uint8>; var ks : array<uint8>; rows, n : int64; seed : uint) {
    var inscope blk <- fmt == 4 ? q4k_disk_superblock() : (fmt == 40 ? build_q40_disk() : build_q6k_disk())
    kq_region_of(fmt, blk, kq, ks, rows, n, seed, 0x85EBCA6Bu)
}

//! `kq_region_of` over format `fmt`'s synthetic superblock (`synth_block`)
def synth_kq_region(fmt : int; var kq : array<uint8>; var ks : array<uint8>; rows, n : int64; seed, step : uint) {
    var inscope blk <- synth_block(fmt)
    kq_region_of(fmt, blk, kq, ks, rows, n, seed, step)
}

//! `a * sin(i * fa) + b * cos(i * fb)` at element i - an activation fill's closed form
struct Wave {
    a : float
    fa : float
    b : float
    fb : float
}

def wave_at(w : Wave; i : int64) : float => w.a * sin(float(i) * w.fa) + w.b * cos(float(i) * w.fb)

//! Q8_K activation rows: `src`'s rows of `n` quantized, a scale per 256 and a sum per 16 of the scaled quants
struct Q8kActs {
    src : array<float>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>
}

//! `ntok` rows of `n` elements of `src` in the Q8_K form
def q8k_acts(src : array<float>; ntok, n : int64) : Q8kActs {
    var a : Q8kActs
    a.src := src
    a.xq |> resize(ntok * n)
    a.xs |> resize(ntok * n / 256l)
    a.xbs |> resize(ntok * n / 16l)
    requant_rows_q8k_bs(a.src, n, ntok, a.xq, a.xs, a.xbs, false)
    return <- a
}

//! `ntok` rows of `n` elements of `w`, through the taint seam, in the Q8_K form
def build_q8k_acts(ntok, n : int64; w : Wave) : Q8kActs {
    var inscope src : array<float>
    src |> resize(ntok * n)
    for (i in range64(ntok * n)) {
        src[i] = opaque(wave_at(w, i))
    }
    return <- q8k_acts(src, ntok, n)
}

//! row `row` of a format-`fmt` region (`nsb` superblocks a row) dotted with the Q8_K activation row at quant `xoff`, scale
//! `soff` and sum `boff`, in disk order
def kq_row_dot(fmt : int; kq : array<uint8>; ks : array<uint8>; row, nsb : int64;
               xq : array<int8>; xs : array<float>; xbs : array<int>;
               xoff, soff, boff, n : int64) : float {
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        let kqr = addr<uint8 const?>(kq[row * nsb * qsb])
        let ksr = addr<uint8 const?>(ks[row * nsb * ssb])
        let xqp = addr<int8 const?>(xq[xoff])
        let xsp = addr<float const?>(xs[soff])
        let xbp = addr<int const?>(xbs[boff])
        return dot_kq(kqr, ksr, xqp, xsp, xbp, n, kq_fmt_of_id(fmt, "disk_dot"))
    }
}

def mix32(seed, idx : uint) : uint {
    var s = seed + idx * 2654435761u
    s = s ^ (s >> 15u)
    s = s * 2246822519u
    s = s ^ (s >> 13u)
    s = s * 3266489917u
    return s ^ (s >> 16u)
}

def unit_at(seed, idx : uint) : float {
    return float(int(mix32(seed, idx) % 20001u) - 10000) * 1.0e-4
}

//! `cnt` values in [-1, 1] on the 1e-4 grid, each an index hash off `seed`
def fill_unit(var a : array<float>; cnt : int64; seed : uint) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = unit_at(seed, uint(int(i)))
    }
}

//! `ntok` rows of `n` elements of `src` in the Q8_0 form: a quant per element, a scale per 32
def quantize_rows(src : array<float>; ntok, n : int64; var xq : array<int8>; var xs : array<float>) {
    xq |> resize(ntok * n)
    xs |> resize(ntok * (n / 32l))
    requant_rows_q8(src, n, ntok, xq, xs, false)
}

//! One row quantized as a device feeds its GEMV: the quants, the scales, and the dequantized row the GEMV reads
struct Q8Round {
    xq : array<int8>
    xs : array<float>
    deq : array<float>
}

//! `n` elements of `src` through the engine's Q8_K quantizer (a scale per 256) under `k_form`, else its Q8_0 one (a scale per 32)
def q8_roundtrip(src : array<float>; n : int64; k_form : bool) : Q8Round {
    var r : Q8Round
    r.xq |> resize(n)
    r.deq |> resize(n)
    if (!k_form) {
        r.xs |> resize(n / 32l)
        quantize_q8_0_into(src, n, r.xq, r.xs, 0l, 0l)
        for (i in range64(n)) {
            r.deq[i] = float(r.xq[i]) * r.xs[i / 32l]
        }
    } else {
        var xbs : array<int>
        r.xs |> resize(n / 256l)
        xbs |> resize(n / 16l)
        quantize_q8_k_into(src, n, r.xq, r.xs, xbs, 0l, 0l, 0l)
        for (i in range64(n)) {
            r.deq[i] = float(r.xq[i]) * r.xs[i / 256l]
        }
        delete xbs
    }
    return <- r
}

//! the per-32 sums of `ntok` Q8_0 rows of `n` quants, times -128: the bias term a tile's unsigned-weight arm adds back
def activation_bsums(xq : array<int8>; ntok, n : int64; var xbs : array<int>) {
    let nb = n / 32l
    xbs |> resize(ntok * nb)
    for (p in range64(ntok)) {
        for (bi in range64(nb)) {
            var sm = 0
            for (j in range64(32l)) {
                sm += int(xq[p * n + bi * 32l + j])
            }
            xbs[p * nb + bi] = -128 * sm
        }
    }
}

let REF_ULP = 1.0e-6   //! `ref_bar`'s default bar a block term

//! a row's f32 dot adds `nb` block terms, each add rounding within 2^-24 (6e-8) of the envelope `env` - the sum of the terms' magnitudes - so the row sits within nb * 6e-8 * env of the fp64 sum; REF_ULP holds that at 1e-6, the scale products' own rounding inside the margin
def ref_bar(env : double; nb : int64; ulp : float = REF_ULP) : float {
    return ulp * float(nb) * max(float(env), 1.0)
}

//! The independent fp64 truth for one Q8_0 x Q8_0 row; `env` is the same sum on block magnitudes, the envelope the bar derives from. Layout-free, so repacked and row-major backends judge alike.
def ref_dot_q8_f64(wq : array<int8>; ws : array<float>; woff : int64;
                   xq : array<int8>; xs : array<float>; xoff, n : int64) : tuple<dot : double; env : double> {
    var res : tuple<dot : double; env : double>
    for (bi in range64(n / 32l)) {
        var iacc = 0
        for (k in range64(32l)) {
            iacc += int(wq[woff + bi * 32l + k]) * int(xq[xoff + bi * 32l + k])
        }
        let term = double(iacc) * double(ws[woff / 32l + bi]) * double(xs[xoff / 32l + bi])
        res.dot += term
        res.env += abs(term)
    }
    return res
}

//! the fp64 truth for one Q8_0 row against an f32 activation row, with its envelope
def ref_dot_q8f_f64(wq : array<int8>; ws : array<float>; woff : int64;
                    x : array<float>; n : int64) : tuple<dot : double; env : double> {
    var res : tuple<dot : double; env : double>
    for (bi in range64(n / 32l)) {
        var bacc : double
        for (k in range64(32l)) {
            bacc += double(int(wq[woff + bi * 32l + k])) * double(x[bi * 32l + k])
        }
        let term = bacc * double(ws[woff / 32l + bi])
        res.dot += term
        res.env += abs(term)
    }
    return res
}

let MX4_E2M1 <- [0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0]

def mx4_val(nib : int) : double {
    let m = MX4_E2M1[nib & 7]
    return double((nib & 8) != 0 ? -m : m)
}

def pow2d(k : int) : double {
    var v = 1.0lf
    for (_i in range(abs(k))) {
        v = k >= 0 ? v * 2.0lf : v * 0.5lf
    }
    return v
}

//! the fp64 truth for one MXFP4 row (nibbles from byte `roff`, e8m0 scales from `eoff`) against a Q8_0 row, with its envelope
def ref_dot_mx4_f64(wn : array<uint8>; we : array<uint8>; roff, eoff : int64;
                    xq : array<int8>; xs : array<float>; xoff, n : int64) : tuple<dot : double; env : double> {
    var res : tuple<dot : double; env : double>
    for (bi in range64(n / 32l)) {
        var bacc : double
        for (j in range64(16l)) {
            let b = int(wn[roff + bi * 16l + j])
            bacc += mx4_val(b & 15) * double(int(xq[xoff + bi * 32l + j]))
            bacc += mx4_val(b >> 4) * double(int(xq[xoff + bi * 32l + j + 16l]))
        }
        let term = bacc * pow2d(int(we[eoff + bi]) - 127) * double(xs[xoff / 32l + bi])
        res.dot += term
        res.env += abs(term)
    }
    return res
}

//! rows [t0, te) of an image `d` wide into `ref` and `bar` (grown to te x d where short): element (p, r) is `dot(p, r)`'s fp64
//! sum and `ref_bar` of its envelope over `nb` block terms at `ulp`
def ref_plane(var ref : array<float>; var bar : array<float>; t0, te, d, nb : int64; ulp : float;
              dot : block<(p, r : int64) : tuple<dot : double; env : double>>) {
    ref |> resize(max(long_length(ref), te * d))
    bar |> resize(max(long_length(bar), te * d))
    for (p in range64(t0, te)) {
        for (r in range64(d)) {
            let rf = invoke(dot, p, r)
            ref[p * d + r] = float(rf.dot)
            bar[p * d + r] = ref_bar(rf.env, nb, ulp)
        }
    }
}

// the synthetic superblocks pack the disk layout in the PACK direction, independent of the reader's unpack, so a gate knows every element

// synthetic quant patterns: every nibble/5-bit/6-bit value, position-dependent so swaps can't cancel
def q4_pat(k : int) : int => (k * 7 + 3) % 16
def q5_pat(k : int) : int => (k * 3 + 7) % 32
def q6_pat(k : int) : int => (k * 5 + 11) % 64
def q3_pat(k : int) : int => (k * 5 + 3) % 8
def q2_pat(k : int) : int => (k * 3 + 1) % 4

// The shared synthetic scale/min sets: 6-bit values > 15 exercise the split high-bit arms of
// the 6-bit scale/min unpack (j >= 4); d/dmin are f16-exact so expected-value math uses the f32 value.
let SYNTH_SC <- [63, 21, 37, 5, 48, 17, 60, 33]
let SYNTH_MN <- [1, 62, 19, 44, 7, 58, 25, 36]
let SYNTH_D = 0.5
let SYNTH_DMIN = 0.25
let SYNTH_SCQ <- [3, -5, 60, -60, 17, 127, -128, 1, 0, 33, -77, 90, -2, 45, -101, 7]

// Pack the 4-byte header + 12 scale/min bytes shared by the Q4_K/Q5_K synthetic superblocks.
def pack_kq_header(var blkb : array<uint8>) {
    put_f16(blkb, 0, SYNTH_D)
    put_f16(blkb, 2, SYNTH_DMIN)
    for (j in range(4)) {
        blkb[4 + j] = uint8((SYNTH_SC[j] & 63) | (((SYNTH_SC[j + 4] >> 4) & 3) << 6))
        blkb[8 + j] = uint8((SYNTH_MN[j] & 63) | (((SYNTH_MN[j + 4] >> 4) & 3) << 6))
        blkb[12 + j] = uint8((SYNTH_SC[j + 4] & 15) | ((SYNTH_MN[j + 4] & 15) << 4))
    }
}

def build_q4k_block() : array<uint8> {
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

def build_q5k_block() : array<uint8> {
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

def build_q6k_block() : array<uint8> {
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
    put_f16(blkb, 208, 0.25)
    return <- blkb
}

// iq3xxs synthetic superblock: 98B IQ3_XXS disk block — f16 d, 64 grid-index bytes, 8 x aux32
// (4x7-bit ksigns indices + a 4-bit scale in the top nibble). Any byte pattern is legal.
def iq3xxs_qs_pat(i : int) : int => (i * 13 + 5) % 256
def iq3xxs_aux_pat(p : int) : int => (p * 9 + 17) % 256
def build_iq3xxs_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(98)
    put_f16(blkb, 0, 0.002)
    for (i in range(64)) {
        blkb[2 + i] = uint8(iq3xxs_qs_pat(i))
    }
    for (p in range(32)) {
        blkb[66 + p] = uint8(iq3xxs_aux_pat(p))
    }
    return <- blkb
}

// iq4nl synthetic superblock: 8 x 18B IQ4_NL disk blocks (per block f16 d + 16 nibble bytes,
// q40's exact disk shape). Any byte pattern is legal.
def iq4nl_qs_pat(i : int) : int => (i * 19 + 7) % 256
def build_iq4nl_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(144)
    for (b in range(8)) {
        put_f16(blkb, b * 18, 0.002 + 0.0003 * float(b))
        for (i in range(16)) {
            blkb[b * 18 + 2 + i] = uint8(iq4nl_qs_pat(b * 16 + i))
        }
    }
    return <- blkb
}

// iq4xs synthetic superblock: 136B disk block — f16 d = SYNTH_D, the 8 six-bit sub-scales are
// SYNTH_SC (packed low nibbles into scales_l, top two bits into scales_h), the shared nibble
// pattern in the k/k+16 pairing — weight k = (d * (SYNTH_SC[k/32] - 32)) * IQ4NL_LUT[q4_pat(k)]
def build_iq4xs_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(136)
    put_f16(blkb, 0, SYNTH_D)
    var sh = 0
    for (blk in range(8)) {
        sh |= ((SYNTH_SC[blk] >> 4) & 3) << (2 * blk)
    }
    blkb[2] = uint8(sh & 0xFF)
    blkb[3] = uint8(sh >> 8)
    for (j in range(4)) {   // scales_l[j]: block 2j's low nibble, block 2j+1's in the high nibble
        blkb[4 + j] = uint8((SYNTH_SC[2 * j] & 15) | ((SYNTH_SC[2 * j + 1] & 15) << 4))
    }
    for (blk in range(8)) {
        for (j in range(16)) {   // byte j: weight blk*32+j in the low nibble, +16 in the high
            blkb[8 + blk * 16 + j] = uint8(q4_pat(blk * 32 + j) | (q4_pat(blk * 32 + 16 + j) << 4))
        }
    }
    return <- blkb
}

// k3 synthetic superblock: 110B Q3_K disk block — 16 six-bit sub-scales SYNTH_SC[is % 8] packed in
// the disk kmask form (written in the PACK direction), f16 d = SYNTH_D, the 2-bit lanes + hmask bits
// of q3_pat — weight k = (d * (SYNTH_SC[(k/16) % 8] - 32)) * (q3_pat(k) - 4)
def build_q3k_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(110)
    var hm : array<int>
    var qs : array<int>
    var sc : array<int>
    hm |> resize(32)
    qs |> resize(64)
    sc |> resize(12)
    for (k in range(256)) {   // block b = k/32: qs byte (b/4)*32 + l holds the 2-bit lane at shift 2*(b%4); hmask byte l bit b
        let b = k / 32
        let l = k % 32
        let q = q3_pat(k)
        qs[(b / 4) * 32 + l] |= (q & 3) << (2 * (b % 4))
        hm[l] |= (q >> 2) << b
    }
    for (i in range(16)) {   // scale i: low nibble in byte i (i < 8) / high nibble of byte i-8, top two bits in byte 8 + i%4 at 2*(i/4)
        let v = SYNTH_SC[i % 8]
        if (i < 8) {
            sc[i] |= v & 15
        } else {
            sc[i - 8] |= (v & 15) << 4
        }
        sc[8 + i % 4] |= ((v >> 4) & 3) << (2 * (i / 4))
    }
    for (i in range(32)) {
        blkb[i] = uint8(hm[i])
    }
    for (i in range(64)) {
        blkb[32 + i] = uint8(qs[i])
    }
    for (i in range(12)) {
        blkb[96 + i] = uint8(sc[i])
    }
    put_f16(blkb, 108, SYNTH_D)
    return <- blkb
}

//! `n` scale nibbles SYNTH_SC[g % 8] & 15 OR-ed into `blkb` from byte `at`: nibble g the low half of byte g / 2 for an even g,
//! the high half for an odd one
def private pack_sc_nibbles(var blkb : array<uint8>; at, n : int) {
    for (g in range(n)) {
        let v = SYNTH_SC[g % 8] & 15
        blkb[at + g / 2] = uint8(int(blkb[at + g / 2]) | (g % 2 == 0 ? v : v << 4))
    }
}

// iq3s synthetic superblock: 110B IQ3_S disk block — 4-bit sub-scales SYNTH_SC[b] & 15, f16 d =
// SYNTH_D, grid-index/qh/sign byte patterns (written in the PACK direction); the gate recomposes
// each weight through IQ3S_GRID
def iq3s_qs_pat(i : int) : int => (i * 11 + 3) % 256
def iq3s_qh_pat(b : int) : int => (b * 5 + 1) % 256
def iq3s_sg_pat(p : int) : int => (p * 7 + 13) % 256
def build_iq3s_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(110)
    put_f16(blkb, 0, SYNTH_D)
    for (i in range(64)) {
        blkb[2 + i] = uint8(iq3s_qs_pat(i))
    }
    for (b in range(8)) {
        blkb[66 + b] = uint8(iq3s_qh_pat(b))
    }
    for (p in range(32)) {
        blkb[74 + p] = uint8(iq3s_sg_pat(p))
    }
    pack_sc_nibbles(blkb, 106, 8)
    return <- blkb
}

// k2 synthetic superblock: 84B Q2_K disk block - per-group sc/min nibble pairs (SYNTH_SC & 15,
// SYNTH_MN & 15), f16 d/dmin, the 2-bit lanes of q2_pat (written in the PACK direction) -
// weight k = SYNTH_D * (SYNTH_SC[g % 8] & 15) * q2_pat(k) - SYNTH_DMIN * (SYNTH_MN[g % 8] & 15), g = k / 16
def build_q2k_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(84)
    var qs : array<int>
    qs |> resize(64)
    for (k in range(256)) {   // group g = k/16: qs byte (g/8)*32 + (g%2)*16 + k%16 holds the 2-bit lane at shift 2*((g%8)/2)
        let g = k / 16
        qs[(g / 8) * 32 + (g % 2) * 16 + k % 16] |= q2_pat(k) << (2 * ((g % 8) / 2))
    }
    for (g in range(16)) {
        blkb[g] = uint8((SYNTH_SC[g % 8] & 15) | ((SYNTH_MN[g % 8] & 15) << 4))
    }
    for (i in range(64)) {
        blkb[16 + i] = uint8(qs[i])
    }
    put_f16(blkb, 80, SYNTH_D)
    put_f16(blkb, 82, SYNTH_DMIN)
    return <- blkb
}

// iq2s synthetic superblock: 82B IQ2_S disk block - per-16 scale nibbles SYNTH_SC & 15, f16 d,
// idx/qh/sign byte patterns (written in the PACK direction); the gate recomposes each weight
// through IQ2S_GRID
def iq2s_ix_pat(i : int) : int => (i * 13 + 5) % 256
def iq2s_qh_pat(b : int) : int => (b * 3 + 1) % 256
def iq2s_sg_pat(p : int) : int => (p * 7 + 11) % 256
def build_iq2s_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(82)
    put_f16(blkb, 0, SYNTH_D)
    for (i in range(32)) {
        blkb[2 + i] = uint8(iq2s_ix_pat(i))
    }
    for (p in range(32)) {
        blkb[34 + p] = uint8(iq2s_sg_pat(p))
    }
    for (b in range(8)) {
        blkb[66 + b] = uint8(iq2s_qh_pat(b))
    }
    pack_sc_nibbles(blkb, 74, 16)
    return <- blkb
}

// iq2xs synthetic superblock: 74B IQ2_XS disk block - per-16 scale nibbles SYNTH_SC & 15,
// f16 d, u16 qs words carrying a 9-bit grid index + 7-bit ksigns index pattern (written in the
// PACK direction); the gate recomposes each weight through IQ2XS_GRID + KSIGNS_IQ2XS
def iq2xs_ix_pat(i : int) : int => (i * 29 + 7) % 512
def iq2xs_sg7_pat(i : int) : int => (i * 5 + 3) % 128
def build_iq2xs_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(74)
    put_f16(blkb, 0, SYNTH_D)
    for (w in range(32)) {
        let v = iq2xs_ix_pat(w) | (iq2xs_sg7_pat(w) << 9)
        blkb[2 + w * 2] = uint8(v & 0xFF)
        blkb[3 + w * 2] = uint8(v >> 8)
    }
    pack_sc_nibbles(blkb, 66, 16)
    return <- blkb
}

// iq2xxs synthetic superblock: 66B IQ2_XXS disk block - f16 d, per block 4 grid byte indices
// + an aux32 packing four 7-bit ksigns indices and the per-32 scale nibble (written in the
// PACK direction); the gate recomposes each weight through IQ2XXS_GRID + the shared KSIGNS_IQ2XS
def iq2xxs_ix_pat(i : int) : int => (i * 29 + 7) % 256
def iq2xxs_sg7_pat(i : int) : int => (i * 5 + 3) % 128
def build_iq2xxs_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(66)
    put_f16(blkb, 0, SYNTH_D)
    for (b in range(8)) {
        for (l in range(4)) {
            blkb[2 + b * 8 + l] = uint8(iq2xxs_ix_pat(b * 4 + l))
        }
        var aux = 0
        for (l in range(4)) {
            aux = aux | (iq2xxs_sg7_pat(b * 4 + l) << (7 * l))
        }
        aux = aux | ((SYNTH_SC[b % 8] & 15) << 28)
        for (k in range(4)) {
            blkb[2 + b * 8 + 4 + k] = uint8((aux >> (8 * k)) & 0xFF)
        }
    }
    return <- blkb
}

// q40 synthetic superblock: 8 x 18B q4_0 disk blocks, per-block f16-exact d = (blk+1)/16 and
// the shared nibble pattern — weight k = d(k/32) * (q4_pat(k) - 8)
def q40_d(blk : int) : float => float(blk + 1) * 0.0625

def build_q40_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(144)
    for (blk in range(8)) {
        put_f16(blkb, blk * 18, q40_d(blk))
        for (j in range(16)) {   // byte j: weight blk*32+j in the low nibble, +16 in the high
            blkb[blk * 18 + 2 + j] = uint8(q4_pat(blk * 32 + j) | (q4_pat(blk * 32 + 16 + j) << 4))
        }
    }
    return <- blkb
}

// q51 synthetic 32-block index `blk`: f16-exact d/m varied per block, elem k = q5_pat(k + blk)
// (shifted per block so an inter-block offset bug can't cancel); disk layout [d][m][qh][qs].
def q51_d(blk : int) : float => float(blk % 7 + 1) * 0.0625
def q51_m(blk : int) : float => float(blk % 5) * 0.25 - 0.5

def build_q51_block(blk : int) : array<uint8> {
    var b : array<uint8>
    b |> resize(24)
    put_f16(b, 0, q51_d(blk))
    put_f16(b, 2, q51_m(blk))
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

// iq4nl32 synthetic 32-block index `blk`: f16-exact d varied per block, nibble byte j =
// iq4nl_qs_pat(blk*16 + j) (shifted per block so an inter-block offset bug can't cancel); disk
// layout [d][16 nibble bytes] - IQ4_NL's own 18 B block.
def iq4nl32_d(blk : int) : float => float(blk % 7 + 1) * 0.0625

def build_iq4nl32_block(blk : int) : array<uint8> {
    var b : array<uint8>
    b |> resize(18)
    put_f16(b, 0, iq4nl32_d(blk))
    for (j in range(16)) {
        b[2 + j] = uint8(iq4nl_qs_pat(blk * 16 + j))
    }
    return <- b
}

//! the synthetic superblock of format id `fmt` (35 = iq3s4 reads iq3s's; an id with no builder of its own reads k6's)
def synth_block(fmt : int) : array<uint8> {
    return <- (fmt == 4 ? build_q4k_block() : (fmt == 5 ? build_q5k_block() : (fmt == 40 ? build_q40_block() : (fmt == 44 ? build_iq4xs_block() : (fmt == 3 ? build_q3k_block() : (fmt == 33 || fmt == 35 ? build_iq3s_block() : (fmt == 34 ? build_iq3xxs_block() : (fmt == 45 ? build_iq4nl_block() : (fmt == 2 ? build_q2k_block() : (fmt == 23 ? build_iq2s_block() : (fmt == 24 ? build_iq2xs_block() : (fmt == 25 ? build_iq2xxs_block() : build_q6k_block()))))))))))))
}
