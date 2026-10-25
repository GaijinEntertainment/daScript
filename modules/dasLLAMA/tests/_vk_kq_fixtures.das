options gen2
options _dasllama_internal = true

module _vk_kq_fixtures shared public   //! shared: the harness's probe fixture (a shared module) requires it

require daslib/f16_cvt
require dasllama/dasllama_convert
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_kqformat   // KQ_B32_ELEMS: the q8_0 block the stacks' scales step by
require dasllama/dasllama_gemm_schema
require _compares
require daslib/enum_trait
require daslib/shader_lingua_franca
require daslib/math_bits   // float_bits_to_uint: the nv4 device row's per-tensor scale word

// Synthetic K-quant expert stacks in the vulkan tier's DEVICE layout, shared by the tier suite
// (test_vulkan_tier) and the cm2 expert-chain suite (test_vulkan_moe_cm2): the pow2 scale sets
// keep |gate| in the sane silu regime, the dequant mirrors the kernels' extraction bit-exact.

//! a float rounded through f16 and back - the value the device reads where it stages a plane in halves
def f16r(v : float) : float => f16_to_f32(f32_to_f16(v))

//! `v` as a little-endian f16 at byte `off` of `b`
def put_f16(var b : array<uint8>; off : int64; v : float) {
    let bits = f32_to_f16(v)
    b[off] = uint8(bits & 0xFFu)
    b[off + 1l] = uint8(bits >> 8u)
}

def put_f16(var b : array<uint8>; off : int; v : float) {
    put_f16(b, int64(off), v)
}

//! the little-endian f16 at byte `off` of `b`, widened
def get_f16(b : array<uint8>; off : int64) : float => f16_to_f32(rd_u16(b, off))

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
            put_f16(wsb, so + 16, tiny_d ? pow2_tiny(sb + seed) : pow2_small(sb + seed))
            wsb[so + 18] = uint8(0)
            wsb[so + 19] = uint8(0)
        } elif (fmt == 4) {
            for (blk in range(8)) {   // one f16 d per 32-block, words 0..3; 16..19 pad
                put_f16(wsb, so + blk * 2, tiny_d ? pow2_tiny(sb + seed + int64(blk)) : pow2_small(sb + seed + int64(blk)))
            }
            for (i in range(16, 20)) {
                wsb[so + i] = uint8(0)
            }
        } else {
            put_f16(wsb, so, tiny_d ? pow2_tiny(sb + seed) : pow2_small(sb + seed))
            put_f16(wsb, so + 2, tiny_d ? pow2_tiny(sb + seed + 1l) : pow2_small(sb + seed + 1l))
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
                    let d = get_f16(wsb, so + blk * 2l)
                    for (m in range64(16l)) {
                        let b = int(wqb[qo + blk * 16l + m])
                        w[wo + blk * 32l + m] = d * float((b & 15) - 8)
                        w[wo + blk * 32l + 16l + m] = d * float((b >> 4) - 8)
                    }
                }
            } elif (fmt == 3) {
                let d = get_f16(wsb, so + 16l)
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
                let d = get_f16(wsb, so)
                let dmin = get_f16(wsb, so + 2l)
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

//! the second q8 scale set: 1, 1/2, 2, 1/4, 1/8 by index
def pow2_scale(i : int64) : float {
    let p = i % 5l
    return p == 0l ? 1.0 : (p == 1l ? 0.5 : (p == 2l ? 2.0 : (p == 3l ? 0.25 : 0.125)))
}

//! a q8_0 stack's quants `(i * 7 + seed) % 255 - 127` and its per-32 scales, `pow2_small` or `pow2_scale`
def fill_q8_quants(var wq : array<int8>; var ws : array<float>; rows, n : int64; seed : int64; small_scales : bool) {
    wq |> resize(rows * n)
    ws |> resize(rows * (n / KQ_B32_ELEMS))
    for (i in range64(rows * n)) {
        wq[i] = int8((i * 7l + seed) % 255l - 127l)
    }
    for (i in range64(rows * (n / KQ_B32_ELEMS))) {
        ws[i] = small_scales ? pow2_small(i + seed) : pow2_scale(i + seed)
    }
}

//! a q8_0 stack on the upload wire: the int8 quants as bytes, the f16 per-32 scales as LE bytes
def pack_q8_wire(wq : array<int8>; ws : array<float>; var wqb : array<uint8>; var wsb : array<uint8>) {
    wqb |> resize(length(wq))
    for (q, b in wq, wqb) {
        b = uint8(q)
    }
    wsb |> resize(length(ws) * 2)
    for (i in range(length(ws))) {
        put_f16(wsb, i * 2, ws[i])
    }
}

//! a q8_0 stack's dequantized rows
def dequant_q8_rows(wq : array<int8>; ws : array<float>; rows, n : int64; var w : array<float>) {
    w |> resize(rows * n)
    for (i in range64(rows * n)) {
        w[i] = float(wq[i]) * ws[i / KQ_B32_ELEMS]
    }
}

//! a q8_0 stack packed onto the wire and uploaded at `woff`; false = the upload was refused
def q8_upload(wq : array<int8>; ws : array<float>; woff, n, rows : int64; stream : bool = false) : bool {
    var wqb, wsb : array<uint8>
    pack_q8_wire(wq, ws, wqb, wsb)
    let ok = moe_gpu_upload_stack(wqb, wsb, woff, n, rows, 0, stream)
    delete wqb
    delete wsb
    return ok
}

//! a q8_0 plane under the `pow2_small` scales on the upload wire, plus its dequantized rows for a reference
def fill_q8_stack(rows, n : int64; seed : int64; var wqb : array<uint8>; var wsb : array<uint8>; var w : array<float>) {
    var wq : array<int8>
    var ws : array<float>
    fill_q8_quants(wq, ws, rows, n, seed, true)
    pack_q8_wire(wq, ws, wqb, wsb)
    dequant_q8_rows(wq, ws, rows, n, w)
    delete wq
    delete ws
}

//! one plane in format `fmt` (0 = q8 under `fill_q8_stack`) uploaded at `woff` and dequantized into `w`; false = the upload was refused
def plane_up(fmt : int; rows, n, seed, woff : int64; var w : array<float>; tiny_d : bool = true; stream : bool = false) : bool {
    var wqb, wsb : array<uint8>
    if (fmt == 0) {
        fill_q8_stack(rows, n, seed, wqb, wsb, w)
    } else {
        fill_kq_stack(fmt, rows, n, seed, tiny_d, wqb, wsb)
        dequant_kq_stack(fmt, wqb, wsb, rows, n, w)
    }
    let ok = moe_gpu_upload_stack(wqb, wsb, woff, n, rows, fmt, stream)
    delete wqb
    delete wsb
    return ok
}

//! one hashed word of synthetic quant bytes for index i - the kernel suite's and the probes' plane filler
def hash_word(i : uint) : uint {
    var h = i * 2654435761u
    h ^= h >> 16u
    h *= 2246822519u
    return h ^ (h >> 13u)
}

//! `n` hashed words `hash_word(i * mul + add)`: a synthetic quant plane
def hashed_words(n : int; mul, add : uint) : array<uint> =>
    <- [for (i in range(n)); hash_word(uint(i) * mul + add)]

//! `n` hashed floats `float(hash_word(i + salt) % modulus) / div - off`: a cell's unit-range input fill
def hash_fill(n : int; salt, modulus : uint; div, off : float) : array<float> =>
    <- [for (i in range(n)); float(hash_word(uint(i) + salt) % modulus) / div - off]

//! `n` hashed floats `float(hash_word(i + salt) % 2001) * 0.001 - 1`: the combine cells' rows, in steps of 1e-3 over [-1, 1]
def hash_steps(n : int; salt : uint) : array<float> =>
    <- [for (i in range(n)); float(hash_word(uint(i) + salt) % 2001u) * 0.001 - 1.0]

//! a finite half from a small table, so a synthetic scale word never carries a NaN
def safe_half(i : uint) : uint {
    let tab = fixed_array(0x3C00u, 0xB800u, 0x3400u, 0x4200u, 0x2E00u, 0xBC00u, 0x3E00u, 0xB000u)
    return tab[i % 8u]
}

def ws_word(i : uint) : uint => safe_half(i * 7u + 3u) | (safe_half(i * 3u + 1u) << 16u)

//! `n` scale words `ws_word(i * mul + add)`: two finite halves a word
def ws_plane(n : int; mul, add : uint) : array<uint> =>
    <- [for (i in range(n)); ws_word(uint(i) * mul + add)]

//! `ws_plane` under a format's device-row rule: an nv4 row's fifth word is one finite per-tensor scale a plane, as f32 bits (`../ARCHITECTURE_ENGINE_FORMATS_NV4.md#nv4-site-scale`), where a hashed word would read as a wild f32
def kq_ws_plane(fmt : int; n : int; mul, add : uint) : array<uint> {
    var w <- ws_plane(n, mul, add)
    if (fmt == int(KqFmt.nv4)) {
        for (sb in range(n / 5)) {
            w[sb * 5 + 4] = float_bits_to_uint(1.25)
        }
    }
    return <- w
}

//! `nblk` f16 block scales two a word, the small positive ramp `0.002 x (1 + b % 7)`: the CUDA q8 GEMV cells' scale plane
def ramp_ws_plane(nblk : int) : array<uint> =>
    <- [for (i in range((nblk + 1) / 2)); packHalf2x16(float2(0.002 * float(1 + (2 * i) % 7), 0.002 * float(1 + (2 * i + 1) % 7)))]

//! one q8 fmt-0 host plane of `totblk` blocks: the s8 quant words hashed at `i * wq_mul + wq_add`, the f16 scale pairs at `i * ws_mul + ws_add`
def q8_plane(totblk : int; wq_mul, wq_add, ws_mul, ws_add : uint; var wqh, wsh : array<uint>) {
    let qwords = totblk * int(Q8_BLOCK_ELEMS / 4l)   // a byte an element: q8 has no `kq_desc` stride row (`kq_qsb` panics on it by design)
    let swords = (totblk * 2 + 3) / 4   // an f16 scale a block
    delete wqh
    delete wsh
    wqh <- hashed_words(qwords, wq_mul, wq_add)
    wsh <- ws_plane(swords, ws_mul, ws_add)
}

//! rows [0, cnt0) off weight block 0 and [cnt0, cnt0 + cnt1) off `wblk1`, `extra` zero words past the map; `wgs` the map's length
def two_region_sched(cnt0, cnt1, wblk1, ttile, wtiles, extra : int) : tuple<words : array<uint>; wgs : int> {
    let wgs0 = ((cnt0 + ttile - 1) / ttile) * wtiles
    let wgs = wgs0 + ((cnt1 + ttile - 1) / ttile) * wtiles
    var words : array<uint>
    words |> resize(2 * 4 + wgs + extra)
    words[2] = uint(cnt0)
    words[4] = uint(wblk1)
    words[5] = uint(cnt0)
    words[6] = uint(cnt1)
    words[7] = uint(wgs0)
    for (w in range(wgs)) {
        words[8 + w] = w < wgs0 ? 0u : 1u
    }
    return <- (words = words, wgs = wgs)
}

//! a decode GEMV cell's host planes: the weight quants and scales, the region records, one activation row's quants and scales
struct GemvFix {
    wqh, wsh, xqh : array<uint>
    xsh : array<float>
    regs : array<uint>
}

//! region r's weights at r x d x `units` units; the activation scales `xs0 + (i % xs_mod) * xs_step`
def gemv_fix(nreg, d, units, wq_words : int; wq_seed : uint; var wsh : array<uint>; xunits, xq_words : int; xq_mul, xq_add : uint;
             xs0, xs_step : float; xs_mod : int) : GemvFix {
    var f <- GemvFix(wqh <- hashed_words(nreg * d * units * wq_words, 1u, wq_seed), wsh <- wsh,
        xqh <- hashed_words(xunits * xq_words, xq_mul, xq_add), xsh <- row_fill(xunits, 1, 0, xs_step, -xs0, xs_mod))
    f.regs |> resize(nreg * 2)
    for (r in range(nreg)) {
        f.regs[r * 2] = uint(r * d * units)
    }
    return <- f
}

//! the superblock decode GEMV family's kq members: the fourteen leaves past q8, in enum order
def kq_leaf_fmts() : array<int> => <- [for (f in type<KqFmt>); int(f); where kq_sb(f)]

let TILE_CELL_K = 768       //! every per-format tile cell's K: three unrolled superblock passes on both depths (a K-quant width is a multiple of 256, so no K-quant stamp ever takes the k loop's tail)
let TILE_CELL_D = 160       //! every tile cell's weight rows a plane: one full 128-tile and a 32-row remainder - the KHR arm's edge tile, the cm2 arms' clamped second tile over rows 32..160
let TILE_CELL_ROWS0 = 300   //! the tile cells' two regions: on every arm's rows a workgroup, whole tiles and an edge
let TILE_CELL_ROWS1 = 260

