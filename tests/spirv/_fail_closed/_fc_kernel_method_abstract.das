// Fail-closed fixture ([spirv_kernel]): a call to an ABSTRACT method must be rejected — the
// field init carries no function address to devirtualize to.
expect 50501

options gen2

require spirv/spirv_shader

class FcAbs {
    @ssbo @binding = 0 xs : array<float>

    def abstract getv : float

    [spirv_kernel(local_size_x=64, name="fckma_spv")]
    def fc {
        xs[0] = self->getv()
    }
}
