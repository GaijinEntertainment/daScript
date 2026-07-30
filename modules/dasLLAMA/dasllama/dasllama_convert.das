options gen2
options _comment_hygiene = true

module dasllama_convert shared public

require math
require llvm/daslib/f16_cvt   // f16<->f32 halfword codecs — the Q5_K encoder + every f16 scale read
require daslib/jobque_boost public   // quantize_k5_plane threads its superblock loop
require dasllama/dasllama_par        // maybe_parallel_for
require dasllama/dasllama_math       // count_inline_run/count_parallel_dispatch_thru — the par macro's counters (same inversion dasllama_quant documents)

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
// e < 2 lands in fp32 denormal territory -> precomputed bit patterns (ggml_e8m0_to_fp32_half).
def e8m0_to_f32_half(e : uint) : float {
    let bits = e < 2u ? (0x00200000u << e) : ((e - 1u) << 23u)
    return unsafe(reinterpret<float>(bits))
}

// ===== K-quant superblock dequant (256-weight superblocks, ggml Q4_K / Q6_K) =====

// int8 read (K-quant sub-scales are signed bytes)
def private rd_i8(b : array<uint8> | #; o : int64) : int {
    let u = int(b[o])
    return u < 128 ? u : u - 256
}

// 6-bit sub-scale/min unpack for Q4_K/Q5_K superblocks (ggml get_scale_min_k4). The 12 scale
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
//! qh[l], qh base fixed per superblock. Matches llama.cpp dequantize_row_q5_K op-for-op.
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
//! w = (d*sc)*q with q = (ql | qh<<4) - 32. Matches llama.cpp dequantize_row_q6_K op-for-op.
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

// llama.cpp's nearest_int — round-half-to-even via the fp32 add-magic trick, replicated bit-for-bit
// so encoder output stays byte-comparable against llama.cpp's. Caller guarantees |fval| <= 4194303.

def private nearest_int(fval : float) : int {
    let v = fval + 12582912.0
    let i = int(unsafe(reinterpret<uint>(v)))
    return (i & int(0x007fffff)) - int(0x00400000)
}

// Weighted joint scale/min fit for one 32-weight sub-block (llama.cpp make_qkx2_quants, nmax=31,
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
// dequant_k5_plane_superblock, following llama.cpp quantize_row_q5_K_ref op-for-op.
def private quantize_k5_superblock_p(x : float const?; var kq : uint8?; kqo : int64; var ks : uint8?; kso : int64) {  // nolint:STYLE038 — verbatim llama.cpp make_qkx2_quants port; splitting the oracle-matched encoder invites drift
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
//! (elem k reads qh bit k, elem k+16 bit k+16 — the llama.cpp Q5_1 bit map, kept verbatim).
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

// K-quant plane strides (bytes per 256-weight superblock)
let K4_QSB = 128l
let K4_SSB = 20l   // kq v2: 16B verbatim disk scale block + 4B pad (repack decodes in place)
let K5_QSB = 160l
let K5_SSB = 20l   // kq v2: 16B verbatim disk scale block + 4B pad (the k4 form)
let K6_QSB = 192l
let K6_SSB = 18l
let Q40_QSB = 128l   // q40 tier: 8 x 16 nibble bytes (the k4 pairing, k / k+16 per 32-block)
let Q40_SSB = 16l    // 8 x f16 d, verbatim disk halfwords
// q51 tier strides are per 32-block, NOT per superblock: Q5_1 exists precisely for rows that are
// % 32 but not % 256 (llama.cpp's quantizer fallback), so the planes carry no superblock structure
let Q51_QB = 20l   // 16 nibble bytes (k / k+16 pairing) + 4 qh bytes, verbatim disk splits
let Q51_SB = 4l    // d, m f16 halfwords, verbatim

// Raw byte copy via unaligned u64 moves + byte tail — the workhorse of the pointerized transcode

//! One-time destination-range guard for the pointerized copy loops: the raw-pointer writes bypass
//! per-element bounds checks, so fail loudly up front instead — same trap, O(1) cost per tensor.
def guard_dst(name : string; what : string; off, need, have : int64) {
    if (off < 0l || need < 0l || off + need > have) {
        panic("dasLLAMA: tensor '{name}' {what} range [{off}, +{need}) exceeds {have}")
    }
}

// Shared header for the three file-level transcodes: locate + type-check + bounds-check, return
