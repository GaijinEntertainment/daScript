// Fail-closed fixture: a recursive method in STATEMENT position. The value-position twin
// (_fc_method_recursion) is refused by the splice binder before it can recurse; this one reaches
// emit_method_splice, which lowers by recursion — the scan's verdict must stop the emit pass.

expect 50501

options gen2

require metal/msl_shader

class FcMRecStmt {
    @ssbo @binding = 0 dat : array<int>
    acc : int

    def mrec(x : int) {
        if (x > 0) {
            acc += x
            mrec(x - 1)
        }
    }

    [metal_kernel(name="fc_mrec_stmt_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        mrec(dat[i])
        dat[i] = acc
    }
}
