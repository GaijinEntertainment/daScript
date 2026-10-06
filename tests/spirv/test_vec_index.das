options gen2
options indenting = 4

// Vector component under an INDEX (GLSL's v[i]), and the sampler3D explicit-lod read that needs it.
//
// The lowering is decided by whether the base names MEMORY, and glslang is the arbiter: a vector that
// has an OpVariable (a `var` local) is chained -- OpAccessChain + OpLoad -- while a vector that is a
// pure VALUE (a call result, a `let` local, a parameter) has no pointer to chain off and its component
// is extracted off the loaded vector -- OpVectorExtractDynamic. Both shapes appear in one GLSL line of
// the cloud extinction chain (`textureLod(lut, npos, lod)[i]` beside `h_min[i]`), so both are pinned
// here, and the exact ExtractDynamic COUNT is what proves the addressable base did NOT take the value
// path (counting AccessChains cannot: the ssbo reads emit them too).
//
// The fp16 lane is pinned with them: a half4 `let` extracts a %half component directly, with no widen-
// to-float-and-narrow-back round trip, which is the same rule the 16-bit ctors already follow.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis             // count_op / op_has_operand_at / foreach_inst
require spirv/spirv_grammar         // SpvOp / SpvDim / SpvStorageClass

var @binding = 0 vix_lut : sampler3D
var @ssbo @binding = 1 vix_in : array<float4>
var @ssbo @binding = 2 vix_out : array<float>

[compute_shader(local_size_x=64, name="vix_spv"), marker(no_coverage)]
def vix_comp {
    let i = int(gl_GlobalInvocationID.x)
    let lane = i % 4
    // VALUE base: the sample result has no OpVariable -> OpVectorExtractDynamic.
    let n = textureLod(vix_lut, float3(0.5, 0.5, 0.5), 0.0)[lane]
    // VALUE base, 16-bit lanes: a `let` is SSA too -> OpVectorExtractDynamic on %half. The ONE
    // OpFConvert in the blob is this narrowing ctor; the int() below must not spend a second one.
    let bx = half4(vix_in[i])
    let period = int(bx[lane])   // %half -> %int DIRECTLY: OpConvertFToS, no widening OpFConvert
    // ADDRESSABLE base: a `var` local vector IS an OpVariable -> OpAccessChain + OpLoad.
    var h = vix_in[i]
    h.z = 1.0
    vix_out[i] = n + float(period) + h[lane]
}

// The result-type WIDTH of every OpVectorExtractDynamic, and whether any OpAccessChain lands on a
// Function-storage float pointer. Counting opcodes alone cannot tell a native 16-bit extract from one
// widened through float, nor an addressable base from a value one -- the types are what say it.
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

[test]
def test_vec_index(t : T?) {
    t |> run("v[i]: a value base extracts, an addressable base chains; sampler3D textureLod") <| @(t : T?) {
        var words <- clone_to_move(vix_spv)
        // the sampler3D explicit-lod read
        t |> success(count_op(words, SpvOp.TypeImage) == 1, "one OpTypeImage (the 3D LUT)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim._3D)), "Dim 3D")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 6, 1u), "Sampled 1 (sampled, not storage)")
        t |> success(count_op(words, SpvOp.ImageSampleExplicitLod) == 1, "one OpImageSampleExplicitLod")
        // THREE dynamic component reads, only TWO of them extracts: the `var` local chained instead.
        var widths <- extract_widths(words)
        t |> success(length(widths) == 2, "two OpVectorExtractDynamic (the two VALUE bases), not three")
        t |> success(find_index(widths, 32) >= 0, "one extracts a 32-bit float lane (the sample result)")
        t |> success(find_index(widths, 16) >= 0, "one extracts a %half lane natively (no widen round trip)")
        t |> success(has_function_float_chain(words),
            "the `var` local chained: an OpAccessChain on a Function-storage float pointer")
        // int(half) lowers STRAIGHT off the %half. Widening first (int(float(x))) would spend a
        // second OpFConvert beside the half4 narrowing ctor, which is the instruction glslang does
        // not spend -- so the census of ONE is what proves the direct convert.
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
