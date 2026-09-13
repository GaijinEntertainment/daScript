// Fail-closed fixture: the simdgroup intrinsics' CPU stubs are generics over the operand type, so
// the emitter is the gate - a reduction over a value that is not a float, int or uint scalar is
// refused with the operand named, never emitted for the Metal compiler to reject at pipeline time.

expect 50501

options gen2

require metal/msl_shader

class FcSimdBadElem {
    @ssbo @binding = 0 xa : array<float>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_simd_bad_elem_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        let v = float4(xa[i], xa[i + 1u], xa[i + 2u], xa[i + 3u])
        let s = simd_sum(v)
        oc[i] = s.x
    }
}
