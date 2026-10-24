//! The FP4 activation feed on the CPU - the form the CUDA home's FP4 tensor rung reads, built from the same conversions the
//! device uses: a token row's e2m1 nibbles in the (k, k + 8) pairing inside every 16-strip, a UE4M3 code a strip (four a k64
//! word), one f32 row scale (amax / (6 x 448)); the cells' oracle for the feed kernels and the FP4 GEMM's activation side
options gen2
options _dasllama_internal = true

module _fp4_feed shared

require cuda/cuda_intrinsics   // cvt_e2m1x2 / cvt_ue4m3: the conversions' CPU bodies
require dasllama/dasllama_gpu_math   // ue4m3_half_f32
require dasllama/dasllama_kqformat   // MX4_LUT
require math

struct Fp4Feed {
    xq : array<uint>    //! [token][k / 8] words of nibbles
    xsw : array<uint>   //! [token][k / 64] words of four UE4M3 codes
    xrs : array<float>  //! [token] row scales
}

let FP4_ROW_SCALE_DIV = 2688.0   //! 6 x 448: the e2m1 range times the UE4M3 range, the row scale's denominator

//! the UE4M3 code's value (bias 7, the subnormals m x 2^-9)
def ue4m3_value(code : uint) : float => ue4m3_half_f32(code) * 2.0

//! one 16-strip of a row already divided by the row scale: its UE4M3 code off the strip's largest magnitude over 6, and the
//! two nibble words (k, k + 8) a byte under that code's value
def fp4_strip_words(v : float[16]; var code : uint&; var w0, w1 : uint&) {
    var sub = 0.0
    for (i in range(16)) {
        sub = max(sub, abs(v[i]))
    }
    code = cvt_ue4m3(sub / 6.0)
    let sval = ue4m3_value(code)
    let inv = sval > 0.0 ? 1.0 / sval : 0.0
    w0 = cvt_e2m1x2(v[0] * inv, v[8] * inv) | (cvt_e2m1x2(v[1] * inv, v[9] * inv) << 8u) | (cvt_e2m1x2(v[2] * inv, v[10] * inv) << 16u) | (cvt_e2m1x2(v[3] * inv, v[11] * inv) << 24u)
    w1 = cvt_e2m1x2(v[4] * inv, v[12] * inv) | (cvt_e2m1x2(v[5] * inv, v[13] * inv) << 8u) | (cvt_e2m1x2(v[6] * inv, v[14] * inv) << 16u) | (cvt_e2m1x2(v[7] * inv, v[15] * inv) << 24u)
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

//! the value the feed stands for at element `kk` of row `tok`: e2m1 x the strip's UE4M3 x the row scale
def fp4_feed_value(f : Fp4Feed; k, tok, kk : int) : float {
    let j = kk % 16
    let bi = tok * (k / 2) + (kk / 16) * 8 + j % 8
    let nib = (f.xq[bi / 4] >> uint((bi % 4) * 8 + (j / 8) * 4)) & 15u
    let code = (f.xsw[tok * (k / 64) + kk / 64] >> uint(((kk / 16) % 4) * 8)) & 0xFFu
    return float(MX4_LUT[int(nib)]) * 0.5 * ue4m3_value(code) * f.xrs[tok]
}
