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
