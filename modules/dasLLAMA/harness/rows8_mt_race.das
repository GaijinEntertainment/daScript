options gen2
options persistent_heap
options _dasllama_internal = true

// ROWS8 MATRIX-UNIT RACE - the raw-MSL eight-row GEMM prototypes (rows8_mt.metal: the k4, q8, k6 and affine register-left
// kernels the engine's rows8 form was derived from) on the 27B's plane shapes at several K ways, against a CPU reference on the
// first rows, beside the one-row references (k4_gemv1 / k6_gemv1 / mlx4_gemv1 / mlx8_gemv1) and - with DASLLAMA_METAL_MSL_DUMP naming a directory a
// model run filled - the engine's own generated kernels. Light on purpose: no engine kernel module, so an edit to the
// .metal reruns in seconds. DASLLAMA_ROWS8_ARMS (comma list of arm names) and DASLLAMA_ROWS8_SHAPES (comma list of tag words)
// narrow a run; DASLLAMA_ROWS8_TG=1 races the in-threadgroup K split arms against the split-K form and its reduce.
//
//   bin/daslang -jit modules/dasLLAMA/harness/rows8_mt_race.das

require daslib/fio
require daslib/strings_boost
require daslib/strings_convert
require daslib/math_bits
require math
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_env
require dasllama/dasllama_kqformat
require dasllama/dasllama_math

let CH = 8
let REF_ROWS = 64
let RING_BYTES = 256l * 1048576l   //! one plane copy past this size a copy
let RING_FLOOR = 140l * 1048576l   //! below it, three to eight copies, enough to stream at least this many bytes past the SLC

enum Fmt {
    k4
    q8
    k6
    mlx4b   //! the affine 4-bit member under BF16 pairs, the 27B snapshot's planes
    mlx8b
    iq4xs   //! IQ4_XS in the engine's split planes, the 35B's dense planes
}

let NFMT = 6

//! the x table a variant reads: the k-slot order of the 32 / 64-deep k4 kernels, the block-diagonal [16][K] table,
//! the 128-deep table (k4_rows8_d128), x in natural order (k6, the one-row references), or the affine CONTIG arms'
//! 64-group order (slot 32 c + 16 i + 4 kq + e at physical 16 kq + 8 c + 4 i + e)
enum XTable {
    kslot
    bd
    d128
    natural
    contig
}

struct Shape {
    tag : string
    fmt : Fmt
    n : int64
    d : int64
    ways : int64[3]       //! the way counts raced; the last is the shape's even count, where the diag arms race; 0 skips
    one_row_us : double   //! the engine's one-row GEMV pass on this box (rows8_gemm_race), 0 where none was measured
}

//! a raced kernel: its entry per format ("" where it has none), the -D flags its source compiles with, its x table
struct Variant {
    name : string
    flags : string = ""
    entry : string = ""
    entry_q8 : string = ""
    entry_k6 : string = ""
    entry_mlx4 : string = ""
    entry_mlx8 : string = ""
    entry_iq4 : string = ""
    tab : XTable = XTable.kslot
    diag : bool = false      //! computes nothing checkable; races at the shape's even way count only
    one_row : bool = false   //! a one-token GEMV: one dispatch of d / 8 threadgroups, checked on token 0
    rs : int = 4             //! row tiles of 16 a threadgroup
    ks : int = 1             //! K slices a threadgroup (the in-threadgroup split; 1 is the shipped form's one slice a simdgroup)
}

typedef Planes = tuple<bq : MetalBuffer?; bs : MetalBuffer?; doff : uint64>
typedef BufPtr = MetalBuffer?

//! the engine kernel's bind order: ksc / wsh, kqu / wqb, xt, yk, xs, kdim, ndim
def private enc_engine(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; pl : Planes; bt, out, bk, bd, bxs : MetalBuffer?; d, nsk : int64) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, pl.bs, 0ul, 0)
    metal_set_buffer(enc, pl.bq, 0ul, 1)
    metal_set_buffer(enc, bt, 0ul, 2)
    metal_set_buffer(enc, out, 0ul, 3)
    metal_set_buffer(enc, bxs, 0ul, 4)
    metal_set_buffer(enc, bk, 0ul, 5)
    metal_set_buffer(enc, bd, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint(d / 64l), 1u, uint(nsk)), uint3(128u, 1u, 1u))
}

def private read_msl(path : string) : string {
    var src = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            src = fread(f)
        }
    }
    return src
}

def private chain_ms(queue : MetalCommandQueue?; step : block<(enc : MetalComputeEncoder?; i : int) : void>) : double {
    var err = ""
    var gpu = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu) $(enc) {
        for (i in range(CH)) {
            invoke(step, enc, i)
            metal_memory_barrier(enc)
        }
    }
    if (!ok) {
        print("chain: {err}\n")
    }
    return ok ? gpu : -1.0lf
}

def private site_us(queue : MetalCommandQueue?; step : block<(enc : MetalComputeEncoder?; i : int) : void>) : double {
    var burn = 0.0lf
    var rounds = 0
    while (burn < 150.0lf && rounds < 256) {
        let ms = chain_ms(queue, step)
        return -1.0lf if (ms < 0.0lf)
        burn += ms
        rounds++
    }
    var best = -1.0lf
    for (_r in range(7)) {
        let ms = chain_ms(queue, step)
        best = best < 0.0lf ? ms : min(best, ms)
    }
    return best * 1000.0lf / double(CH)
}

def private enc_mt(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; pl : Planes; bt, out, bk, bd, bnsk, bxs : MetalBuffer?; d, nsk : int64; one_row : bool;
                   rs : int = 4; ks : int = 1) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, pl.bs, 0ul, 0)
    metal_set_buffer(enc, pl.bq, 0ul, 1)
    metal_set_buffer(enc, bt, 0ul, 2)
    metal_set_buffer(enc, out, 0ul, 3)
    metal_set_buffer(enc, bk, 0ul, 4)
    metal_set_buffer(enc, bd, 0ul, 5)
    metal_set_buffer(enc, bnsk, 0ul, 6)
    metal_set_buffer(enc, bxs, 0ul, 7)
    if (one_row) {
        metal_dispatch_threadgroups(enc, uint3(uint(d / 8l), 1u, 1u), uint3(128u, 1u, 1u))
    } else {
        let rows_tg = 16 * rs
        metal_dispatch_threadgroups(enc, uint3(uint((int(d) + rows_tg - 1) / rows_tg), 1u, uint(nsk)), uint3(uint(32 * rs * ks), 1u, 1u))
    }
}

//! the split-K reduce the shipped form dispatches after a split site: `nsk` planes of [8][d] in `part` into `y`
def private enc_reduce(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; part, y, btot, bnsk : MetalBuffer?; d : int64) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, part, 0ul, 0)
    metal_set_buffer(enc, y, 0ul, 1)
    metal_set_buffer(enc, btot, 0ul, 2)
    metal_set_buffer(enc, bnsk, 0ul, 3)
    metal_dispatch_threadgroups(enc, uint3(uint((2l * d + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

//! the split-K form's ways rule (the 16 MiB scratch): the smallest divisor of `blocks` leaving two blocks a way that puts 2048
//! threadgroups on the grid, at most 64; else the largest that keeps the rest
def private engine_ways(tgs, blocks, rows : int64) : int64 {
    var best = 1l
    for (nsk in range64(2l, 65l)) {
        continue if (blocks % nsk != 0l || blocks / nsk < 2l || nsk * 8l * rows * 4l > 16l * 1048576l)
        return nsk if (tgs * nsk >= 2048l)
        best = nsk
    }
    return best
}

//! deterministic affine planes (mlx4b / mlx8b): hashed code words, one BF16 (scale, bias) pair a 64-group - pow2 scales,
//! eighth-valued biases of both signs, varying by group
def private mlx_planes(dev : MetalDevice?; d, n : int64; a8 : bool) : Planes {
    let words = d * n / (a8 ? 4l : 8l)
    var qplane : array<uint>
    qplane |> reserve_resize(words)
    for (i in range64(words)) {
        qplane[i] = uint(i) * 2654435761u
    }
    let ng = d * n / 64l
    var splane : array<uint>
    splane |> reserve_resize(ng)
    for (g in range64(ng)) {
        let s = 0.0078125 * float(1 << int(g % 3l))
        let b = 0.125 * float(int((g * 5l) % 7l) - 3)
        splane[g] = (float_bits_to_uint(s) >> 16u) | (float_bits_to_uint(b) & 0xFFFF0000u)
    }
    let bq = metal_buffer_from(dev, uint64(words) * 4ul, unsafe(addr<void?>(qplane[0])))
    let bs = metal_buffer_from(dev, uint64(ng) * 4ul, unsafe(addr<void?>(splane[0])))
    delete qplane
    delete splane
    return (bq = bq, bs = bs, doff = 0ul)
}

//! y[t][n] for the first REF_ROWS rows of affine planes (mlx_planes) off the plane bytes and the f16-rounded x
def private cpu_ref_mlx(pl : Planes; xa : array<float>; n : int64; a8 : bool) : array<float> {
    var ref : array<float>
    ref |> resize(8 * REF_ROWS)
    let ng = int(n / 64l)
    let rowb = int(n) / (a8 ? 1 : 2)
    unsafe {
        let qs = reinterpret<uint8?>(metal_buffer_contents(pl.bq))
        let ss = reinterpret<uint?>(metal_buffer_contents(pl.bs))
        for (row in range(REF_ROWS)) {
            var acc : float[8]
            for (g in range(ng)) {
                let p = ss[row * ng + g]
                let s = uint_bits_to_float(p << 16u)
                let b = uint_bits_to_float(p & 0xFFFF0000u)
                for (e in range(64)) {
                    let k = 64 * g + e
                    let q = a8 ? int(qs[row * rowb + k]) : (int(qs[row * rowb + k / 2]) >> (4 * (k & 1))) & 15
                    let w = s * float(q) + b
                    for (t in range(8)) {
                        acc[t] += w * float(float16(xa[t * int(n) + k]))
                    }
                }
            }
            for (t in range(8)) {
                ref[t * REF_ROWS + row] = acc[t]
            }
        }
    }
    return <- ref
}

//! y[t][n] for the first REF_ROWS rows of a q8_0 blob (34-byte blocks, row-major)
def private cpu_ref_q8(pl : Planes; xa : array<float>; n : int64) : array<float> {
    var ref : array<float>
    ref |> resize(8 * REF_ROWS)
    let nkb = int(n / 32l)
    unsafe {
        let blob = reinterpret<uint8?>(metal_buffer_contents(pl.bq))
        for (row in range(REF_ROWS)) {
            var acc : float[8]
            for (kb in range(nkb)) {
                let bp = blob + (row * nkb + kb) * 34
                let dd = float(reinterpret<float16?>(bp)[0])
                for (l in range(32)) {
                    let q = float(reinterpret<int8?>(bp + 2)[l])
                    let k = 32 * kb + l
                    for (t in range(8)) {
                        acc[t] += dd * q * float(float16(xa[t * int(n) + k]))
                    }
                }
            }
            for (t in range(8)) {
                ref[t * REF_ROWS + row] = acc[t]
            }
        }
    }
    return <- ref
}

//! deterministic q8_0 blob, 34-byte blocks: int8 codes of both signs, pow2 f16 scales
def private q8_blob(dev : MetalDevice?; nblk : int) : Planes {
    var blob : array<int8>
    blob |> resize(nblk * 34)
    for (blk in range(nblk)) {
        for (k in range(32)) {
            blob[blk * 34 + 2 + k] = int8((blk * 7 + k * 3) % 19 - 9)
        }
    }
    unsafe {
        var p8 = addr(blob[0])
        for (blk in range(nblk)) {
            var ph = reinterpret<float16?>(p8 + blk * 34)
            ph[0] = float16(0.5 * float(1 << (blk % 3)))
        }
    }
    let b = metal_buffer_from(dev, uint64(nblk * 34), unsafe(addr<void?>(blob[0])))
    return (bq = b, bs = b, doff = 0ul)
}

//! deterministic Q6_K planes in the engine's split form: 192-byte quant blocks of pseudo-random bits, the scale plane's
//! 16-byte strips of signed int8 scales varying by block and slot, then the f16 d tail (pow2, varying by block)
def private k6_planes(dev : MetalDevice?; nblk : int) : Planes {
    var qplane : array<uint>
    qplane |> reserve_resize(nblk * 48)
    for (i in range(nblk * 48)) {
        qplane[i] = uint(i) * 2654435761u
    }
    var splane : array<int8>
    splane |> reserve_resize(nblk * 18)
    for (blk in range(nblk)) {
        for (j in range(16)) {
            splane[blk * 16 + j] = int8((blk * 5 + j * 3) % 23 - 11)
        }
    }
    unsafe {
        var ph = addr<float16?>(splane[nblk * 16])
        for (blk in range(nblk)) {
            ph[blk] = float16(0.125 * float(1 << (blk % 3)))
        }
    }
    let bq = metal_buffer_from(dev, uint64(nblk) * 192ul, unsafe(addr<void?>(qplane[0])))
    let bs = metal_buffer_from(dev, uint64(nblk) * 18ul, unsafe(addr<void?>(splane[0])))
    delete qplane
    delete splane
    return (bq = bq, bs = bs, doff = uint64(nblk) * 16ul)
}

//! y[t][n] for the first REF_ROWS rows of Q6_K planes (k6_planes) off the plane bytes and the f16-rounded x
def private cpu_ref_k6(pl : Planes; xa : array<float>; n, d : int64) : array<float> {
    var ref : array<float>
    ref |> resize(8 * REF_ROWS)
    let nsb = int(n / 256l)
    unsafe {
        let qs = reinterpret<uint8?>(metal_buffer_contents(pl.bq))
        let ss = reinterpret<int8?>(metal_buffer_contents(pl.bs))
        let dt = reinterpret<float16?>(ss + d * int64(nsb) * 16l)
        for (row in range(REF_ROWS)) {
            var acc : float[8]
            for (sb in range(nsb)) {
                let blk = row * nsb + sb
                let dd = float(dt[blk])
                let ql = qs + blk * 192
                let qh = ql + 128
                for (k in range(256)) {
                    let h = k / 128
                    let g = (k % 128) / 32
                    let l = k % 32
                    let lo = (int(ql[64 * h + 32 * (g & 1) + l]) >> (4 * (g >> 1))) & 15
                    let hb = (int(qh[32 * h + l]) >> (2 * g)) & 3
                    let w = dd * float(ss[blk * 16 + 8 * h + 2 * g + l / 16]) * float((lo | (hb << 4)) - 32)
                    for (t in range(8)) {
                        acc[t] += w * float(float16(xa[t * int(n) + 256 * sb + k]))
                    }
                }
            }
            for (t in range(8)) {
                ref[t * REF_ROWS + row] = acc[t]
            }
        }
    }
    return <- ref
}

//! deterministic IQ4_XS planes in the engine's split form: 128-byte quant blocks of pseudo-random bits, the scale plane's
//! 16-byte strips (the eight signed sub-scales ls - 32 in bytes 0..7, varying by block and slot), then the f16 d tail
def private iq4_planes(dev : MetalDevice?; nblk : int) : Planes {
    var qplane : array<uint>
    qplane |> reserve_resize(nblk * 32)
    for (i in range(nblk * 32)) {
        qplane[i] = uint(i) * 2654435761u
    }
    var splane : array<int8>
    splane |> reserve_resize(nblk * 18)
    for (blk in range(nblk)) {
        for (j in range(16)) {
            splane[blk * 16 + j] = int8(j < 8 ? (blk * 5 + j * 3) % 23 - 11 : 0)
        }
    }
    unsafe {
        var ph = addr<float16?>(splane[nblk * 16])
        for (blk in range(nblk)) {
            ph[blk] = float16(0.0078125 * float(1 << (blk % 3)))
        }
    }
    let bq = metal_buffer_from(dev, uint64(nblk) * 128ul, unsafe(addr<void?>(qplane[0])))
    let bs = metal_buffer_from(dev, uint64(nblk) * 18ul, unsafe(addr<void?>(splane[0])))
    delete qplane
    delete splane
    return (bq = bq, bs = bs, doff = uint64(nblk) * 16ul)
}

//! y[t][n] for the first REF_ROWS rows of IQ4_XS planes (iq4_planes) off the plane bytes and the f16-rounded x
def private cpu_ref_iq4(pl : Planes; xa : array<float>; n, d : int64) : array<float> {
    var ref : array<float>
    ref |> resize(8 * REF_ROWS)
    let nsb = int(n / 256l)
    let lut = iq4nl_lut()
    unsafe {
        let qs = reinterpret<uint8?>(metal_buffer_contents(pl.bq))
        let ss = reinterpret<int8?>(metal_buffer_contents(pl.bs))
        let dt = reinterpret<float16?>(ss + d * int64(nsb) * 16l)
        for (row in range(REF_ROWS)) {
            var acc : float[8]
            for (sb in range(nsb)) {
                let blk = row * nsb + sb
                let dd = float(dt[blk])
                for (k in range(256)) {
                    let js = k / 32
                    let i = k % 32
                    let q = (int(qs[blk * 128 + 16 * js + (i & 15)]) >> (4 * (i >> 4))) & 15
                    let w = dd * float(ss[blk * 16 + js]) * float(lut[q])
                    for (t in range(8)) {
                        acc[t] += w * float(float16(xa[t * int(n) + 256 * sb + k]))
                    }
                }
            }
            for (t in range(8)) {
                ref[t * REF_ROWS + row] = acc[t]
            }
        }
    }
    return <- ref
}

//! q4_K sc / min of sub-block js from the 12 kmask bytes
def private kmask(q : uint8?; js : int; var sc, mn : int&) {
    unsafe {
        if (js < 4) {
            sc = int(q[js]) & 63
            mn = int(q[js + 4]) & 63
        } else {
            sc = (int(q[js + 4]) & 15) | ((int(q[js - 4]) >> 6) << 4)
            mn = (int(q[js + 4]) >> 4) | ((int(q[js]) >> 6) << 4)
        }
    }
}

//! y[t][n] for the first REF_ROWS rows, off the plane bytes and the f16-rounded x
def private cpu_ref(pl : Planes; xa : array<float>; n : int64) : array<float> {
    var ref : array<float>
    ref |> resize(8 * REF_ROWS)
    let nsb = int(n / 256l)
    unsafe {
        let qs = reinterpret<uint8?>(metal_buffer_contents(pl.bq))
        let ss = reinterpret<uint8?>(metal_buffer_contents(pl.bs))
        for (row in range(REF_ROWS)) {
            var acc : float[8]
            for (sb in range(nsb)) {
                let blk = row * nsb + sb
                let sp = ss + blk * 16
                let dd = float(reinterpret<float16?>(sp)[0])
                let dm = float(reinterpret<float16?>(sp)[1])
                let qp = qs + blk * 128
                for (s in range(8)) {
                    var sc = 0
                    var mn = 0
                    kmask(sp + 4, s, sc, mn)
                    let dsc = dd * float(sc)
                    let dmn = dm * float(mn)
                    for (l in range(32)) {
                        let q = (int(qp[32 * (s / 2) + l]) >> (4 * (s & 1))) & 15
                        let w = dsc * float(q) - dmn
                        let k = 256 * sb + 32 * s + l
                        for (t in range(8)) {
                            acc[t] += w * float(float16(xa[t * int(n) + k]))
                        }
                    }
                }
            }
            for (t in range(8)) {
                ref[t * REF_ROWS + row] = acc[t]
            }
        }
    }
    return <- ref
}

//! the raced kernels: per variant its pipeline for each format (null where it has no entry for that format) and the
//! engine's dumped sources where present
struct Race {
    variants : array<Variant>
    psos : array<MetalComputePipeline?>   //! variant v's pipeline for format f at NFMT v + int(f)
    p_eng_k4, p_eng_q8 : MetalComputePipeline?
}

//! the x table in k-slot order and the 32-block token sums off the panel `xa` [8][n]
def private build_tables(xa : array<float>; n : int64; var xp : array<float16>; var xsum : array<float>) {
    let nsbx = int(n / 256l)
    xp |> resize(8 * int(n))
    xsum |> resize(nsbx * 8 * 8)
    for (t in range(8)) {
        for (sg in range(nsbx * 8)) {
            var sum = 0.0
            for (slot in range(32)) {
                let kq = (slot % 16) / 4
                let i = slot % 4
                let phys = slot < 16 ? 8 * kq + i : 8 * kq + 4 + i
                let v = float16(xa[t * int(n) + 32 * sg + phys])
                xp[t * int(n) + 32 * sg + slot] = v
                sum += float(v)
            }
            xsum[sg * 8 + t] = sum
        }
    }
}

//! the block-diagonal x table [16][n] off the k-slot table `xp` [8][n]: column c < 8 keeps token c's x on each 64-group's
//! first 32 k, column 8 + c on its last 32, zero elsewhere
def private build_bd_table(xp : array<float16>; n : int64) : array<float16> {
    var xb : array<float16>
    xb |> resize(16l * n)
    for (t in range64(8l)) {
        for (k in range64(n)) {
            let col = (k & 63l) < 32l ? t : 8l + t
            xb[col * n + k] = xp[t * n + k]
        }
    }
    return <- xb
}

//! the 128-deep x table [8][n] (k4_rows8_d128): product slot k = 32 c + 16 i + 4 kq + e of product G (superblock G / 2,
//! half-pair jp = G % 2) carries the x of sub-block 4 jp + 2 (kq >> 1) + (c >> 1), position 16 (kq & 1) + 4 (2 (c & 1) + i) + e
def private build_d128_table(xa : array<float>; n : int64) : array<float16> {
    var xd : array<float16>
    xd |> resize(8l * n)
    for (t in range64(8l)) {
        for (k in range64(n)) {
            let sb = k / 256l
            let jp = (k / 128l) & 1l
            let slot = k & 127l
            let c = slot >> 5l
            let i = (slot >> 4l) & 1l
            let kq = (slot >> 2l) & 3l
            let e = slot & 3l
            let s = 4l * jp + 2l * (kq >> 1l) + (c >> 1l)
            let p = 16l * (kq & 1l) + 4l * (2l * (c & 1l) + i) + e
            xd[t * n + k] = float16(xa[t * n + sb * 256l + 32l * s + p])
        }
    }
    return <- xd
}

//! the CONTIG arms' table [8][n]: 64-group slot 32 c + 16 i + 4 kq + e carries physical element 16 kq + 8 c + 4 i + e
def private build_contig_table(xa : array<float>; n : int64) : array<float16> {
    var xc : array<float16>
    xc |> resize(8l * n)
    for (t in range64(8l)) {
        for (k in range64(n)) {
            let slot = k & 63l
            let c = slot >> 5l
            let i = (slot >> 4l) & 1l
            let kq = (slot >> 2l) & 3l
            let e = slot & 3l
            xc[t * n + k] = float16(xa[t * n + (k & ~63l) + 16l * kq + 8l * c + 4l * i + e])
        }
    }
    return <- xc
}

//! x in natural order [8][n] and its 16-group token sums [n / 16][8]
def private build_natural(xa : array<float>; n : int64; var xn : array<float16>; var xs16 : array<float>) {
    xn |> resize(8l * n)
    xs16 |> resize(n / 2l)
    for (t in range64(8l)) {
        for (k in range64(n)) {
            let v = float16(xa[t * n + k])
            xn[t * n + k] = v
            xs16[(k / 16l) * 8l + t] += float(v)
        }
    }
}

//! the largest difference of `by`'s first rows from the reference over its first `toks` tokens, and the reference's largest magnitude
def private worst_diff(by : MetalBuffer?; ref : array<float>; d : int64; toks : int) : tuple<diff : float; mag : float> {
    var worst = 0.0
    var mag = 0.0
    unsafe {
        let py = reinterpret<float?>(metal_buffer_contents(by))
        for (t in range(toks)) {
            for (row in range(REF_ROWS)) {
                worst = max(worst, abs(py[t * int(d) + row] - ref[t * REF_ROWS + row]))
                mag = max(mag, abs(ref[t * REF_ROWS + row]))
            }
        }
    }
    return (diff = worst, mag = mag)
}

//! fill the checked rows of `by` with 1e30, so a kernel that leaves them unwritten fails the check instead of inheriting the last one's
def private poison(by : MetalBuffer?; d : int64) {
    unsafe {
        var py = reinterpret<float?>(metal_buffer_contents(by))
        for (t in range(8)) {
            for (row in range(REF_ROWS)) {
                py[t * int(d) + row] = 1.0e30
            }
        }
    }
}

def private print_arm(name : string; us : double; bytes : int64; ref_us : double) {
    let gbs = us > 0.0lf ? double(bytes) / (us * 1000.0lf) : 0.0lf
    let x = ref_us > 0.0lf ? us / ref_us : 0.0lf
    print("  {name}\t{us:8.1f} us  {gbs:6.1f} GB/s  x{x:.2f}\n")
}

//! the per-shape buffers the arms bind: the ring of plane copies, each x table, the sums, the outputs
struct ShapeBufs {
    ring : array<Planes>
    tabs : BufPtr[5]   //! by XTable
    bxs : MetalBuffer?       //! the 32-block sums (k4, q8) or the 16-group sums (k6)
    y : MetalBuffer?
    part : MetalBuffer?
    bk : MetalBuffer?
    bd : MetalBuffer?
}

def private wanted(list : string; name : string) : bool {
    return true if (empty(list))
    for (w in split(list, ",")) {
        return true if (w == name)
    }
    return false
}

def private shape_wanted(list : string; tag : string) : bool {
    return true if (empty(list))
    for (w in split(list, ",")) {
        return true if (find(tag, w) >= 0)
    }
    return false
}

//! the shape's CPU reference on the first rows
def private cpu_ref_of(sh : Shape; pl : Planes; xa : array<float>) : array<float> {
    return <- cpu_ref_q8(pl, xa, sh.n) if (sh.fmt == Fmt.q8)
    return <- cpu_ref_k6(pl, xa, sh.n, sh.d) if (sh.fmt == Fmt.k6)
    return <- cpu_ref_iq4(pl, xa, sh.n, sh.d) if (sh.fmt == Fmt.iq4xs)
    return <- cpu_ref(pl, xa, sh.n) if (sh.fmt == Fmt.k4)
    return <- cpu_ref_mlx(pl, xa, sh.n, sh.fmt == Fmt.mlx8b)
}

//! every kernel at one way against the CPU reference on the first rows
def private check_shape(dev : MetalDevice?; queue : MetalCommandQueue?; r : Race; sh : Shape; sbf : ShapeBufs; xa : array<float>; arms : string) {
    var err = ""
    let d = sh.d
    let pl = sbf.ring[0]
    var bnsk1 = race_uniform_u32(dev, 1u)
    var inscope ref <- cpu_ref_of(sh, pl, xa)
    for (v, vi in r.variants, range(length(r.variants))) {
        let ps = r.psos[NFMT * vi + int(sh.fmt)]
        continue if (ps == null || v.diag || !wanted(arms, v.name))
        poison(sbf.y, d)
        var gms = 0.0lf
        with_compute_encoder_timed(queue, err, gms) $(enc) {
            enc_mt(enc, ps, pl, sbf.tabs[int(v.tab)], sbf.y, sbf.bk, sbf.bd, bnsk1, sbf.bxs, d, 1l, v.one_row, v.rs, v.ks)
        }
        let w = worst_diff(sbf.y, ref, d, v.one_row ? 1 : 8)
        print("  {v.name}@1 vs cpu ref (first {REF_ROWS} rows, {v.one_row ? 1 : 8} tokens): max |diff| {w.diff} of max |y| {w.mag}\n")
    }
    let peng = sh.fmt == Fmt.q8 ? r.p_eng_q8 : (sh.fmt == Fmt.k4 ? r.p_eng_k4 : null)
    if (peng != null) {
        poison(sbf.y, d)
        var gms = 0.0lf
        with_compute_encoder_timed(queue, err, gms) $(enc) {
            enc_engine(enc, peng, pl, sbf.tabs[int(XTable.kslot)], sbf.y, sbf.bk, sbf.bd, sbf.bxs, d, 1l)
        }
        let w = worst_diff(sbf.y, ref, d, 8)
        print("  engine@1 vs cpu ref (first {REF_ROWS} rows): max |diff| {w.diff}\n")
    }
    metal_release(bnsk1)
}

def private shape_bytes(sh : Shape) : int64 {
    if (sh.fmt == Fmt.q8) {
        return sh.d * sh.n / 32l * 34l
    }
    if (sh.fmt == Fmt.mlx4b || sh.fmt == Fmt.mlx8b) {
        return sh.d * sh.n / 64l * (sh.fmt == Fmt.mlx8b ? 68l : 36l)
    }
    return sh.d * sh.n / 256l * (sh.fmt == Fmt.k6 ? 210l : (sh.fmt == Fmt.iq4xs ? 136l : 144l))
}

def private make_bufs(dev : MetalDevice?; sh : Shape; xa : array<float>) : ShapeBufs {
    let n = sh.n
    let d = sh.d
    var sbf : ShapeBufs
    let sb = shape_bytes(sh)
    let copies = sb > RING_BYTES ? 1 : int(clamp((RING_FLOOR + sb - 1l) / sb, 3l, 8l))
    sbf.ring |> reserve(copies)
    for (_c in range(copies)) {
        if (sh.fmt == Fmt.q8) {
            sbf.ring |> push(q8_blob(dev, int(d * n / 32l)))
        } elif (sh.fmt == Fmt.k6) {
            sbf.ring |> push(k6_planes(dev, int(d * n / 256l)))
        } elif (sh.fmt == Fmt.iq4xs) {
            sbf.ring |> push(iq4_planes(dev, int(d * n / 256l)))
        } elif (sh.fmt == Fmt.mlx4b || sh.fmt == Fmt.mlx8b) {
            sbf.ring |> push(mlx_planes(dev, d, n, sh.fmt == Fmt.mlx8b))
        } else {
            sbf.ring |> push(race_kq_planes(dev, int(d * n / 256l), 32, KqRaceScaleForm.compact))
        }
    }
    var inscope xp : array<float16>
    var inscope xsum : array<float>
    build_tables(xa, n, xp, xsum)
    var inscope xb <- build_bd_table(xp, n)
    var inscope xd <- build_d128_table(xa, n)
    var inscope xn : array<float16>
    var inscope xs16 : array<float>
    build_natural(xa, n, xn, xs16)
    var inscope xc <- build_contig_table(xa, n)
    sbf.tabs[int(XTable.contig)] = metal_buffer_from(dev, uint64(long_length(xc)) * 2ul, unsafe(addr<void?>(xc[0])))
    sbf.tabs[int(XTable.kslot)] = metal_buffer_from(dev, uint64(long_length(xp)) * 2ul, unsafe(addr<void?>(xp[0])))
    sbf.tabs[int(XTable.bd)] = metal_buffer_from(dev, uint64(long_length(xb)) * 2ul, unsafe(addr<void?>(xb[0])))
    sbf.tabs[int(XTable.d128)] = metal_buffer_from(dev, uint64(long_length(xd)) * 2ul, unsafe(addr<void?>(xd[0])))
    sbf.tabs[int(XTable.natural)] = metal_buffer_from(dev, uint64(long_length(xn)) * 2ul, unsafe(addr<void?>(xn[0])))
    if (sh.fmt == Fmt.k6) {
        sbf.bxs = metal_buffer_from(dev, uint64(long_length(xs16)) * 4ul, unsafe(addr<void?>(xs16[0])))
    } else {
        sbf.bxs = metal_buffer_from(dev, uint64(long_length(xsum)) * 4ul, unsafe(addr<void?>(xsum[0])))
    }
    var maxw = 1l
    for (w in sh.ways) {
        maxw = max(maxw, w)
    }
    sbf.y = metal_buffer_from(dev, uint64(8l * d * 4l), null)
    sbf.part = metal_buffer_from(dev, uint64(maxw * 8l * d * 4l), null)
    sbf.bk = race_uniform_u32(dev, uint(n))
    sbf.bd = race_uniform_u32(dev, uint(d))
    return <- sbf
}

def private free_bufs(sbf : ShapeBufs) {
    for (pl in sbf.ring) {
        metal_release(pl.bq)
        if (pl.bs != pl.bq) {
            metal_release(pl.bs)
        }
    }
    for (t in sbf.tabs) {
        metal_release(t)
    }
    metal_release(sbf.bxs)
    metal_release(sbf.y)
    metal_release(sbf.part)
    metal_release(sbf.bk)
    metal_release(sbf.bd)
}

//! one plane shape: the buffers, the one-row reference first (the x-column's base where the shape has no engine figure),
//! every arm timed, every kernel checked at one way
def private race_shape(dev : MetalDevice?; queue : MetalCommandQueue?; r : Race; sh : Shape; arms : string) {
    let n = sh.n
    let d = sh.d
    let bytes = shape_bytes(sh)
    var inscope xa <- race_x_f32(int(8l * n))
    var sbf <- make_bufs(dev, sh, xa)
    let nring = length(sbf.ring)
    print("\n{sh.tag}  K={n} rows={d}  {bytes / 1048576l} MB  engine one-row pass {sh.one_row_us} us  ring {nring}\n")
    var ref_us = sh.one_row_us
    for (round in range(2)) {
        for (v, vi in r.variants, range(length(r.variants))) {
            let ps = r.psos[NFMT * vi + int(sh.fmt)]
            continue if (ps == null || v.one_row != (round == 0) || !wanted(arms, v.name))
            let tab = sbf.tabs[int(v.tab)]
            for (nsk in sh.ways) {
                continue if (nsk == 0l || (v.diag && nsk != sh.ways[2]) || (v.one_row && nsk != sh.ways[0]))
                var bnsk = race_uniform_u32(dev, uint(nsk))
                let us = site_us(queue) $(enc; i) {
                    enc_mt(enc, ps, sbf.ring[i % nring], tab, sbf.part, sbf.bk, sbf.bd, bnsk, sbf.bxs, d, nsk, v.one_row, v.rs, v.ks)
                }
                if (v.one_row && ref_us <= 0.0lf) {
                    ref_us = us
                }
                print_arm(v.one_row ? v.name : "{v.name}@{nsk}", us, bytes, ref_us)
                metal_release(bnsk)
            }
        }
    }
    let peng = sh.fmt == Fmt.q8 ? r.p_eng_q8 : (sh.fmt == Fmt.k4 ? r.p_eng_k4 : null)
    if (peng != null) {
        for (nsk in [8l, 16l]) {
            print_arm("engine@{nsk}", site_us(queue) $(enc; i) {
                enc_engine(enc, peng, sbf.ring[i % nring], sbf.tabs[int(XTable.kslot)], sbf.part, sbf.bk, sbf.bd, sbf.bxs, d, nsk)
            }, bytes, ref_us)
        }
    }
    check_shape(dev, queue, r, sh, sbf, xa, arms)
    free_bufs(sbf)
}

//! ROWS8_TG mode: the shipped form (`shipped`: one slice a simdgroup at the engine's ways, the reduce dispatch after it) against
//! every in-threadgroup split arm (`ks` > 1) at one and two outer ways (the reduce after two), each checked at one way, the
//! one-row reference first as the x column's base
def private race_shape_tg(dev : MetalDevice?; queue : MetalCommandQueue?; r : Race; sh : Shape; arms, shipped : string; p_red : MetalComputePipeline?) {
    let n = sh.n
    let d = sh.d
    let bytes = shape_bytes(sh)
    var inscope xa <- race_x_f32(int(8l * n))
    var sbf <- make_bufs(dev, sh, xa)
    let nring = length(sbf.ring)
    let blocks = sh.fmt == Fmt.k4 || sh.fmt == Fmt.iq4xs ? n / 256l : n / 64l
    let ways = engine_ways((d + 63l) / 64l, blocks, d)
    var part = metal_buffer_from(dev, uint64(max(ways, 2l) * 8l * d * 4l), null)
    var btot = race_uniform_u32(dev, uint(2l * d))
    print("\n{sh.tag}  K={n} rows={d}  {bytes / 1048576l} MB  ring {nring}  engine ways {ways}\n")
    var arms_at : array<tuple<vi : int; nsk : int64>>
    arms_at |> reserve(2 * length(r.variants))
    for (v, vi in r.variants, range(length(r.variants))) {
        continue if (r.psos[NFMT * vi + int(sh.fmt)] == null || !(v.one_row || v.name == shipped || v.ks > 1) || !wanted(arms, v.name))
        var inscope outer <- v.one_row ? [1l] : (v.ks > 1 ? [1l, 2l] : [ways])
        for (nsk in outer) {
            arms_at |> push((vi = vi, nsk = nsk))
        }
    }
    let rounds = max(1, g_env_harness.rows8_rounds)
    var times : array<array<double>>
    times |> resize(length(arms_at))
    for (tv in times) {
        tv |> resize(rounds)
    }
    for (round in range(rounds)) {   //! each round times every arm once before the next starts
        for (a, ai in arms_at, range(length(arms_at))) {
            let v = r.variants[a.vi]
            let ps = r.psos[NFMT * a.vi + int(sh.fmt)]
            var bnsk = race_uniform_u32(dev, uint(a.nsk))
            times[ai][round] = site_us(queue) $(enc; i) {
                enc_mt(enc, ps, sbf.ring[i % nring], sbf.tabs[int(v.tab)], a.nsk > 1l ? part : sbf.y, sbf.bk, sbf.bd, bnsk, sbf.bxs, d, a.nsk, v.one_row, v.rs, v.ks)
                if (a.nsk > 1l) {
                    metal_memory_barrier(enc)
                    enc_reduce(enc, p_red, part, sbf.y, btot, bnsk, d)
                }
            }
            metal_release(bnsk)
        }
    }
    var ref_us = 0.0lf
    var ship_us = 0.0lf
    for (a, ai in arms_at, range(length(arms_at))) {
        let v = r.variants[a.vi]
        var sum = 0.0lf
        for (us in times[ai]) {
            sum += us
        }
        let mean = sum / double(rounds)
        var dev2 = 0.0lf
        for (us in times[ai]) {
            dev2 += (us - mean) * (us - mean)
        }
        let cv = mean > 0.0lf ? sqrt(dev2 / double(rounds)) / mean * 100.0lf : 0.0lf
        if (v.one_row) {
            ref_us = mean
        } elif (v.name == shipped) {
            ship_us = mean
        }
        let vs = ship_us > 0.0lf && !v.one_row ? mean / ship_us : 0.0lf
        let gbs = mean > 0.0lf ? double(bytes) / (mean * 1000.0lf) : 0.0lf
        let x = ref_us > 0.0lf ? mean / ref_us : 0.0lf
        print("  {v.name}@{a.nsk}\t{mean:8.1f} us cv {cv:4.1f}%  {gbs:6.1f} GB/s  x{x:.2f} of one-row  {vs:.3f} of shipped{cv > 3.0lf ? "  VOID" : ""}\n")
    }
    times |> clear()
    arms_at |> clear()
    check_shape(dev, queue, r, sh, sbf, xa, arms)
    metal_release(part)
    metal_release(btot)
    free_bufs(sbf)
}

def private race_variants() : array<Variant> {
    //! `diag` arms (the split: loads alone, loads and fill, op alone) compute nothing checkable
    return <- [Variant(name = "one-row", entry = "k4_gemv1", entry_k6 = "k6_gemv1", entry_mlx4 = "mlx4_gemv1", entry_mlx8 = "mlx8_gemv1", entry_iq4 = "iq4_gemv1",
                       tab = XTable.natural, one_row = true),
               iq4_variant(0, 32), iq4_variant(1, 32), iq4_variant(2, 32), iq4_variant(0, 256), iq4_variant(1, 256), iq4_variant(2, 256),
               iq4_variant(1, 32, 16), iq4_variant(2, 32, 16), iq4_variant(1, 256, 16), iq4_variant(2, 256, 16), iq4_variant(2, 32, 4),
               Variant(name = "mlx h", entry_mlx4 = "mlx4_rows8", entry_mlx8 = "mlx8_rows8"),
               Variant(name = "mlx hcvt", flags = "#define LT_HCVT 1\n", entry_mlx4 = "mlx4_rows8", entry_mlx8 = "mlx8_rows8"),
               Variant(name = "mlx u8", flags = "#define LT_U8 1\n", entry_mlx4 = "mlx4_rows8", entry_mlx8 = "mlx8_rows8"),
               Variant(name = "mlx i8", flags = "#define LT_I8 1\n", entry_mlx4 = "mlx4_rows8", entry_mlx8 = "mlx8_rows8"),
               Variant(name = "mlx h contig", flags = "#define CONTIG 1\n", entry_mlx4 = "mlx4_rows8", entry_mlx8 = "mlx8_rows8", tab = XTable.contig),
               Variant(name = "mlx u8 contig", flags = "#define LT_U8 1\n#define CONTIG 1\n", entry_mlx4 = "mlx4_rows8", entry_mlx8 = "mlx8_rows8", tab = XTable.contig),
               Variant(name = "mlx i8 contig", flags = "#define LT_I8 1\n#define CONTIG 1\n", entry_mlx4 = "mlx4_rows8", entry_mlx8 = "mlx8_rows8", tab = XTable.contig),
               Variant(name = "k64", entry = "k4_rows8_mt64", entry_q8 = "q8_rows8_mt"),
               Variant(name = "k64 loadsonly", flags = "#define LOADSONLY 1\n", entry = "k4_rows8_mt64", entry_q8 = "q8_rows8_mt", diag = true),
               Variant(name = "k64 loadonly", flags = "#define LOADONLY 1\n", entry = "k4_rows8_mt64", entry_q8 = "q8_rows8_mt", diag = true),
               Variant(name = "k64 oponly", flags = "#define OPONLY 1\n", entry = "k4_rows8_mt64", entry_q8 = "q8_rows8_mt", diag = true),
               Variant(name = "d128", entry = "k4_rows8_d128", tab = XTable.d128),
               Variant(name = "d128 loadsonly", flags = "#define LOADSONLY 1\n", entry = "k4_rows8_d128", tab = XTable.d128, diag = true),
               Variant(name = "d128 loadonly", flags = "#define LOADONLY 1\n", entry = "k4_rows8_d128", tab = XTable.d128, diag = true),
               Variant(name = "d128 oponly", flags = "#define OPONLY 1\n", entry = "k4_rows8_d128", tab = XTable.d128, diag = true),
               Variant(name = "mtg128", flags = "#define KK 128\n", entry = "k4_rows8_mtg"),
               Variant(name = "k6 d16", entry_k6 = "k6_rows8_d16", tab = XTable.natural),
               Variant(name = "k6 d16 u8", flags = "#define K6_U8 1\n", entry_k6 = "k6_rows8_d16", tab = XTable.natural),
               Variant(name = "k6 d16 loadsonly", flags = "#define LOADSONLY 1\n", entry_k6 = "k6_rows8_d16", tab = XTable.natural, diag = true),
               Variant(name = "k6 d16 oponly", flags = "#define OPONLY 1\n", entry_k6 = "k6_rows8_d16", tab = XTable.natural, diag = true),
               Variant(name = "k64 pf", flags = "#define PF 1\n", entry = "k4_rows8_mt64"),
               Variant(name = "k64 pipe2", flags = "#define DEPTH 2\n", entry = "k4_rows8_mt64p"),
               Variant(name = "k64 pipe3", flags = "#define DEPTH 3\n", entry = "k4_rows8_mt64p"),
               Variant(name = "k64 subn", flags = "#define SUBN 1\n", entry = "k4_rows8_mt64"),
               Variant(name = "k4_rows8_u8", flags = "#define U8 1\n", entry = "k4_rows8_32"),
               Variant(name = "k4_rows8_h32", entry = "k4_rows8_32"),
               Variant(name = "k4_rows8_h32 subn", flags = "#define SUBN 1\n", entry = "k4_rows8_32"),
               Variant(name = "k4_rows8_bd", entry = "k4_rows8_bd", tab = XTable.bd),
               Variant(name = "q8_rows8_u8", entry_q8 = "q8_rows8_32"),
               Variant(name = "q8_rows8_i8", flags = "#define Q8_LT int8_t\n#define Q8_I8 1\n", entry_q8 = "q8_rows8_32"),
               Variant(name = "q8_rows8_h32", flags = "#define Q8_LT half\n#define Q8_HALF 1\n", entry_q8 = "q8_rows8_32"),
               Variant(name = "q8_rows8_bd", entry_q8 = "q8_rows8_bd", tab = XTable.bd),
               tg_variant(1, 2, false), tg_variant(1, 4, false), tg_variant(1, 8, false), tg_variant(1, 16, false),
               tg_variant(2, 4, false), tg_variant(2, 8, false), tg_variant(4, 2, false), tg_variant(4, 4, false),
               tg_variant(1, 4, true), tg_variant(1, 8, true), tg_variant(2, 4, true), tg_variant(4, 4, true),
               tg_variant(1, 16, true), tg_variant(2, 8, true)]
}

//! an IQ4_XS arm on the engine's geometry (one tile a threadgroup, eight interleaved slices): the codebook lookup `lut` (0 the
//! register byte-select, 1 the 16-entry table, 2 the 256-entry byte table) and the slices' unit `blk` (32 or 256 elements)
def private iq4_variant(lut, blk : int; ks : int = 8) : Variant {
    let names = fixed_array("reg", "tg16", "tg256")
    let ksn = ks != 8 ? " ks" + string(ks) : ""
    return Variant(name = "iq4 {names[lut]} b{blk}{ksn}",
                   flags = "#define TG_RS 1\n#define TG_KS {ks}\n#define TG_IL 1\n#define IQ_LUT {lut}\n#define IQ_BLK {blk}u\n",
                   entry_iq4 = "iq4_rows8_tg", tab = XTable.natural, rs = 1, ks = ks)
}

//! an in-threadgroup split arm: `rs` row tiles and `ks` K slices a threadgroup, the slices contiguous or interleaved (`il`)
def private tg_variant(rs, ks : int; il : bool) : Variant {
    let flags = "#define TG_RS {rs}\n#define TG_KS {ks}\n{il ? "#define TG_IL 1\n" : ""}"
    return Variant(name = "tg rs{rs} ks{ks}{il ? " il" : ""}", flags = flags, entry = "k4_rows8_tg", entry_mlx4 = "mlx4_rows8_tg", entry_mlx8 = "mlx8_rows8_tg",
                   rs = rs, ks = ks)
}

//! the raced plane shapes: the 27B's K-quant and affine planes, the 35B's IQ4_XS dense planes
def private race_shapes() : array<Shape> {
    return <- [Shape(tag = "ffn gate/up k4", fmt = Fmt.k4, n = 5120l, d = 17408l, ways = fixed_array<int64>(8l, 16l, 10l), one_row_us = 87.0lf),
               Shape(tag = "ffn down k4", fmt = Fmt.k4, n = 17408l, d = 5120l, ways = fixed_array<int64>(8l, 16l, 34l), one_row_us = 88.0lf),
               Shape(tag = "dn qkv q8", fmt = Fmt.q8, n = 5120l, d = 10240l, ways = fixed_array<int64>(8l, 16l, 0l), one_row_us = 94.3lf),
               Shape(tag = "attn wo k6", fmt = Fmt.k6, n = 5120l, d = 6144l, ways = fixed_array<int64>(5l, 20l, 10l), one_row_us = 0.0lf),
               Shape(tag = "classifier k6", fmt = Fmt.k6, n = 5120l, d = 248320l, ways = fixed_array<int64>(1l, 4l, 2l), one_row_us = 0.0lf),
               Shape(tag = "mlx ffn gate/up", fmt = Fmt.mlx4b, n = 5120l, d = 17408l, ways = fixed_array<int64>(4l, 16l, 8l), one_row_us = 0.0lf),
               Shape(tag = "mlx ffn down", fmt = Fmt.mlx4b, n = 17408l, d = 5120l, ways = fixed_array<int64>(17l, 68l, 34l), one_row_us = 0.0lf),
               Shape(tag = "mlx dn in", fmt = Fmt.mlx4b, n = 5120l, d = 16480l, ways = fixed_array<int64>(4l, 16l, 8l), one_row_us = 0.0lf),
               Shape(tag = "mlx dn out", fmt = Fmt.mlx4b, n = 6144l, d = 5120l, ways = fixed_array<int64>(16l, 48l, 32l), one_row_us = 0.0lf),
               Shape(tag = "mlx attn q", fmt = Fmt.mlx4b, n = 5120l, d = 12288l, ways = fixed_array<int64>(8l, 20l, 16l), one_row_us = 0.0lf),
               Shape(tag = "mlx attn kv", fmt = Fmt.mlx4b, n = 5120l, d = 1024l, ways = fixed_array<int64>(20l, 0l, 40l), one_row_us = 0.0lf),
               Shape(tag = "mlx head", fmt = Fmt.mlx4b, n = 5120l, d = 248320l, ways = fixed_array<int64>(1l, 0l, 2l), one_row_us = 0.0lf),
               Shape(tag = "mlx8 ffn gate/up", fmt = Fmt.mlx8b, n = 5120l, d = 17408l, ways = fixed_array<int64>(4l, 16l, 8l), one_row_us = 0.0lf),
               Shape(tag = "mlx8 ffn down", fmt = Fmt.mlx8b, n = 17408l, d = 5120l, ways = fixed_array<int64>(17l, 68l, 34l), one_row_us = 0.0lf),
               Shape(tag = "iq4 35b dn qkv | attn q", fmt = Fmt.iq4xs, n = 2048l, d = 8192l, ways = fixed_array<int64>(1l, 0l, 0l), one_row_us = 0.0lf),
               Shape(tag = "iq4 35b dn z", fmt = Fmt.iq4xs, n = 2048l, d = 4096l, ways = fixed_array<int64>(1l, 0l, 0l), one_row_us = 0.0lf),
               Shape(tag = "iq4 35b attn k v | shexp gate up", fmt = Fmt.iq4xs, n = 2048l, d = 512l, ways = fixed_array<int64>(1l, 0l, 0l), one_row_us = 0.0lf),
               Shape(tag = "iq4 35b dn out | attn o", fmt = Fmt.iq4xs, n = 4096l, d = 2048l, ways = fixed_array<int64>(1l, 0l, 0l), one_row_us = 0.0lf),
               Shape(tag = "iq4 35b head", fmt = Fmt.iq4xs, n = 2048l, d = 248320l, ways = fixed_array<int64>(1l, 0l, 0l), one_row_us = 0.0lf)]
}

[export]
def main() : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        let src = read_msl("modules/dasLLAMA/harness/rows8_mt.metal")
        if (empty(src)) {
            print("rows8_mt_race: run from the repo root\n")
            return 1
        }
        let arms = g_env_harness.rows8_arms
        var r : Race
        r.variants <- race_variants()
        //! the engine arm: the sources the engine wrote under DASLLAMA_METAL_MSL_DUMP (a model run with it set), raced as they are
        let dump = g_env_metal.msl_dump
        let eng_k4 = empty(dump) ? "" : read_msl(path_join(dump, "MetalRows8K4_metal_rows8.metal"))
        let eng_q8 = empty(dump) ? "" : read_msl(path_join(dump, "MetalRows8Q8_metal_rows8.metal"))
        if (!empty(eng_k4)) {
            r.p_eng_k4 = pso_from_source(dev, eng_k4, "MetalRows8K4_metal_rows8", true, err)
        }
        if (!empty(eng_q8)) {
            r.p_eng_q8 = pso_from_source(dev, eng_q8, "MetalRows8Q8_metal_rows8", true, err)
        }
        if ((!empty(eng_k4) && r.p_eng_k4 == null) || (!empty(eng_q8) && r.p_eng_q8 == null)) {
            print("engine pso - {err}\n")
            return 1
        }
        r.psos |> reserve(NFMT * length(r.variants))
        for (v in r.variants) {
            for (entry in [v.entry, v.entry_q8, v.entry_k6, v.entry_mlx4, v.entry_mlx8, v.entry_iq4]) {
                var pso = empty(entry) || !wanted(arms, v.name) ? null : pso_from_source(dev, v.flags + src, entry, true, err)
                if (pso == null && !empty(entry) && wanted(arms, v.name)) {
                    print("{v.name}: pso - {err}\n")
                    return 1
                }
                r.psos |> push(pso)
            }
        }
        let shapes <- race_shapes()
        let tags = g_env_harness.rows8_shapes
        let tg_mode = g_env_harness.rows8_tg
        var p_red = tg_mode ? pso_from_source(dev, src, "rows8_reduce", true, err) : null
        if (tg_mode && p_red == null) {
            print("rows8_reduce: pso - {err}\n")
            return 1
        }
        for (sh in shapes) {
            if (!shape_wanted(tags, sh.tag)) {
                continue
            } elif (!tg_mode) {
                race_shape(dev, queue, r, sh, arms)
            } elif (sh.fmt == Fmt.k4 || sh.fmt == Fmt.mlx4b || sh.fmt == Fmt.mlx8b || sh.fmt == Fmt.iq4xs) {
                race_shape_tg(dev, queue, r, sh, arms, sh.fmt == Fmt.k4 ? "k64" : (sh.fmt == Fmt.iq4xs ? "iq4 tg16 b32" : "mlx h"), p_red)
            }
        }
        return 0
    } else {
        print("rows8_mt_race: no das_metal\n")
        return 1
    }
}
