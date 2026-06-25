options gen2
options indenting = 4
options no_optimization = true

// `with (block) { ... }` is a transparent name-resolution scope: infer rewrites the body's bare field
// names to block.field, and the optimizer's BlockFolding normally inlines the body and drops the
// subject BEFORE the shader-emit patch runs. With optimization off (this file, and lint compiles) the
// `with` survives to the emitter, so this pins that the SPIR-V emitter emits the body transparently
// instead of rejecting `with`. (The GL rail mirrors this; the OpenGL path tracers are its regression.)

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_dis
require spirv/spirv_grammar

struct WithPC {
    scale  : float
    offset : float
}
var @push_constant with_pc : WithPC
var @ssbo @binding = 0 with_buf : array<float>

// `with (with_pc)` reads scale / offset by bare name; the body multiply-adds into the ssbo.
[compute_shader(local_size_x=1, name="with_spv"), marker(no_coverage)]
def cs_with {
    let i = gl_GlobalInvocationID.x
    with (with_pc) {
        with_buf[i] = with_buf[i] * scale + offset
    }
}

[test]
def test_with_block(t : T?) {
    t |> run("with: transparent scope over a push_constant block — body emits, subject dropped") <| @(t : T?) {
        var words <- clone_to_move(with_spv)
        // the body resolved `scale`/`offset` to pc members and emitted the multiply-add into the ssbo
        t |> success(has_op(words, SpvOp.AccessChain), "with: emits OpAccessChain (pc members + ssbo element)")
        t |> success(has_op(words, SpvOp.FMul), "with: buf[i] * pc.scale")
        t |> success(has_op(words, SpvOp.FAdd), "with: + pc.offset")
        t |> success(has_op(words, SpvOp.Store), "with: OpStore into the ssbo")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "with: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
