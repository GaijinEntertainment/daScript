options gen2
options indenting = 4

// Stage-1 math-shader-breadth: step + smoothstep are GLSL.std.450 ext-inst opcodes 48/49. The dasSpirv
// emitter maps the daslang `step`/`smoothstep` names through glsl_ext_op; this test pins each call by
// its GLSLstd450 sub-opcode (OpExtInst operand index 3 = the instruction literal) and spirv-val's the
// whole module so per-arg type-checking runs.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_builtins
require spirv/spirv_dis
require spirv/spirv_grammar
require math

// Dedicated fragment fixture: scalar + float3 inputs feed step and smoothstep so both the float and
// vec3 ext-inst variants are emitted. spirv-val rejects mismatched ext-inst operand types.
var @in @location = 0 ss_in : float4
var @out @location = 0 ss_out : float4
[fragment_shader(name="stepfn_spv"), marker(no_coverage)]
def stepfn {
    let x = ss_in.x
    let y = ss_in.y
    let v = float3(x, y, x)
    let w = float3(y, x, y)
    let s_scalar = step(0.5f, x)
    let s_vec = step(v, w)
    let ss_scalar = smoothstep(0.0f, 1.0f, x)
    let ss_vec = smoothstep(v, w, float3(0.5f, 0.5f, 0.5f))
    ss_out = float4(s_scalar + ss_scalar + s_vec.x + ss_vec.x, 0.0f, 0.0f, 1.0f)
}

def private stepfn_words : array<uint> {
    var w : array<uint>
    w |> push_from(stepfn_spv)
    return <- w
}

def private has_ext(words : array<uint>; op : GLSLstd450) : bool {
    return op_has_operand_at(words, SpvOp.ExtInst, 3, uint(op))   // operand 3 = GLSLstd450 instruction literal
}

[test]
def test_glsl_std450_step(t : T?) {
    t |> run("GLSL.std.450: step + smoothstep map to Step/SmoothStep") <| @(t : T?) {
        var words <- stepfn_words()
        t |> success(has_ext(words, GLSLstd450.Step), "step -> Step")
        t |> success(has_ext(words, GLSLstd450.SmoothStep), "smoothstep -> SmoothStep")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "stepfn: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
