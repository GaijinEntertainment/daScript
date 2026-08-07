// Fail-closed fixture ([spirv_kernel]): a recursive method must be rejected — SPIR-V forbids
// call cycles, and method discovery carries the call chain.
expect 50501

options gen2

require spirv/spirv_shader

class FcRec {
    @ssbo @binding = 0 xs : array<float>

    def rec(v : float) : float {
        return v <= 0.0 ? v : self->rec(v - 1.0)
    }

    [spirv_kernel(local_size_x=64, name="fckmr_spv")]
    def fc {
        xs[0] = self->rec(xs[1])
    }
}
