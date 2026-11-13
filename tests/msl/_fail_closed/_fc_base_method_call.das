// Fail-closed fixture: a kernel class that overrides a method and
// calls the base class's by name. A self call lowers through the
// kernel class's own table, so the named base method is another
// function than the one a kernel reaches - refused, not lowered to
// the override.

expect 50501

options gen2

require metal/msl_shader

class FcBaseCall {
    @ssbo @binding = 0 dat : array<float>

    def scale(v : float) : float => v * 2.0
}

class FcBaseCallKid : FcBaseCall {
    def override scale(v : float) : float => v * 3.0

    [metal_kernel(name="fc_base_method_call_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = FcBaseCall`scale(self, dat[i])
    }
}
