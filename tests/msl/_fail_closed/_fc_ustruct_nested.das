// Fail-closed fixture: a @uniform struct holding another struct. Only one hop of field access
// lowers, and a nested struct's own alignment is exactly what the 4-byte-scalar rule exists to
// keep out — so the nested field must be refused, not flattened by guesswork.

expect 50501

options gen2

require metal/msl_shader

struct FcUInner {
    a : uint
}

struct FcUOuter {
    n : uint
    inner : FcUInner
}

class FcUstructNested {
    @ssbo @binding = 0 dat : array<float>
    @uniform @binding = 1 ka : FcUOuter

    [metal_kernel(name="fc_ustruct_nested_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        if (i < ka.n) {
            dat[i] = float(ka.inner.a)
        }
    }
}
