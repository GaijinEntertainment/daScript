// Fail-closed fixture: a max_threads that is not a positive int. The entry would declare no
// threadgroup cap, and the driver may cap the pipeline under the dispatch's threadgroup.

expect 50501

options gen2

require metal/msl_shader

class FcMaxThreadsNonpositive {
    @ssbo @binding = 0 dst : array<uint>

    [metal_kernel(name="fc_max_threads_nonpositive_msl", max_threads=0)]
    def kern {
        let i = gl_GlobalInvocationID.x
        dst[i] = i
    }
}
