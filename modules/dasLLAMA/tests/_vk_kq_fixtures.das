options gen2
options _dasllama_internal = true

require llvm/daslib/f16_cvt
require math

// Synthetic K-quant expert stacks in the vulkan tier's DEVICE layout, shared by the tier suite
// (test_vulkan_tier) and the cm2 expert-chain suite (test_vulkan_moe_cm2): the pow2 scale sets
// keep |gate| in the sane silu regime, the dequant mirrors the kernels' extraction bit-exact.

// the small set for gate/up/x scale planes
def pow2_small(i : int64) : float {
    let p = i % 3l
    return p == 0l ? 0.00390625 : (p == 1l ? 0.001953125 : 0.0078125)   // 2^-8 / 2^-9 / 2^-7
}

// tiny pow2 set for the kq gate/up d planes — with 6-bit sc (<=63) and 4/5-bit quants the
// per-weight magnitude lands where |gate| stays in the sane silu regime
def pow2_tiny(i : int64) : float {
    let p = i % 3l
    return p == 0l ? 0.000244140625 : (p == 1l ? 0.0001220703125 : 0.00048828125)   // 2^-12/-13/-11
}

def lcg_byte(var st : uint64&) : uint8 {
    st = st * 6364136223846793005ul + 1442695040888963407ul
    return uint8((st >> 33ul) & 255ul)
}

//! Synthesize one kq stack (fmt 1 = k4, 2 = k5, 3 = k6, 4 = q40) directly in the tier's device
//! layout: quant payload (any bytes are valid nibble/qh payloads) + 20B scale rows (f16 d/dmin
//! from the pow2 sets, 6-bit sc/mn, signed i8 sub-scales, or q40's 8 per-block f16 d).
def fill_kq_stack(fmt : int; rows, n : int64; seed : int64; tiny_d : bool;
                  var wqb : array<uint8>; var wsb : array<uint8>) {
    let nsb = n / 256l
    let qsb = fmt == 2 ? 160l : (fmt == 3 ? 192l : 128l)
    wqb |> resize(rows * nsb * qsb)
    wsb |> resize(rows * nsb * 20l)
    var st = uint64(seed) * 2654435761ul + 12345ul
    for (c in wqb) {
        c = lcg_byte(st)
    }
    var sb = 0l
    while (sb < rows * nsb) {
        let so = int(sb * 20l)
        if (fmt == 3) {
            for (i in range(16)) {   // signed i8 sub-scales in a modest range
                wsb[so + i] = uint8(int(lcg_byte(st)) % 63 - 31)
            }
            let h = f32_to_f16(tiny_d ? pow2_tiny(sb + seed) : pow2_small(sb + seed))
            wsb[so + 16] = uint8(h & 0xFFu)
            wsb[so + 17] = uint8((h >> 8u) & 0xFFu)
            wsb[so + 18] = uint8(0)
            wsb[so + 19] = uint8(0)
        } elif (fmt == 4) {
            for (blk in range(8)) {   // one f16 d per 32-block, words 0..3; 16..19 pad
                let h = f32_to_f16(tiny_d ? pow2_tiny(sb + seed + int64(blk)) : pow2_small(sb + seed + int64(blk)))
                wsb[so + blk * 2] = uint8(h & 0xFFu)
                wsb[so + blk * 2 + 1] = uint8((h >> 8u) & 0xFFu)
            }
            for (i in range(16, 20)) {
                wsb[so + i] = uint8(0)
            }
        } else {
            let hd = f32_to_f16(tiny_d ? pow2_tiny(sb + seed) : pow2_small(sb + seed))
            let hm = f32_to_f16(tiny_d ? pow2_tiny(sb + seed + 1l) : pow2_small(sb + seed + 1l))
            wsb[so] = uint8(hd & 0xFFu)
            wsb[so + 1] = uint8((hd >> 8u) & 0xFFu)
            wsb[so + 2] = uint8(hm & 0xFFu)
            wsb[so + 3] = uint8((hm >> 8u) & 0xFFu)
            for (i in range(16)) {   // 8 sc + 8 mn, 6-bit
                wsb[so + 4 + i] = uint8(int(lcg_byte(st)) & 63)
            }
        }
        sb++
    }
}

//! Dequantize one device-layout stack to floats — the same extraction the GPU kernels do,
//! validated bit-exact against the plane-dequant oracles by the kq gather probe.
def dequant_kq_stack(fmt : int; wqb : array<uint8>; wsb : array<uint8>; rows, n : int64;
                     var w : array<float>) {
    let nsb = n / 256l
    let qsb = fmt == 2 ? 160l : (fmt == 3 ? 192l : 128l)
    w |> resize(rows * n)
    for (rr in range64(rows)) {
        for (sbi in range64(nsb)) {
            let qo = (rr * nsb + sbi) * qsb
            let so = (rr * nsb + sbi) * 20l
            let wo = rr * n + sbi * 256l
            if (fmt == 4) {
                for (blk in range64(8l)) {
                    let d = f16_to_f32(uint(wsb[so + blk * 2l]) | (uint(wsb[so + blk * 2l + 1l]) << 8u))
                    for (m in range64(16l)) {
                        let b = int(wqb[qo + blk * 16l + m])
                        w[wo + blk * 32l + m] = d * float((b & 15) - 8)
                        w[wo + blk * 32l + 16l + m] = d * float((b >> 4) - 8)
                    }
                }
            } elif (fmt == 3) {
                let d = f16_to_f32(uint(wsb[so + 16l]) | (uint(wsb[so + 17l]) << 8u))
                for (blk in range64(8l)) {
                    let shift = int(2l * (blk % 4l))
                    let vlo = int(wsb[so + 2l * blk])
                    let vhi = int(wsb[so + 2l * blk + 1l])
                    let slo = float(vlo >= 128 ? vlo - 256 : vlo)
                    let shi = float(vhi >= 128 ? vhi - 256 : vhi)
                    for (m in range64(16l)) {
                        let b = int(wqb[qo + blk * 16l + m])
                        let hb = int(wqb[qo + 128l + (blk / 4l) * 32l + m])
                        let hb2 = int(wqb[qo + 128l + (blk / 4l) * 32l + 16l + m])
                        let qlo = ((b & 15) | (((hb >> shift) & 3) << 4)) - 32
                        let qhi = ((b >> 4) | (((hb2 >> shift) & 3) << 4)) - 32
                        w[wo + blk * 32l + m] = d * slo * float(qlo)
                        w[wo + blk * 32l + 16l + m] = d * shi * float(qhi)
                    }
                }
            } else {
                let d = f16_to_f32(uint(wsb[so]) | (uint(wsb[so + 1l]) << 8u))
                let dmin = f16_to_f32(uint(wsb[so + 2l]) | (uint(wsb[so + 3l]) << 8u))
                for (blk in range64(8l)) {
                    let d1 = d * float(uint(wsb[so + 4l + blk]))
                    let m1 = dmin * float(uint(wsb[so + 12l + blk]))
                    for (m in range64(16l)) {
                        let b = int(wqb[qo + blk * 16l + m])
                        var qlo = b & 15
                        var qhi = b >> 4
                        if (fmt == 2) {
                            let hb = int(wqb[qo + 128l + blk * 4l + m / 4l])
                            let tt = int(m % 4l)
                            qlo |= ((hb >> tt) & 1) << 4
                            qhi |= ((hb >> (4 + tt)) & 1) << 4
                        }
                        w[wo + blk * 32l + m] = d1 * float(qlo) - m1
                        w[wo + blk * 32l + 16l + m] = d1 * float(qhi) - m1
                    }
                }
            }
        }
    }
}

//! a q8_0 plane on the upload wire (row-major int8 quants as bytes, f16 per-32 scales as LE
//! bytes) plus its dequantized rows for a reference
def fill_q8_stack(rows, n : int64; seed : int64; var wqb : array<uint8>; var wsb : array<uint8>; var w : array<float>) {
    let nb = n / 32l
    wqb |> resize(rows * n)
    wsb |> resize(rows * nb * 2l)
    w |> resize(rows * n)
    var scales : array<float>
    scales |> resize(rows * nb)
    for (i in range64(rows * nb)) {
        scales[i] = pow2_small(i + seed)
        let h = f32_to_f16(scales[i])
        wsb[i * 2l] = uint8(h & 0xFFu)
        wsb[i * 2l + 1l] = uint8((h >> 8u) & 0xFFu)
    }
    for (i in range64(rows * n)) {
        let q = int((i * 7l + seed) % 255l - 127l)
        wqb[i] = uint8(q)
        w[i] = float(q) * scales[i / 32l]
    }
}

//! the device-vs-reference bars every Vulkan cell shares: an element is off when it misses BOTH
//! the absolute bar and the relative one (a fraction of the reference row's max)
def count_bad(gpu, refv : array<float>; abs_bar, rel_bar : float) : int {
    var bad = 0
    for (g, r in gpu, refv) {
        let d = abs(g - r)
        if (d > abs_bar && d > rel_bar) {
            bad++
        }
    }
    return bad
}

def max_abs(v : array<float>) : float {
    var m = 0.0
    for (x in v) {
        m = max(m, abs(x))
    }
    return m
}

def max_diff(a, b : array<float>) : float {
    var m = 0.0
    for (av, bv in a, b) {
        m = max(m, abs(av - bv))
    }
    return m
}
