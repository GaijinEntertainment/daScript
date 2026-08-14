options gen2
options indenting = 4

// Subgroup (wave) operations -- the GL_KHR_shader_subgroup_* family lowered to OpGroupNonUniform*.
//
// One compute fixture reaches every op family and every subgroup builtin, so the blob carries all five
// per-family capabilities at once.  The asserts pin the three things a surface gate later depends on:
// the OPCODE each daslang name selects (including the S/U/F pick that the operand's scalar class
// drives), the GroupOperation LITERAL that separates a plain reduction (Reduce) from a clustered one
// (ClusteredReduce), and the CAPABILITY set.  The Subgroup scope is a constant <id> rather than a
// literal, so its value is left to spirv-val -- the structural oracle for scope and cluster-size rules.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [compute_shader] annotation
require spirv/spirv_builtins public // subgroup ops + gl_Subgroup* builtins
require spirv/spirv_dis             // has_op / op_has_operand / op_has_operand_at
require spirv/spirv_grammar         // SpvOp / SpvCapability / SpvGroupOperation

// The GroupOperation is operand 3 of an arithmetic reduction: result-type(0), result-id(1),
// scope(2), GroupOperation(3), value(4).
let GROUP_OP_OPERAND = 3

// The capability is operand 0 of OpCapability; positional, so an id equal to the enum can't match.
let CAP_OPERAND = 0

var @ssbo @binding = 0 sg_in : array<float>
var @ssbo @binding = 1 sg_out : array<float>
var @ssbo @binding = 2 sg_iout : array<int>

[compute_shader(local_size_x=64, name="sg_all_ops_spv"), marker(no_coverage)]
def subgroup_all_ops {
    let i = gl_GlobalInvocationID.x
    let v = sg_in[i]
    let iv = int(v)
    let uv = uint(iv)

    // vote -> OpGroupNonUniformAll / OpGroupNonUniformAny (GroupNonUniformVote)
    let uniform_sign = subgroupAll(v > 0.0f)
    let any_zero = subgroupAny(v == 0.0f)

    // ballot -> OpGroupNonUniformBroadcastFirst / OpGroupNonUniformBroadcast (GroupNonUniformBallot)
    let first = subgroupBroadcastFirst(float3(v, v, v))
    let lane0 = subgroupBroadcast(v, 0u)

    // shuffle -> OpGroupNonUniformShuffle / OpGroupNonUniformShuffleXor (GroupNonUniformShuffle)
    let shuffled = subgroupShuffle(float3(v, v, v), gl_SubgroupInvocationID)
    let butterfly = subgroupShuffleXor(v, 1u)

    // elect -> OpGroupNonUniformElect, whose family IS the base capability (single-lane writeback guard)
    let leader = subgroupElect()

    // arithmetic reductions -> Reduce, with the S/U/F opcode picked by the operand's scalar class
    let fsum = subgroupAdd(float3(v, v, v))
    let isum = subgroupAdd(iv)
    let fmin = subgroupMin(v)
    let fmax = subgroupMax(v)
    let imin = subgroupMin(iv)
    let imax = subgroupMax(iv)
    let umin = subgroupMin(uv)
    let umax = subgroupMax(uv)

    // clustered -> ClusteredReduce + a constant power-of-two ClusterSize (GroupNonUniformClustered)
    let quad_sum = subgroupClusteredAdd(float4(v, v, v, v), 4u)

    // the RHS is wrapped: a continuation line that begins with `+` would otherwise parse as a
    // separate unary-plus statement and be silently dropped
    sg_out[i] = ((uniform_sign ? 1.0f : 0.0f) + (any_zero ? 2.0f : 0.0f)
        + first.x + lane0 + shuffled.y + butterfly + (leader ? 8.0f : 0.0f)
        + fsum.z + fmin + fmax + quad_sum.w
        + float(gl_SubgroupSize) + float(gl_NumSubgroups) + float(gl_SubgroupID))
    sg_iout[i] = isum + imin + imax + int(umin) + int(umax)
}

// A subgroup op carries no execution-model restriction, and gl_SubgroupInvocationID / gl_SubgroupSize
// are legal in every stage -- unlike gl_SubgroupID / gl_NumSubgroups, which name a place in a workgroup
// (that pair is gated, and _fc_subgroup_id_stage holds the rejection).  A fragment shader is the cheapest
// witness for both halves.
var @out @location = 0 sg_frag_color : float4

[fragment_shader(name="subgroup_frag_spv"), marker(no_coverage)]
def subgroup_in_fragment {
    let lane = float(gl_SubgroupInvocationID) / float(gl_SubgroupSize)
    let all_lit = subgroupAll(lane > 0.0f)
    sg_frag_color = float4(lane, all_lit ? 1.0f : 0.0f, 0.0f, 1.0f)
}

// OpGroupNonUniformBroadcast's Id operand had to be a CONSTANT before SPIR-V 1.5, which relaxed it to
// dynamically uniform.  A dynamic lane index is the natural spelling (a loop counter, an invocation id),
// so the emitter raises the module version for it rather than rejecting it -- and leaves a constant Id
// on the default version.  The bump has to settle before the entry-point interface is built, since the
// 1.4+ rule lists every global; that ordering is exactly what spirv-val checks here.
var @ssbo @binding = 0 dyn_in : array<float>
var @ssbo @binding = 1 dyn_out : array<float>

[compute_shader(local_size_x=64, name="subgroup_dyn_spv"), marker(no_coverage)]
def subgroup_dynamic_broadcast {
    let i = gl_GlobalInvocationID.x
    dyn_out[i] = subgroupBroadcast(dyn_in[i], gl_SubgroupInvocationID)
}

// words[1] is the SPIR-V version word of the module header (magic, version, generator, bound, schema).
let VERSION_WORD = 1

def private has_group_op(words : array<uint>; op : SpvOp; gop : SpvGroupOperation) : bool {
    return op_has_operand_at(words, op, GROUP_OP_OPERAND, uint(gop))
}

def private has_cap(words : array<uint>; cap : SpvCapability) : bool {
    return op_has_operand_at(words, SpvOp.Capability, CAP_OPERAND, uint(cap))
}

def private has_builtin(words : array<uint>; bi : SpvBuiltIn) : bool {
    return op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(bi))
}

[test]
def test_subgroup_ops(t : T?) { // nolint:STYLE038 - flat list of opcode/capability assertions
    t |> run("subgroup ops lower to OpGroupNonUniform* with the right opcode, group operation and caps") <| @(t : T?) {
        var words <- clone_to_move(sg_all_ops_spv)

        // ----- vote -----
        t |> success(has_op(words, SpvOp.GroupNonUniformAll), "subgroupAll -> OpGroupNonUniformAll")
        t |> success(has_op(words, SpvOp.GroupNonUniformAny), "subgroupAny -> OpGroupNonUniformAny")

        // ----- ballot -----
        t |> success(has_op(words, SpvOp.GroupNonUniformBroadcastFirst),
            "subgroupBroadcastFirst -> OpGroupNonUniformBroadcastFirst")
        t |> success(has_op(words, SpvOp.GroupNonUniformBroadcast),
            "subgroupBroadcast -> OpGroupNonUniformBroadcast")

        // ----- shuffle -----
        t |> success(has_op(words, SpvOp.GroupNonUniformShuffle), "subgroupShuffle -> OpGroupNonUniformShuffle")
        t |> success(has_op(words, SpvOp.GroupNonUniformShuffleXor),
            "subgroupShuffleXor -> OpGroupNonUniformShuffleXor")

        // ----- elect -----
        t |> success(has_op(words, SpvOp.GroupNonUniformElect), "subgroupElect -> OpGroupNonUniformElect")

        // ----- arithmetic: the scalar class picks F / S / U -----
        t |> success(has_op(words, SpvOp.GroupNonUniformFAdd), "subgroupAdd(float3) -> OpGroupNonUniformFAdd")
        t |> success(has_op(words, SpvOp.GroupNonUniformIAdd), "subgroupAdd(int) -> OpGroupNonUniformIAdd")
        t |> success(has_op(words, SpvOp.GroupNonUniformFMin), "subgroupMin(float) -> OpGroupNonUniformFMin")
        t |> success(has_op(words, SpvOp.GroupNonUniformFMax), "subgroupMax(float) -> OpGroupNonUniformFMax")
        t |> success(has_op(words, SpvOp.GroupNonUniformSMin), "subgroupMin(int) -> OpGroupNonUniformSMin")
        t |> success(has_op(words, SpvOp.GroupNonUniformSMax), "subgroupMax(int) -> OpGroupNonUniformSMax")
        t |> success(has_op(words, SpvOp.GroupNonUniformUMin), "subgroupMin(uint) -> OpGroupNonUniformUMin")
        t |> success(has_op(words, SpvOp.GroupNonUniformUMax), "subgroupMax(uint) -> OpGroupNonUniformUMax")

        // ----- the GroupOperation literal separates a plain reduction from a clustered one -----
        t |> success(has_group_op(words, SpvOp.GroupNonUniformIAdd, SpvGroupOperation.Reduce),
            "subgroupAdd(int) carries GroupOperation Reduce")
        t |> success(has_group_op(words, SpvOp.GroupNonUniformFMin, SpvGroupOperation.Reduce),
            "subgroupMin(float) carries GroupOperation Reduce")
        t |> success(has_group_op(words, SpvOp.GroupNonUniformFAdd, SpvGroupOperation.ClusteredReduce),
            "subgroupClusteredAdd carries GroupOperation ClusteredReduce")
        // both FAdd flavours are present: the plain reduction AND the clustered one
        t |> success(has_group_op(words, SpvOp.GroupNonUniformFAdd, SpvGroupOperation.Reduce),
            "subgroupAdd(float3) carries GroupOperation Reduce")

        // ----- capabilities: the base one plus every family the fixture reaches -----
        t |> success(has_cap(words, SpvCapability.GroupNonUniform), "OpCapability GroupNonUniform")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformVote), "OpCapability GroupNonUniformVote")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformBallot), "OpCapability GroupNonUniformBallot")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformArithmetic), "OpCapability GroupNonUniformArithmetic")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformShuffle), "OpCapability GroupNonUniformShuffle")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformClustered), "OpCapability GroupNonUniformClustered")
        // the base capability is emitted once, not once per op family
        t |> success(count_op_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.GroupNonUniform)) == 1,
            "GroupNonUniform capability is emitted exactly once")

        // ----- the four subgroup builtins -----
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupSize), "gl_SubgroupSize -> BuiltIn SubgroupSize")
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupLocalInvocationId),
            "gl_SubgroupInvocationID -> BuiltIn SubgroupLocalInvocationId")
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupId), "gl_SubgroupID -> BuiltIn SubgroupId")
        t |> success(has_builtin(words, SpvBuiltIn.NumSubgroups), "gl_NumSubgroups -> BuiltIn NumSubgroups")

        // a CONSTANT broadcast Id needs no version bump
        t |> success(words[VERSION_WORD] == SPV_VERSION_1_3, "constant broadcast Id stays on SPIR-V 1.3")

        // spirv-val is the oracle for the Subgroup scope <id> and the constant power-of-two ClusterSize
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("a dynamic broadcast Id raises the module to SPIR-V 1.5, interface rule included") <| @(t : T?) {
        var words <- clone_to_move(subgroup_dyn_spv)
        t |> success(words[VERSION_WORD] == SPV_VERSION_1_5, "dynamic broadcast Id raises the module to SPIR-V 1.5")
        t |> success(has_op(words, SpvOp.GroupNonUniformBroadcast), "dynamic: OpGroupNonUniformBroadcast emitted")
        // the 1.4+ interface rule: every referenced global is listed, the two SSBOs included. The
        // version settled before the entry point was built, so spirv-val finds no unlisted interface.
        let r = validate_spirv(words, "vulkan1.2")
        if (r.ran) {
            t |> success(r.ok, "dynamic: spirv-val (vulkan1.2): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("subgroup ops and the invocation builtins are legal outside a workgroup stage") <| @(t : T?) {
        var words <- clone_to_move(subgroup_frag_spv)
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "fragment: EntryPoint model is Fragment")
        t |> success(has_op(words, SpvOp.GroupNonUniformAll), "fragment: subgroupAll lowers (no stage gate)")
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupLocalInvocationId),
            "fragment: gl_SubgroupInvocationID is legal outside a workgroup stage")
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupSize),
            "fragment: gl_SubgroupSize is legal outside a workgroup stage")
        t |> success(has_cap(words, SpvCapability.GroupNonUniform), "fragment: OpCapability GroupNonUniform")
        // an integer fragment Input cannot be interpolated: both uint builtins must be Flat
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Flat)) == 2,
            "fragment: both uint subgroup builtins are Flat-decorated")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "fragment: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
