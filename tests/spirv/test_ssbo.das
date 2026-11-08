options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_builtins public
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_reflect

// Descriptor-array-of-SSBOs gate.
//
// `array<array<T>>` under @ssbo is a runtime-sized DESCRIPTOR array of storage buffers (GLSL
// `buffer Block { T d[]; } g[];`): one buffer per slot, indexed g[slot][i]. The outer runtime array
// is over the Block struct and carries NO ArrayStride -- a descriptor slot has no byte layout --
// while the inner one strides std430 like any SSBO, and the whole access lowers to ONE
// OpAccessChain (var, slot, member 0, i), exactly as glslang chains it. RuntimeDescriptorArray rides
// along. Both the slot and the element index are runtime values, so nothing folds away.
// sa_out's element type differs from the buffers' on purpose: an identical one would DEDUP its
// runtime array (and its Block struct) with theirs, and the type counts below would prove nothing.
var @ssbo @binding = 0 sa_bufs : array<array<uint>>
var @ssbo @binding = 1 sa_out : array<float>

[compute_shader(local_size_x=64, name="sa_spv")]
def sa_comp {
    let i = int(gl_GlobalInvocationID.x)
    sa_bufs[i][i + 1] = 7u
    sa_out[i] = float(sa_bufs[i + 2][i])
}

// How many instructions with `op` carry exactly `n` operands (the opcode word excluded). An
// OpAccessChain into a descriptor array of SSBOs spends 6: result type, result id, the variable,
// the slot, the member index, the element index -- two more than a plain SSBO's (var, 0, i).
def private count_op_with_operands(words : array<uint>; op : SpvOp; n : int) : int {
    var c = 0
    foreach_inst(words) $(opcode : uint; _w : array<uint>; _s, oc : int) {
        if (opcode == uint(op) && oc == n) {
            c++
        }
    }
    return c
}

[test(description = "An array of @ssbo arrays becomes a descriptor array of storage buffers with one OpAccessChain per access.")]
def test_ssbo_array(t : T?) {
    t |> run("array<array<T>> @ssbo -> descriptor array of SSBOs, one OpAccessChain per access") <| @(t : T?) {
        var words <- clone_to_move(sa_spv)
        // three runtime arrays: the uint element array inside the Block, the outer array OVER the
        // Block (the descriptor array), and the plain output SSBO's.
        t |> success(count_op(words, SpvOp.TypeRuntimeArray) == 3, "three OpTypeRuntimeArray")
        // ...but only the two ELEMENT arrays are laid out; the descriptor array has no byte stride.
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.ArrayStride)) == 2,
            "ArrayStride on the two element arrays only (none on the descriptor array)")
        t |> success(count_op_with_operands(words, SpvOp.AccessChain, 6) == 2,
            "both descriptor-array accesses are a single 4-index OpAccessChain")
        // The capability is operand 0 of OpCapability, so an id equal to the enum cannot match.
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.RuntimeDescriptorArray)),
            "OpCapability RuntimeDescriptorArray")
        // The host sizes the pool, so the binding reflects a variable descriptor count (0).
        let refl = decode_reflection(sa_spv_reflect)
        var found = false
        for (b in refl.bindings) {
            if (b.binding == 0) {
                found = true
                t |> success(b.kind == SpirvDescriptorKind.storage_buffer, "reflection: storage_buffer kind")
                t |> success(b.count == 0, "reflection: variable descriptor count")
            }
        }
        t |> success(found, "reflection: the descriptor array is reported at its binding")
        // the descriptor array raised the module to 1.5, so vulkan1.1 (SPIR-V 1.3) would reject the
        // binary on its VERSION alone -- validate against the environment it actually targets.
        let r = validate_spirv(words, "vulkan1.2")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.2): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Phase-10.1 gate: SSBO runtime arrays with vector and struct elements, laid out std430. Asserts the
// exact ArrayStride for each vector element (float2 -> 8, float3/float4 -> 16), the full std430 member
// layout of a struct element (pos@0, vel@8, life@16) + its ArrayStride (24), that the element struct is
// NOT Block-decorated (only the outer wrapper is: TypeStruct count == 2, exactly one Block), and that
// field access lowers to chained OpAccessChain/OpLoad/OpStore. Also gates the @readonly access
// qualifier -> a variable-level NonWritable decoration (not member-level: the block struct is deduped).
// Every emitted module passes spirv-val.
def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

def private has_array_stride(words : array<uint>; stride : uint) : bool =>
    op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride), stride)

def private has_member_offset(words : array<uint>; off : uint) : bool =>
    op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), off)

[test(description = "@ssbo arrays of float2, float3 and float4 get std430 ArrayStrides of 8, 16 and 16.")]
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

[test(description = "An @ssbo array of structs lowers to a std430 element struct with chained field access.")]
def test_ssbo_struct_elements(t : T?) {
    t |> run("ssbo struct elements: array<Particle> -> std430 element struct + chained field access") <| @(t : T?) {
        var words <- ssbostruct_words()
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.GLCompute),
            "ssbostruct: EntryPoint model is GLCompute")
        // two OpTypeStruct: the element struct (non-Block) + the outer Block wrapper
        t |> success(count_op(words, SpvOp.TypeStruct) == 2,
            "ssbostruct: two OpTypeStruct (element + Block wrapper), got {count_op(words, SpvOp.TypeStruct)}")
        // EXACTLY one Block decoration -- the wrapper. The element struct must NOT be Block-decorated
        // (a Block on a runtime-array element is invalid; this pins it directly, not just via spirv-val).
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block)) == 1,
            "ssbostruct: exactly one Block decoration (the wrapper, not the element struct), got {count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block))}")
        // std430 element layout: vel@8 and life@16 are unique to the element struct (the wrapper's only
        // member is at offset 0, so pos@0 alone wouldn't discriminate -- the 8/16 offsets + stride do).
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

[test(description = "@readonly marks the SSBO variable NonWritable, not the deduplicated block member.")]
def test_ssbo_readonly(t : T?) {
    t |> run("ssbo @readonly: NonWritable on the variable, not the deduped block member") <| @(t : T?) {
        var words <- rossbo_words()
        // exactly one NonWritable OpDecorate -- the @readonly source variable (the writable dest has none)
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonWritable)) == 1,
            "rossbo: exactly one NonWritable OpDecorate (the @readonly source), got {count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonWritable))}")
        // and NONE at member level -- the deduped block struct must stay writable-capable so a writable
        // SSBO sharing the element type does not inherit read-only-ness (NonWritable is member operand idx 2)
        t |> success(count_op_operand_at(words, SpvOp.MemberDecorate, 2, uint(SpvDecoration.NonWritable)) == 0,
            "rossbo: no member-level NonWritable, got {count_op_operand_at(words, SpvOp.MemberDecorate, 2, uint(SpvDecoration.NonWritable))}")
        validate(t, words, "rossbo")
        delete words
    }
}

[test(description = "@coherent decorates only its own SSBO variable and leaves a plain SSBO beside it undecorated.")]
def test_ssbo_coherent(t : T?) {
    t |> run("ssbo @coherent: Coherent on the variable alone, the plain SSBO beside it undecorated") <| @(t : T?) {
        var words <- cossbo_words()
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Coherent)) == 1,
            "cossbo: exactly one Coherent OpDecorate (the @coherent hand-off buffer), got {count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Coherent))}")
        t |> success(count_op_operand_at(words, SpvOp.MemberDecorate, 2, uint(SpvDecoration.Coherent)) == 0,
            "cossbo: no member-level Coherent, got {count_op_operand_at(words, SpvOp.MemberDecorate, 2, uint(SpvDecoration.Coherent))}")
        t |> success(has_op(words, SpvOp.AtomicIAdd), "cossbo: the counter's atomic is emitted")
        var scopes <- spv_memory_barrier_scopes(words)
        var all_device = !empty(scopes)
        for (s in scopes) {
            all_device = all_device && s == uint(SpvScope.Device)
        }
        t |> success(all_device, "cossbo: every OpMemoryBarrier (the two hand-off barriers) at Device scope, got {scopes}")
        delete scopes
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_3, "cossbo: SPIR-V header version 1.3 (the default vulkan1.1 target admits it)")
        validate(t, words, "cossbo")
        delete words
    }
}

// A single-struct SSBO block: @ssbo on a STRUCT (not array<T>) lowers to a StorageBuffer,
// Block-decorated struct wrapping the payload -- GLSL's `buffer Block { S s; }`, the shape a small
// per-dispatch control buffer needs.  It differs from the array<T> SSBO (whose block member is a
// runtime array) and from the @uniform block (Uniform storage instead of StorageBuffer).
struct SsbPayload {
    a : int
    b : int
    c : float
}

struct SsbBlock {
    payload : SsbPayload
}

var @ssbo @set = 0 @binding = 3 g_ssb : SsbBlock
var @binding = 4 @format = "r32f" @writeonly g_out : image2D

[compute_shader(local_size_x=1, name="ssbo_struct_spv"), marker(no_coverage)]
def ssbo_struct {
    let s = float(g_ssb.payload.a + g_ssb.payload.b) + g_ssb.payload.c
    imageStore(g_out, int2(0, 0), float4(s, 0.0f, 0.0f, 0.0f))
}

[test(description = "An @ssbo struct becomes a single-struct StorageBuffer Block whose member reads lower.")]
def test_ssbo_struct(t : T?) {
    t |> run("@ssbo on a struct -> single-struct StorageBuffer Block, member reads lower") <| @(t : T?) {
        var words <- clone_to_move(ssbo_struct_spv)
        let vr = validate_spirv(words)
        if (vr.ran) {
            t |> success(vr.ok, "single-struct ssbo is valid SPIR-V: {vr.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        // NOT a runtime-array SSBO: the block wraps the payload struct directly, no OpTypeRuntimeArray.
        t |> success(!has_op(words, SpvOp.TypeRuntimeArray),
            "single-struct ssbo has no runtime array (distinct from array<T> ssbo)")
        // StorageBuffer storage (distinct from a @uniform block's Uniform storage): exactly g_ssb.
        t |> success(count_op_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.StorageBuffer)) == 1,
            "the struct ssbo is a StorageBuffer OpVariable")
        // one Block-decorated struct (the wrapper); the nested payload is non-Block.
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block)) == 1,
            "the wrapper struct is Block-decorated, the nested payload is not")
        // the two-level member read (g_ssb.payload.a) lowers to an OpAccessChain in StorageBuffer.
        t |> success(has_op(words, SpvOp.AccessChain), "g_ssb.payload.a lowers to an OpAccessChain")
    }
}
