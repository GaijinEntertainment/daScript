// Fail-closed fixture: a plain (state) member with @binding. State members are kernel-entry
// thread-locals, not bindable buffers — a binding index on one is a category error.

expect 50501

options gen2

require metal/msl_shader

class FcStateBinding {
    @ssbo @binding = 0 dat : array<float>
    @binding = 1 acc : float

    [metal_kernel(name="fc_state_binding_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        acc += dat[i]
        dat[i] = acc
    }
}
