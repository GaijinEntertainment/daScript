options gen2

// Shared GPU plumbing + eyeball-dump compares for the per-class Metal kernel-oracle suites
// (test_metal_*_kernels). Generic bodies instantiate only inside each suite's Apple static_if,
// so off-Apple this module parses and drops out whole.
module _metal_kernel_common shared public

require math
require dasllama/dasllama_convert   // dequant_k{4,5,6}_plane_superblock — the kq reference dequant
require llvm/daslib/f16_cvt         // f32_to_f16 — packing synthetic scale planes
require ?das_metal metal/das_metal_boost

// ===== device buffers =====

def buf_upload(dev; arr : array<auto(TT)>) : auto {
    var buf = metal_new_buffer(dev, uint64(long_length(arr)) * uint64(typeinfo sizeof(arr[0])))
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(length(arr))) {
        unsafe(p[k]) = arr[k]
    }
    return buf
}

def buf_store(buf; arr : array<auto(TT)>) {
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(length(arr))) {
        unsafe(p[k]) = arr[k]
    }
}

def buf_fill(dev; nn : int; value : auto(TT)) : auto {
    var buf = metal_new_buffer(dev, uint64(nn) * uint64(typeinfo sizeof(value)))
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(nn)) {
        unsafe(p[k]) = value
    }
    return buf
}

def buf_u32(dev; v : uint) : auto {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def buf_f32(dev; v : float) : auto {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def buf_download(buf; nn : int; t : type<auto(TT)>) : array<TT -const> {
    var out : array<TT -const>
    out |> resize(nn)
    let p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(nn)) {
        out[k] = unsafe(p[k])
    }
    return <- out
}

// ===== compares =====
// Every mismatch dumps the first 6 diffs with gpu/cpu values — enough to tell tolerance drift
// from garbage at a glance, before any rerun (the vulkan kernel suite's eyeball rail).

let private DUMP_CAP = 6

// relative bar + an absolute floor (absf > 0 serves f16-rounded outputs, whose error scales
// with magnitude, not the possibly-cancelled-small result)
def buf_mismatch_bars(buf; want : array<float>; rel, absf : float; tag : string) : int {
    let p = unsafe(reinterpret<float const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = unsafe(p[k])
        let w = want[k]
        if (abs(g - w) > absf && abs(g - w) > rel * max(1.0, max(abs(g), abs(w)))) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w}\n")
            }
            bad++
        }
    }
    return bad
}

def buf_mismatch_approx(buf; want : array<float>; tag : string) : int {
    return buf_mismatch_bars(buf, want, 1e-4, 0.0, tag)
}

// f16-holding buffer vs an f32 expectation: dequant per element, compare under bars (the f16
// store rounds at 2^-11 relative — pass rel >= 1e-3 so the rounding never reads as a red)
def buf_mismatch_f16(buf; want : array<float>; rel, absf : float; tag : string) : int {
    let p = unsafe(reinterpret<float16 const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = float(unsafe(p[k]))
        let w = want[k]
        if (abs(g - w) > absf && abs(g - w) > rel * max(1.0, max(abs(g), abs(w)))) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w}\n")
            }
            bad++
        }
    }
    return bad
}

// envelope-aware compare for float-accumulating GEMV/GEMM kernels: |gpu-cpu| <= max(rel * env,
// 1e-3) where env is the output's dot envelope (sum |w·x|); sentinel positions carry env 0
def buf_mismatch_env(buf; want, env : array<float>; rel : float; tag : string) : int {
    let p = unsafe(reinterpret<float const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = unsafe(p[k])
        let w = want[k]
        if (abs(g - w) > max(rel * env[k], 1e-3)) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w} env {env[k]}\n")
            }
            bad++
        }
    }
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
// [12 packed 6-bit bytes] per superblock, K6 = [16B sub-scales] x nsb + f16 d tail
def kq_fill_planes(fmt, nsb : int; var kq : array<uint8>; var ks : array<uint8>) {
    let qsb = fmt == 4 ? 128 : (fmt == 5 ? 160 : 192)
    kq |> resize(nsb * qsb)
    for (i in range(nsb * qsb)) {
        kq[i] = kq_hash_byte(i * 3 + fmt)
    }
    if (fmt == 6) {
        ks |> resize(nsb * 18)
        for (i in range(nsb * 16)) {
            ks[i] = kq_hash_byte(i * 7 + 13)
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

// reference dequant of one row (nb superblocks starting at superblock sb_base) off the planes
def kq_row_ref(fmt, sb_base, nb, nsb_total : int; kq, ks : array<uint8>; var wrow : array<float>) {
    for (ib in range(nb)) {
        let sb = sb_base + ib
        if (fmt == 4) {
            dequant_k4_plane_superblock(kq, int64(sb * 128), ks, int64(sb * 16), wrow, int64(ib * 256))
        } elif (fmt == 5) {
            dequant_k5_plane_superblock(kq, int64(sb * 160), ks, int64(sb * 16), wrow, int64(ib * 256))
        } else {
            dequant_k6_plane_superblock_at(kq, int64(sb * 192), ks, int64(sb * 16),
                int64(nsb_total * 16 + sb * 2), wrow, int64(ib * 256))
        }
    }
}
