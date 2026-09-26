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

//! tanh clamped to +-15, where it already saturates in f32: past that a device tanh built from exp answers NaN, the CPU +-1
def tts_tanh(v : float) : float => tanh(clamp(v, -15.0, 15.0))

//! the CPU `sigmoid_f`
def tts_sigmoid(v : float) : float => 1.0 / (1.0 + exp(-v))

def tts_resize_mix_torch(x0, x1, l0, l1 : float) : float => mad(l0, x0, l1 * x1)

//! onnxruntime's trilinear mix in its operation order: the last channel through all seven fma terms, the rest their two nonzero ones
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

//! sin of a phase past a million radians: 2 pi in pieces of five significant bits, so k times a piece is exact
def tts_sin_big(phase : float) : float {
    let k = round(phase * 0.15915493667125702)
    var r = mad(-k, 6.25, phase)
    r = mad(-k, 0.033203125, r)
    r = mad(-k, -1.781781975296326e-05, r)
    r = mad(-k, -6.608047442568932e-13, r)
    return sin(r)
}

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
