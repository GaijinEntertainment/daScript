options gen2
options indenting = 4

// The struct-value-parameter spill (`sparam` below), and the direct-[spirv_decode] fixtures whose calls need it (`spdirect`, `spelem`, both in _spirv_common).

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_builtins public
require spirv/spirv_dis
require spirv/spirv_grammar

struct SpArr {
    arr : float[4]
    k : float
}

var @ssbo @binding = 0 sp_data : array<float>

def private sp_pick(c : SpArr; i : int) : float {
    return c.arr[i] * c.k
}

[compute_shader(local_size_x=64, name="sparam_spv"), marker(no_coverage)]
def sparam {
    let i = int(gl_GlobalInvocationID.x)
    var s : SpArr
    s.arr[0] = sp_data[i]
    s.arr[1] = sp_data[i] + 1.0f
    s.arr[2] = sp_data[i] + 2.0f
    s.arr[3] = sp_data[i] + 3.0f
    s.k = 0.5f
    sp_data[i] = sp_pick(s, i & 3)
}

def private validate(t : T?; words : array<uint>; lbl : string; target_env : string = "vulkan1.1") {
    let r = validate_spirv(words, target_env)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_struct_param(t : T?) {
    t |> run("struct value param: spilled to a Function local, its array member indexed at run time") <| @(t : T?) {
        var words <- clone_to_move(sparam_spv)
        //! entry + sp_pick = 2 OpFunctions, one call; sp_pick(c, i) = 2 by-value parameters
        t |> equal(count_op(words, SpvOp.Function), 2)
        t |> equal(count_op(words, SpvOp.FunctionCall), 1)
        t |> equal(count_op(words, SpvOp.FunctionParameter), 2)
        //! OpVariables: gid + sp_data + the kernel's `var s` + the spill of `c` - 3 without the spill
        t |> equal(count_op(words, SpvOp.Variable), 4)
        t |> success(has_op(words, SpvOp.Store), "sparam: the struct parameter is stored into its Function local")
        t |> success(has_op(words, SpvOp.AccessChain), "sparam: the array member is access-chained off the spill")
        //! SpArr + the SSBO Block = 2 OpTypeStruct; a spill is no interface block, so Offset stays the only member decoration
        t |> equal(count_op(words, SpvOp.TypeStruct), 2)
        t |> equal(count_op(words, SpvOp.MemberDecorate), 1)
        t |> success(words[1] < SPV_VERSION_1_4, "sparam: a spill alone leaves the module at the 1.3 floor, got {words[1]}")
        validate(t, words, "sparam")
        delete words
    }
    t |> run("a [spirv_decode] method called directly is a plain OpFunctionCall in a module with no coopmat capability") <| @(t : T?) {
        var words <- spdirect_words()
        //! run + decode = 2 OpFunctions, called once; decode's (blk, bc, cib) = 3 parameters
        t |> equal(count_op(words, SpvOp.Function), 2)
        t |> equal(count_op(words, SpvOp.FunctionCall), 1)
        t |> equal(count_op(words, SpvOp.FunctionParameter), 3)
        t |> success(!op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixTensorAddressingNV)),
            "spdirect: no CooperativeMatrixTensorAddressingNV capability")
        t |> success(!op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "spdirect: no CooperativeMatrixKHR capability")
        t |> success(!op_has_operand(words, SpvOp.Capability, uint(SpvCapability.PhysicalStorageBufferAddresses)),
            "spdirect: no PhysicalStorageBufferAddresses capability (the block travels by value, not by device address)")
        t |> success(has_op(words, SpvOp.Store), "spdirect: the block parameter is stored into its Function local")
        t |> success(has_op(words, SpvOp.CopyLogical), "spdirect: the laid-out block value is OpCopyLogical'd to the parameter's plain struct")
        t |> success(words[1] >= SPV_VERSION_1_4, "spdirect: the module version is at least 1.4 (OpCopyLogical's floor), got {words[1]}")
        validate(t, words, "spdirect", "vulkan1.2")
        delete words
    }
    t |> run("a [spirv_decode] method called directly on the plane element takes the element's index and chains through the plane") <| @(t : T?) {
        var words <- spelem_words()
        //! run + decode = 2 OpFunctions, called once; decode's (blk, bc, cib) = 3 parameters, blk a uint index
        t |> equal(count_op(words, SpvOp.Function), 2)
        t |> equal(count_op(words, SpvOp.FunctionCall), 1)
        t |> equal(count_op(words, SpvOp.FunctionParameter), 3)
        //! OpVariables: gid + wq/ws/y + pa = 5, no sixth - the element form spills nothing
        t |> equal(count_op(words, SpvOp.Variable), 5)
        t |> success(!has_op(words, SpvOp.CopyLogical), "spelem: no OpCopyLogical - the element is never loaded as a value")
        t |> success(words[1] < SPV_VERSION_1_4, "spelem: the module version stays below 1.4, got {words[1]}")
        //! >= 3: the callee's entry chain into the plane plus its member reads through it
        t |> success(count_op(words, SpvOp.AccessChain) >= 3, "spelem: the callee chains through the plane ({count_op(words, SpvOp.AccessChain)} access chains)")
        t |> success(!op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixTensorAddressingNV)),
            "spelem: no CooperativeMatrixTensorAddressingNV capability")
        validate(t, words, "spelem")
        delete words
    }
    t |> run("[spirv_decode] called on a plane element with an int index: the index bitcasts to uint") <| @(t : T?) {
        var words <- spelemi_words()
        var uwords <- spelem_words()
        //! the same module as spelem plus two OpBitcasts: the int() of the lane id, and the index back to uint at the call
        t |> equal(count_op(words, SpvOp.Function), 2)
        t |> equal(count_op(words, SpvOp.FunctionParameter), 3)
        t |> equal(count_op(words, SpvOp.Variable), 5)
        t |> equal(count_op(words, SpvOp.Bitcast), count_op(uwords, SpvOp.Bitcast) + 2)
        t |> success(!has_op(words, SpvOp.CopyLogical), "spelemi: no OpCopyLogical - the element is never loaded as a value")
        t |> success(words[1] < SPV_VERSION_1_4, "spelemi: the module version stays below 1.4, got {words[1]}")
        validate(t, words, "spelemi")
        delete words
        delete uwords
    }
}
