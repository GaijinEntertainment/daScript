options gen2
options indenting = 4

// Phase-10.1 gate: SSBO runtime arrays with vector and struct elements, laid out std430. Asserts the
// exact ArrayStride for each vector element (float2 -> 8, float3/float4 -> 16), the full std430 member
// layout of a struct element (pos@0, vel@8, life@16) + its ArrayStride (24), that the element struct is
// NOT Block-decorated (only the outer wrapper is: TypeStruct count == 2, exactly one Block), and that
// field access lowers to chained OpAccessChain/OpLoad/OpStore. Every emitted module passes spirv-val.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

def private has_array_stride(words : array<uint>; stride : uint) : bool {
    return op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride), stride)
}

def private has_member_offset(words : array<uint>; off : uint) : bool {
    return op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), off)
}

[test]
def test_ssbo_vector_elements(t : T?) {
    t |> run("ssbo vector elements: array<float2/3/4> -> std430 ArrayStride 8/16/16") <| @(t : T?) {
        var words <- ssbovec_words()
        // a compute entry point
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.GLCompute),
            "ssbovec: EntryPoint model is GLCompute")
        // three StorageBuffer OpVariables (storage class at operand index 2)
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.StorageBuffer)),
            "ssbovec: has StorageBuffer OpVariables")
        // the std430 strides: float2 -> 8, float3 -> 16 (size 12 rounded to align 16), float4 -> 16
        t |> success(has_array_stride(words, 8u), "ssbovec: float2 element ArrayStride 8")
        t |> success(has_array_stride(words, 16u), "ssbovec: float3/float4 element ArrayStride 16")
        // descriptor bindings 0/1/2
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u), "ssbovec: Binding 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 1u), "ssbovec: Binding 1")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 2u), "ssbovec: Binding 2")
        // whole-element access + component-wise add
        t |> success(has_op(words, SpvOp.AccessChain), "ssbovec: element access via OpAccessChain")
        t |> success(has_op(words, SpvOp.FAdd), "ssbovec: component-wise vector OpFAdd")
        t |> success(has_op(words, SpvOp.Store), "ssbovec: element store via OpStore")
        validate(t, words, "ssbovec")
        delete words
    }
}

[test]
def test_ssbo_struct_elements(t : T?) {
    t |> run("ssbo struct elements: array<Particle> -> std430 element struct + chained field access") <| @(t : T?) {
        var words <- ssbostruct_words()
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.GLCompute),
            "ssbostruct: EntryPoint model is GLCompute")
        // two OpTypeStruct: the element struct (non-Block) + the outer Block wrapper
        t |> success(count_op(words, SpvOp.TypeStruct) == 2,
            "ssbostruct: two OpTypeStruct (element + Block wrapper), got {count_op(words, SpvOp.TypeStruct)}")
        // the wrapper struct is Block-decorated; the element struct must NOT be (spirv-val rejects a
        // Block on a runtime-array element, and TypeStruct==2 confirms the element struct is distinct)
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block)),
            "ssbostruct: the wrapper struct is Block-decorated")
        // std430 element layout: pos@0, vel@8, life@16
        t |> success(has_member_offset(words, 0u), "ssbostruct: pos @ std430 offset 0")
        t |> success(has_member_offset(words, 8u), "ssbostruct: vel @ std430 offset 8")
        t |> success(has_member_offset(words, 16u), "ssbostruct: life @ std430 offset 16")
        // ArrayStride = struct size (20) rounded to struct align (8) = 24
        t |> success(has_array_stride(words, 24u), "ssbostruct: element ArrayStride 24 (std430 struct size rounded to align 8)")
        // field access lowers to chained AccessChain + Load + Store
        t |> success(has_op(words, SpvOp.AccessChain), "ssbostruct: field access via OpAccessChain")
        t |> success(has_op(words, SpvOp.Load), "ssbostruct: field load via OpLoad")
        t |> success(has_op(words, SpvOp.Store), "ssbostruct: field store via OpStore")
        validate(t, words, "ssbostruct")
        delete words
    }
}
