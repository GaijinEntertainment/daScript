options gen2
options indenting = 4

require math

//! Prints `fused` when float4 sin reduces its argument with one
//! multiply-add - the JIT's code for a target with FMA - `unfused`
//! otherwise: 4e-3 off libm against 1e-6 at this magnitude.

var g_x = 99557.055   //! mutable, so the call is not folded at compile time

[export, never_inline]
def wave(x : float4) : float4 => sin(x)

[export]
def main {
    let off = abs(wave(float4(g_x)).x - float(sin(double(g_x))))
    print("{off < 1.0e-4 ? "fused" : "unfused"}\n")
}
