// Fail-closed fixture: a @uniform struct field that is not a 4-byte scalar. The whole point of
// the kargs form is that the das struct and the MSL struct are the same bytes, and a field whose
// size or alignment differs across the seam silently misreads every field after it.

expect 50501

options gen2

require metal/msl_shader

struct FcUArgs {
    n : uint
    v : float4    // 16-byte aligned in MSL, so every following field shifts
}

class FcUstructField {
    @ssbo @binding = 0 dat : array<float>
    @uniform @binding = 1 ka : FcUArgs

    [metal_kernel(name="fc_ustruct_field_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        if (i < ka.n) {
            dat[i] = ka.v.x
        }
    }
}
