options gen2
options indenting = 4

module _tq4_probe_fixture shared public

require daslib/f16_cvt
require math

//! probe-local 8-bit symmetric amax block quant (f16-rounded scale) over v[i0, i0 + n), for the mixed 8/4 arm
def quant_int8_block(var v : array<float>; i0, n : int64) {
    var amax = 0.0
    for (j in range64(i0, i0 + n)) {
        amax = max(amax, abs(v[j]))
    }
    let d = f16_to_f32(f32_to_f16(amax / 127.0))
    let id = d != 0.0 ? 1.0 / d : 0.0
    for (j in range64(i0, i0 + n)) {
        v[j] = float(clamp(int(floor(v[j] * id + 0.5)), -127, 127)) * d
    }
}
