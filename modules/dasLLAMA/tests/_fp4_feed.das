//! The FP4 activation feed on the CPU - the form the CUDA home's FP4 tensor rung reads, built from the same conversions the
//! device uses: a token row's e2m1 nibbles in the (k, k + 8) pairing inside every 16-strip, a UE4M3 code a strip (four a k64
//! word), one f32 row scale (amax / (6 x 448)); the cells' oracle for the feed kernels and the FP4 GEMM's activation side
options gen2
options _dasllama_internal = true

module _fp4_feed shared

require cuda/cuda_intrinsics   // cvt_e2m1x2 / cvt_ue4m3: the conversions' CPU bodies
require dasllama/dasllama_gpu_math   // ue4m3_half_f32
require dasllama/dasllama_kqformat   // MX4_LUT
require dasllama/dasllama_cuda_classes   // FP4_CODE_MAX / FP4_ROW_SCALE_DIV: the code range and the row scale, the kernels' own
require math

struct Fp4Feed {
    xq : array<uint>    //! [token][k / 8] words of nibbles
    xsw : array<uint>   //! [token][k / 64] words of four UE4M3 codes
    xrs : array<float>  //! [token] row scales
}

//! the UE4M3 code's value (bias 7, the subnormals m x 2^-9)
def ue4m3_value(code : uint) : float => ue4m3_half_f32(code) * 2.0

//! the smallest UE4M3 code whose scale holds a strip's largest magnitude `m` at or under the e2m1 top (6), the top code where none does:
//! what the kernel's `fp4_strip_code` reaches as the nearest code stepped up once where it saturates, found here by a scan
def fp4_strip_code_scan(m : float) : uint {
    for (c in range(int(FP4_CODE_MAX))) {
        return uint(c) if (ue4m3_value(uint(c)) * 6.0 >= m)
    }
    return FP4_CODE_MAX
}

//! one 16-strip's two nibble words off its values already under their scale: (k, k + 8) a byte
def fp4_strip_nibbles(v : float[16]; var w0, w1 : uint&) {
    w0 = cvt_e2m1x2(v[0], v[8]) | (cvt_e2m1x2(v[1], v[9]) << 8u) | (cvt_e2m1x2(v[2], v[10]) << 16u) | (cvt_e2m1x2(v[3], v[11]) << 24u)
    w1 = cvt_e2m1x2(v[4], v[12]) | (cvt_e2m1x2(v[5], v[13]) << 8u) | (cvt_e2m1x2(v[6], v[14]) << 16u) | (cvt_e2m1x2(v[7], v[15]) << 24u)
}

//! one 16-strip of a row already divided by the row scale: its UE4M3 code off the strip's largest magnitude and its two nibble words, (k, k + 8) a byte under that code's value
def fp4_strip_words(v : float[16]; var code : uint&; var w0, w1 : uint&) {
    var strip_amax = 0.0
    for (i in range(16)) {
        strip_amax = max(strip_amax, abs(v[i]))
    }
    code = fp4_strip_code_scan(strip_amax)
    let sval = ue4m3_value(code)
    let inv = sval > 0.0 ? 1.0 / sval : 0.0
    var sv : float[16]
    for (i in range(16)) {
        sv[i] = v[i] * inv
    }
    fp4_strip_nibbles(sv, w0, w1)
}

//! the feed of `rows` rows of `k` floats (row-major, `k` a multiple of 64)
def fp4_feed_rows(x : array<float>; rows, k : int) : Fp4Feed {
    var f : Fp4Feed
    f.xq |> resize(rows * k / 8)
    f.xsw |> resize(rows * k / 64)
    f.xrs |> resize(rows)
    for (tok in range(rows)) {
        var amax = 0.0
        for (e in range(k)) {
            amax = max(amax, abs(x[tok * k + e]))
        }
        let rs = amax / FP4_ROW_SCALE_DIV
        let inv_rs = rs > 0.0 ? 1.0 / rs : 0.0
        f.xrs[tok] = rs
        for (s in range(k / 16)) {
            var v : float[16]
            for (i in range(16)) {
                v[i] = x[tok * k + s * 16 + i] * inv_rs
            }
            var code = 0u
            var w0 = 0u
            var w1 = 0u
            fp4_strip_words(v, code, w0, w1)
            f.xq[tok * (k / 8) + s * 2] = w0
            f.xq[tok * (k / 8) + s * 2 + 1] = w1
            f.xsw[tok * (k / 64) + s / 4] |= code << uint((s % 4) * 8)
        }
    }
    return <- f
}

//! the MXFP4 feed of `rows` rows of `k` floats (`k` a multiple of 64): a UE8M0 code a 32-block by `mx4_block_code`, the nibbles under it in the (k, k + 8) pairing, two codes a k64 word, the row scale 1
def mx4_feed_rows(x : array<float>; rows, k : int) : Fp4Feed {
    var f : Fp4Feed
    f.xq |> resize(rows * k / 8)
    f.xsw |> resize(rows * k / 64)
    f.xrs |> resize(rows)
    for (tok in range(rows)) {
        f.xrs[tok] = 1.0
        for (b in range(k / 32)) {
            var amax = 0.0
            for (e in range(32)) {
                amax = max(amax, abs(x[tok * k + b * 32 + e]))
            }
            let code = mx4_block_code(amax)
            let inv = mx4_code_inv(code)
            f.xsw[tok * (k / 64) + b / 2] |= code << uint((b % 2) * 8)
            for (s in range(2)) {
                var v : float[16]
                for (i in range(16)) {
                    v[i] = x[tok * k + b * 32 + s * 16 + i] * inv
                }
                var w0 = 0u
                var w1 = 0u
                fp4_strip_nibbles(v, w0, w1)
                f.xq[tok * (k / 8) + (b * 2 + s) * 2] = w0
                f.xq[tok * (k / 8) + (b * 2 + s) * 2 + 1] = w1
            }
        }
    }
    return <- f
}

//! the value the feed stands for at element `kk` of row `tok`: e2m1 x the strip's UE4M3 x the row scale, or under `mx` (the MXFP4
//! feed) e2m1 x the 32-block's UE8M0 (2^(code - 127))
def fp4_feed_value(f : Fp4Feed; k, tok, kk : int; mx : bool) : float {
    let j = kk % 16
    let bi = tok * (k / 2) + (kk / 16) * 8 + j % 8
    let nib = (f.xq[bi / 4] >> uint((bi % 4) * 8 + (j / 8) * 4)) & 15u
    let e = float(MX4_LUT[int(nib)])
    let word = f.xsw[tok * (k / 64) + kk / 64]
    return e * e8m0_half_f32((word >> uint(((kk / 32) % 2) * 8)) & 0xFFu) if (mx)
    return e * ue4m3_half_f32((word >> uint(((kk / 16) % 4) * 8)) & 0xFFu) * f.xrs[tok]
}
