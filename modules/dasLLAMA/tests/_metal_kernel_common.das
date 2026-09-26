options gen2
options _dasllama_internal = true

// Shared GPU plumbing + eyeball-dump compares for the per-class Metal kernel-oracle suites
// (test_metal_*_kernels). Generic bodies instantiate only inside each suite's Apple static_if,
// so off-Apple this module parses and drops out whole.
module _metal_kernel_common shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_convert   // dequant_k{4,5,6}_plane_superblock — the kq reference dequant
require dasllama/dasllama_gemm_schema  // kq_qsb — the ONE per-format quant-plane stride source
require daslib/f16_cvt              // f32_to_f16 — packing synthetic scale planes
require dastest/testing_boost
require ?das_metal metal/das_metal_boost public

def buf_u32(dev; v : uint) : auto => buf_fill(dev, 1, v)

def buf_f32(dev; v : float) : auto => buf_fill(dev, 1, v)

//! One 64-lane 1-D dispatch of `total` threads: the pipeline set, the block binds, the threadgroups dispatched.
def dispatch_64_lanes(t : T?; queue; tag : string; pso; total : int; blk : block<(enc : MetalComputeEncoder?)>) : bool {   //! the block's type is what lets a caller write `$(enc)`; every consumer loads this module behind `?das_metal`
    var err2 : string
    let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        invoke(blk, enc)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag} dispatch: {err2}")
    return ran
}

def sigmoid_d(x : double) : double => 1.0lf / (1.0lf + exp(-x))

def silu_d(x : double) : double => x * sigmoid_d(x)

// ===== compares =====
// Every mismatch dumps the first 6 diffs with gpu/cpu values — enough to tell tolerance drift
// from garbage at a glance, before any rerun (the vulkan kernel suite's eyeball rail).

let private DUMP_CAP = 6

// relative bar + an absolute floor (absf > 0 serves f16-rounded outputs, whose error scales
// with magnitude, not the possibly-cancelled-small result)
//! the compare over a buffer of `t` elements read as floats - float, or the f16 payload of a half slab
[unused_argument(t)]
def private buf_mismatch_bars_as(buf; t : type<auto(TT)>; want : array<float>; rel, absf : float; tag : string) : int {
    let p = unsafe(reinterpret<TT const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = float(unsafe(p[k]))   // nolint:PERF020,LINT019 - the half payload's widening; the float instance's identity
        let w = want[k]
        if (!(abs(g - w) <= absf) && !(abs(g - w) <= rel * max(1.0, max(abs(g), abs(w))))) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w}\n")
            }
            bad++
        }
    }
    return bad
}

def buf_mismatch_bars(buf; want : array<float>; rel, absf : float; tag : string) : int => buf_mismatch_bars_as(buf, type<float>, want, rel, absf, tag)

def buf_mismatch_approx(buf; want : array<float>; tag : string) : int {
    return buf_mismatch_bars(buf, want, 1e-4, 0.0, tag)
}

// f16-holding buffer vs an f32 expectation: dequant per element, compare under bars (the f16
// store rounds at 2^-11 relative — pass rel >= 1e-3 so the rounding never reads as a red)
def buf_mismatch_f16(buf; want : array<float>; rel, absf : float; tag : string) : int => buf_mismatch_bars_as(buf, type<float16>, want, rel, absf, tag)

// envelope-aware compare for float-accumulating GEMV/GEMM kernels: |gpu-cpu| <= max(rel * env,
// 1e-3) where env is the output's dot envelope (sum |w·x|); sentinel positions carry env 0
// the float16-payload twin: reads the buffer as halfs (f16 slabs), same bands
[unused_argument(t)]
def private buf_mismatch_env_as(buf; t : type<auto(TT)>; want, env : array<float>; rel : float; tag : string) : int {
    let p = unsafe(reinterpret<TT const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = float(unsafe(p[k]))   // nolint:PERF020,LINT019 - the half payload's widening; the float instance's identity
        let w = want[k]
        if (!(abs(g - w) <= max(rel * env[k], 1e-3))) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w} env {env[k]}\n")
            }
            bad++
        }
    }
    return bad
}

def buf_mismatch_env_h(buf; want, env : array<float>; rel : float; tag : string) : int => buf_mismatch_env_as(buf, type<float16>, want, env, rel, tag)

def buf_mismatch_env(buf; want, env : array<float>; rel : float; tag : string) : int => buf_mismatch_env_as(buf, type<float>, want, env, rel, tag)

//! The f16 twin `bh` of the float panel `bf` over `n` elements: bit-equal to `float16(panel)` over the `live` prefix, the -1000 sentinel past it.
def hx_narrow_mismatch(bh, bf; n, live : int; tag : string) : int {
    var fgpu <- buf_download(bf, n, type<float>)
    var wanth : array<float16>
    wanth |> resize(n)
    for (k in range(n)) {
        wanth[k] = k < live ? float16(fgpu[k]) : float16(-1000.0)
    }
    let bad = buf_mismatch_exact(bh, wanth, tag)
    delete fgpu
    delete wanth
    return bad
}

def buf_mismatch_exact(buf; want : array<auto(TT)>; tag : string) : int {
    let p = unsafe(reinterpret<TT -const const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = unsafe(p[k])
        if (g != want[k]) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {want[k]}\n")
            }
            bad++
        }
    }
    return bad
}

// ===== synthetic K-quant split planes (the metal layout) + the reference row dequant =====

def kq_hash_byte(i : int) : uint8 {
    var h = uint(i) * 2654435761u
    h ^= h >> 15u
    h *= 2246822519u
    return uint8((h >> 8u) & 0xFFu)
}

let KQ_SAFE_SCALES = fixed_array(0.25, 0.5, 1.0, 0.125, 2.0, 0.75, 1.5, 0.375)

// quant plane = hash bytes (any bit pattern is valid); scale plane K4/K5 = [d f16][dmin f16]
// [12 packed 6-bit bytes] per superblock, K6 = [16B sub-scales] x nsb + f16 d tail, IQ4_XS (44) =
// the same split form with [8 signed sub-scales in -31..31][8 zero] strips, Q3_K (3) = the k6 form over 96B quants
def kq_fill_planes(fmt, nsb : int; var kq : array<uint8>; var ks : array<uint8>) {
    let qsb = int(kq_qsb(fmt))
    kq |> resize(nsb * qsb)
    for (i in range(nsb * qsb)) {
        kq[i] = kq_hash_byte(i * 3 + fmt)
    }
    if (fmt == 2) {
        // k2: the metal split form - [16 pair-byte strips x nsb][4B d+dmin tail x nsb]
        ks |> resize(nsb * 20)
        for (i in range(nsb * 16)) {
            ks[i] = kq_hash_byte(i * 7 + 13)
        }
        for (b in range(nsb)) {
            let dbits = f32_to_f16(KQ_SAFE_SCALES[b % 8])
            let mbits = f32_to_f16(KQ_SAFE_SCALES[(b + 3) % 8])
            ks[nsb * 16 + b * 4] = uint8(dbits & 0xFFu)
            ks[nsb * 16 + b * 4 + 1] = uint8(dbits >> 8u)
            ks[nsb * 16 + b * 4 + 2] = uint8(mbits & 0xFFu)
            ks[nsb * 16 + b * 4 + 3] = uint8(mbits >> 8u)
        }
    } elif (fmt == 45 || fmt == 40) {
        // iq4nl and q40: 16B/sb = 8 per-32-block f16 d's, verbatim (no strips)
        ks |> resize(nsb * 16)
        for (b in range(nsb * 8)) {
            let dbits = f32_to_f16(KQ_SAFE_SCALES[(b + (b / 8) * 3) % 8])
            ks[b * 2] = uint8(dbits & 0xFFu)
            ks[b * 2 + 1] = uint8(dbits >> 8u)
        }
    } elif (fmt == 6 || fmt == 44 || fmt == 3 || fmt == 33 || fmt == 34 || fmt == 23 || fmt == 24 || fmt == 25) {
        ks |> resize(nsb * 18)
        for (i in range(nsb * 16)) {
            ks[i] = fmt == 6 || fmt == 3 || fmt == 23 || fmt == 24 || fmt == 25 ? kq_hash_byte(i * 7 + 13) : uint8(i % 16 < 8 ? int(kq_hash_byte(i * 7 + 13)) % 63 - 31 : 0)   // 44/33: signed strip bytes + zero pad; 23: 16 unsigned strips
        }
        for (b in range(nsb)) {
            let dbits = f32_to_f16(KQ_SAFE_SCALES[b % 8])
            ks[nsb * 16 + b * 2] = uint8(dbits & 0xFFu)
            ks[nsb * 16 + b * 2 + 1] = uint8(dbits >> 8u)
        }
    } else {
        ks |> resize(nsb * 16)
        for (b in range(nsb)) {
            let dbits = f32_to_f16(KQ_SAFE_SCALES[b % 8])
            let mbits = f32_to_f16(KQ_SAFE_SCALES[(b + 3) % 8])
            ks[b * 16] = uint8(dbits & 0xFFu)
            ks[b * 16 + 1] = uint8(dbits >> 8u)
            ks[b * 16 + 2] = uint8(mbits & 0xFFu)
            ks[b * 16 + 3] = uint8(mbits >> 8u)
            for (i in range(12)) {
                ks[b * 16 + 4 + i] = kq_hash_byte(b * 31 + i * 5 + 7)
            }
        }
    }
}

// ===== q8 (34B-block) blob fixture =====

// one 34B-block q8 blob (nrows x nkb blocks) + its exact dequant: pow2 scales and small-int
// quants keep every staged f16/f32 value exact, leaving only accumulation order to the envelope
//! `span`: the quants' value count - 16 stays exact under f16 staging, 256 covers the whole int8 range
def q8_blob_fill(nrows, nkb : int; var blob : array<uint8>; var wdeq : array<float>; span : int = 16) {
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    blob |> resize(nrows * nkb * 34)
    wdeq |> resize(nrows * nkb * 32)
    for (b in range(nrows * nkb)) {
        let d = lut[(b * 7 + 1) % 4]
        let dbits = f32_to_f16(d)
        blob[b * 34] = uint8(dbits & 0xFFu)
        blob[b * 34 + 1] = uint8(dbits >> 8u)
        for (c in range(32)) {
            let q = (b * 5 + c * 11) % span - span / 2
            blob[b * 34 + 2 + c] = uint8(q & 0xFF)
            wdeq[b * 32 + c] = float(q) * d
        }
    }
}

// exact-under-f16-staging X rows: small ints scaled by pow2 (the q8 gates' shared shape)
def q8_x_fill(nn : int; var xf : array<float>) {
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    xf |> resize(nn)
    for (i in range(nn)) {
        xf[i] = float((i * 5 + i / 17) % 16 - 8) * lut[(i / 3) % 4]
    }
}

//! One row's reference dequant (nb superblocks from sb_base) off split-scale planes, through the engine's own plane-row dequant.
def kq_row_ref(fmt, sb_base, nb, nsb_total : int; kq, ks : array<uint8>; var wrow : array<float>) {
    let tail = fmt == 2 ? 4l : 2l   //! the engine finds the d tail from the plane's length: the fixture's plane holds nsb_total superblocks exactly
    assert(fmt == 4 || fmt == 5 || fmt == 40 || fmt == 45 || long_length(ks) == int64(nsb_total) * (16l + tail), "kq_row_ref: the scale plane holds nsb_total superblocks")
    let f = kq_fmt_of_id(fmt, "kq_row_ref")
    kq_plane_row_f32(f, kq, ks, int64(nb) * kq_elems(f), 1l, false, 0l, wrow, 0l, int64(sb_base), true)
}
