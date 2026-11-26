options gen2
options indenting = 4
options _jit_fast_math = true   // match gemm_1core_probe / the end-to-end config
options _dasllama_internal = true

module _rm_fixture public

//! The quantized fixtures gen_tune_probe and rm_row_jam_race share: the mx4, kq and per-32 planes with their scalar
//! oracles, the row-major families' fixture (disk planes through the engine's gather, one activation row, the oracle),
//! the streamed shape the tuner races each family at, the family's bar, and the witness read that tells a stamp that
//! emitted from one that declined to its reference body.

require _gen_probe_fixture
require ../tests/_kq_dot.das    // kq_grp_row_dot_rt - the kq fixture's reference body
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_repack
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_gguf   // kq_transcode_superblock - the kq fixture pack rail
require dasllama/dasllama_common   // Model, kq_plane_pair, layout_gather_dispatch
require dasllama/dasllama_layout   // installs the layout hooks layout_gather_dispatch dispatches through
require daslib/f16_cvt
require daslib/random

struct Mx4Fixture {
    n, d, nb : int64
    wn : array<uint8>     // row-major nibble plane master — repacked per variant into a copy
    we : array<uint8>     // row-major E8M0 exponent plane master
    xq : array<int8>
    xs : array<float>
    yref : array<float>
}

def build_mx4_fixture(n, d : int64) : Mx4Fixture {
    var fx = Mx4Fixture(n = n, d = d, nb = n / 32l)
    var seed = int4(20260704, 5, 9, 2)
    fx.wn <- [for (_i in range64(d * n / 2l)); uint8(random_int(seed) % 256)]
    fx.we <- [for (_i in range64(d * fx.nb)); uint8(118 + random_int(seed) % 14)]
    var xrow : array<float>
    xrow |> resize(n)
    fx.xq |> resize(n)
    fx.xs |> resize(fx.nb)
    act_row(xrow, n, 0l)
    quantize_q8_0_into(xrow, n, fx.xq, fx.xs, 0l, 0l)
    // the oracle: hand interleaved-mx4 semantics off a grp4 plane copy (fold order is
    // layout-independent, like the Q8 oracle)
    fx.yref |> resize(d)
    var n4 := fx.wn
    var e4 := fx.we
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        repack_kq_grp(KqFmt.mx4, addr(n4[0]), addr(e4[0]), n, d, 4l)
        let lutp = addr(lut[0])
        let np = addr(n4[0])
        let ep = addr(e4[0])
        var yrp = addr(fx.yref[0])
        for (g in range64(d / 4l)) {
            let f = dot_mx4q8_laneq4(lutp, np + g * 4l * (n / 2l), ep + g * 4l * fx.nb, addr(fx.xq[0]), addr(fx.xs[0]), n)
            yrp[g * 4l + 0l] = f.x
            yrp[g * 4l + 1l] = f.y
            yrp[g * 4l + 2l] = f.z
            yrp[g * 4l + 3l] = f.w
        }
    }
    delete n4
    delete e4
    return <- fx
}

// ===== the kq companions (kq stage 4/5 — k4/k5/k6 gemv + tile) =====

struct KqFixture {
    fmt : int64           // 4 / 5 / 6 / 40
    n, d, ntok, nsb : int64
    kq : array<uint8>     // row-major master planes — repacked per variant into a copy
    ks : array<uint8>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>      // plain per-16 quant sums (the kq bsums contract)
    yref : array<float>
}

// int64 forwards to the ONE stride source (dasllama_gemm_schema) — the fixtures carry fmt as
// int64; the schema helpers keep the panic-on-unknown contract
def kq_qsb(fmt : int64) : int64 => dasllama_gemm_schema::kq_qsb(int(fmt))
def kq_ssb(fmt : int64) : int64 => dasllama_gemm_schema::kq_ssb(int(fmt))

// synthetic disk superblocks, varied per (row, superblock) so no two blocks repeat: any byte
// is a legal quant, and the small d/dmin keep the f16 (s, o) pairs finite and row sums tame
def pack_kq_scale_header(var blkb : array<uint8>; base : int) {
    let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
    let mbits = f32_to_f16(0.001 + 0.0001 * float(base % 7))
    blkb[0] = uint8(dbits & 0xFF)
    blkb[1] = uint8(dbits >> 8u)
    blkb[2] = uint8(mbits & 0xFF)
    blkb[3] = uint8(mbits >> 8u)
    for (j in range(4)) {
        let sc0 = (base * 5 + j * 11) % 64
        let sc4 = (base * 5 + (j + 4) * 11) % 64
        let mn0 = (base * 3 + j * 13) % 64
        let mn4 = (base * 3 + (j + 4) * 13) % 64
        blkb[4 + j] = uint8((sc0 & 63) | (((sc4 >> 4) & 3) << 6))
        blkb[8 + j] = uint8((mn0 & 63) | (((mn4 >> 4) & 3) << 6))
        blkb[12 + j] = uint8((sc4 & 15) | ((mn4 & 15) << 4))
    }
}

def pack_kq_disk_block(fmt : int64; var blkb : array<uint8>; base : int) {   // nolint:STYLE037,STYLE038 — the flat per-format ladder (one arm per kq format) is the honest shape
    if (fmt == 40l) {
        for (blk in range(8)) {   // 8 x 18B q4_0 disk blocks: f16 d + 16 nibble bytes
            let dbits = f32_to_f16(0.002 + 0.0001 * float((base + blk) % 13))
            blkb[blk * 18] = uint8(dbits & 0xFF)
            blkb[blk * 18 + 1] = uint8(dbits >> 8u)
            for (i in range(16)) {
                blkb[blk * 18 + 2 + i] = uint8((base * 37 + blk * 59 + i * 101 + 29) % 256)
            }
        }
    } elif (fmt == 44l) {
        // 136B IQ4_XS disk block: f16 d, u16 scales_h, 4 scales_l bytes, 128 nibble bytes — every byte pattern is legal
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[0] = uint8(dbits & 0xFF)
        blkb[1] = uint8(dbits >> 8u)
        blkb[2] = uint8((base * 7 + 3) % 256)
        blkb[3] = uint8((base * 11 + 5) % 256)
        for (i in range(4)) {
            blkb[4 + i] = uint8((base * 13 + i * 17 + 9) % 256)
        }
        for (i in range(128)) {
            blkb[8 + i] = uint8((base * 37 + i * 101 + 29) % 256)
        }
    } elif (fmt == 3l) {
        // 110B Q3_K disk block: 32 hmask, 64 qs, 12 packed 6-bit scales, f16 d — every byte pattern is legal
        for (i in range(96)) {
            blkb[i] = uint8((base * 37 + i * 101 + 29) % 256)
        }
        for (i in range(12)) {
            blkb[96 + i] = uint8((base * 29 + i * 71 + 5) % 256)
        }
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[108] = uint8(dbits & 0xFF)
        blkb[109] = uint8(dbits >> 8u)
    } elif (fmt == 33l || fmt == 35l) {
        // 110B IQ3_S disk block: f16 d, 64 grid-index qs, 8 qh, 32 signs, 4 packed 4-bit scales — every byte pattern is legal
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[0] = uint8(dbits & 0xFF)
        blkb[1] = uint8(dbits >> 8u)
        for (i in range(104)) {
            blkb[2 + i] = uint8((base * 37 + i * 101 + 29) % 256)
        }
        for (i in range(4)) {
            blkb[106 + i] = uint8((base * 29 + i * 71 + 5) % 256)
        }
    } elif (fmt == 34l) {
        // 98B IQ3_XXS disk block: f16 d, 64 grid-index qs, 8 x aux32 — every byte pattern is legal
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[0] = uint8(dbits & 0xFF)
        blkb[1] = uint8(dbits >> 8u)
        for (i in range(96)) {
            blkb[2 + i] = uint8((base * 37 + i * 101 + 29) % 256)
        }
    } elif (fmt == 45l) {
        // 8 x 18B IQ4_NL disk blocks: per block f16 d + 16 nibble bytes — every pattern is legal
        for (b in range(8)) {
            let dbits = f32_to_f16(0.002 + 0.0001 * float((base + b) % 13))
            blkb[b * 18] = uint8(dbits & 0xFF)
            blkb[b * 18 + 1] = uint8(dbits >> 8u)
            for (i in range(16)) {
                blkb[b * 18 + 2 + i] = uint8((base * 41 + b * 17 + i * 89 + 7) % 256)
            }
        }
    } elif (fmt == 49l) {
        // 4 x 36B NVFP4 disk blocks: four UE4M3 sub-block scales then 32 nibble bytes - every byte pattern is legal (0x7F reads as a zero scale)
        for (b in range(4)) {
            for (i in range(36)) {
                blkb[b * 36 + i] = uint8((base * 43 + b * 19 + i * 97 + 11) % 256)
            }
        }
    } elif (fmt == 2l) {
        // 84B Q2_K disk block: 16 sc/min bytes, 64 qs, f16 d, f16 dmin - every byte pattern is legal
        for (i in range(80)) {
            blkb[i] = uint8((base * 37 + i * 101 + 29) % 256)
        }
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[80] = uint8(dbits & 0xFF)
        blkb[81] = uint8(dbits >> 8u)
        let mbits = f32_to_f16(0.001 + 0.0001 * float((base + 5) % 11))
        blkb[82] = uint8(mbits & 0xFF)
        blkb[83] = uint8(mbits >> 8u)
    } elif (fmt == 24l) {
        // 74B IQ2_XS disk block: f16 d, 32 u16 qs words, 8 packed scale nibbles - every byte pattern is legal
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[0] = uint8(dbits & 0xFF)
        blkb[1] = uint8(dbits >> 8u)
        for (i in range(72)) {
            blkb[2 + i] = uint8((base * 41 + i * 97 + 17) % 256)
        }
    } elif (fmt == 25l) {
        // 66B IQ2_XXS disk block: f16 d, then 8 blocks x [4 grid bytes][aux32 signs + ls] - every byte pattern is legal
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[0] = uint8(dbits & 0xFF)
        blkb[1] = uint8(dbits >> 8u)
        for (i in range(64)) {
            blkb[2 + i] = uint8((base * 41 + i * 97 + 17) % 256)
        }
    } elif (fmt == 23l) {
        // 82B IQ2_S disk block: f16 d, 32 idx + 32 sign bytes, 8 qh, 8 packed scale nibbles - every byte pattern is legal
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[0] = uint8(dbits & 0xFF)
        blkb[1] = uint8(dbits >> 8u)
        for (i in range(80)) {
            blkb[2 + i] = uint8((base * 37 + i * 101 + 29) % 256)
        }
    } elif (fmt == 4l) {
        pack_kq_scale_header(blkb, base)
        for (i in range(128)) {
            blkb[16 + i] = uint8((base * 37 + i * 101 + 29) % 256)
        }
    } elif (fmt == 5l) {
        pack_kq_scale_header(blkb, base)
        for (i in range(32)) {
            blkb[16 + i] = uint8((base * 53 + i * 89 + 17) % 256)   // qh
        }
        for (i in range(128)) {
            blkb[48 + i] = uint8((base * 37 + i * 101 + 29) % 256)  // qs
        }
    } else {
        for (i in range(128)) {
            blkb[i] = uint8((base * 37 + i * 101 + 29) % 256)       // ql
        }
        for (i in range(64)) {
            blkb[128 + i] = uint8((base * 53 + i * 89 + 17) % 256)  // qh
        }
        for (i in range(16)) {
            blkb[192 + i] = uint8((base * 29 + i * 71 + 5) % 256)   // signed sub-scales
        }
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[208] = uint8(dbits & 0xFF)
        blkb[209] = uint8(dbits >> 8u)
    }
}

def repack_kq_grp_fmt(fmt : int64; var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    repack_kq_grp(kq_fmt_of_id(int(fmt), "repack_kq_grp_fmt"), kq, ks, n, d, mr)
}

def build_kq_fixture(fmt : int64; n, d, ntok : int64) : KqFixture {
    var fx = KqFixture(fmt = fmt, n = n, d = d, ntok = ntok, nsb = n / 256l)
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    fx.kq |> reserve(d * fx.nsb * qsb)
    fx.ks |> reserve(d * fx.nsb * ssb)
    fx.kq |> resize(d * fx.nsb * qsb)
    fx.ks |> resize(d * fx.nsb * ssb)
    let kf = kq_fmt_of_id(int(fmt), "build_kq_fixture")
    var blkb : array<uint8>
    blkb |> resize(kq_desc(kf).disk_bytes)
    for (r in range64(d)) {
        for (s in range64(fx.nsb)) {
            let base = int(r * 31l + s * 7l)
            pack_kq_disk_block(fmt, blkb, base)
            let sb = r * fx.nsb + s
            kq_transcode_superblock(kf, blkb, 0l, fx.kq, sb * qsb, fx.ks, sb * ssb)
        }
    }
    delete blkb
    // activations: the production quantizer, whose bsums plane IS the kq contract (per-16
    // sums). kq v2: every format takes the Q8_K form — ONE scale per 256-superblock
    var xrow : array<float>
    xrow |> resize(n)
    fx.xq |> resize(ntok * n)
    fx.xs |> resize(ntok * (n / 256l))
    fx.xbs |> resize(ntok * (n / 16l))
    for (tk in range64(ntok)) {
        act_row(xrow, n, tk)
        quantize_q8_k_into(xrow, n, fx.xq, fx.xs, fx.xbs, tk * n, tk * (n / 256l), tk * (n / 16l))
    }
    delete xrow
    // the oracle: the scalar grp walk off a grp4 copy (fold order is layout-independent)
    fx.yref |> resize(ntok * d)
    var q4 := fx.kq
    var s4 := fx.ks
    unsafe {
        repack_kq_grp_fmt(fmt, addr(q4[0]), addr(s4[0]), n, d, 4l)
        for (g in range64(d / 4l)) {
            let kqg = addr(q4[0]) + g * 4l * fx.nsb * qsb
            let ksg = addr(s4[0]) + g * 4l * fx.nsb * ssb
            for (tk in range64(ntok)) {
                for (r in range64(4l)) {
                    fx.yref[tk * d + g * 4l + r] = kq_grp_row_dot_rt(kf, kqg, ksg, r, 4l,
                        addr(fx.xq[0]) + tk * n, addr(fx.xs[0]) + tk * (n / 256l), addr(fx.xbs[0]) + tk * (n / 16l), n)
                }
            }
        }
    }
    delete q4
    delete s4
    return <- fx
}

//! whether a variant's stamp emitted on this box - its witness companion's answer; the reference row counts, being the body every declined variant also runs
def emits_of(wvs : array<tuple<string; function<() : bool>>>; suffix : string) : bool {
    return true if (suffix == "reference")
    for (v in wvs) {
        if (v._0 == suffix) {
            return invoke(v._1)
        }
    }
    return false
}

// k6's signed sub-scales cancel within a row, so element-relative tolerance is too strict at
// production shapes (intermediate terms are large, the sum is small) — gate against the
// fixture's magnitude instead; a real generator bug (wrong stride/lane/scale) is orders louder
// than fast-math fold-order noise
def kq_tol(yref : array<float>; count : int) : float => close_tol(max_abs(yref, count))

struct B32Fixture {
    fmt : KqFmt
    n, d : int64
    nb : int64
    qb, sb : int64          // the format's quant / scale bytes a block (kq_desc's strides)
    qq : array<uint8>       // disk-order quant plane
    qs : array<uint8>       // disk-order f16 scale plane (q51: d, m; iq4nl32: d)
    xq : array<int8>
    xs : array<float>
    xbs : array<int>        // per-32 activation sums (the per-32 gemv contract; the q51 fold reads them)
    yref : array<float>
}

//! an affine member's scalar disk-order dot
def private mlx_scalar_dot(fmt : KqFmt; wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?; n : int64) : float {
    if (fmt == KqFmt.mlx4) return dot_mlxq8_scalar(wq, ws, xq, xs, n, KqTag_mlx4())
    if (fmt == KqFmt.mlx8) return dot_mlxq8_scalar(wq, ws, xq, xs, n, KqTag_mlx8())
    if (fmt == KqFmt.mlx4b) return dot_mlxq8_scalar(wq, ws, xq, xs, n, KqTag_mlx4b())
    if (fmt == KqFmt.mlx8b) return dot_mlxq8_scalar(wq, ws, xq, xs, n, KqTag_mlx8b())
    return dot_mlxq8_scalar(wq, ws, xq, xs, n, KqTag_mlx6b())
}

//! the format's scalar disk-order dot - the oracle every per-32 variant is held to
def b32_scalar_dot(fmt : KqFmt; wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?; n : int64) : float {
    return mlx_scalar_dot(fmt, wq, ws, xq, xs, n) if (kq_mlx_affine(fmt))
    return (fmt == KqFmt.q51 ? dot_q51q8_scalar(wq, ws, xq, xs, n) : (fmt == KqFmt.q4032 ? dot_q4032q8_scalar(wq, ws, xq, xs, n)
        : (fmt == KqFmt.q5032 ? dot_q5032q8_scalar(wq, ws, xq, xs, n) : dot_iq4nl32q8_scalar(wq, ws, xq, xs, n))))
}

//! scale-plane half `bits` (an f16 pattern) as the format's half: BF16 on the affine BF16 members, the f16 pattern otherwise
def private b32_half_bits(fmt : KqFmt; bits : int) : int {
    return bits if (fmt != KqFmt.mlx4b && fmt != KqFmt.mlx8b && fmt != KqFmt.mlx6b)
    return int(unsafe(reinterpret<uint>(f16_to_f32(uint(bits)))) >> 16u)
}

def build_b32_fixture(fmt : KqFmt; n, d : int64) : B32Fixture {
    var fx = B32Fixture(fmt = fmt, n = n, d = d, nb = n / 32l, qb = kq_qsb(fmt), sb = kq_ssb(fmt))
    fx.qq |> reserve(d * fx.nb * fx.qb)
    fx.qs |> reserve(d * fx.nb * fx.sb)
    fx.qq |> resize(d * fx.nb * fx.qb)
    fx.qs |> resize(d * fx.nb * fx.sb)
    // valid small f16 scale patterns cycled a block (d, then m where the row holds one; an affine group's two blocks its scale, then its bias); codes an LCG walk
    let f16s = fixed_array(0x2E66, 0x3266, 0xB18F, 0x2C00, 0x3000, 0xB266, 0x2A8F, 0x3400)
    var seed = 0x12345
    for (i in range64(length(fx.qq))) {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff
        fx.qq[i] = uint8(seed >> 16)
    }
    for (r in range64(d)) {
        for (bi in range64(fx.nb)) {
            let sb = (r * fx.nb + bi) * fx.sb
            let dv = b32_half_bits(fmt, int(f16s[int(r * 3l + bi) % 8]))
            fx.qs[sb] = uint8(dv & 255)
            fx.qs[sb + 1l] = uint8(dv >> 8)
            if (fx.sb >= 4l) {
                let mv = f16s[int(r * 5l + bi * 7l + 3l) % 8]
                fx.qs[sb + 2l] = uint8(mv & 255)
                fx.qs[sb + 3l] = uint8(mv >> 8)
            }
        }
    }
    var xrow : array<float>
    xrow |> resize(n)
    for (i in range64(n)) {
        xrow[i] = float((i * 13l) % 37l) * 0.05 - 0.9
    }
    fx.xq |> resize(n)
    fx.xs |> resize(fx.nb)
    quantize_q8_0_into(xrow, n, fx.xq, fx.xs, 0l, 0l)
    delete xrow
    fx.xbs |> resize(fx.nb)
    for (bi in range64(fx.nb)) {
        var s = 0
        for (j in range64(32l)) {
            s += int(fx.xq[bi * 32l + j])
        }
        fx.xbs[bi] = s
    }
    fx.yref |> resize(d)
    unsafe {
        for (r in range64(d)) {
            fx.yref[r] = b32_scalar_dot(fmt, addr(fx.qq[0]) + r * fx.nb * fx.qb, addr(fx.qs[0]) + r * fx.nb * fx.sb,
                                        addr(fx.xq[0]), addr(fx.xs[0]), n)
        }
    }
    return <- fx
}

//! the per-32 rows' bar: they fold the block scale in float, so an absolute floor over the relative one
def b32_tol(yref : array<float>; count : int) : float => max(1.0e-3, max_abs(yref, count) * 5.0e-5)

//! the expert width each per-32 rail exists for: the 26B-A4B's q51 down, Flash-Next's iq4nl32 down, Qwen1.5-MoE's q4032 and q5032 down
def b32_rail_n(fmt : KqFmt) : int64 => fmt == KqFmt.q51 ? 704l : (fmt == KqFmt.q4032 || fmt == KqFmt.q5032 ? 1408l : 640l)

//! the planes a row-major family reads, one activation row, and the generator fixture's scalar oracle over it
struct RmFixture {
    fmt : KqFmt
    n, d : int64
    q : array<uint8>
    s : array<uint8>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>
    yref : array<float>
}

//! the q8 rows read binary16 scales, so their oracle is the fixture's dot over the narrowed scales, summed in double
def private rm_q8_oracle(qfx : Fixture) : array<float> {
    var y : array<float>
    y |> resize(qfx.d)
    for (r in range64(qfx.d)) {
        var acc = 0.0lf
        for (b in range64(qfx.nb)) {
            var s = 0
            for (j in range64(32l)) {
                s += int(qfx.q[r * qfx.n + b * 32l + j]) * int(qfx.xq[b * 32l + j])
            }
            acc += double(f16_to_f32(f32_to_f16(qfx.s[r * qfx.nb + b])) * qfx.xs[b]) * double(s)
        }
        y[r] = float(acc)
    }
    return <- y
}

def private put_planes(var m : Model; f : KqFmt; var q, s : array<uint8>) {
    kq_plane_pair(m, f) $(var mq : array<uint8>; var ms : array<uint8>) {
        delete mq
        delete ms
        mq <- q
        ms <- s
    }
}

//! the family's disk-order planes on the Model shell, and the activation row its generator fixture quantized
def private rm_disk_planes(f : KqFmt; n, d : int64; var m : Model; var fx : RmFixture) {
    if (f == KqFmt.q8) {
        var qfx <- build_fixture(n, d, 1l)
        fx.yref <- rm_q8_oracle(qfx)
        m.qblob <- qfx.q
        m.qscales <- qfx.s
        fx.xq <- qfx.xq
        fx.xs <- qfx.xs
        delete qfx
    } elif (f == KqFmt.mx4) {
        var mfx <- build_mx4_fixture(n, d)
        put_planes(m, f, mfx.wn, mfx.we)
        fx.xq <- mfx.xq
        fx.xs <- mfx.xs
        fx.xbs |> resize(mfx.nb)   //! never read by the mx4 rows; sized so the call carries a real pointer
        fx.yref <- mfx.yref
        delete mfx
    } elif (kq_sb(f)) {
        var kfx <- build_kq_fixture(int64(kq_desc(f).schema_id), n, d, 1l)
        put_planes(m, f, kfx.kq, kfx.ks)
        fx.xq <- kfx.xq
        fx.xs <- kfx.xs
        fx.xbs <- kfx.xbs
        fx.yref <- kfx.yref
        delete kfx
    } else {
        var bfx <- build_b32_fixture(f, n, d)
        put_planes(m, f, bfx.qq, bfx.qs)
        fx.xq <- bfx.xq
        fx.xs <- bfx.xs
        fx.xbs <- bfx.xbs
        fx.yref <- bfx.yref
        delete bfx
    }
}

[arch(at = "../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#rm-family-race")]
def build_rm_fixture(f : KqFmt; n, d : int64) : RmFixture {
    var fx = RmFixture(fmt = f, n = n, d = d)
    var m = Model()
    rm_disk_planes(f, n, d, m, fx)
    layout_gather_dispatch(m, f, 0l, n, d, d, 1l, 32l, 0l, fx.q, fx.s, true)
    delete m
    return <- fx
}

def rm_streamed_shape(f : KqFmt) : tuple<n : int64; d : int64> {
    if (f == KqFmt.q8 || f == KqFmt.mx4) return (n = 2048l, d = 8192l)
    if (kq_sb(f)) return (n = 2048l, d = 16384l)
    return (n = b32_rail_n(f), d = 32768l)
}

//! the family's bar: the kq and q8 rows against the fixture's magnitude, the per-32 rows against their float fold
def rm_tol(fx : RmFixture) : float {
    return kq_sb(fx.fmt) || fx.fmt == KqFmt.q8 ? kq_tol(fx.yref, int(fx.d)) : b32_tol(fx.yref, int(fx.d))
}

//! a row-major family's gemv over every row of `fx`: `q8` for the q8 family, `kq` for every other
def rm_run(q8 : MatmulQ8Q8RowsFnS16; kq : MatmulKqRowsFn; fx : RmFixture; var y : array<float>) {
    unsafe {
        if (fx.fmt == KqFmt.q8) {
            invoke(q8, addr(y[0]), addr<int8 const?>(fx.q[0]), addr<uint16 const?>(fx.s[0]), addr<int8 const?>(fx.xq[0]),
                   addr<float const?>(fx.xs[0]), fx.n, 0l, fx.d)
        } else {
            invoke(kq, addr(y[0]), addr<uint8 const?>(fx.q[0]), addr<uint8 const?>(fx.s[0]), addr<int8 const?>(fx.xq[0]),
                   addr<float const?>(fx.xs[0]), addr<int const?>(fx.xbs[0]), fx.n, 0l, fx.d)
        }
    }
}
