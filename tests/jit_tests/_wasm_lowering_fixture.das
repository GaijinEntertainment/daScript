// test_wasm_lowerings compiles this for the wasm64 cross target
// and the host, test_half_widen_without_f16c for the x86-base
// class, each dumping the IR: a hinted loop that rounds each float
// to a byte, one that widens halves to floats, a function under a
// function hint and two loop hints, and a loop of float4 math
// entries.
options gen2

require math
require daslib/f16_cvt

[hint(unsafe_range_check, noalias = dst, noalias = src)]
def quant_rows(var dst : int8?; src : float const?; n : int; scale : float) {
    for [vectorize_width = 4] (i in range(n)) {
        unsafe {
            dst[i] = int8(round(src[i] * scale))
        }
    }
}

[hint(unsafe_range_check, noalias = dst, noalias = src)]
def widen_rows(var dst : float?; src : uint16 const?; n : int) {
    for [vectorize_width = 4] (i in range(n)) {
        unsafe {
            dst[i] = f16_to_f32(uint(src[i]))
        }
    }
}

[hint(unsafe_range_check, noalias = dst, noalias = src)]
def wave_rows(var dst : float4?; src : float4 const?; n : int) {
    for (i in range(n)) {
        unsafe {
            let x = src[i]
            dst[i] = sin(x) + cos(x) * exp(x) + exp2(x) + tanh(x)
        }
    }
}

[hint(noinline)]
def plain_sum(src : float const?; n : int) : float {
    var s = 0.
    for [vectorize_disable, unroll_disable] (i in range(n)) {
        s += unsafe(src[i])
    }
    return s
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let n = 64 + length(args)
    var src : array<float>
    var halves : array<uint16>
    var bytes : array<int8>
    var wide : array<float>
    src |> resize(n)
    halves |> resize(n)
    bytes |> resize(n)
    wide |> resize(n)
    for (i in range(n)) {
        src[i] = float(i - 30) * 0.37
        halves[i] = uint16(0x3C00 + i * 97)
    }
    var waves, wout : array<float4>
    waves |> resize(n)
    wout |> resize(n)
    for (i in range(n)) {
        waves[i] = float4(float(i) * 0.11, -0.5, 0.25, 1.) * 0.3
    }
    var total = 0.
    unsafe {
        wave_rows(addr(wout[0]), addr<float4 const?>(waves[0]), n)
        quant_rows(addr(bytes[0]), addr<float const?>(src[0]), n, 1.7)
        widen_rows(addr(wide[0]), addr<uint16 const?>(halves[0]), n)
        total = plain_sum(addr<float const?>(src[0]), n)
    }
    print("QUANT=[{int(bytes[0])} {int(bytes[n - 1])}] WIDE=[{wide[0]} {wide[n - 1]}]\n")
    print("SUM=[{total}] WAVE=[{wout[n - 1]}]\n")
}
