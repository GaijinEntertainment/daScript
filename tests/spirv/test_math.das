options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_shader
require math
require daslib/shader_lingua_franca

// Phase-8.3 GLSL.std.450 math breadth. The opcode census tracks OpExtInst as a single opcode, so it
// cannot enforce per-function coverage; this test pins each ext-inst by its GLSLstd450 sub-opcode
// (OpExtInst operand index 3 = the instruction literal). The blob is also spirv-val'd, which is the
// real per-function oracle (it type-checks each ext-inst's operands).
def private has_ext(words : array<uint>; op : GLSLstd450) : bool =>
    op_has_operand_at(words, SpvOp.ExtInst, 3, uint(op))  // operand 3 = GLSLstd450 instruction literal

[test(description = "Every reachable math builtin maps to its GLSL.std.450 ext-inst sub-opcode.")]
def test_glsl_std450_breadth(t : T?) {
    t |> run("GLSL.std.450: every reachable ext-inst maps to its sub-opcode") <| @(t : T?) {
        var words <- mathx_words()
        // trig
        t |> success(has_ext(words, GLSLstd450.Sin), "sin -> Sin")
        t |> success(has_ext(words, GLSLstd450.Cos), "cos -> Cos")
        t |> success(has_ext(words, GLSLstd450.Tan), "tan -> Tan")
        t |> success(has_ext(words, GLSLstd450.Asin), "asin -> Asin")
        t |> success(has_ext(words, GLSLstd450.Acos), "acos -> Acos")
        t |> success(has_ext(words, GLSLstd450.Atan), "atan -> Atan")
        t |> success(has_ext(words, GLSLstd450.Atan2), "atan2 -> Atan2")
        // exponential
        t |> success(has_ext(words, GLSLstd450.Pow), "pow -> Pow")
        t |> success(has_ext(words, GLSLstd450.Exp), "exp -> Exp")
        t |> success(has_ext(words, GLSLstd450.Log), "log -> Log")
        t |> success(has_ext(words, GLSLstd450.Exp2), "exp2 -> Exp2")
        t |> success(has_ext(words, GLSLstd450.Log2), "log2 -> Log2")
        t |> success(has_ext(words, GLSLstd450.InverseSqrt), "rsqrt -> InverseSqrt")
        // rounding / fraction
        t |> success(has_ext(words, GLSLstd450.Floor), "floor -> Floor")
        t |> success(has_ext(words, GLSLstd450.Ceil), "ceil -> Ceil")
        t |> success(has_ext(words, GLSLstd450.Round), "round -> Round")
        t |> success(has_ext(words, GLSLstd450.Trunc), "trunc -> Trunc")
        t |> success(has_ext(words, GLSLstd450.Fract), "fract -> Fract")
        // common
        t |> success(has_ext(words, GLSLstd450.FAbs), "abs -> FAbs")
        t |> success(has_ext(words, GLSLstd450.FSign), "sign -> FSign")
        t |> success(has_ext(words, GLSLstd450.FMin), "min -> FMin")
        t |> success(has_ext(words, GLSLstd450.FMax), "max -> FMax")
        t |> success(has_ext(words, GLSLstd450.FMix), "lerp -> FMix")
        // geometric (vector)
        t |> success(has_ext(words, GLSLstd450.Length), "length -> Length")
        t |> success(has_ext(words, GLSLstd450.Distance), "distance -> Distance")
        t |> success(has_ext(words, GLSLstd450.Cross), "cross -> Cross")
        t |> success(has_ext(words, GLSLstd450.Normalize), "normalize -> Normalize")
        t |> success(has_ext(words, GLSLstd450.Reflect), "reflect -> Reflect")
        t |> success(has_ext(words, GLSLstd450.Refract), "refract -> Refract")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "mathx: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Stage-2 math-shader-breadth: hyperbolic (sinh/cosh/tanh/asinh/acosh/atanh), fmod, mad,
// radians, degrees all map to GLSL.std.450 ext-inst sub-opcodes. The dasSpirv emitter routes
// each daslang name through glsl_ext_op; this test pins every Stage-2 sub-opcode by
// (OpExtInst operand index 3 == GLSLstd450 instruction literal) and spirv-val's the blob.
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

def private mathb_words : array<uint> => clone_to_move(mathb_spv)

[test(description = "The hyperbolic functions, fmod, mad, radians and degrees map to their GLSL.std.450 sub-opcodes.")]
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

// Stage-1 math-shader-breadth: step + smoothstep are GLSL.std.450 ext-inst opcodes 48/49. The dasSpirv
// emitter maps the daslang `step`/`smoothstep` names through glsl_ext_op; this test pins each call by
// its GLSLstd450 sub-opcode (OpExtInst operand index 3 = the instruction literal) and spirv-val's the
// whole module so per-arg type-checking runs.
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

def private stepfn_words : array<uint> => clone_to_move(stepfn_spv)

[test(description = "step and smoothstep map to the GLSL.std.450 Step and SmoothStep.")]
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

// mod / fmod are two DIFFERENT core opcodes, not one name spelled twice: GLSL's mod(x, y) is
// x - y*floor(x/y) and takes the DIVISOR's sign (OpFMod), while daslang's own fmod is C's and
// takes the DIVIDEND's sign (OpFRem). They agree only where the dividend is non-negative, so a
// port of a GLSL mod() that reached fmod would compile, spirv-val and silently disagree on
// negative input. This gate pins each name to its own opcode and asserts both are present, so a
// future emitter change cannot quietly collapse the two.
var @ssbo @binding = 0 fmod_io : array<float4>

[compute_shader(local_size_x=1, name="fmodm_spv"), marker(no_coverage)]
def fmodm {
    let x = fmod_io[0].x
    let y = fmod_io[0].y
    let v = float3(x, y, x)
    let w = float3(y, x, y)
    fmod_io[1] = float4(
        fmod(x, y) + mod(x, y),
        (fmod(v, w) + mod(v, w)).x,
        0.0f, 1.0f)
}

def private fmodm_words : array<uint> => clone_to_move(fmodm_spv)

[test(description = "mod lowers to OpFMod and fmod to OpFRem, on scalars and on vectors.")]
def test_fmod_vs_mod(t : T?) {
    t |> run("mod is OpFMod and fmod is OpFRem, on scalar and vector") <| @(t : T?) {
        var words <- fmodm_words()
        t |> success(has_op(words, SpvOp.FMod), "mod: emits FMod")
        t |> success(has_op(words, SpvOp.FRem), "fmod: emits FRem")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "fmodm: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
