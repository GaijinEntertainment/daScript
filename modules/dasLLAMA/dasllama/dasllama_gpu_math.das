options gen2
//! The GPU kernel helpers both kernel homes splice into their shader bodies: pure ALU, no table, no backend
//! lowering - the Metal and the Vulkan emitters each carry a call into its shader as a function of that shader.
module dasllama_gpu_math shared public
require dasllama/dasllama_lint public
require math

//! ksigns_iq2xs without the table: bit 7 of the sign byte is the EVEN PARITY of the 7-bit index - five ALU ops, nothing staged.
def ksign7(v : uint) : uint {
    var t = v ^ (v >> 4u)
    t = t ^ (t >> 2u)
    t = t ^ (t >> 1u)
    return v | ((t & 1u) << 7u)
}

//! the iq3s sign-apply over a grid word by xor-and-add: the magnitudes are odd, so no byte carries into the next
def iq3s_signed(g, nib : uint) : uint {
    let m1 = (nib * 0x00204081u) & 0x01010101u
    let mask = m1 * 255u
    return (g ^ mask) + m1
}

//! gemma's soft cap cap * tanh(v / cap) in softcap4's exp form: a shader tanh NaNs past ~44, while exp(INF) zeroes the term and saturates.
def softcap_exp(v, cap : float) : float => cap * (1.0 - 2.0 / (1.0 + exp(2.0 * v / cap)))

//! tanh clamped to +-15, where f32 tanh already saturates: a device tanh built from exp answers NaN past about +-44, the CPU +-1
def tts_tanh(v : float) : float => tanh(clamp(v, -15.0, 15.0))

//! the logistic sigmoid, one form for the CPU chains and both kernel homes
def sigmoid_f32(v : float) : float => 1.0 / (1.0 + exp(-v))

//! SiLU as the one quotient v / (1 + exp(-v)), the CPU `silu`'s form
def silu_f32(v : float) : float => v / (1.0 + exp(-v))

//! `x` gated by the logistic of `g` as the one quotient x / (1 + exp(-g)), not x times `sigmoid_f32(g)`
def sig_gate(x, g : float) : float => x / (1.0 + exp(-g))

//! Softplus with the standard large-x shortcut: x > 20 → x, else log(1 + exp(x)).
def softplus(x : float) : float => x > 20.0 ? x : log(1.0 + exp(x))

//! the gated delta rule's decay for a head's `a` = -exp(A_log) and its raw gate plus dt_bias
def dn_decay(a, g : float) : float => exp(a * softplus(g))

//! the CPU `gelu`'s tanh form in f32 (not the f16 LUT form the vision towers read), its tanh clamped by `tts_tanh`
def tts_gelu_tanh(v : float) : float => 0.5 * v * (1.0 + tts_tanh(0.7978845608028654 * mad(0.044715 * v * v, v, v)))

//! the erf GELU in the CPU `gelu_erf`'s form - the Numerical Recipes erfc - in f32, the one both homes evaluate
def gelu_erf_f32(v : float) : float {
    let z = abs(v) * 0.7071067811865476
    let t = 1.0 / (1.0 + 0.5 * z)
    let erfc = t * exp(-z * z - 1.26551223 + t * (1.00002368 + t * (0.37409196 + t * (0.09678418 +
        t * (-0.18628806 + t * (0.27886807 + t * (-1.13520398 + t * (1.48851587 +
        t * (-0.82215223 + t * 0.17087277)))))))))
    let half_v_erfc = 0.5 * v * erfc
    return v >= 0.0 ? v - half_v_erfc : half_v_erfc
}

//! the fp16-LUT tanh GELU in the CPU table's form (`gelu_tanh_lut`): the +-10 clamps and both f16 round trips in the kernel, the
//! tanh through the exp identity (Metal's tanh NaNs past about 44; the clamped argument reaches 43.7)
def gelu_tanh_lut_f32(v : float) : float {
    if (v <= -10.0) {
        return 0.0
    }
    if (v >= 10.0) {
        return v
    }
    let xr = float(float16(v))
    let gv = 0.5 * xr * (2.0 - 2.0 / (1.0 + exp(2.0 * (0.79788456080286535587989211986876 * xr * (1.0 + 0.044715 * xr * xr)))))
    return float(float16(gv))
}

//! GELU-quick in the CPU table's form (`geglu_quick_lut_rows`): both f16 round trips in the kernel, so the residual stream sees the
//! same activation bits as the CPU lane
def gelu_quick_lut_f32(v : float) : float {
    let xr = float(float16(v))
    let gv = xr * (1.0 / (1.0 + exp(-1.702 * xr)))
    return float(float16(gv))
}

//! a template constant's value where the stamp fixed it (a nonzero literal), else the run-time field: the head width or tap count a stamp serves
def gk_fixed_or(fixed, run : uint) : uint => fixed != 0u ? fixed : run

//! A forward conv's source row for output row r at tap - (r0 + r) * stride + tap * dil - pad_l - or -1 off the input.
def conv_src_fwd_row(r, tap, r0, pad_l, dil, stride, t_in : uint) : int {
    let st = int((r0 + r) * stride + tap * dil) - int(pad_l)
    return (st >= 0 && uint(st) < t_in) ? st : -1
}

//! The transposed conv's: the source row whose stride lands the tap on output row r, or -1; stride at least one.
def conv_src_tr_row(r, tap, r0, pad_l, dil, stride, t_in : uint) : int {
    let q = int(r0 + r + pad_l) - int(tap * dil)
    return (q >= 0 && uint(q) % stride == 0u && uint(q) / stride < t_in) ? int(uint(q) / stride) : -1
}

def tts_resize_mix_torch(x0, x1, l0, l1 : float) : float => mad(l0, x0, l1 * x1)

//! the trilinear mix the onnx-law stamps follow, in its operation order: the last channel through all seven fma terms, the rest their two nonzero ones
def tts_resize_mix_onnx(x0, x1, l0, l1 : float; last : bool) : float {
    if (last) {
        let q0 = (l0 * 0.5) * 0.5
        let q1 = (l1 * 0.5) * 0.5
        var acc = mad(q0, x0, q1 * x1)
        acc = mad(q0, x0, acc)
        acc = mad(q1, x1, acc)
        acc = mad(q0, x0, acc)
        acc = mad(q1, x1, acc)
        acc = mad(q0, x0, acc)
        return mad(q1, x1, acc)
    }
    let w0 = l0 * 0.5
    let w1 = l1 * 0.5
    var acc = mad(w0, x0, w1 * x1)
    acc = mad(w0, x0, acc)
    return mad(w1, x1, acc)
}

//! the CPU `sine_phase_frames`' rad of cycles `cyc` (divided by the caller): its fraction, `nu` folded into the overtones' sample 0
def tts_src_rad(cyc, nu : float; i, h : uint) : float {
    let r = cyc - floor(cyc)
    return (i == 0u && h > 0u) ? r + nu : r
}

//! the linear resample taps the torch-law stamps follow, at output `to` over `n` inputs; `inv_scale` is 1 / scale, divided by the caller its home's way
def tts_resize_taps_torch(n, to : uint; inv_scale : float; var i0, i1 : uint&; var l0, l1 : float&) {
    let src = max(0.0, mad(inv_scale, float(to) + 0.5, -0.5))
    i0 = min(uint(src), n - 1u)
    i1 = min(i0 + 1u, n - 1u)
    l1 = src - float(i0)
    l0 = 1.0 - l1
}

//! the taps the onnx-law stamps follow, at `q` = (to + 0.5) / scale, divided by the caller its home's way; equal weights where the taps coincide
def tts_resize_taps_onnx(n : uint; q : float; var i0, i1 : uint&; var l0, l1 : float&) {
    let in_x = clamp(q - 0.5, 0.0, float(n - 1u))
    i0 = min(uint(in_x), n - 1u)
    i1 = min(i0 + 1u, n - 1u)
    if (i0 == i1) {
        l0 = 0.5
        l1 = 0.5
    } else {
        l0 = abs(in_x - float(i1))
        l1 = abs(in_x - float(i0))
    }
}

//! sin of a phase past a million radians: 2 pi in pieces of five significant bits, so k times a piece is exact
def tts_sin_big(phase : float) : float {
    let k = round(phase * 0.15915493667125702)
    var r = mad(-k, 6.25, phase)
    r = mad(-k, 0.033203125, r)
    r = mad(-k, -1.781781975296326e-05, r)
    r = mad(-k, -6.608047442568932e-13, r)
    return sin(r)
}

def tts_noise_amp(uv, noise_std, sine_amp : float) : float => uv * noise_std + (1.0 - uv) * sine_amp / 3.0

def tts_src_term(w, s, uv, noise_amp, nz : float) : float => w * (s * uv + noise_amp * nz)

//! one two-float step of the torch law's double accumulator, x into `acc` and its rounding error into `lo`; needs a `precise` body
def tts_two_sum_add(var acc, lo : float&; x : float) {
    let s = acc + x
    let bv = s - acc
    lo += (acc - (s - bv)) + (x - bv)
    let s2 = s + lo
    lo = lo - (s2 - s)
    acc = s2
}

//! the phase in radians of the cycles `acc` summed at the low rate, `up` samples a frame
def tts_phase_rad(acc : float; up : uint) : float => ((acc * 2.0) * 3.14159265358979) * float(up)

//! the reflect pad's source index of padded position p in a signal of n samples, mirrored past either end
def tts_pad_reflect(p, n : int) : int {
    if (p < 0) {
        return -p
    }
    return p >= n ? 2 * (n - 1) - p : p
}

//! the CPU `magnitude_phase`'s magnitude
def tts_stft_mag(re, im, eps : float) : float => sqrt(re * re + im * im + eps)

//! the CPU `magnitude_phase`'s phase in the reference's angle convention: a zero imaginary part reads +pi on a negative real, where atan2 reads -pi
def tts_stft_phase(re, im : float) : float => im == 0.0 ? (re < 0.0 ? 3.14159265358979 : 0.0) : atan2(im, re)

//! one bin's inverse-STFT term: magnitude exp(logmag) at phase sin(phrow) through the transposed conv's two basis weights
def tts_istft_term(logmag, phrow, wre, wim : float) : float {
    let mag = exp(logmag)
    let ph = sin(phrow)
    return mag * cos(ph) * wre - mag * sin(ph) * wim
}

//! the CPU `adain_affine`'s fold of the column stats over `tn` rows, the norm's `g` / `gb` and the style's gamma / beta
def tts_adain_affine(mean, sqsum, tn, eps, g, gb, gamma, beta : float; var scale, shift : float&) {
    let rs = 1.0 / sqrt(sqsum / tn + eps)
    let g1 = 1.0 + gamma
    scale = rs * g * g1
    shift = (gb - mean * rs * g) * g1 + beta
}

def tts_ln_affine(x, mean, inv, w, b : float) : float => (x - mean) * inv * w + b

def tts_modulate(v, shift, scale : float) : float => v * (1.0 + scale) + shift

//! the causal window's first key for the query at position p: the last `ctx` keys up to itself, every key at ctx 0
def tts_window_start(p, ctx : uint) : uint => (ctx != 0u && p + 1u > ctx) ? p + 1u - ctx : 0u

//! the source's own noise draw's counter hash
def tts_hash_u32(x : uint) : uint {
    let a = x * 0x9E3779B1u
    let b = (a ^ (a >> 15u)) * 0x85EBCA77u
    let c = (b ^ (b >> 13u)) * 0xC2B2AE3Du
    return c ^ (c >> 16u)
}

//! a counter-keyed Box-Muller normal, not the CPU's PCG32 stream; the seed hashed first, else seeds four apart alias
def tts_hash_normal(seed, idx : uint) : float {
    let sh = tts_hash_u32(seed)
    let a = tts_hash_u32(sh ^ (idx * 2u + 1u))
    let b = tts_hash_u32(sh ^ (idx * 2u + 2u) ^ 0x6A09E667u)
    let u1 = (float(a >> 8u) + 1.0) * (1.0 / 16777217.0)
    let u2 = float(b >> 8u) * (1.0 / 16777216.0)
    return sqrt(-2.0 * log(u1)) * cos(6.28318530717959 * u2)
}

//! a / b rounded as the CPU's IEEE divide: a SPIR-V divide sits up to 2.5 ulp off, which the phase sums carry; the correction lands the same quotient over Metal's IEEE divide
def tts_div(a, b : float) : float {
    var r = 1.0 / b
    r = mad(mad(-b, r, 1.0), r, r)
    let q = a * r
    return mad(mad(-q, b, a), r, q)
}

//! `v` stored as a half, saturated at the half's largest finite magnitude instead of overflowing to infinity
def half_sat(v : float) : float16 => float16(clamp(v, -65504.0, 65504.0))

let BIAS_ACT_NONE = 0u        //! `GkBiasActArgs.act`: the bias row alone
let BIAS_ACT_GELU_TANH = 1u   //! ... then the tanh-LUT GELU (the vision towers, the whisper family)
let BIAS_ACT_GELU_ERF = 2u    //! ... then the erf GELU (qwen2audio, voxtral, ultravox, the Omni audio towers, Qwen3-ASR)
let BIAS_ACT_SILU = 3u        //! ... then silu (the FastConformer's FFN)
let BIAS_ACT_RELU = 4u        //! ... then relu (the FastConformer's subsample stack)

//! the tower activations by `GkBiasActArgs.act` code - the row stamps' ladder, one free function so either emitter inlines it; the codes
//! are spelled as their literals (the Metal lens admits no global in a kernel): 0 none, 1 the tanh-LUT GELU, 2 the erf GELU, 3 silu, 4 relu
def act_f32(v : float; act : uint) : float {
    if (act == 0u) {
        return v
    }
    if (act == 2u) {
        return gelu_erf_f32(v)
    }
    if (act == 3u) {
        return silu_f32(v)
    }
    if (act == 4u) {
        return max(v, 0.0)
    }
    return gelu_tanh_lut_f32(v)
}
