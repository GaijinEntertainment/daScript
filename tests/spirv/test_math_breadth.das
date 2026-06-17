options gen2
options indenting = 4

// Stage-2 math-shader-breadth: hyperbolic (sinh/cosh/tanh/asinh/acosh/atanh), fmod, mad,
// radians, degrees all map to GLSL.std.450 ext-inst sub-opcodes. The dasSpirv emitter routes
// each daslang name through glsl_ext_op; this test pins every Stage-2 sub-opcode by
// (OpExtInst operand index 3 == GLSLstd450 instruction literal) and spirv-val's the blob.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_builtins
require spirv/spirv_dis
require spirv/spirv_grammar
require math

// Dedicated fragment fixture: scalar + float3 inputs exercise both scalar and vec3 ext-inst variants.
// spirv-val rejects mismatched ext-inst operand types.
var @in @location = 0 mb_in : float4
var @out @location = 0 mb_out : float4
[fragment_shader(name="mathb_spv"), marker(no_coverage)]
def mathb {
    let x = mb_in.x
    let y = mb_in.y
    let v = float3(x, y, x)
    let w = float3(y, x, y)
    var r = 0.0f
    r += sinh(x) + cosh(x) + tanh(x)
    r += asinh(x) + acosh(x + 1.5f) + atanh(x * 0.5f)
    r += fmod(x, y) + mad(x, y, x)
    r += radians(x) + degrees(x)
    let vh = sinh(v) + cosh(v) + tanh(v)
    let vi = asinh(v) + acosh(v + float3(1.5f, 1.5f, 1.5f)) + atanh(v * 0.5f)
    let vf = fmod(v, w) + mad(v, w, v)
    let vr = radians(v) + degrees(v)
    mb_out = float4(r + vh.x + vi.x + vf.x + vr.x, 0.0f, 0.0f, 1.0f)
}

def private mathb_words : array<uint> {
    var w : array<uint>
    w |> push_from(mathb_spv)
    return <- w
}

def private has_ext(words : array<uint>; op : GLSLstd450) : bool {
    return op_has_operand_at(words, SpvOp.ExtInst, 3, uint(op))   // operand 3 = GLSLstd450 instruction literal
}

[test]
def test_glsl_std450_breadth_stage2(t : T?) {
    t |> run("GLSL.std.450: Stage-2 hyperbolic + fmod + mad + radians/degrees") <| @(t : T?) {
        var words <- mathb_words()
        t |> success(has_ext(words, GLSLstd450.Sinh), "sinh -> Sinh")
        t |> success(has_ext(words, GLSLstd450.Cosh), "cosh -> Cosh")
        t |> success(has_ext(words, GLSLstd450.Tanh), "tanh -> Tanh")
        t |> success(has_ext(words, GLSLstd450.Asinh), "asinh -> Asinh")
        t |> success(has_ext(words, GLSLstd450.Acosh), "acosh -> Acosh")
        t |> success(has_ext(words, GLSLstd450.Atanh), "atanh -> Atanh")
        // fmod uses the SPIR-V core OpFRem (C fmod semantics — sign-of-dividend),
        // NOT a GLSL.std.450 ext-inst. GLSL.std.450 has no FMod sub-opcode.
        t |> success(has_op(words, SpvOp.FRem), "fmod -> OpFRem")
        t |> success(has_ext(words, GLSLstd450.Fma), "mad -> Fma")
        t |> success(has_ext(words, GLSLstd450.Radians), "radians -> Radians")
        t |> success(has_ext(words, GLSLstd450.Degrees), "degrees -> Degrees")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "mathb: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
