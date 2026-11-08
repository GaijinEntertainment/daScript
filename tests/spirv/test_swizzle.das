options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_shader
require spirv/spirv_builtins public

// Phase-10.2 gate: multi-component swizzle of a bare global (the Phase-8 out-of-scope note). A
// single-component global swizzle stays an lvalue (OpAccessChain to the component); a multi-component
// one loads the whole global and OpVectorShuffle's the lanes. Asserts both opcodes are present (shuffle
// for .xy/.xyz/.yx, access-chain for the single-component .w) and that the module is spirv-val-clean --
// the real check that the loaded-then-shuffled lanes form a valid vector result.
def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "A multi-component global swizzle loads then emits OpVectorShuffle and a single-component one emits OpAccessChain.")]
def test_global_swizzle(t : T?) {
    t |> run("swizzle: multi-component global -> load + OpVectorShuffle; single-component -> OpAccessChain") <| @(t : T?) {
        var words <- gswizzle_words()
        // a fragment entry point
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "gswizzle: EntryPoint model is Fragment")
        // multi-component global swizzles (gl_FragCoord.xy, sw_in.xyz, sw_in.yx) -> OpVectorShuffle
        t |> success(has_op(words, SpvOp.VectorShuffle), "gswizzle: multi-component swizzle -> OpVectorShuffle")
        // the single-component global swizzle (sw_in.w) -> OpAccessChain to the component
        t |> success(has_op(words, SpvOp.AccessChain), "gswizzle: single-component global swizzle -> OpAccessChain")
        // and the loaded lanes feed a float4 constructor
        t |> success(has_op(words, SpvOp.CompositeConstruct), "gswizzle: result via OpCompositeConstruct")
        validate(t, words, "gswizzle")
        delete words
    }
}

// The write-swizzle lvalue (GLSL's `ssbo[i].xyz = pos`, `groups.xy = …`).
//
// SPIR-V has no partial store, so a write-swizzle has two possible lowerings, and they are NOT
// interchangeable. glslang picks the second one for a memory target, and a port must match it:
//   load-shuffle-store   OpLoad the whole destination, OpVectorShuffle the new lanes in, OpStore it
//                        back whole. Reads memory the shader never asked to read.
//   per-lane store       one OpAccessChain to each destination component, one OpCompositeExtract of
//                        the matching rhs lane, one OpStore. Touches only the lanes written.
// So the load-shuffle-store form is what the assertions below RULE OUT: every shader here builds its
// rhs with OpCompositeConstruct and reads no swizzle, which makes OpVectorShuffle == 0 the exact
// witness that no destination was loaded and shuffled.
//
// daslang only accepts a CONTIGUOUS ascending mask as a reference (`v.xz = …` is error[30915], "can
// only copy to a reference"), so the lanes are always distinct and each is written exactly once.
//
// THE LANE MAPPING IS THE SUBTLE PART, and wsw_ofs_shader is what pins it: `l.yzw = rhs` writes
// DESTINATION lanes 1,2,3 from SOURCE lanes 0,1,2. An implementation that reused the loop index for
// both would store to 0,1,2 -- still valid SPIR-V, still spirv-val-clean, and wrong. The test asserts
// the component AccessChains carry constants 1,2,3 and NOT 0.
var @ssbo @binding = 0 wsw_f4 : array<float4>
var @ssbo @binding = 1 wsw_i4 : array<int4>
var @ssbo @binding = 2 wsw_src : array<float>

// Two write-swizzles into SSBO elements, over both a float and an int vector. The rhs of each is an
// OpCompositeConstruct, never a read-swizzle, so the shader's OpVectorShuffle count is exactly the
// number of destinations that were load-shuffle-stored -- which must be zero.
[compute_shader(local_size_x=64, name="wsw_spv"), marker(no_coverage)]
def wsw_shader {
    let i = int(gl_GlobalInvocationID.x)
    let a = wsw_src[i]
    wsw_f4[i].xyz = float3(a, a + 1.0, a + 2.0)
    wsw_i4[i].xy = int2(int(a), int(a) + 1)
}

// A write-swizzle into a `var` local (Function storage), at a lane OFFSET: destination lanes 1,2,3
// from source lanes 0,1,2. This is the mapping wsw_ofs pins below.
[compute_shader(local_size_x=64, name="wsw_ofs_spv"), marker(no_coverage)]
def wsw_ofs_shader {
    let i = int(gl_GlobalInvocationID.x)
    let a = wsw_src[i]
    var l = float4(0.0, 0.0, 0.0, 0.0)
    l.yzw = float3(a, a + 1.0, a + 2.0)
    wsw_f4[i] = l
}

// The id of OpTypeInt 32 0 (the uint type). Operands are [result-id, width, signedness].
def private uint_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3 && w[s + 1] == 32u && w[s + 2] == 0u) {
            id = w[s]
        }
    }
    return id
}

// The id of the uint OpConstant with `value`. Operands are [result-type, result-id, value].
def private uint_const_id(words : array<uint>; uint_t : uint; value : uint) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Constant) && n >= 3 && w[s] == uint_t && w[s + 2] == value) {
            id = w[s + 1]
        }
    }
    return id
}

// The id of the first Function-storage OpVariable. Operands are [result-type, result-id, storage].
def private fn_var_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Variable) && n >= 3
            && w[s + 2] == uint(SpvStorageClass.Function) && id == 0u) {
            id = w[s + 1]
        }
    }
    return id
}

[test(description = "A write-swizzle emits one AccessChain, CompositeExtract and Store per lane, never load-shuffle-store.")]
def test_write_swizzle_per_lane(t : T?) {
    t |> run("write-swizzle: one AccessChain + CompositeExtract + Store per lane, never load-shuffle-store") <| @(t : T?) {
        var words <- clone_to_move(wsw_spv)
        // THE CRUX: the rhs of both writes is an OpCompositeConstruct and neither reads a swizzle, so
        // any OpVectorShuffle at all would be a destination that was loaded and shuffled back.
        t |> success(count_op(words, SpvOp.VectorShuffle) == 0,
            "wsw: no OpVectorShuffle -- the destination is never loaded and shuffled")
        // .xyz (3 lanes) + .xy (2 lanes) = 5 stores, each fed by one lane of the rhs
        t |> success(count_op(words, SpvOp.Store) == 5, "wsw: one OpStore per written lane (5)")
        t |> success(count_op(words, SpvOp.CompositeExtract) == 5,
            "wsw: one OpCompositeExtract per written lane (5)")
        validate(t, words, "wsw")
        delete words
    }
}

[test(description = "l.yzw = rhs writes destination lanes 1, 2 and 3 from source lanes 0, 1 and 2.")]
def test_write_swizzle_lane_offset(t : T?) {
    t |> run("write-swizzle: `l.yzw = rhs` writes destination lanes 1,2,3 from source lanes 0,1,2") <| @(t : T?) {
        var words <- clone_to_move(wsw_ofs_spv)
        let uint_t = uint_type_id(words)
        t |> success(uint_t != 0u, "wsw_ofs: uint type present")
        let lvar = fn_var_id(words)
        t |> success(lvar != 0u, "wsw_ofs: the `var l` local has a Function OpVariable")
        // an OpAccessChain is [result-type, result-id, base, index...], so `base` immediately followed
        // by the component constant is the component pointer for that lane.
        for (lane in range(1, 4)) {
            let cid = uint_const_id(words, uint_t, uint(lane))
            t |> success(cid != 0u && op_has_operand_pair(words, SpvOp.AccessChain, lvar, cid),
                "wsw_ofs: component pointer into lane {lane}")
        }
        // and lane 0 is NOT written -- an implementation that reused the source index would store there
        let c0 = uint_const_id(words, uint_t, 0u)
        t |> success(!op_has_operand_pair(words, SpvOp.AccessChain, lvar, c0),
            "wsw_ofs: lane 0 is never written (the mapping is destination-lane, not source-lane)")
        // the source lanes are 0,1,2: one extract per lane, and no shuffle
        t |> success(count_op(words, SpvOp.VectorShuffle) == 0, "wsw_ofs: no OpVectorShuffle")
        t |> success(count_op(words, SpvOp.CompositeExtract) == 3,
            "wsw_ofs: one OpCompositeExtract per written lane (3)")
        validate(t, words, "wsw_ofs")
        delete words
    }
}
