options gen2
options indenting = 4
options no_aot     // requires the no_aot _spirv_xmod fixture (optimizer-folded shape; AOT hash unstable)

// Both-branches-return if/else (found lowering the one-source path tracer's shared core module).
//
// When BOTH branches of an if/else terminate (return), the selection-merge block is unreachable.
// The emitter used to unconditionally reset terminated=false after the merge label, so a function
// BODY ending on such an if was misread as "not every control-flow path returns a value". Now the
// merge block is closed with OpUnreachable and the function stays terminated. Two sources of the
// shape: explicit if/else-both-return in the same file, and a REQUIRED module (see _spirv_xmod.das)
// whose optimizer-folded functions arrive with trailing code folded into `else`. spirv-val'd.

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require _spirv_xmod                // optimized cross-module helpers
require spirv/spirv_shader         // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis            // has_op / count_op
require spirv/spirv_grammar        // SpvOp

var @ssbo @binding = 0 rp_data : array<float>

// explicit both-return if/else as the WHOLE body (same-file, unoptimized AST)
def private rp_pick(x : float) : float {
    if (x < 0.0) {
        return 0.0
    } else {
        return x * 2.0
    }
}

[compute_shader(local_size_x=64, name="rp_spv"), marker(no_coverage)]
def rp {
    let i = int(gl_GlobalInvocationID.x)
    var v = 0.0
    if (xm_clip(rp_data[i], v)) {
        rp_data[i] = rp_pick(v) + xm_sign_mag(v)
    }
}

[test]
def test_return_paths(t : T?) {
    t |> run("if/else with both branches returning: merge closes with OpUnreachable; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(rp_spv)
        t |> success(length(words) > 0, "shader with both-return if/else bodies lowers")
        t |> success(has_op(words, SpvOp.Unreachable), "unreachable merge block closed with OpUnreachable")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
