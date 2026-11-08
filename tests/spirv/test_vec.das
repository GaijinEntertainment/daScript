options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_builtins public
require spirv/spirv_dis
require spirv/spirv_grammar

// Whole-vector `==` / `!=`. In daslang (and GLSL) comparing two
// vectors with `==` / `!=` yields a SINGLE bool (all lanes equal /
// any lane differs), but the SPIR-V compare op is component-wise and
// produces a bvecN — so the result must be reduced with OpAll (`==`)
// / OpAny (`!=`) to the scalar bool the branch consumes. The emitter
// used to type the compare's result `bool` directly on vector
// operands, which is invalid SPIR-V (spirv-val + drivers reject
// `OpFOrdEqual %bool %vecN %vecN`); the first shader to hit it (a
// vertex shader culling a black star with `if (rgb == float3(0))`)
// compiled but crashed the driver's shader compiler. This gates the
// OpAll/OpAny reduction and spirv-val's the blob (the invalid form
// never validated because no prior fixture compared vectors).
var @in @location = 0 vc_a : float3
var @in @location = 1 vc_b : float3
var @out @location = 0 vc_col : float3
[vertex_shader(name="vec_cmp_spv"), marker(no_coverage)]
def vec_cmp {
    // `==` on vec3 -> OpFOrdEqual (bvec3) + OpAll -> scalar bool for the
    // branch
    if (vc_a == float3(0.0f, 0.0f, 0.0f)) {
        gl_Position = float4(0.0f, 0.0f, 2.0f, 1.0f)
        vc_col = float3(0.0f, 0.0f, 0.0f)
        return
    }
    // `!=` on vec3 -> OpFUnordNotEqual/OpFOrdNotEqual (bvec3) + OpAny ->
    // scalar bool
    var tint = vc_a
    if (vc_a != vc_b) {
        tint = vc_b
    }
    gl_Position = float4(tint, 1.0f)
    vc_col = tint
}

def public vec_cmp_words : array<uint> => clone_to_move(vec_cmp_spv)

[test(description = "Whole-vector == and != reduce through OpAll and OpAny to a scalar bool and validate.")]
def test_vec_compare(t : T?) {
    t |> run("whole-vector ==/!= reduce via OpAll/OpAny to a scalar bool; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vec_cmp_spv)
        t |> success(!empty(words), "vector-compare shader lowers")
        t |> success(has_op(words, SpvOp.All), "== reduces with OpAll")
        t |> success(has_op(words, SpvOp.Any), "!= reduces with OpAny")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Same-width vector component-kind conversions (found writing the
// tutorial-15 raygen; the natural idiom
// `int2(gl_GlobalInvocationID.xy)` used to fail "component-kind
// conversion ... not supported").
//
// A single-argument vecN constructor whose argument is a vecN of a
// DIFFERENT component kind is a plain conversion:
// OpConvertSToF/UToF/FToS/FToU for value conversions, OpBitcast for
// int<->uint — all vector-valid in SPIR-V (result type = the vector
// type). Cross-width cross-kind stays rejected. spirv-val'd.
var @ssbo @binding = 0 vc_out : array<float4>

[compute_shader(local_size_x=64, name="vc_spv"), marker(no_coverage)]
def vc {
    let gid = gl_GlobalInvocationID.xy      // uint2
    let f = float2(gid)                     // uint2 -> float2 : OpConvertUToF
    let si = int2(gid)                      // uint2 -> int2   : OpBitcast
    let f3 = float3(int3(si.x, si.y, 3))    // int3 -> float3  : OpConvertSToF
    let u2 = uint2(f * 4.0)                 // float2 -> uint2 : OpConvertFToU
    let i2 = int2(f)                        // float2 -> int2  : OpConvertFToS
    let i = int(gl_GlobalInvocationID.x)
    vc_out[i] = float4(f.x + f3.x, float(si.y), float(u2.x), float(i2.y))
}

[test(description = "Same-width vector kind conversions lower to OpConvert and OpBitcast on vectors and validate.")]
def test_vec_convert(t : T?) {
    t |> run("same-width vector kind conversions -> OpConvert*/OpBitcast on vectors; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vc_spv)
        t |> success(has_op(words, SpvOp.ConvertUToF), "float2(uint2) -> OpConvertUToF")
        t |> success(has_op(words, SpvOp.ConvertSToF), "float3(int3) -> OpConvertSToF")
        t |> success(has_op(words, SpvOp.ConvertFToU), "uint2(float2) -> OpConvertFToU")
        t |> success(has_op(words, SpvOp.ConvertFToS), "int2(float2) -> OpConvertFToS")
        t |> success(has_op(words, SpvOp.Bitcast), "int2(uint2) -> OpBitcast")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Vector component under an INDEX (GLSL's v[i]), and the sampler3D
// explicit-lod read that needs it.
//
// The lowering is decided by whether the base names MEMORY, and
// glslang is the arbiter: a vector that has an OpVariable (a `var`
// local) is chained -- OpAccessChain + OpLoad -- while a vector that
// is a pure VALUE (a call result, a `let` local, a parameter) has no
// pointer to chain off and its component is extracted off the loaded
// vector -- OpVectorExtractDynamic. Both shapes appear in one GLSL
// line of the cloud extinction chain (`textureLod(lut, npos, lod)[i]`
// beside `h_min[i]`), so both are pinned here, and the exact
// ExtractDynamic COUNT is what proves the addressable base did NOT
// take the value path (counting AccessChains cannot: the ssbo reads
// emit them too).
//
// The fp16 lane is pinned with them: a half4 `let` extracts a %half
// component directly, with no widen- to-float-and-narrow-back round
// trip, which is the same rule the 16-bit ctors already follow.
var @binding = 0 vix_lut : sampler3D
var @ssbo @binding = 1 vix_in : array<float4>
var @ssbo @binding = 2 vix_out : array<float>

[compute_shader(local_size_x=64, name="vix_spv"), marker(no_coverage)]
def vix_comp {
    let i = int(gl_GlobalInvocationID.x)
    let lane = i % 4
    // VALUE base: the sample result has no OpVariable ->
    // OpVectorExtractDynamic.
    let n = textureLod(vix_lut, float3(0.5, 0.5, 0.5), 0.0)[lane]
    // VALUE base, 16-bit lanes: a `let` is SSA too ->
    // OpVectorExtractDynamic on %half. The ONE OpFConvert in the blob is
    // this narrowing ctor; the int() below must not spend a second one.
    let bx = half4(vix_in[i])
    let period = int(bx[lane])   // %half -> %int DIRECTLY: OpConvertFToS, no widening OpFConvert
    // ADDRESSABLE base: a `var` local vector IS an OpVariable ->
    // OpAccessChain + OpLoad.
    var h = vix_in[i]
    h.z = 1.0
    vix_out[i] = n + float(period) + h[lane]
}

// The result-type WIDTH of every OpVectorExtractDynamic, and whether
// any OpAccessChain lands on a Function-storage float pointer.
// Counting opcodes alone cannot tell a native 16-bit extract from one
// widened through float, nor an addressable base from a value one --
// the types are what say it.
def private extract_widths(words : array<uint>) : array<int> {
    var float_width : table<uint; int>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeFloat) && n >= 2) {
            float_width[w[s]] = int(w[s + 1])
        }
    }
    var widths : array<int>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.VectorExtractDynamic) && n >= 1) {
            widths |> push(float_width?[w[s]] ?? 0)
        }
    }
    return <- widths
}

def private has_function_float_chain(words : array<uint>) : bool {
    var float_ids, fn_float_ptr : table<uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeFloat) && n >= 2 && w[s + 1] == 32u) {
            float_ids |> insert(w[s])
        } elif (opcode == uint(SpvOp.TypePointer) && n >= 3) {
            if (w[s + 1] == uint(SpvStorageClass.Function) && key_exists(float_ids, w[s + 2])) {
                fn_float_ptr |> insert(w[s])
            }
        }
    }
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.AccessChain) && n >= 1 && key_exists(fn_float_ptr, w[s])) {
            found = true
        }
    }
    return found
}

[test(description = "v[i] extracts from a value base and chains through an addressable base, and textureLod on a sampler3D works.")]
def test_vec_index(t : T?) {
    t |> run("v[i]: a value base extracts, an addressable base chains; sampler3D textureLod") <| @(t : T?) {
        var words <- clone_to_move(vix_spv)
        // the sampler3D explicit-lod read
        t |> success(count_op(words, SpvOp.TypeImage) == 1, "one OpTypeImage (the 3D LUT)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim._3D)), "Dim 3D")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 6, 1u),
                     "Sampled 1 (sampled, not storage)")
        t |> success(count_op(words, SpvOp.ImageSampleExplicitLod) == 1,
                     "one OpImageSampleExplicitLod")
        // THREE dynamic component reads, only TWO of them extracts: the `var`
        // local chained instead.
        var widths <- extract_widths(words)
        t |> success(length(widths) == 2,
                     "two OpVectorExtractDynamic (the two VALUE bases), not three")
        t |> success(find_index(widths, 32) >= 0,
                     "one extracts a 32-bit float lane (the sample result)")
        t |> success(find_index(widths, 16) >= 0,
                     "one extracts a %half lane natively (no widen round trip)")
        t |> success(has_function_float_chain(words),
            "the `var` local chained: an OpAccessChain on a Function-storage float pointer")
        // int(half) lowers STRAIGHT off the %half. Widening first
        // (int(float(x))) would spend a second OpFConvert beside the half4
        // narrowing ctor, which is the instruction glslang does not spend --
        // so the census of ONE is what proves the direct convert.
        t |> success(count_op(words, SpvOp.ConvertFToS) == 1, "one OpConvertFToS (int(half))")
        t |> success(count_op(words, SpvOp.FConvert) == 1,
            "exactly one OpFConvert (the half4 narrowing ctor) -- int(half) widened nothing")
        delete widths
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Vector modulo gate: `uvec3 % uvec3` lowers to ONE native OpUMod on
// v3uint.
//
// GLSL's `%` is closed over the integer vector families; daslang
// declares it only on scalars, so shader_lingua_franca declares
// operator %(uint3, uint3). The emitter needed nothing -- binop_code
// already keys `%` off the operand's class -- but the lowering has to
// be PINNED BY RESULT TYPE, not by opcode count alone: three scalar
// OpUMods and one vector OpUMod are told apart by the width of the
// result, and only the vector form matches what glslang gives a uvec3
// modulo.
//
// The signed twin is deliberately NOT declared: daslang's scalar `%`
// takes the dividend's sign (OpSRem) where GLSL's takes the divisor's
// (OpSMod), so a signed vector `%` would lower to an opcode that
// disagrees with the glslang reference on a negative dividend. This
// gate pins zero of both.
var @ssbo @binding = 0 vmod_uv : array<uint4>
[compute_shader(local_size_x=64, name="vmod_spv"), marker(no_coverage)]
def vmod {
    let i = gl_GlobalInvocationID.x
    let a = vmod_uv[i].xyz
    let m = uint3(vmod_uv[i].w)     // the vector-scalar form, splatted as glslang splats it
    vmod_uv[i] = uint4(a % m, 0u)
}

// Every OpUMod's result must be a 3-component vector of 32-bit
// UNSIGNED int. A lane-by-lane lowering would report a scalar result
// here, which is exactly what this rules out.
def private all_umods_are_v3uint(words : array<uint>) : bool {
    var int_info : table<uint; uint2>       // id -> (width, signedness)
    var vec_info : table<uint; uint2>       // id -> (component type, count)
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3) {
            int_info[w[s]] = uint2(w[s + 1], w[s + 2])
        } elif (opcode == uint(SpvOp.TypeVector) && n >= 3) {
            vec_info[w[s]] = uint2(w[s + 1], w[s + 2])
        }
    }
    var all_v3uint = true
    var seen = 0
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.UMod) && n >= 1) {
            seen++
            let v = vec_info?[w[s]] ?? uint2(0u, 0u)
            let c = int_info?[v.x] ?? uint2(0u, 9u)
            if (v.y != 3u || c.x != 32u || c.y != 0u) {
                all_v3uint = false
            }
        }
    }
    return all_v3uint && seen == 1
}

[test(description = "uvec3 % uvec3 is a single native OpUMod on a v3uint.")]
def test_vec_mod(t : T?) {
    t |> run("uvec3 % uvec3 is one native OpUMod on v3uint; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vmod_spv)
        t |> success(count_op(words, SpvOp.UMod) == 1, "one OpUMod (not three scalar ones)")
        t |> success(all_umods_are_v3uint(words), "the OpUMod's result type is v3uint")
        t |> success(count_op(words, SpvOp.SRem) == 0, "no OpSRem (unsigned operands)")
        t |> success(count_op(words, SpvOp.SMod) == 0, "no OpSMod (unsigned operands)")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Vector / scalar arithmetic broadcast (found by spirv-val on the
// one-source path tracer: `acc / float(frame + 1)` emitted a
// shape-mismatched OpFDiv).
//
// SPIR-V arithmetic requires both operands to match the result type.
// Of daslang's binops only * and / broadcast vec <op> scalar (+/- are
// typer-rejected); `*` was already routed through OpVectorTimesScalar
// (test_vector_times_scalar), so `/` — plain and compound — is the
// exposed path. The emitter now splats the scalar operand to the
// vector width via OpCompositeConstruct. spirv-val'd.
var @ssbo @binding = 0 vsa_out : array<float4>
var @ssbo @binding = 1 vsa_iout : array<int4>

[compute_shader(local_size_x=64, name="vsa_spv"), marker(no_coverage)]
def vsa {
    let i = int(gl_GlobalInvocationID.x)
    let n = float(i + 1)
    var v = vsa_out[i].xyz
    v = v / n                       // FDiv: scalar rhs splat
    var acc = vsa_out[i]
    acc /= n                        // compound /= with scalar rhs (float4)
    vsa_out[i] = float4(v, 0.0) + acc
    var iv = vsa_iout[i].xyz
    iv = iv / (i + 2)               // SDiv: int vector / int scalar splat
    vsa_iout[i] = int4(iv, 0)
}

[test(description = "Vector divided by a scalar splats the scalar operand for FDiv and SDiv, in plain and compound forms.")]
def test_vec_scalar_arith(t : T?) {
    t |> run("vec / scalar splats the scalar operand (FDiv/SDiv, plain + compound); spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vsa_spv)
        t |> success(has_op(words, SpvOp.FDiv), "v / s -> OpFDiv (splat rhs)")
        t |> success(has_op(words, SpvOp.SDiv), "ivec / int -> OpSDiv (splat rhs)")
        t |> success(has_op(words, SpvOp.CompositeConstruct),
                     "scalar operands splatted via OpCompositeConstruct")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Vector shift gate: the splat takes the SHIFT operand's component
// type, not the result's.
//
// `uintN >> int` is the only vector-shift spelling daslang has (there
// is no uintN >> uintN overload), so the shift count reaches the
// emitter as an int while the base is a uint vector. SPIR-V requires
// Base and Shift to agree on component COUNT, not on component type
// -- so the scalar count splats to an int vector. Splatting it to the
// RESULT type instead built a v4uint out of int constituents: valid
// enough to emit, rejected by spirv-val ("Expected Constituents to be
// scalars or vectors of the same type as Result Type components").
// Both operand widths and the compound form are exercised.
var @ssbo @binding = 0 vsh_uv : array<uint4>
var @ssbo @binding = 1 vsh_iv : array<int2>
[compute_shader(local_size_x=64, name="vsh_spv"), marker(no_coverage)]
def vsh {
    let i = gl_GlobalInvocationID.x
    // uint4 >> int -> splat to int4, OpShiftRightLogical
    vsh_uv[i] = vsh_uv[i] >> 8
    vsh_uv[i] = vsh_uv[i] << 8                  // uint4 << int -> splat to int4, OpShiftLeftLogical
    vsh_iv[i] = vsh_iv[i] >> 3                  // int2 >> int  -> splat to int2, arithmetic shift
}

// The component type of every OpCompositeConstruct's result: signed
// iff OpTypeInt's signedness is 1. A splat that took the base's type
// would report unsigned here, which is exactly the bug.
def private all_splats_are_signed_int(words : array<uint>) : bool {
    var int_signedness, vec_comp : table<uint; uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3) {
            int_signedness[w[s]] = w[s + 2]
        } elif (opcode == uint(SpvOp.TypeVector) && n >= 3) {
            vec_comp[w[s]] = w[s + 1]
        }
    }
    var all_signed = true
    var seen = 0
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.CompositeConstruct) && n >= 1) {
            seen++
            let comp = vec_comp?[w[s]] ?? 0u
            if ((int_signedness?[comp] ?? 0u) != 1u) {
                all_signed = false
            }
        }
    }
    return all_signed && seen == 3
}

[test(description = "vecN >> int splats the shift count to an int vector.")]
def test_vec_shift(t : T?) {
    t |> run("vecN >> int splats the count to an int vector; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vsh_spv)
        t |> success(count_op(words, SpvOp.ShiftRightLogical) == 1,
                     "one OpShiftRightLogical (uint4)")
        t |> success(count_op(words, SpvOp.ShiftLeftLogical) == 1, "one OpShiftLeftLogical (uint4)")
        t |> success(count_op(words, SpvOp.ShiftRightArithmetic) == 1,
                     "one OpShiftRightArithmetic (int2)")
        t |> success(all_splats_are_signed_int(words),
                     "the three splats are int vectors, not the base's type")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Phase-8.2 vector arithmetic + folded const-vector operand.
// Component-wise +/-/ lower to OpFAdd/OpFSub/OpFDiv on the vector
// type; unary - to OpFNegate; a float2(...) literal operand folds to
// OpConstantComposite. (This is the exact shape Phase 7 deferred.)
// The blob is spirv-val'd.
def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "Component-wise vector +, -, unary - and a folded constant operand lower to the matching vector opcodes.")]
def test_vector_arithmetic(t : T?) {
    t |> run("vector arithmetic: component-wise +/-/ + unary - + folded const operand") <| @(t : T?) {
        var words <- vecarith_words()
        t |> success(has_op(words, SpvOp.FAdd),
                     "vecarith: va + float2(..) -> component-wise OpFAdd")
        t |> success(has_op(words, SpvOp.FSub), "vecarith: a - b -> component-wise OpFSub")
        t |> success(has_op(words, SpvOp.FDiv), "vecarith: c / b -> component-wise OpFDiv")
        t |> success(has_op(words, SpvOp.FNegate), "vecarith: -d -> component-wise OpFNegate")
        t |> success(has_op(words, SpvOp.ConstantComposite),
                     "vecarith: float2(0.5,0.5) -> OpConstantComposite")
        validate(t, words, "vecarith")
        delete words
    }
}

[test(description = "vec *= scalar emits OpVectorTimesScalar for constant and runtime scalars, not a shape-mismatched OpFMul.")]
def test_vector_times_scalar(t : T?) {
    t |> run("vec *= scalar -> OpVectorTimesScalar (const + runtime), not a shape-mismatched OpFMul") <| @(t : T?) {
        var words <- vts_words()
        // Both `v *= 0.5` (const scalar -- the historical crash) and `w *=
        // vts_in.x` (runtime scalar) must lower to OpVectorTimesScalar; a
        // component-wise OpFMul(vec, scalar) is illegal SPIR-V.
        t |> success(has_op(words, SpvOp.VectorTimesScalar),
                     "vts: vec *= scalar -> OpVectorTimesScalar")
        let nvts = count_op(words, SpvOp.VectorTimesScalar)
        t |> success(nvts == 2, "vts: two VectorTimesScalar (const + runtime scalar) (got {nvts})")
        validate(t, words, "vts")
        delete words
    }
}

// Integer vector * scalar gate.
//
// SPIR-V has no OpVectorTimesScalar for integers (that op is
// float-only), so `ivecN * int` must splat the scalar to the vector
// width (OpCompositeConstruct) and then do a component-wise OpIMul.
// Before the fix the emitter rejected this with "integer
// vector/scalar mixes need a splat" (fail-closed, but a missing
// feature). Both operand orders (vec*scalar, scalar*vec) and both int
// + uint are exercised.
//
// The vectors are loaded from SSBO array<ivec> elements (NOT
// constructed), so the only OpCompositeConstruct instructions are the
// three scalar->vector splats — the counts are exact and attributable
// even when spirv-val soft-skips locally.
var @ssbo @binding = 0 ivs_iv : array<int3>
var @ssbo @binding = 1 ivs_uv : array<uint3>
var @ssbo @binding = 2 ivs_is : array<int>
var @ssbo @binding = 3 ivs_us : array<uint>
[compute_shader(local_size_x=64, name="ivs_spv"), marker(no_coverage)]
def ivs {
    let i = gl_GlobalInvocationID.x
    ivs_iv[i] = ivs_iv[i] * ivs_is[i]       // int3 * int   -> splat + OpIMul
    ivs_iv[i] = ivs_is[i] * ivs_iv[i]       // int * int3   -> splat + OpIMul (reverse order)
    ivs_uv[i] = ivs_uv[i] * ivs_us[i]       // uint3 * uint -> splat + OpIMul
}

[test(description = "An integer vector times an int splats the scalar with OpCompositeConstruct and multiplies with OpIMul.")]
def test_int_vec_scalar(t : T?) {
    t |> run("ivecN * int -> splat (OpCompositeConstruct) + OpIMul; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(ivs_spv)
        // Three integer vector*scalar multiplies -> three OpIMul.
        t |> success(count_op(words, SpvOp.IMul) == 3,
                     "three OpIMul (component-wise integer multiply)")
        // The only OpCompositeConstruct are the three scalar->vec splats
        // (vectors are loaded, not built).
        t |> success(count_op(words, SpvOp.CompositeConstruct) == 3,
                     "three OpCompositeConstruct == the splats")
        // Integers must NOT route through OpVectorTimesScalar (that op is
        // float-only).
        t |> success(!has_op(words, SpvOp.VectorTimesScalar),
                     "no OpVectorTimesScalar (ints use IMul)")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
