// Fail-closed fixture: a kernel class that overrides a method and
// calls the base class's by name. A self call lowers through the
// kernel class's own table, so the named base method is another
// function than the one a kernel reaches - refused, not lowered to
// the override.

expect 50501

options gen2

require spirv/spirv_shader

class FcBaseCall {
    @ssbo @binding = 0 dat : array<float>

    def scale(v : float) : float => v * 2.0
}

class FcBaseCallKid : FcBaseCall {
    def override scale(v : float) : float => v * 3.0

    [spirv_kernel(local_size_x=64, name="fc_base_method_call_spv")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = FcBaseCall`scale(self, dat[i])
    }
}
