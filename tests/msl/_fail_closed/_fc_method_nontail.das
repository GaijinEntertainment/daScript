// Fail-closed fixture: a method body carrying `return`, spliced at a call site that is NOT in
// kernel tail position — the spliced return would exit the whole kernel, skipping the statements
// that follow the call.

expect 50501

options gen2

require metal/msl_shader

class FcMNonTail {
    @ssbo @binding = 0 dat : array<float>
    acc : float

    def guard(i : uint) {
        if (i >= 8u) {
            return
        }
        acc += dat[i]
    }

    [metal_kernel(name="fc_mnontail_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        guard(i)
        dat[i] = acc
    }
}
