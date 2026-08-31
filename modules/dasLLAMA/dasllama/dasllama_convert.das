options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_convert shared public

require dasllama/dasllama_lint public
require math
require llvm/daslib/f16_cvt   // f16<->f32 halfword codecs — the Q5_K encoder + every f16 scale read
require daslib/jobque_boost public   // quantize_k5_plane threads its superblock loop
require dasllama/dasllama_par        // maybe_parallel_for (+ its emitted dispatch counters)
require dasllama/dasllama_math       // get_dispatch_lanes — the requant wrappers shape their fan-out
require dasllama/dasllama_kqformat public   // KqFmt + the plane stride truth (K4_QSB et al.) — re-exported down the gguf chain
require dasllama/dasllama_tune       // [tuned]: the f16 row converts carry per-box loop hints

// Tensor format conversion, platform-neutral: the pure codec layer — LE byte readers, K-quant
// superblock dequants (+ pointer twins), superblock->plane transcodes, plane dequants, the
// fp32->Q5_K encoder, and the plane stride constants. GGUF-coupled drivers (metadata lookup,
// threading, dst guards) stay in dasllama_gguf, which requires this module PUBLIC so the
// byte-reader surface (rd_u16/rd_u32/rd_f32) keeps reaching the bin readers through it.

// ===== little-endian byte readers =====

def rd_u16(b : array<uint8> | #; o : int64) : uint {
    return uint(b[o]) | (uint(b[o + 1l]) << 8u)
}

def rd_u32(b : array<uint8> | #; o : int64) : uint {
    return uint(b[o]) | (uint(b[o + 1l]) << 8u) | (uint(b[o + 2l]) << 16u) | (uint(b[o + 3l]) << 24u)
}

def rd_f32(b : array<uint8> | #; o : int64) : float {
    return unsafe(reinterpret<float>(rd_u32(b, o)))
}

// E8M0 block scale -> fp32 * 0.5 = 2^(e-128) (the half matches the doubled MXFP4_KVALUES).
// e < 2 lands in fp32 denormal territory -> precomputed bit patterns.
def e8m0_to_f32_half(e : uint) : float {
    let bits = e < 2u ? (0x00200000u << e) : ((e - 1u) << 23u)
    return unsafe(reinterpret<float>(bits))
}

// ===== K-quant superblock dequant (256-weight superblocks, on-disk Q4_K / Q6_K) =====

// int8 read (K-quant sub-scales are signed bytes)
def private rd_i8(b : array<uint8> | #; o : int64) : int {
    let u = int(b[o])
    return u < 128 ? u : u - 256
}

// 6-bit sub-scale/min unpack for Q4_K/Q5_K superblocks. The 12 scale
// bytes hold 8 scales + 8 mins: j<4 live in the low 6 bits of bytes j / j+4; j>=4 split as
// low-4 in byte j+4 (scale = low nibble, min = high) + top-2 of bytes j-4 / j.
def private q4k_scale_min(b : array<uint8> | #; so, j : int64) : int2 {
    if (j < 4l) {
        return int2(int(b[so + j]) & 63, int(b[so + j + 4l]) & 63)
    }
    let sc = (int(b[so + j + 4l]) & 15) | ((int(b[so + j - 4l]) >> 6) << 4)
    let mn = (int(b[so + j + 4l]) >> 4) | ((int(b[so + j]) >> 6) << 4)
    return int2(sc, mn)
}

//! Dequantize one Q4_K superblock (144B at `bo`) into dst[doff..doff+256). w = (d*sc)*q - (dmin*m),
//! q the RAW nibble (min term absorbs offset); 32 quant bytes carry weights 0..31 low nibble, 32..63

def dequant_q4k_superblock(bytes : array<uint8> | #; bo : int64; var dst : array<float>; doff : int64) {
    let d = f16_to_f32(rd_u16(bytes, bo))
    let dmin = f16_to_f32(rd_u16(bytes, bo + 2l))
    let so = bo + 4l    // 12 scale/min bytes
    var qo = bo + 16l   // 128 quant bytes
    var dsto = doff
    for (js in range64(4l)) {   // 4 chunks of 64 weights = sub-blocks 2js, 2js+1
        let s0 = q4k_scale_min(bytes, so, 2l * js)
        let s1 = q4k_scale_min(bytes, so, 2l * js + 1l)
        let d1 = d * float(s0.x)
        let m1 = dmin * float(s0.y)
        let d2 = d * float(s1.x)
        let m2 = dmin * float(s1.y)
        for (l in range64(32l)) {
            let b = int(bytes[qo + l])
            dst[dsto + l] = d1 * float(b & 15) - m1
            dst[dsto + 32l + l] = d2 * float(b >> 4) - m2
        }
        qo += 32l
        dsto += 64l
    }
}

//! Dequantize one Q5_K superblock (176B at `bo`) into dst[doff..doff+256). w = (d*sc)*q - (dmin*m),
//! q = nibble | (high bit << 4); the high bit for chunk js lives in bit 2js (low)/2js+1 (high) of
//! qh[l], qh base fixed per superblock. Bit-exact with the on-disk Q5_K dequant.
def dequant_q5k_superblock(bytes : array<uint8> | #; bo : int64; var dst : array<float>; doff : int64) {
    let d = f16_to_f32(rd_u16(bytes, bo))
    let dmin = f16_to_f32(rd_u16(bytes, bo + 2l))
    let so = bo + 4l     // 12 scale/min bytes
    let qho = bo + 16l   // 32 high-bit bytes (fixed base for the whole superblock)
    var qo = bo + 48l    // 128 nibble bytes
    var dsto = doff
    for (js in range64(4l)) {
        let s0 = q4k_scale_min(bytes, so, 2l * js)
        let s1 = q4k_scale_min(bytes, so, 2l * js + 1l)
        let d1 = d * float(s0.x)
        let m1 = dmin * float(s0.y)
        let d2 = d * float(s1.x)
        let m2 = dmin * float(s1.y)
        let u1 = 1 << (int(js) * 2)
        let u2 = 2 << (int(js) * 2)
        for (l in range64(32l)) {
            let b = int(bytes[qo + l])
            let h = int(bytes[qho + l])
            dst[dsto + l] = d1 * float((b & 15) + ((h & u1) != 0 ? 16 : 0)) - m1
            dst[dsto + 32l + l] = d2 * float((b >> 4) + ((h & u2) != 0 ? 16 : 0)) - m2
        }
        qo += 32l
        dsto += 64l
    }
}

//! Dequantize one Q6_K superblock (210 bytes at `bo`: 128 low-nibble bytes ql, 64 high-2-bit
//! bytes qh, 16 int8 sub-scales for 16 sub-blocks of 16, fp16 d) into dst[doff .. doff+256).
//! w = (d*sc)*q with q = (ql | qh<<4) - 32. Bit-exact with the on-disk Q6_K dequant.
def dequant_q6k_superblock(bytes : array<uint8> | #; bo : int64; var dst : array<float>; doff : int64) {
    let d = f16_to_f32(rd_u16(bytes, bo + 208l))
    var qlo = bo          // ql cursor (64 bytes per 128-weight half)
    var qho = bo + 128l   // qh cursor (32 bytes per half)
    var sco = bo + 192l   // sub-scale cursor (8 per half)
    var dsto = doff
    for (_half in range(2)) {
        for (l in range64(32l)) {
            let si = l / 16l
            let hl = int(bytes[qho + l])
            let q1 = ((int(bytes[qlo + l]) & 15) | (((hl >> 0) & 3) << 4)) - 32
            let q2 = ((int(bytes[qlo + 32l + l]) & 15) | (((hl >> 2) & 3) << 4)) - 32
            let q3 = ((int(bytes[qlo + l]) >> 4) | (((hl >> 4) & 3) << 4)) - 32
            let q4 = ((int(bytes[qlo + 32l + l]) >> 4) | (((hl >> 6) & 3) << 4)) - 32
            dst[dsto + l] = d * float(rd_i8(bytes, sco + si)) * float(q1)
            dst[dsto + 32l + l] = d * float(rd_i8(bytes, sco + si + 2l)) * float(q2)
            dst[dsto + 64l + l] = d * float(rd_i8(bytes, sco + si + 4l)) * float(q3)
            dst[dsto + 96l + l] = d * float(rd_i8(bytes, sco + si + 6l)) * float(q4)
        }
        qlo += 64l
        qho += 32l
        sco += 8l
        dsto += 128l
    }
}

// Pointer twins of the three superblock dequants — same math op-for-op, no bounds checks; the
// threaded gguf_read_tensor_f32 arms drive these from worker lanes (raw pointers only).
def private rd_u16_p(p : uint8 const?; o : int64) : uint {
    unsafe {
        return uint(p[o]) | (uint(p[o + 1l]) << 8u)
    }
}

def private rd_i8_p(p : uint8 const?; o : int64) : int {
    unsafe {
        let v = int(p[o])
        return v < 128 ? v : v - 256
    }
}

def private q4k_scale_min_p(p : uint8 const?; so, j : int64) : int2 {
    unsafe {
        if (j < 4l) {
            return int2(int(p[so + j]) & 63, int(p[so + j + 4l]) & 63)
        }
        let sc = (int(p[so + j + 4l]) & 15) | ((int(p[so + j - 4l]) >> 6) << 4)
        let mn = (int(p[so + j + 4l]) >> 4) | ((int(p[so + j]) >> 6) << 4)
        return int2(sc, mn)
    }
}

def dequant_q4k_superblock_p(sp : uint8 const?; bo : int64; var dp : float?; doff : int64) {
    unsafe {
        let d = f16_to_f32(rd_u16_p(sp, bo))
        let dmin = f16_to_f32(rd_u16_p(sp, bo + 2l))
        let so = bo + 4l
        var qo = bo + 16l
        var dsto = doff
        for (js in range64(4l)) {
            let s0 = q4k_scale_min_p(sp, so, 2l * js)
            let s1 = q4k_scale_min_p(sp, so, 2l * js + 1l)
            let d1 = d * float(s0.x)
            let m1 = dmin * float(s0.y)
            let d2 = d * float(s1.x)
            let m2 = dmin * float(s1.y)
            for (l in range64(32l)) {
                let b = int(sp[qo + l])
                dp[dsto + l] = d1 * float(b & 15) - m1
                dp[dsto + 32l + l] = d2 * float(b >> 4) - m2
            }
            qo += 32l
            dsto += 64l
        }
    }
}

def dequant_q5k_superblock_p(sp : uint8 const?; bo : int64; var dp : float?; doff : int64) {
    unsafe {
        let d = f16_to_f32(rd_u16_p(sp, bo))
        let dmin = f16_to_f32(rd_u16_p(sp, bo + 2l))
        let so = bo + 4l
        let qho = bo + 16l
        var qo = bo + 48l
        var dsto = doff
        for (js in range64(4l)) {
            let s0 = q4k_scale_min_p(sp, so, 2l * js)
            let s1 = q4k_scale_min_p(sp, so, 2l * js + 1l)
            let d1 = d * float(s0.x)
            let m1 = dmin * float(s0.y)
            let d2 = d * float(s1.x)
            let m2 = dmin * float(s1.y)
            let u1 = 1 << (int(js) * 2)
            let u2 = 2 << (int(js) * 2)
            for (l in range64(32l)) {
                let b = int(sp[qo + l])
                let h = int(sp[qho + l])
                dp[dsto + l] = d1 * float((b & 15) + ((h & u1) != 0 ? 16 : 0)) - m1
                dp[dsto + 32l + l] = d2 * float((b >> 4) + ((h & u2) != 0 ? 16 : 0)) - m2
            }
            qo += 32l
            dsto += 64l
        }
    }
}

def dequant_q6k_superblock_p(sp : uint8 const?; bo : int64; var dp : float?; doff : int64) {
    unsafe {
        let d = f16_to_f32(rd_u16_p(sp, bo + 208l))
        var qlo = bo
        var qho = bo + 128l
        var sco = bo + 192l
        var dsto = doff
        for (_half in range(2)) {
            for (l in range64(32l)) {
                let si = l / 16l
                let hl = int(sp[qho + l])
                let q1 = ((int(sp[qlo + l]) & 15) | (((hl >> 0) & 3) << 4)) - 32
                let q2 = ((int(sp[qlo + 32l + l]) & 15) | (((hl >> 2) & 3) << 4)) - 32
                let q3 = ((int(sp[qlo + l]) >> 4) | (((hl >> 4) & 3) << 4)) - 32
                let q4 = ((int(sp[qlo + 32l + l]) >> 4) | (((hl >> 6) & 3) << 4)) - 32
                dp[dsto + l] = d * float(rd_i8_p(sp, sco + si)) * float(q1)
                dp[dsto + 32l + l] = d * float(rd_i8_p(sp, sco + si + 2l)) * float(q2)
                dp[dsto + 64l + l] = d * float(rd_i8_p(sp, sco + si + 4l)) * float(q3)
                dp[dsto + 96l + l] = d * float(rd_i8_p(sp, sco + si + 6l)) * float(q4)
            }
            qlo += 64l
            qho += 32l
            sco += 8l
            dsto += 128l
        }
    }
}

// ===== K-quant native-plane transcode (superblock payloads -> split quant/scale planes) =====
// k4/k5/k6 quant+scale plane byte strides below (K4_QSB etc.); scale planes keep the verbatim DISK
// scale block (kq v2 fold scheme) so every plane dequant is bit-exact vs the disk dequant.

//! Transcode one Q4_K superblock (144 bytes at `bo`) into the k4 planes: nibbles verbatim to
//! kq[kqo..+128), the disk scale block verbatim to ks[kso..+16) (f16 d, f16 dmin, 12 bytes of
//! 6-bit sub-scales/mins) + 4 zero pad bytes (stride 20 — see the plane-layout comment above).
def transcode_q4k_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (i in range64(16l)) {
        ks[kso + i] = bytes[bo + i]   // f16 d, f16 dmin, 12 packed scale/min bytes — disk order
    }
    for (i in range64(4l)) {
        ks[kso + 16l + i] = uint8(0)
    }
    for (i in range64(128l)) {
        kq[kqo + i] = bytes[bo + 16l + i]
    }
}

//! Transcode one Q5_K superblock (176 bytes at `bo`) into the k5 planes: [nibbles][high-bits]
//! to kq[kqo..+160), the disk scale block verbatim to ks[kso..+16) (f16 d, f16 dmin, 12 bytes
//! of 6-bit sub-scales/mins) + 4 zero pad bytes (the k4 form — stride 20).
def transcode_q5k_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (i in range64(16l)) {
        ks[kso + i] = bytes[bo + i]   // f16 d, f16 dmin, 12 packed scale/min bytes — disk order
    }
    for (i in range64(4l)) {
        ks[kso + 16l + i] = uint8(0)
    }
    for (i in range64(128l)) {
        kq[kqo + i] = bytes[bo + 48l + i]   // qs
    }
    for (i in range64(32l)) {
        kq[kqo + 128l + i] = bytes[bo + 16l + i]   // qh
    }
}

//! Transcode one Q6_K superblock (210 bytes at `bo`) into the k6 planes: [ql][qh] to
//! kq[kqo..+192), the native int8 sub-scales + f16 d to ks[kso..+18) — all verbatim (exact).
def transcode_q6k_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (i in range64(192l)) {
        kq[kqo + i] = bytes[bo + i]   // ql (128) then qh (64) — disk order
    }
    for (i in range64(18l)) {
        ks[kso + i] = bytes[bo + 192l + i]   // 16 int8 sub-scales + f16 d halfword
    }
}

//! Reference dequant of one k4-plane superblock (the exact math the k4 kernels must reproduce):
//! w = (d*sc)*q - (dmin*m) with the plane's verbatim disk scale block — bit-identical to the
//! disk dequant (kq v2: the plane IS the disk block, split).
def dequant_k4_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, kso))
    let dmin = f16_to_f32(rd_u16(ks, kso + 2l))
    var dsto = doff
    for (js in range64(4l)) {
        let s0 = q4k_scale_min(ks, kso + 4l, 2l * js)
        let s1 = q4k_scale_min(ks, kso + 4l, 2l * js + 1l)
        let d1 = d * float(s0.x)
        let m1 = dmin * float(s0.y)
        let d2 = d * float(s1.x)
        let m2 = dmin * float(s1.y)
        for (l in range64(32l)) {
            let b = int(kq[kqo + 32l * js + l])
            dst[dsto + l] = d1 * float(b & 15) - m1
            dst[dsto + 32l + l] = d2 * float(b >> 4) - m2
        }
        dsto += 64l
    }
}

//! Reference dequant of one k5-plane superblock: w = (d*sc)*q - (dmin*m), q = nibble | (high
//! bit << 4), with the plane's verbatim disk scale block — bit-identical to the disk dequant.
def dequant_k5_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, kso))
    let dmin = f16_to_f32(rd_u16(ks, kso + 2l))
    var dsto = doff
    for (js in range64(4l)) {
        let s0 = q4k_scale_min(ks, kso + 4l, 2l * js)
        let s1 = q4k_scale_min(ks, kso + 4l, 2l * js + 1l)
        let d1 = d * float(s0.x)
        let m1 = dmin * float(s0.y)
        let d2 = d * float(s1.x)
        let m2 = dmin * float(s1.y)
        let u1 = 1 << (int(js) * 2)
        let u2 = 2 << (int(js) * 2)
        for (l in range64(32l)) {
            let b = int(kq[kqo + 32l * js + l])
            let h = int(kq[kqo + 128l + l])
            dst[dsto + l] = d1 * float((b & 15) + ((h & u1) != 0 ? 16 : 0)) - m1
            dst[dsto + 32l + l] = d2 * float((b >> 4) + ((h & u2) != 0 ? 16 : 0)) - m2
        }
        dsto += 64l
    }
}

// Round-half-to-even via the fp32 add-magic trick - bit-exact with the reference rounding, so
// encoder output stays byte-comparable against it. Caller guarantees |fval| <= 4194303.

def private nearest_int(fval : float) : int {
    let v = fval + 12582912.0
    let i = int(unsafe(reinterpret<uint>(v)))
    return (i & int(0x007fffff)) - int(0x00400000)
}

// Weighted joint scale/min fit for one 32-weight sub-block (nmax=31,
// rmin=-0.5, rdelta=0.1, 16 steps): sweeps iscale around the naive max/min fit and keeps the
// weighted-least-squares (scale, min) with the lowest weighted square error. Returns (scale, -min).
def private k5_fit_sub(x : float const?; wt : float const?; var lout : int?) : float2 {
    unsafe {
        var vmin = x[0]
        var vmax = x[0]
        var sum_w = wt[0]
        var sum_x = wt[0] * x[0]
        for (i in range64(1l, 32l)) {
            vmin = min(vmin, x[i])
            vmax = max(vmax, x[i])
            sum_w += wt[i]
            sum_x += wt[i] * x[i]
        }
        if (vmin > 0.0) {
            vmin = 0.0
        }
        if (vmax == vmin) {
            for (i in range64(32l)) {
                lout[i] = 0
            }
            return float2(0.0, -vmin)
        }
        let iscale0 = 31.0 / (vmax - vmin)
        var scale = 1.0 / iscale0
        var best_mad = 0.0
        for (i in range64(32l)) {
            let l = clamp(nearest_int(iscale0 * (x[i] - vmin)), 0, 31)
            lout[i] = l
            let diff = scale * float(l) + vmin - x[i]
            best_mad += wt[i] * diff * diff
        }
        var laux : int[32]
        for (istep in range64(16l)) {
            let isc = (30.5 + 0.1 * float(istep)) / (vmax - vmin)
            var sum_l = 0.0
            var sum_l2 = 0.0
            var sum_xl = 0.0
            for (i in range64(32l)) {
                let l = clamp(nearest_int(isc * (x[i] - vmin)), 0, 31)
                laux[i] = l
                let fl = float(l)
                sum_l += wt[i] * fl
                sum_l2 += wt[i] * fl * fl
                sum_xl += wt[i] * fl * x[i]
            }
            let dd = sum_w * sum_l2 - sum_l * sum_l
            if (dd <= 0.0) {
                continue
            }
            var this_scale = (sum_w * sum_xl - sum_x * sum_l) / dd
            var this_min = (sum_l2 * sum_x - sum_l * sum_xl) / dd
            if (this_min > 0.0) {   // min is clamped non-positive; refit scale alone
                this_min = 0.0
                this_scale = sum_xl / sum_l2
            }
            var mad = 0.0
            for (i in range64(32l)) {
                let diff = this_scale * float(laux[i]) + this_min - x[i]
                mad += wt[i] * diff * diff
            }
            if (mad < best_mad) {
                for (i in range64(32l)) {
                    lout[i] = laux[i]
                }
                best_mad = mad
                scale = this_scale
                vmin = this_min
            }
        }
        return float2(scale, -vmin)
    }
}

// Encode one 256-weight superblock into the k5 planes — the exact inverse of
// dequant_k5_plane_superblock, bit-exact with the Q5_K reference encoder.
def private quantize_k5_superblock_p(x : float const?; var kq : uint8?; kqo : int64; var ks : uint8?; kso : int64) {  // nolint:STYLE038 — one oracle-matched superblock encoder; splitting it invites drift
    unsafe {
        var lq : int[256]
        var lsub : int[32]
        var wt : float[32]
        var scales : float[8]
        var mins : float[8]
        var max_scale = 0.0
        var max_min = 0.0
        for (j in range64(8l)) {
            var sum_x2 = 0.0
            for (l in range64(32l)) {
                sum_x2 += x[j * 32l + l] * x[j * 32l + l]
            }
            let av_x = sqrt(sum_x2 / 32.0)
            for (l in range64(32l)) {
                wt[l] = av_x + abs(x[j * 32l + l])
            }
            let sm = k5_fit_sub(x + j * 32l, addr(wt[0]), addr(lsub[0]))
            scales[j] = sm.x
            mins[j] = sm.y
            max_scale = max(max_scale, sm.x)
            max_min = max(max_min, sm.y)
            for (l in range64(32l)) {
                lq[j * 32l + l] = lsub[l]
            }
        }
        let inv_scale = max_scale > 0.0 ? 63.0 / max_scale : 0.0
        let inv_min = max_min > 0.0 ? 63.0 / max_min : 0.0
        for (b in range64(16l)) {   // the |= packing below needs a zeroed strip; also clears the 4B pad
            ks[kso + 4l + b] = uint8(0)
        }
        for (j in range64(8l)) {
            let ls = min(63, nearest_int(inv_scale * scales[j]))
            let lm = min(63, nearest_int(inv_min * mins[j]))
            if (j < 4l) {
                ks[kso + 4l + j] = uint8(ls)
                ks[kso + 8l + j] = uint8(lm)
            } else {
                ks[kso + 8l + j] = uint8((ls & 15) | ((lm & 15) << 4))
                ks[kso + j] = uint8(int(ks[kso + j]) | ((ls >> 4) << 6))
                ks[kso + 4l + j] = uint8(int(ks[kso + 4l + j]) | ((lm >> 4) << 6))
            }
        }
        let d = f32_to_f16(max_scale / 63.0)
        let dmin = f32_to_f16(max_min / 63.0)
        ks[kso] = uint8(d & 0xFFu)
        ks[kso + 1l] = uint8((d >> 8u) & 0xFFu)
        ks[kso + 2l] = uint8(dmin & 0xFFu)
        ks[kso + 3l] = uint8((dmin >> 8u) & 0xFFu)
        // second pass: re-derive quants against the ROUNDED 6-bit sc/mn, not the fitted floats
        let df = f16_to_f32(d)
        let dmf = f16_to_f32(dmin)
        for (j in range64(8l)) {
            let sm = q4k_scale_min_p(ks, kso + 4l, j)
            let dj = df * float(sm.x)
            if (dj == 0.0) {
                continue
            }
            let mj = dmf * float(sm.y)
            for (l in range64(32l)) {
                lq[j * 32l + l] = clamp(nearest_int((x[j * 32l + l] + mj) / dj), 0, 31)
            }
        }
        for (l in range64(32l)) {
            kq[kqo + 128l + l] = uint8(0)
        }
        for (js in range64(4l)) {   // sub-blocks 2js/2js+1 share 32 nibble bytes; qh bit 2js / 2js+1
            for (l in range64(32l)) {
                var l1 = lq[js * 64l + l]
                var l2 = lq[js * 64l + l + 32l]
                var h = 0
                if (l1 > 15) {
                    l1 -= 16
                    h |= 1 << int(2l * js)
                }
                if (l2 > 15) {
                    l2 -= 16
                    h |= 2 << int(2l * js)
                }
                kq[kqo + 128l + l] = uint8(int(kq[kqo + 128l + l]) | h)
                kq[kqo + js * 32l + l] = uint8(l1 | (l2 << 4))
            }
        }
    }
}

//! Encode n fp32 weights (n % 256 == 0) into the k5 planes at plane-local element offset `eloff` —
//! the encode counterpart of gguf_transcode_q5k, for disk types that are not already Q5_K.
def quantize_k5_plane(src : array<float> | #; n : int64; var kq : array<uint8>; var ks : array<uint8>; eloff : int64) {
    if (n % 256l != 0l) {
        panic("quantize_k5_plane: n={n} is not a multiple of 256")
    }
    // O(1) source guard — the loop below reads n weights through a raw pointer, bypassing bounds checks
    if (n > long_length(src)) {
        panic("quantize_k5_plane: n={n} exceeds src length {long_length(src)}")
    }
    let nb = n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst("<k5 encode>", "k5 quant plane", (eloff / 256l) * K5_QSB, nb * K5_QSB, long_length(kq))
    guard_dst("<k5 encode>", "k5 scale plane", (eloff / 256l) * K5_SSB, nb * K5_SSB, long_length(ks))
    unsafe {
        let srcp = addr<float const?>(src[0])
        var kqp = addr(kq[(eloff / 256l) * K5_QSB])
        var ksp = addr(ks[(eloff / 256l) * K5_SSB])
        // the qkx2 sweep is ~100x a byte copy per superblock, so thread far earlier than transcode_jobs
        let njobs = is_job_que_available() ? int(clamp(nb / 64l, 1l, 64l)) : 1
        maybe_parallel_for(0, int(nb), njobs) $(rb, re) {
            unsafe {
                for (blk in range64(int64(rb), int64(re))) {
                    quantize_k5_superblock_p(srcp + blk * 256l, kqp, blk * K5_QSB, ksp, blk * K5_SSB)
                }
            }
        }
    }
}

//! Reference dequant of one k6-plane superblock: w = (d*sc16)*(q - 32) — bit-identical to the
//! disk dequant (the k6 planes are verbatim).

def dequant_k6_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    dequant_k6_plane_superblock_at(kq, kqo, ks, kso, kso + 16l, dst, doff)
}

//! The split-layout twin: `dso` names the f16 d halfword's byte offset within ks independently
//! of the 16B sub-scale strip at kso — metal-blob models keep k6s in the GPU split form
//! ([nsb x 16B sub-scales][nsb x f16 d]) where d lives in the plane's tail.
def dequant_k6_plane_superblock_at(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso, dso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, dso))
    var qlo = kqo
    var qho = kqo + 128l
    var sco = kso
    var dsto = doff
    for (_half in range(2)) {
        for (l in range64(32l)) {
            let si = l / 16l
            let hl = int(kq[qho + l])
            let q1 = ((int(kq[qlo + l]) & 15) | (((hl >> 0) & 3) << 4)) - 32
            let q2 = ((int(kq[qlo + 32l + l]) & 15) | (((hl >> 2) & 3) << 4)) - 32
            let q3 = ((int(kq[qlo + l]) >> 4) | (((hl >> 4) & 3) << 4)) - 32
            let q4 = ((int(kq[qlo + 32l + l]) >> 4) | (((hl >> 6) & 3) << 4)) - 32
            dst[dsto + l] = d * float(rd_i8(ks, sco + si)) * float(q1)
            dst[dsto + 32l + l] = d * float(rd_i8(ks, sco + si + 2l)) * float(q2)
            dst[dsto + 64l + l] = d * float(rd_i8(ks, sco + si + 4l)) * float(q3)
            dst[dsto + 96l + l] = d * float(rd_i8(ks, sco + si + 6l)) * float(q4)
        }
        qlo += 64l
        qho += 32l
        sco += 8l
        dsto += 128l
    }
}

//! Transcode one Q4_0 superblock (8 x 18-byte disk blocks at `bo`) into the q40 planes: per
//! block the f16 d halfword to ks[kso + 2*blk), the 16 nibble bytes verbatim to kq (exact).
def transcode_q40_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (blk in range64(8l)) {
        ks[kso + blk * 2l] = bytes[bo + blk * 18l]
        ks[kso + blk * 2l + 1l] = bytes[bo + blk * 18l + 1l]
        for (i in range64(16l)) {
            kq[kqo + blk * 16l + i] = bytes[bo + blk * 18l + 2l + i]
        }
    }
}

//! Reference dequant of one q40-plane superblock: w = d * (q - 8) per 32-block, q = nibble
//! (low = k, high = k+16 — the disk pairing, kept verbatim by the transcode).
def dequant_q40_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    for (blk in range64(8l)) {
        let d = f16_to_f32(rd_u16(ks, kso + blk * 2l))
        for (l in range64(16l)) {
            let b = int(kq[kqo + blk * 16l + l])
            dst[doff + blk * 32l + l] = d * float((b & 15) - 8)
            dst[doff + blk * 32l + 16l + l] = d * float((b >> 4) - 8)
        }
    }
}

//! Transcode one IQ4_XS superblock (136 bytes at `bo`: f16 d, u16 scales_h, 4 scales_l bytes,
//! 128 nibble bytes) into the iq4xs planes: nibbles verbatim (the disk k/k+16 pairing IS the
//! q40 tiling), the scale row DECODED — f16 d, 2 pad, 8 x int8 (ls - 32), 8 pad (exact).
def transcode_iq4xs_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    ks[kso] = bytes[bo]
    ks[kso + 1l] = bytes[bo + 1l]
    ks[kso + 2l] = uint8(0)
    ks[kso + 3l] = uint8(0)
    let sh = uint(bytes[bo + 2l]) | (uint(bytes[bo + 3l]) << 8u)
    for (blk in range64(8l)) {
        let lo = (uint(bytes[bo + 4l + blk / 2l]) >> uint(4l * (blk % 2l))) & 15u
        let ls = int(lo | (((sh >> uint(2l * blk)) & 3u) << 4u))
        ks[kso + 4l + blk] = uint8(ls - 32)
    }
    for (i in range64(8l)) {
        ks[kso + 12l + i] = uint8(0)
    }
    for (i in range64(128l)) {
        kq[kqo + i] = bytes[bo + 8l + i]
    }
}

//! The signed sub-scale of an iq4xs plane row: byte 4 + blk of the 20B row, read as int8.
def iq4xs_sc(ks : array<uint8> | #; kso, blk : int64) : int => rd_i8(ks, kso + 4l + blk)

//! Reference dequant of one iq4xs-plane superblock: w = (d * sc) * IQ4NL_LUT[q] per 32-block
//! (ggml's own float order: dl = d * (ls - 32), then dl * kvalue), q = nibble (low = k, high =
//! k+16 — the disk pairing, kept verbatim by the transcode).
def dequant_iq4xs_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    dequant_iq4xs_plane_superblock_at(kq, kqo, ks, kso + 4l, kso, dst, doff)
}

//! The split-layout twin: `dso` names the f16 d halfword's byte offset independently of the
//! 8-byte signed sub-scale strip at kso — metal-blob models keep iq4xss in the k6 split form
//! ([nsb x 16B strips][nsb x f16 d]); the CPU row is the same math at kso + 4 / kso.
def dequant_iq4xs_plane_superblock_at(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso, dso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, dso))
    let lut = iq4nl_lut()
    for (blk in range64(8l)) {
        let dl = d * float(rd_i8(ks, kso + blk))
        for (l in range64(16l)) {
            let b = int(kq[kqo + blk * 16l + l])
            dst[doff + blk * 32l + l] = dl * float(lut[b & 15])
            dst[doff + blk * 32l + 16l + l] = dl * float(lut[b >> 4])
        }
    }
}

//! Transcode one Q2_K superblock (84 bytes at `bo`: 16 sc/min nibble-pair bytes, 64 qs, f16 d,
//! f16 dmin) into the k2 planes: qs verbatim to kq[kqo..+64), the scale row REORDERED to
//! [f16 d][f16 dmin][16 sc/min bytes] at ks[kso..+20) - all bytes verbatim (exact).
def transcode_q2k_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    ks[kso] = bytes[bo + 80l]
    ks[kso + 1l] = bytes[bo + 81l]
    ks[kso + 2l] = bytes[bo + 82l]
    ks[kso + 3l] = bytes[bo + 83l]
    for (i in range64(16l)) {
        ks[kso + 4l + i] = bytes[bo + i]
    }
    for (i in range64(64l)) {
        kq[kqo + i] = bytes[bo + 16l + i]
    }
}

//! Reference dequant of one k2-plane superblock: w = (d * sc) * q - (dmin * mn) per 16-elem group
//! g (sc/mn = the group's nibble pair); q = the 2-bit lane of qs byte (g/8)*32 + (g%2)*16 + l at
//! shift 2*((g%8)/2) - ggml's dequantize_row_q2_K float order (dl*q - ml).
def dequant_k2_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    dequant_k2_plane_superblock_at(kq, kqo, ks, kso + 4l, kso, dst, doff)
}

//! The split-layout twin: `sso` names the 16 pair-byte strip, `dso` the 4B d+dmin header -
//! independent bases, so the metal-blob split plane reads through the same math.
def dequant_k2_plane_superblock_at(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; sso, dso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, dso))
    let dmin = f16_to_f32(rd_u16(ks, dso + 2l))
    for (g in range64(16l)) {
        let scb = int(ks[sso + g])
        let dl = d * float(scb & 15)
        let ml = dmin * float(scb >> 4)
        let qb = kqo + (g / 8l) * 32l + (g % 2l) * 16l
        let sh = int(((g % 8l) / 2l) * 2l)
        for (l in range64(16l)) {
            dst[doff + g * 16l + l] = dl * float((int(kq[qb + l]) >> sh) & 3) - ml
        }
    }
}

//! The 6-bit sub-scale i (0..15) of a Q3_K disk block off its 12 packed scale bytes at `so` —
//! ggml's kmask unpack: low nibbles of bytes 0..7 carry scales 0..7, their high nibbles 8..15,
//! bytes 8..11 carry every scale's top two bits at shift 2 * (i / 4).
def q3k_scale6(bytes : array<uint8> | #; so, i : int64) : int {
    let lo = i < 8l ? int(bytes[so + i]) & 15 : int(bytes[so + i - 8l]) >> 4
    let hi = int(bytes[so + 8l + i % 4l]) >> int(2l * (i / 4l))
    return lo | ((hi & 3) << 4)
}

//! Transcode one Q3_K superblock (110 bytes at `bo`: 32 hmask, 64 qs, 12 packed 6-bit scales,
//! f16 d) into the k3 planes: [qs 64][hmask 32] verbatim to kq[kqo..+96), the 16 sub-scales
//! DECODED to int8 (scale - 32) + the f16 d to ks[kso..+18) — the k6 row shape (exact).
def transcode_q3k_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (i in range64(64l)) {
        kq[kqo + i] = bytes[bo + 32l + i]
    }
    for (i in range64(32l)) {
        kq[kqo + 64l + i] = bytes[bo + i]
    }
    for (i in range64(16l)) {
        ks[kso + i] = uint8(q3k_scale6(bytes, bo + 96l, i) - 32)
    }
    ks[kso + 16l] = bytes[bo + 108l]
    ks[kso + 17l] = bytes[bo + 109l]
}

//! Reference dequant of one k3-plane superblock: w = (d * sc16) * (q - 4) with q = the 2-bit lane
//! of qs byte h*32 + l at shift 2j | hmask bit (4h + j) of byte l << 2, block 4h + j — ggml's
//! own float order (dl = d * (scale - 32), then dl * (q3 - 4)).
def dequant_k3_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    dequant_k3_plane_superblock_at(kq, kqo, ks, kso, kso + 16l, dst, doff)
}

//! The split-layout twin: `dso` names the f16 d halfword's byte offset independently of the
//! 16-byte sub-scale strip at kso — the k6 split form a metal-blob model keeps.
def dequant_k3_plane_superblock_at(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso, dso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, dso))
    for (blk in range64(8l)) {
        let qb = kqo + (blk / 4l) * 32l
        let sh = int((blk % 4l) * 2l)
        let dl0 = d * float(rd_i8(ks, kso + 2l * blk))
        let dl1 = d * float(rd_i8(ks, kso + 2l * blk + 1l))
        for (l in range64(32l)) {
            let q2 = (int(kq[qb + l]) >> sh) & 3
            let hb = (int(kq[kqo + 64l + l]) >> int(blk)) & 1
            dst[doff + blk * 32l + l] = (l < 16l ? dl0 : dl1) * float((q2 | (hb << 2)) - 4)
        }
    }
}

//! Transcode one IQ3_S superblock (110 bytes at `bo`: f16 d, 64 qs, 8 qh, 32 signs, 4 packed
//! 4-bit scales) into the iq3s planes: [qs][qh][signs] verbatim to kq[kqo..+104), the 8
//! sub-scales DECODED to int8 (1 + 2s) + the f16 d to ks[kso..+20) — the k4 row shape (exact).
def transcode_iq4nl_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (blk in range64(8l)) {
        ks[kso + blk * 2l] = bytes[bo + blk * 18l]
        ks[kso + blk * 2l + 1l] = bytes[bo + blk * 18l + 1l]
        for (i in range64(16l)) {
            kq[kqo + blk * 16l + i] = bytes[bo + blk * 18l + 2l + i]
        }
    }
}

//! Reference dequant of one iq4nl-plane superblock: w = d * IQ4NL_LUT[nibble], the q40 k/k+16
//! pairing, per-32-block f16 d - ggml's dequantize_row_iq4_nl float order.
def dequant_iq4nl_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    for (blk in range64(8l)) {
        let d = f16_to_f32(rd_u16(ks, kso + blk * 2l))
        for (l in range64(16l)) {
            let b = int(kq[kqo + blk * 16l + l])
            dst[doff + blk * 32l + l] = d * float(int(IQ4NL_LUT[b & 15]))
            dst[doff + blk * 32l + 16l + l] = d * float(int(IQ4NL_LUT[b >> 4]))
        }
    }
}

def transcode_iq3xxs_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (i in range64(96l)) {
        kq[kqo + i] = bytes[bo + 2l + i]
    }
    let dh = f16_half_bits(uint(bytes[bo]) | (uint(bytes[bo + 1l]) << 8u))
    ks[kso] = uint8(dh & 0xFFu)
    ks[kso + 1l] = uint8(dh >> 8u)
    ks[kso + 2l] = uint8(0)
    ks[kso + 3l] = uint8(0)
    for (b in range64(8l)) {
        ks[kso + 4l + b] = uint8(1 + 2 * (int(bytes[bo + 66l + b * 4l + 3l]) >> 4))
    }
    for (i in range64(12l, 20l)) {
        ks[kso + i] = uint8(0)
    }
}

//! Reference dequant of one iq3xxs-plane superblock: w = (d_halved * (2ls + 1)) * halved_grid
//! byte * sign — ggml's 0.25 * d * (2ls+1) * grid * sign exactly (both halvings are lossless).
//! Grid entry = one qs byte; sign byte = ksigns_iq2xs[(aux32 >> 7l) & 127], bit j (bit 7 = parity).
def dequant_iq3xxs_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    dequant_iq3xxs_plane_superblock_at(kq, kqo, ks, kso + 4l, kso, dst, doff)
}

//! The split-layout twin: `dso` names the f16 d halfword's byte offset independently of the
//! 8-byte sub-scale strip at kso (the metal split form keeps them apart).
def dequant_iq3xxs_plane_superblock_at(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso, dso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, dso))
    let grid = iq3xxs_grid()
    let signs = ksigns_iq2xs()
    for (b in range64(8l)) {
        let db = d * float(int(int8(ks[kso + b])))
        let aux = uint(kq[kqo + 64l + b * 4l]) | (uint(kq[kqo + 64l + b * 4l + 1l]) << 8u) | (uint(kq[kqo + 64l + b * 4l + 2l]) << 16u) | (uint(kq[kqo + 64l + b * 4l + 3l]) << 24u)
        for (l in range64(4l)) {
            let sgn = int(signs[int((aux >> uint(7l * l)) & 127u)])
            let g1 = grid[int(kq[kqo + b * 8l + 2l * l])]
            let g2 = grid[int(kq[kqo + b * 8l + 2l * l + 1l])]
            for (j in range64(4l)) {
                dst[doff + b * 32l + l * 8l + j] = db * float((g1 >> uint(8l * j)) & 0xFFu) * (((sgn >> int(j)) & 1) != 0 ? -1.0 : 1.0)
                dst[doff + b * 32l + l * 8l + 4l + j] = db * float((g2 >> uint(8l * j)) & 0xFFu) * (((sgn >> int(4l + j)) & 1) != 0 ? -1.0 : 1.0)
            }
        }
    }
}

def transcode_iq3s_superblock(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (i in range64(104l)) {
        kq[kqo + i] = bytes[bo + 2l + i]
    }
    ks[kso] = bytes[bo]
    ks[kso + 1l] = bytes[bo + 1l]
    ks[kso + 2l] = uint8(0)
    ks[kso + 3l] = uint8(0)
    for (b in range64(8l)) {
        let nib = b % 2l == 0l ? int(bytes[bo + 106l + b / 2l]) & 15 : int(bytes[bo + 106l + b / 2l]) >> 4
        ks[kso + 4l + b] = uint8(1 + 2 * nib)
    }
    for (i in range64(12l, 20l)) {
        ks[kso + i] = uint8(0)
    }
}

//! Reference dequant of one iq3s-plane superblock: w = (d * (1 + 2s)) * grid_byte * sign, grid
//! entry qs[b*8 + 2l + half] | (qh[b] bit (2l + half) << 8), sign byte signs[b*4 + l] bit j —
//! ggml's own float order (dequantize_row_iq3_s: db per block, then db * grid * +-1).
def dequant_iq3s_plane_superblock(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    dequant_iq3s_plane_superblock_at(kq, kqo, ks, kso + 4l, kso, dst, doff)
}

//! The split-layout twin: `dso` names the f16 d halfword's byte offset independently of the
//! 8-byte sub-scale strip at kso (the metal split form keeps them apart).
def dequant_iq3s_plane_superblock_at(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso, dso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, dso))
    let grid = iq3s_grid()
    for (b in range64(8l)) {
        let db = d * float(int(int8(ks[kso + b])))
        let qh = int(kq[kqo + 64l + b])
        for (l in range64(4l)) {
            let sgn = int(kq[kqo + 72l + b * 4l + l])
            let g1 = grid[int(kq[kqo + b * 8l + 2l * l]) | ((qh << int(8l - 2l * l)) & 256)]
            let g2 = grid[int(kq[kqo + b * 8l + 2l * l + 1l]) | ((qh << int(7l - 2l * l)) & 256)]
            for (j in range64(4l)) {
                dst[doff + b * 32l + l * 8l + j] = db * float((g1 >> uint(8l * j)) & 0xFFu) * (((sgn >> int(j)) & 1) != 0 ? -1.0 : 1.0)
                dst[doff + b * 32l + l * 8l + 4l + j] = db * float((g2 >> uint(8l * j)) & 0xFFu) * (((sgn >> int(4l + j)) & 1) != 0 ? -1.0 : 1.0)
            }
        }
    }
}

//! Transcode one Q5_1 disk block (24 bytes at `bo`: d f16, m f16, qh u32, 16 nibble bytes) into
//! the q51 planes — verbatim splits, exact. The per-block array form the tests drive; the bulk
//! loader (gguf_transcode_q51) runs the same split pointerized and threaded.
def transcode_q51_block(bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) {
    for (i in range64(4l)) {
        ks[kso + i] = bytes[bo + i]
        kq[kqo + 16l + i] = bytes[bo + 4l + i]
    }
    for (i in range64(16l)) {
        kq[kqo + i] = bytes[bo + 8l + i]
    }
}

//! Reference dequant of one q51-plane 32-block: w = d * q + m with q = nibble | qh_bit << 4
//! (elem k reads qh bit k, elem k+16 bit k+16 — the on-disk Q5_1 bit map, kept verbatim).
def dequant_q51_plane_block(kq : array<uint8> | #; kqo : int64; ks : array<uint8> | #; kso : int64; var dst : array<float> | #; doff : int64) {
    let d = f16_to_f32(rd_u16(ks, kso))
    let m = f16_to_f32(rd_u16(ks, kso + 2l))
    let qh = rd_u32(kq, kqo + 16l)
    for (k in range64(16l)) {
        let b = int(kq[kqo + k])
        let h0 = (int(qh >> uint(k)) & 1) << 4
        let h1 = (int(qh >> uint(k + 16l)) & 1) << 4
        dst[doff + k] = d * float((b & 15) | h0) + m
        dst[doff + k + 16l] = d * float((b >> 4) | h1) + m
    }
}

// Raw byte copy via unaligned u64 moves + byte tail — the workhorse of the pointerized transcode

//! One-time destination-range guard for the pointerized copy loops: the raw-pointer writes bypass
//! per-element bounds checks, so fail loudly up front instead — same trap, O(1) cost per tensor.
def guard_dst(name : string; what : string; off, need, have : int64) {
    if (off < 0l || need < 0l || off + need > have) {
        panic("dasLLAMA: tensor '{name}' {what} range [{off}, +{need}) exceeds {have}")
    }
}

// ===== f16 row converts (generic f32 <-> binary16-in-uint16, [tuned]) =====
// Dual-use and HOT: the f16 KV-cache codec's store/read (per token), the weight-scale plane
// convert and gguf's bulk f16 dequant (load), and the Accelerate batch prep (per prefill chunk).

//! f32 -> f16 row convert. Clamps to ±65504 so inf can NEVER enter a cache/plane —
//! O(kv_dim) per token at the KV store seam, free next to the reads.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template cvt_f32_to_f16_template(var d : uint16 ?; s : float const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = uint16(f32_to_f16(clamp(s[j], -65504.0, 65504.0)))
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def cvt_f32_to_f16(var d : uint16?; s : float const?; n : int64) {}

//! f16 -> f32 row convert (bulk dequant into f32 scratch).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template cvt_f16_to_f32_template(var d : float ?; s : uint16 const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = f16_to_f32(uint(s[j]))
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def cvt_f16_to_f32(var d : float?; s : uint16 const?; n : int64) {}

// ===== fp32 -> Q8_0 / Q8_K / Q4_0 quantizers (split-plane weight layout) + quality stats =====
// These write the SPLIT planes (quants + f32 scales) the GEMM kernels read — the interleaved
// f16-scale KV-cache twins live in dasllama_kv_codec (same math, different contractual bytes).

let QK8 = 32l  // Q8_0 block size: 32 weights share one scale
let QK4 = 32l  // Q4_0 block size: 32 weights share one scale

//! A Q8_0-quantized tensor: int8 quants (length n) plus one fp32 scale per 32-element
//! block. Dequant is w[i] ~= q[i] * scales[i / 32]. Symmetric (zero-centered, no offset).
struct Q8Tensor {
    q : array<int8>
    scales : array<float>
    n : int64
}

//! Quantize a flat fp32 tensor to Q8_0. Per block: d = max(|w|)/127, q = round(w/d)
//! in [-127,127]. The scale factors out of the block so a later dot is d * sum(q[i]*x[i]).
//! A short final block (n not a multiple of 32) is handled.
def quantize_q8_0(src : array<float> | #; n : int64) : Q8Tensor {
    var qt : Q8Tensor
    qt.n = n
    qt.q |> reserve(n)
    qt.q |> resize(n)
    let nblocks = (n + QK8 - 1l) / QK8
    qt.scales |> reserve(nblocks)
    qt.scales |> resize(nblocks)
    for (b in range64(nblocks)) {
        let base = b * QK8
        let blen = min(QK8, n - base)
        var amax = 0.0
        for (i in range64(blen)) {
            amax = max(amax, abs(src[base + i]))
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0  // all-zero block: scale 0, quants 0
        qt.scales[b] = d
        for (i in range64(blen)) {
            qt.q[base + i] = int8(round(src[base + i] * id))
        }
    }
    return <- qt
}

//! Quantize a flat fp32 tensor to Q8_0 directly into pre-allocated slices: quants land at
//! qdst[qoff..], scales at sdst[soff..]. No allocation — the memory-honest loader uses this to
//! quantize one tensor at a time into the model's shared qblob/qscales. n must be a multiple of 32.
def quantize_q8_0_into(src : array<float> | #; n : int64; var qdst : array<int8>; var sdst : array<float>; qoff, soff : int64) {
    assert(n % QK8 == 0l, "quantize_q8_0_into requires n to be a multiple of 32")
    // O(1) range guards — the pointerized path below bypasses per-element bounds checks
    if (n > long_length(src) || qoff < 0l || qoff + n > long_length(qdst) || soff < 0l || soff + n / QK8 > long_length(sdst)) {
        panic("quantize_q8_0_into: [{qoff}, +{n}) / scales [{soff}, +{n / QK8}) exceeds dst ({long_length(qdst)}, {long_length(sdst)}) or src {long_length(src)}")
    }
    // threads only past ~2M elements (the load-time bulk requant); decode-sized calls stay inline
    let njobs = is_job_que_available() ? int(clamp(n / 2097152l, 1l, 64l)) : 1
    unsafe {
        let sp = addr<float const?>(src[0])
        var qp = addr(qdst[0])
        var scp = addr(sdst[0])
        maybe_parallel_for(0, int(n / QK8), njobs) $(rb, re) {
            unsafe {
                quantize_q8_0_into_ptr(sp + int64(rb) * QK8, (int64(re) - int64(rb)) * QK8,
                    qp, scp, qoff + int64(rb) * QK8, soff + int64(rb))
            }
        }
    }
}

[hint(noalias = src, noalias = qdst, noalias = sdst)]
def template quantize_q8_0_into_ptr_template(src : float const ?; n : int64; var qdst : int8 ?; var sdst : float ?; qoff, soff : int64) : void {
    let nblocks = n / QK8
    unsafe {
        let p4 = reinterpret<float4 const?>(src)
        for (b in range64(nblocks)) {
            let base = b * QK8
            // hand-vectorized |x| max: LLVM won't auto-vectorize fmax reduction without nnan/nsz
            // fast-math we don't emit, so reassociate by hand — two float4 lanes hide fmax latency.
            // Bit-exact on finite input (max is order-free); weights/activations are always finite.
            let f4 = base / 4l
            var m0 = abs(p4[f4])
            var m1 = abs(p4[f4 + 1l])
            for (k in range64(1l, 4l)) {
                m0 = max(m0, abs(p4[f4 + 2l * k]))
                m1 = max(m1, abs(p4[f4 + 2l * k + 1l]))
            }
            let m = max(m0, m1)
            let amax = max(max(m.x, m.y), max(m.z, m.w))
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            sdst[soff + b] = d
            for [tune = 1] (i in range64(QK8)) {
                qdst[qoff + base + i] = int8(round(src[base + i] * id))
            }
        }
    }
}

//! Pointer core of quantize_q8_0_into — the batched prefill requant threads over positions in fork
//! contexts, which can only touch raw addresses; destinations are `var int8?`/`var float?`, written
//! directly. Bit-identical to the array overload. fallback "plain" ships unhinted; a box profile can vectorize.
[hint(noalias = src, noalias = qdst, noalias = sdst), tuned(fallback = "plain")]
def quantize_q8_0_into_ptr(src : float const?; n : int64; var qdst : int8?; var sdst : float?; qoff, soff : int64) {}

//! quantize_q8_0_into + per-16 block sums: bdst[boff + i/16] = sum of the 16 int8 quants — the
//! K-quant dot's offset term is o*(sum of activation quants) per weight sub-block, emitted in one pass.
//! Quants/scales are bit-identical to quantize_q8_0_into. n must be a multiple of 32.
def quantize_q8_0_bs_into(src : array<float> | #; n : int64; var qdst : array<int8>; var sdst : array<float>; var bdst : array<int>; qoff, soff, boff : int64) {
    assert(n % QK8 == 0l, "quantize_q8_0_bs_into requires n to be a multiple of 32")
    quantize_q8_0_bs_into_ptr(unsafe(addr(src[0])), n, unsafe(addr(qdst[0])), unsafe(addr(sdst[0])), unsafe(addr(bdst[0])), qoff, soff, boff)
}

[hint(noalias = src, noalias = qdst, noalias = sdst, noalias = bdst)]
def template quantize_q8_0_bs_into_ptr_template(src : float const ?; n : int64; var qdst : int8 ?; var sdst : float ?; var bdst : int ?; qoff, soff, boff : int64) : void {
    let nblocks = n / QK8
    unsafe {
        let p4 = reinterpret<float4 const?>(src)
        for (b in range64(nblocks)) {
            let base = b * QK8
            // amax reduction: same hand-vectorized two-lane shape as quantize_q8_0_into_ptr_template
            let f4 = base / 4l
            var m0 = abs(p4[f4])
            var m1 = abs(p4[f4 + 1l])
            for (k in range64(1l, 4l)) {
                m0 = max(m0, abs(p4[f4 + 2l * k]))
                m1 = max(m1, abs(p4[f4 + 2l * k + 1l]))
            }
            let m = max(m0, m1)
            let amax = max(max(m.x, m.y), max(m.z, m.w))
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            sdst[soff + b] = d
            for [tune = 1] (i in range64(QK8)) {
                qdst[qoff + base + i] = int8(round(src[base + i] * id))
            }
            // per-16 sums of the just-stored quants (L1-hot); plain loop — 16 int8 adds is not
            // the kernel's cost, the fp32 read above is
            for (h in range64(2l)) {
                var bs = 0
                let hb = qoff + base + h * 16l
                for (i in range64(16l)) {
                    bs += int(qdst[hb + i])
                }
                bdst[boff + b * 2l + h] = bs
            }
        }
    }
}

//! Pointer core of quantize_q8_0_bs_into (see there); fork-context callers thread raw addresses.
[hint(noalias = src, noalias = qdst, noalias = sdst, noalias = bdst), tuned(fallback = "plain")]
def quantize_q8_0_bs_into_ptr(src : float const?; n : int64; var qdst : int8?; var sdst : float?; var bdst : int?; qoff, soff, boff : int64) {}

//! Q8_K-form activation quantization (kq v2 — the K-quant dot contract): ONE scale per
//! 256-element superblock (sdst) + per-16 quant sums (bdst, 16/superblock) — lets K-quant kernels
//! fold sub-scale/bsums-x-mins per superblock instead of per 32-block. Separate plane from Q8_0.
def quantize_q8_k_into(src : array<float> | #; n : int64; var qdst : array<int8>; var sdst : array<float>; var bdst : array<int>; qoff, soff, boff : int64) {
    assert(n % 256l == 0l, "quantize_q8_k_into requires n to be a multiple of 256")
    quantize_q8_k_into_ptr(unsafe(addr(src[0])), n, unsafe(addr(qdst[0])), unsafe(addr(sdst[0])), unsafe(addr(bdst[0])), qoff, soff, boff)
}

//! Pointer core of quantize_q8_k_into (see there); fork-context callers thread raw addresses.
[hint(noalias = src, noalias = qdst, noalias = sdst, noalias = bdst)]
def quantize_q8_k_into_ptr(src : float const?; n : int64; var qdst : int8?; var sdst : float?; var bdst : int?; qoff, soff, boff : int64) {
    let nsb = n / 256l
    unsafe {
        let p4 = reinterpret<float4 const?>(src)
        for (s in range64(nsb)) {
            let base = s * 256l
            // amax reduction: the same hand-vectorized two-lane shape as the Q8_0 quantizers
            let f4 = base / 4l
            var m0 = abs(p4[f4])
            var m1 = abs(p4[f4 + 1l])
            for (k in range64(1l, 32l)) {
                m0 = max(m0, abs(p4[f4 + 2l * k]))
                m1 = max(m1, abs(p4[f4 + 2l * k + 1l]))
            }
            let m = max(m0, m1)
            let amax = max(max(m.x, m.y), max(m.z, m.w))
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            sdst[soff + s] = d
            for (i in range64(256l)) {
                qdst[qoff + base + i] = int8(round(src[base + i] * id))
            }
            for (h in range64(16l)) {
                var bs = 0
                let hb = qoff + base + h * 16l
                for (i in range64(16l)) {
                    bs += int(qdst[hb + i])
                }
                bdst[boff + s * 16l + h] = bs
            }
        }
    }
}

//! Dequantize Q8_0 back to fp32 in place: dst[i] = q[i] * scales[i / 32]. The exact
//! reference the dequant-in-dot kernel must reproduce, and the input to the fp32 forward
//! when measuring end-to-end quantization loss. dst must be pre-sized to qt.n.
def dequantize_q8_0(qt : Q8Tensor; var dst : array<float> | #) {
    let nblocks = (qt.n + QK8 - 1l) / QK8
    for (b in range64(nblocks)) {
        let base = b * QK8
        let blen = min(QK8, qt.n - base)
        let d = qt.scales[b]
        for (i in range64(blen)) {
            dst[base + i] = float(qt.q[base + i]) * d
        }
    }
}

//! A Q4_0-quantized tensor in the exact on-disk block layout: 32 weights/block as 16 bytes split-half
//! packed nibbles (byte j: low nibble = element j, high nibble = element j+16) + one scale.
//! w[i] ~= (nibble - 8) * scales[block]; scale is fp32 (GGUF loader transcodes fp16 disk once).
struct Q4Tensor {
    q : array<uint8>
    scales : array<float>
    n : int64
}

//! Quantize a flat fp32 tensor to Q4_0, on-disk convention. Per block: pick the signed element of
//! largest magnitude `smax`, set d = smax / -8 (so that element maps to nibble 0), then
//! nibble = clamp(round(w/d) + 8, 0, 15). The scale factors out so a later dot is d * sum(q[i]*x[i]).
def quantize_q4_0(src : array<float> | #; n : int64) : Q4Tensor {
    assert(n % QK4 == 0l, "Q4_0 requires n to be a multiple of 32")
    let half = QK4 / 2l  // 16 packed bytes per 32-weight block
    var qt : Q4Tensor
    qt.n = n
    qt.q |> reserve(n / 2l)
    qt.q |> resize(n / 2l)
    let nblocks = n / QK4
    qt.scales |> reserve(nblocks)
    qt.scales |> resize(nblocks)
    for (b in range64(nblocks)) {
        let base = b * QK4
        var amax = 0.0
        var smax = 0.0  // signed value achieving amax (the sign is kept)
        for (i in range64(QK4)) {
            let v = src[base + i]
            if (abs(v) > amax) {
                amax = abs(v)
                smax = v
            }
        }
        let d = smax / -8.0
        let id = d != 0.0 ? 1.0 / d : 0.0  // all-zero block: scale 0, nibbles 8 (-> 0)
        qt.scales[b] = d
        let bbyte = b * half
        for (j in range64(half)) {
            let lo = clamp(int(src[base + j] * id + 8.5), 0, 15)
            let hi = clamp(int(src[base + half + j] * id + 8.5), 0, 15)
            qt.q[bbyte + j] = uint8(lo | (hi << 4))
        }
    }
    return <- qt
}

//! Quantize a flat fp32 tensor to Q4_0 (split-half packing) directly into pre-allocated slices:
//! packed nibble bytes land at qdst[qoff_bytes..] (qoff_bytes = weight-element-offset / 2), scales
//! at sdst[soff..]. No allocation — the memory-honest loader's one-tensor-at-a-time path. n % 32 == 0.
def quantize_q4_0_into(src : array<float> | #; n : int64; var qdst : array<uint8>; var sdst : array<float>; qoff_bytes, soff : int64) {
    assert(n % QK4 == 0l, "quantize_q4_0_into requires n to be a multiple of 32")
    let half = QK4 / 2l
    let nblocks = n / QK4
    for (b in range64(nblocks)) {
        let base = b * QK4
        var amax = 0.0
        var smax = 0.0
        for (i in range64(QK4)) {
            let v = src[base + i]
            if (abs(v) > amax) {
                amax = abs(v)
                smax = v
            }
        }
        let d = smax / -8.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        sdst[soff + b] = d
        let bbyte = qoff_bytes + b * half
        for (j in range64(half)) {
            let lo = clamp(int(src[base + j] * id + 8.5), 0, 15)
            let hi = clamp(int(src[base + half + j] * id + 8.5), 0, 15)
            qdst[bbyte + j] = uint8(lo | (hi << 4))
        }
    }
}

//! Dequantize Q4_0 back to fp32 in place: low nibbles -> elements 0..15, high nibbles -> 16..31,
//! dst[i] = (nibble - 8) * scales[block]. The exact reference the dequant-in-dot kernel must
//! reproduce, and the input to the fp32 forward when measuring quant loss. dst pre-sized to qt.n.
def dequantize_q4_0(qt : Q4Tensor; var dst : array<float> | #) {
    let half = QK4 / 2l
    let nblocks = qt.n / QK4
    for (b in range64(nblocks)) {
        let base = b * QK4
        let d = qt.scales[b]
        let bbyte = b * half
        for (j in range64(half)) {
            let raw = int(qt.q[bbyte + j])
            let lo = (raw & int(0x0F)) - 8
            let hi = ((raw >> 4) & int(0x0F)) - 8
            dst[base + j] = float(lo) * d
            dst[base + half + j] = float(hi) * d
        }
    }
}

//! Quantization-error report between an original tensor and its dequantized approximation.
//! snr_db (signal-to-noise ratio) is the headline number: higher = less loss. Q8 typically
//! lands ~45-50 dB, Q4 ~25-30 dB.
struct QuantStats {
    n : int64
    mean_abs_err : double
    max_abs_err : double
    rms_signal : double
    rms_error : double
    snr_db : double
}

//! Reconstruction error of `approx` vs `orig` over n elements. Accumulates in double so
//! the sums stay accurate across millions of weights.
def quant_error_stats(orig : array<float> | #; approx : array<float> | #; n : int64) : QuantStats {
    var st : QuantStats
    st.n = n
    var sum_abs = 0.0lf
    var max_abs = 0.0lf
    var sum_sig2 = 0.0lf
    var sum_err2 = 0.0lf
    for (i in range64(n)) {
        let s = double(orig[i])
        let e = abs(double(approx[i]) - s)
        sum_abs += e
        max_abs = max(max_abs, e)
        sum_err2 += e * e
        sum_sig2 += s * s
    }
    st.mean_abs_err = sum_abs / double(n)
    st.max_abs_err = max_abs
    st.rms_signal = sqrt(sum_sig2 / double(n))
    st.rms_error = sqrt(sum_err2 / double(n))
    st.snr_db = st.rms_error > 0.0lf ? 20.0lf * log10(st.rms_signal / st.rms_error) : 0.0lf
    return st
}

// ===== batch activation requant (token-major image -> per-row Q8 planes) =====

//! Threaded per-row Q8_0 requant of a token-major activation image (row p at x[p*n..] ->
//! xqb[p*n..]). Feeds matmul_q8q8_batch/group3; `par` gates the thread fan-out (callers own
//! their threshold policy).
def requant_rows_q8(x : array<float>; n, npos : int64; var xqb : array<int8>; var xsb : array<float>; par : bool) {
    let nb = n / 32l
    unsafe {
        let xp = addr<float const?>(x[0])
        var qp = addr(xqb[0])
        var sp = addr(xsb[0])
        maybe_parallel_for(par, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    quantize_q8_0_into_ptr(xp + pp * n, n, qp, sp, pp * n, pp * nb)
                }
            }
        }
    }
}

//! Q8_K-form batch requant (kq v2): per-256 scales + per-16 sums of the per-256-scaled quants.
//! k4-weight batch matmuls and MoE gathers consume these rows.
def requant_rows_q8k_bs(x : array<float>; n, npos : int64; var xqb : array<int8>; var xsb : array<float>; var xbsb : array<int>; par : bool) {
    let nsb = n / 256l
    let nh = n / 16l
    unsafe {
        let xp = addr<float const?>(x[0])
        var qp = addr(xqb[0])
        var sp = addr(xsb[0])
        var bp = addr(xbsb[0])
        maybe_parallel_for(par, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    quantize_q8_k_into_ptr(xp + pp * n, n, qp, sp, bp, pp * n, pp * nsb, pp * nh)
                }
            }
        }
    }
}

//! Saturating f32 -> f16 range convert: out-of-range clamps to +-65504, so inf can never enter
//! a downstream GEMM — the f16-plane bake's spelling (`cvt_f32_to_f16` is the raw fast convert).
def cvt_f32_to_f16_sat(var d : uint16?; s : float const?; n : int64) {
    unsafe {
        for (i in range64(n)) {
            d[i] = uint16(f32_to_f16(clamp(s[i], -65504.0, 65504.0)))
        }
    }
}

//! CPU mirror of the GPU dev-W q8 dequant (MetalDequantQ8H), bit-exact: the scale narrows to
//! f16 exactly as the blob stores it, the product computes in f32 (exact for half x int8) and
//! rounds ONCE to f16 — a baked panel equals what the runtime dequant kernel writes
def devw_dequant_q8_core(qp : int8 const?; sf : float const?; nblk : int64; var dst : float16?) {
    unsafe {
        for (b in range64(nblk)) {
            let sh = float16(clamp(sf[b], -65504.0, 65504.0))
            let sw = float(sh)
            for (k in range64(32l)) {
                dst[b * 32l + k] = float16(sw * float(qp[b * 32l + k]))
            }
        }
    }
}

//! CPU mirror of the GPU dev-W k4/k5 dequant (MetalDequantK45T), bit-exact: every f32 product
//! is exact (d*sc <=17 sig bits, dsc*q <=22), so contraction cannot move the one f16 round.
//! sstride = scale row stride: 20 on the raw transcode temp, 16 on the GPU compact form
def devw_dequant_k45_core(kq : uint8 const?; ks : uint8 const?; sstride, nsb : int64; qh : bool; var dst : float16?) {
    let blkw = qh ? 40l : 32l
    unsafe {
        for (sb in range64(nsb)) {
            let sp = reinterpret<float16 const?>(ks + sb * sstride)
            let sv = reinterpret<uint const?>(ks + sb * sstride)
            let qp = reinterpret<uint const?>(kq + sb * blkw * 4l)
            for (jsi in range(8)) {
                let js = uint(jsi)
                var sc = 0u
                var mn = 0u
                if (js < 4u) {
                    sc = (sv[1] >> (8u * js)) & 63u
                    mn = (sv[2] >> (8u * js)) & 63u
                } else {
                    let j2 = js - 4u
                    let hi = sv[3] >> (8u * j2)
                    sc = (hi & 15u) | (((sv[1] >> (8u * j2 + 6u)) & 3u) << 4u)
                    mn = ((hi >> 4u) & 15u) | (((sv[2] >> (8u * j2 + 6u)) & 3u) << 4u)
                }
                let dsc = float(sp[0]) * float(sc)
                let dmn = float(sp[1]) * float(mn)
                for (ei in range(32)) {
                    let e = uint(ei)
                    var q = (qp[(js / 2u) * 8u + e / 4u] >> (8u * (e % 4u) + (js & 1u) * 4u)) & 15u
                    if (qh) {
                        q |= ((qp[32u + e / 4u] >> (8u * (e % 4u) + js)) & 1u) << 4u
                    }
                    dst[sb * 256l + int64(js) * 32l + int64(e)] = float16(dsc * float(q) - dmn)
                }
            }
        }
    }
}

//! CPU mirror of the GPU dev-W k6 dequant (MetalDequantK6H), bit-exact: every f32 product is
//! exact (d*s6 <=18 sig bits, dsc*q <=24), so contraction cannot move the one f16 round.
//! ks/sstride = 16B sub-scale rows; kd/dstride = f16 d rows (raw temp 18, GPU split form 2)
def devw_dequant_k6_core(kq : uint8 const?; ks : uint8 const?; sstride : int64; kd : uint8 const?; dstride, nsb : int64; var dst : float16?) {
    unsafe {
        for (sb in range64(nsb)) {
            let dall = float(reinterpret<float16 const?>(kd + sb * dstride)[0])
            let sbp = ks + sb * sstride
            let qp = reinterpret<uint const?>(kq + sb * 192l)
            for (jsi in range(8)) {
                let js = uint(jsi)
                let half6 = js / 4u
                let gg = js % 4u
                let nsh = (gg / 2u) * 4u
                let hsh = gg * 2u
                for (h in range(2)) {
                    let ile = uint(h)
                    let si = half6 * 8u + gg * 2u + ile
                    let s6 = float((int(sbp[si]) ^ 128) - 128)
                    let dsc = dall * s6
                    for (ti in range(16)) {
                        let k = uint(ti) / 4u
                        let c = uint(ti) % 4u
                        let u = qp[half6 * 16u + (gg & 1u) * 8u + ile * 4u + k]
                        let hu = qp[32u + half6 * 8u + ile * 4u + k]
                        let q = ((u >> (nsh + 8u * c)) & 15u) | (((hu >> (hsh + 8u * c)) & 3u) << 4u)
                        dst[sb * 256l + int64(js) * 32l + int64(ile) * 16l + int64(ti)] = float16(dsc * (float(q) - 32.0))
                    }
                }
            }
        }
    }
}
