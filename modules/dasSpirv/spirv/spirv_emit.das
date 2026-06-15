options gen2

// daslang AST -> SPIR-V codegen via the SpirvEmit AstVisitor (modeled on llvm_jit's SSA backend).
// SPIR-V's value/pointer model (SSA + OpAccessChain) is threaded through per-node side-maps: e2id for
// rvalue ids, e2ptr/e2pty for lvalue pointers; ExprRef2Value (still present at the annotation's patch()
// pre-fold stage) is the explicit load marker. generate_spirv is the [macro_function] entry, called by
// both the shader annotation's patch and the unit tests, so opcode assertions hit the real codegen path.

module spirv_emit shared public

require spirv_builder
require spirv_types
require spirv_grammar
require spirv_reflect
require daslib/ast
require daslib/rtti
require daslib/ast_boost

// The shader stage drives the entry-point execution model + execution mode and which builtins
// are legal. Vertex needs no execution mode; Fragment needs OriginUpperLeft; Compute needs LocalSize.
enum public ShaderStage {
    Compute
    Vertex
    Fragment
}

// ===== per-global classification =====
struct GlobalInfo {
    var_id     : uint
    storage    : SpvStorageClass
    value_type : uint               // SPIR-V type-id of the whole variable's value
    is_ssbo    : bool               // wrapped in a Block-decorated struct (member 0 = the array)
    is_block   : bool               // a Block-decorated struct (ssbo OR ubo); member access -> OpAccessChain by field index
    is_sampler : bool               // a combined image sampler (UniformConstant OpTypeSampledImage); used via texture()
}

// A memory-backed local: an OpVariable in Function storage. Mutable locals (`var x`) and loop
// induction variables get one; reads become OpLoad, writes OpStore. The driver mem2reg's them,
// so we never hand-roll OpPhi. Immutable value-lets (`let x`) stay pure SSA ids (ctx.locals).
struct LocalVar {
    ptr_id     : uint    // OpVariable result-id (Function storage)
    value_type : uint    // SPIR-V type-id of the pointee
}

struct EmitCtx {
    globals       : table<uint64; GlobalInfo>   // intptr(Variable) -> classification
    locals        : table<uint64; uint>         // intptr(Variable) -> SSA result-id (value lets)
    local_vars    : table<uint64; LocalVar>     // intptr(Variable) -> memory-backed local (var / loop var)
    interface_ids : array<uint>                 // EntryPoint interface (Input/Output vars, SPIR-V <= 1.3)
    loop_stack    : array<LoopTargets>          // innermost loop's {merge, continue} for break/continue
    terminated    : bool                        // current block already has a terminator
    glsl_ext      : uint                        // OpExtInstImport "GLSL.std.450" set id (0 = not imported yet)
    stage         : ShaderStage                 // the shader stage (texture() implicit-LOD sampling is fragment-only)
}

// break -> branch to `merge`, continue -> branch to `cont`. Pushed per active loop.
struct LoopTargets {
    merge : uint
    cont  : uint
}

// ===== per-control-flow-node label ids (visitor side-maps; allocated to match the hand-walker's
// linear id order so the emitted blob stays byte-identical) =====
struct IteIds {
    merge   : uint
    then_l  : uint
    else_l  : uint   // == merge when there is no else branch (no separate else label emitted)
}

struct LoopIds {
    header : uint
    cond_l : uint
    body_l : uint
    cont_l : uint
    merge  : uint
}

def finalize(var c : EmitCtx) {
    delete c.globals
    delete c.locals
    delete c.local_vars
    delete c.interface_ids
    delete c.loop_stack
}

// ===== builtin name -> SpvBuiltIn + storage class + the stage it's legal in =====
// Most builtins are read-only Inputs; gl_Position is the one written Output. The storage class
// decides Input vs Output pointer type; `req` is the single stage the builtin is valid in, so
// classify_global can reject a cross-stage reference (e.g. gl_FragCoord in a vertex shader) with a
// clean error instead of emitting a blob that only spirv-val would catch.
def private builtin_info(name : string) : tuple<ok : bool; bi : SpvBuiltIn; storage : SpvStorageClass; req : ShaderStage> {
    // compute
    if (name == "gl_GlobalInvocationID") return (ok = true, bi = SpvBuiltIn.GlobalInvocationId, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_LocalInvocationID") return (ok = true, bi = SpvBuiltIn.LocalInvocationId, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_WorkGroupID") return (ok = true, bi = SpvBuiltIn.WorkgroupId, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_NumWorkGroups") return (ok = true, bi = SpvBuiltIn.NumWorkgroups, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_LocalInvocationIndex") return (ok = true, bi = SpvBuiltIn.LocalInvocationIndex, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    // vertex
    if (name == "gl_Position") return (ok = true, bi = SpvBuiltIn.Position, storage = SpvStorageClass.Output, req = ShaderStage.Vertex)
    if (name == "gl_VertexIndex") return (ok = true, bi = SpvBuiltIn.VertexIndex, storage = SpvStorageClass.Input, req = ShaderStage.Vertex)
    if (name == "gl_InstanceIndex") return (ok = true, bi = SpvBuiltIn.InstanceIndex, storage = SpvStorageClass.Input, req = ShaderStage.Vertex)
    // fragment
    if (name == "gl_FragCoord") return (ok = true, bi = SpvBuiltIn.FragCoord, storage = SpvStorageClass.Input, req = ShaderStage.Fragment)
    return (ok = false, bi = SpvBuiltIn.Position, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
}

def private stage_name(stage : ShaderStage) : string {
    if (stage == ShaderStage.Vertex) return "vertex"
    if (stage == ShaderStage.Fragment) return "fragment"
    return "compute"
}

// ===== std140 layout for a UBO interface block =====
// std140 base alignment: scalar -> 4, vec2 -> 8, vec3/vec4 -> 16. The base *size* of a vec3 is 12
// (only its alignment is 16), so a scalar can pack into the trailing 4 bytes after a vec3.
def private std140_align(bt : Type) : int {
    let cc = component_count(bt)
    if (cc == 1) return 4
    if (cc == 2) return 8
    return 16
}

def private std140_size(bt : Type) : int {
    return component_count(bt) * 4
}

def private round_up(off, a : int) : int {
    return ((off + a - 1) / a) * a
}

// Build a Block-decorated interface struct from a daslang struct's fields, laid out std140. Shared
// by @uniform and @push_constant blocks. 32-bit scalar/vector/matrix members are supported; nested
// structs and arrays are not yet. Returns the struct type-id; the caller wraps it in the right
// storage-class pointer + descriptor decorations.
def private build_block_struct(var m : SpirvModule; sname : string; st : Structure?; var errors : array<string>) : tuple<ok : bool; type_id : uint; size : int> {
    if (st == null || empty(st.fields)) {
        errors |> push("interface block '{sname}' must be a struct with at least one field")
        return (ok = false, type_id = 0u, size = 0)
    }
    var members : array<uint>
    var offsets : array<int>
    var mat_strides : array<int>
    var off = 0
    for (fld in st.fields) {
        let mi = matrix_info(fld._type)
        if (mi.ok) {
            // std140 matrix: ColMajor, each column padded to vec4 (MatrixStride 16), align 16,
            // size = 16 * cols. (daslang float3x3 stores packed columns, so its bytes need
            // repacking host-side before upload; float4x4 columns are already vec4 -> direct.)
            off = round_up(off, 16)
            members |> push(emit_type(m, fld._type))
            offsets |> push(off)
            mat_strides |> push(16)
            off += 16 * mi.cols
            continue
        }
        let fbt = fld._type.baseType
        // only 32-bit int/uint/float scalars + their vectors. scalar_class rejects bool (and any
        // non-numeric): OpTypeBool has no physical size in a std140/std430 interface block, so a
        // bool member would emit an invalid layout — reject it fail-closed rather than emit it.
        if (scalar_class(fbt) < 0) {
            errors |> push("interface block '{sname}' field '{fld.name}': type {fbt} is not supported (only 32-bit int/uint/float scalars, their vectors, and matrices; bool has no physical interface-block layout in Vulkan)")
            delete members
            delete offsets
            delete mat_strides
            return (ok = false, type_id = 0u, size = 0)
        }
        off = round_up(off, std140_align(fbt))
        members |> push(emit_type(m, fld._type))
        offsets |> push(off)
        mat_strides |> push(0)
        off += std140_size(fbt)
    }
    let id = type_struct_block(m, members, offsets, mat_strides)
    delete members
    delete offsets
    delete mat_strides
    return (ok = true, type_id = id, size = off)
}

// The single SpirvStageFlags bit for one shader stage. A shader module is one stage, so every
// binding/range it reflects carries this single bit; the host ORs across stages when merging.
def private stage_flag(stage : ShaderStage) : SpirvStageFlags {
    if (stage == ShaderStage.Vertex) return SpirvStageFlags.vertex
    if (stage == ShaderStage.Fragment) return SpirvStageFlags.fragment
    return SpirvStageFlags.compute
}

// ===== combined image sampler detection =====
// sampler globals are recognized by their opaque marker struct name (sampler2D, ...). Returns the
// SPIR-V image dimensionality so classify_global can build the OpTypeImage / OpTypeSampledImage.
def private sampler_info(t : TypeDecl?) : tuple<ok : bool; dim : SpvDim> {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) return (ok = false, dim = SpvDim._2D)
    if ("{t.structType.name}" == "sampler2D") return (ok = true, dim = SpvDim._2D)
    return (ok = false, dim = SpvDim._2D)
}

// ===== global variable classification + emission =====
def private classify_global(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; stage : ShaderStage; var reflection : SpirvReflection; var errors : array<string>) {
    let vname = "{v.name}"
    let bi = builtin_info(vname)
    if (bi.ok) {
        if (bi.req != stage) {
            errors |> push("builtin '{vname}' is only available in the {stage_name(bi.req)} stage, not {stage_name(stage)}")
            return
        }
        let vt = emit_type(m, v._type)
        let pt = type_pointer(m, bi.storage, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(bi.storage))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.BuiltIn), uint(bi.bi))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = bi.storage, value_type = vt, is_ssbo = false))
        ctx.interface_ids |> push(vid)   // Input + Output builtins both list in the entry interface
        return
    }
    // stage I/O: @in / @out user variables carry an explicit @location. They become Input/Output
    // OpVariables with a Location decoration and list in the entry-point interface.
    let is_in = v.annotation |> find_arg("in") is tBool
    let is_out = v.annotation |> find_arg("out") is tBool
    if (is_in || is_out) {
        if (stage == ShaderStage.Compute) {
            errors |> push("stage variable '{vname}': @in/@out Location I/O is not valid in a compute shader")
            return
        }
        if (is_in && is_out) {
            errors |> push("stage variable '{vname}' cannot be both @in and @out")
            return
        }
        let loc = v.annotation |> find_arg("location") ?as tInt ?? -1
        if (loc < 0) {
            errors |> push("stage variable '{vname}' must specify a non-negative @location (e.g. @in @location = 0)")
            return
        }
        let storage = is_in ? SpvStorageClass.Input : SpvStorageClass.Output
        let vt = emit_type(m, v._type)
        let pt = type_pointer(m, storage, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(storage))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Location), uint(loc))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = storage, value_type = vt, is_ssbo = false))
        ctx.interface_ids |> push(vid)
        return
    }
    // combined image sampler: a global of an opaque sampler type (sampler2D) -> UniformConstant
    // OpTypeSampledImage + DescriptorSet/Binding. Used only via texture(); not in the entry interface.
    let si = sampler_info(v._type)
    if (si.ok) {
        let img = type_image(m, type_float(m, 32u), si.dim)
        let simg = type_sampled_image(m, img)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, simg)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        if (set < 0 || binding < 0) {
            errors |> push("sampler '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant, value_type = simg, is_sampler = true))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.combined_image_sampler, count = 1, stages = stage_flag(stage)))
        return
    }
    if (v.annotation |> find_arg("ssbo") is tBool) {
        if (v._type.baseType != Type.tArray || v._type.firstType == null) {
            errors |> push("ssbo '{vname}' must be an array<T>")
            return
        }
        // std430 array stride == element sizeOf only for scalars; vec3/struct elements need
        // larger aligned strides we don't compute yet. Reject non-scalars rather than emit a
        // wrong/non-portable layout (Phase 4 adds real std430 layout).
        let et = v._type.firstType.baseType
        if (et != Type.tInt && et != Type.tUInt && et != Type.tFloat) {
            errors |> push("ssbo '{vname}' element must be a 32-bit scalar (int/uint/float) for now — std430 vector/struct layout is not yet implemented")
            return
        }
        let elem_t = emit_type(m, v._type.firstType)
        let stride = v._type.firstType.sizeOf
        let rta = type_runtime_array(m, elem_t, stride)
        var members <- [rta]
        var offsets <- [0]
        let st = type_struct_block(m, members, offsets)
        delete members
        delete offsets
        let pt = type_pointer(m, SpvStorageClass.StorageBuffer, st)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.StorageBuffer))
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        if (set < 0 || binding < 0) {
            errors |> push("ssbo '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.StorageBuffer, value_type = st, is_ssbo = true, is_block = true))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.storage_buffer, count = 1, stages = stage_flag(stage)))
        return
    }
    // uniform buffer: @uniform struct -> Uniform storage + Block-decorated struct (std140) +
    // DescriptorSet/Binding. Member reads go through ExprField -> OpAccessChain by field index.
    if (v.annotation |> find_arg("uniform") is tBool) {
        if (v._type.baseType != Type.tStructure || v._type.structType == null) {
            errors |> push("uniform '{vname}' must be a struct")
            return
        }
        let bs = build_block_struct(m, vname, v._type.structType, errors)
        if (!bs.ok) return
        let pt = type_pointer(m, SpvStorageClass.Uniform, bs.type_id)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.Uniform))
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        if (set < 0 || binding < 0) {
            errors |> push("uniform '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.Uniform, value_type = bs.type_id, is_ssbo = false, is_block = true))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.uniform_buffer, count = 1, stages = stage_flag(stage)))
        return
    }
    // push constants: @push_constant struct -> PushConstant storage + Block-decorated struct. Same
    // std140-style member layout + ExprField access as a UBO, but NO descriptor set/binding (push
    // constants are not descriptors) and not listed in the entry interface (SPIR-V <= 1.3).
    if (v.annotation |> find_arg("push_constant") is tBool) {
        if (v._type.baseType != Type.tStructure || v._type.structType == null) {
            errors |> push("push_constant '{vname}' must be a struct")
            return
        }
        let bs = build_block_struct(m, vname, v._type.structType, errors)
        if (!bs.ok) return
        let pt = type_pointer(m, SpvStorageClass.PushConstant, bs.type_id)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.PushConstant))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.PushConstant, value_type = bs.type_id, is_ssbo = false, is_block = true))
        reflection.push_constants |> push(SpirvPushConstantRange(offset = 0, size = bs.size, stages = stage_flag(stage)))
        return
    }
    errors |> push("unsupported global '{vname}' (expected gl_* builtin, @ssbo, @uniform, @push_constant, or a sampler type e.g. sampler2D)")
}

// ===== member index of a struct field by name =====
// ExprField.fieldIndex is still -1 at the annotation's patch (pre-fold) stage, so resolve the
// member position by name from the (fully-known) struct type instead.
def private field_index_by_name(st : Structure?; name : string) : int {
    if (st == null) return -1
    var i = 0
    for (fld in st.fields) {
        if ("{fld.name}" == name) return i
        i++
    }
    return -1
}

// ===== emit a global ExprVar's Variable (or null) =====
def private global_var_of(e : ExpressionPtr) : Variable const? {
    let ev = e ?as ExprVar
    if (ev == null || ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block) return null
    return ev.variable
}

// ===== arithmetic opcode selection =====
// 0 = signed int, 1 = unsigned int, 2 = float, -1 = other. Vectors classify by their lane kind
// (IAdd/FAdd/… operate component-wise on vectors with the same opcode as the scalar).
def private scalar_class(bt : Type) : int {
    if (bt == Type.tInt || bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4) return 0
    if (bt == Type.tUInt || bt == Type.tUInt2 || bt == Type.tUInt3 || bt == Type.tUInt4) return 1
    if (bt == Type.tFloat || bt == Type.tFloat2 || bt == Type.tFloat3 || bt == Type.tFloat4) return 2
    return -1
}

// daslang binary op -> SPIR-V opcode. Add/Sub/Mul are two's-complement sign-agnostic (IAdd for
// both int kinds). Div/Rem split by kind. daslang `/` truncates toward zero and `%` takes the
// dividend's sign (probed) -> SDiv/SRem (not the floored SMod); unsigned -> UDiv/UMod; float ->
// FDiv/FRem (fmod, dividend's sign).
def private binop_code(op : string; cls : int) : tuple<ok : bool; code : SpvOp> {
    if (cls < 0) return (ok = false, code = SpvOp.Nop)   // unsupported scalar class: don't masquerade as int
    if (op == "+") return (ok = true, code = cls == 2 ? SpvOp.FAdd : SpvOp.IAdd)
    if (op == "-") return (ok = true, code = cls == 2 ? SpvOp.FSub : SpvOp.ISub)
    if (op == "*") return (ok = true, code = cls == 2 ? SpvOp.FMul : SpvOp.IMul)
    if (op == "/") {
        if (cls == 2) return (ok = true, code = SpvOp.FDiv)
        if (cls == 0) return (ok = true, code = SpvOp.SDiv)
        if (cls == 1) return (ok = true, code = SpvOp.UDiv)
    }
    if (op == "%") {
        if (cls == 2) return (ok = true, code = SpvOp.FRem)
        if (cls == 0) return (ok = true, code = SpvOp.SRem)
        if (cls == 1) return (ok = true, code = SpvOp.UMod)
    }
    return (ok = false, code = SpvOp.Nop)
}

// daslang unary op -> SPIR-V opcode. `-` negates (FNegate/SNegate); `~` is bitwise Not (int/uint).
def private unop_code(op : string; cls : int) : tuple<ok : bool; code : SpvOp> {
    if (cls < 0) return (ok = false, code = SpvOp.Nop)   // unsupported scalar class
    if (op == "-") return (ok = true, code = cls == 2 ? SpvOp.FNegate : SpvOp.SNegate)
    if (op == "~" && (cls == 0 || cls == 1)) return (ok = true, code = SpvOp.Not)   // bitwise: int/uint only
    return (ok = false, code = SpvOp.Nop)
}

// daslang comparison op -> SPIR-V opcode. Result is always bool. Equality (==/!=) is two's-complement
// sign-agnostic on integers (IEqual/INotEqual). Ordering splits signed/unsigned/float. `F*` uses the
// ordered (non-NaN) forms — matching daslang's total order on finite shader values.
def private cmp_code(op : string; cls : int) : tuple<ok : bool; code : SpvOp> {
    let is_int = cls == 0 || cls == 1
    if (op == "==") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdEqual)
        if (is_int) return (ok = true, code = SpvOp.IEqual)
    }
    if (op == "!=") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdNotEqual)
        if (is_int) return (ok = true, code = SpvOp.INotEqual)
    }
    if (op == "<") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdLessThan)
        if (cls == 0) return (ok = true, code = SpvOp.SLessThan)
        if (cls == 1) return (ok = true, code = SpvOp.ULessThan)
    }
    if (op == ">") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdGreaterThan)
        if (cls == 0) return (ok = true, code = SpvOp.SGreaterThan)
        if (cls == 1) return (ok = true, code = SpvOp.UGreaterThan)
    }
    if (op == "<=") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdLessThanEqual)
        if (cls == 0) return (ok = true, code = SpvOp.SLessThanEqual)
        if (cls == 1) return (ok = true, code = SpvOp.ULessThanEqual)
    }
    if (op == ">=") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdGreaterThanEqual)
        if (cls == 0) return (ok = true, code = SpvOp.SGreaterThanEqual)
        if (cls == 1) return (ok = true, code = SpvOp.UGreaterThanEqual)
    }
    return (ok = false, code = SpvOp.Nop)
}

// number of scalar lanes in a type (scalar = 1, vecN = N, anything else = 0/unknown).
def private component_count(bt : Type) : int {
    if (bt == Type.tInt || bt == Type.tUInt || bt == Type.tFloat || bt == Type.tBool) return 1
    if (bt == Type.tInt2 || bt == Type.tUInt2 || bt == Type.tFloat2) return 2
    if (bt == Type.tInt3 || bt == Type.tUInt3 || bt == Type.tFloat3) return 3
    if (bt == Type.tInt4 || bt == Type.tUInt4 || bt == Type.tFloat4) return 4
    return 0
}

// vector constructor name (float2/3/4, int2/3/4, uint2/3/4) -> lane count + component class
// (0 signed, 1 unsigned, 2 float), else ok=false. The class lets the constructor reject
// component-kind conversions (e.g. int4(float4(...))), which need conversion ops we don't emit yet.
def private vector_ctor(name : string) : tuple<ok : bool; n : int; cls : int> {
    if (name == "float2") return (ok = true, n = 2, cls = 2)
    if (name == "float3") return (ok = true, n = 3, cls = 2)
    if (name == "float4") return (ok = true, n = 4, cls = 2)
    if (name == "int2") return (ok = true, n = 2, cls = 0)
    if (name == "int3") return (ok = true, n = 3, cls = 0)
    if (name == "int4") return (ok = true, n = 4, cls = 0)
    if (name == "uint2") return (ok = true, n = 2, cls = 1)
    if (name == "uint3") return (ok = true, n = 3, cls = 1)
    if (name == "uint4") return (ok = true, n = 4, cls = 1)
    return (ok = false, n = 0, cls = -1)
}

// OpExtInstImport "GLSL.std.450" once per module; returns the cached set id.
def private ensure_glsl_ext(var m : SpirvModule; var ctx : EmitCtx) : uint {
    if (ctx.glsl_ext != 0u) return ctx.glsl_ext
    let id = alloc_id(m)
    var ops <- [id]
    var nm <- string_words("GLSL.std.450")
    ops |> push_from(nm)
    delete nm
    emit_n(m, SEC_EXTIMPORT, SpvOp.ExtInstImport, ops)
    delete ops
    ctx.glsl_ext = id
    return id
}

// daslang math builtin -> GLSL.std.450 ext-inst opcode. abs/min/max/clamp/sign pick the F/S/U
// variant by operand class; the rest are float-only. Operand class comes from arg 0 (vectors
// classify by lane kind, so length/normalize/cross/etc. read as float-class). ok=false => not a
// GLSL.std.450 builtin (or wrong class) -> caller emits a clean error.
def private glsl_ext_op(name : string; cls : int) : tuple<ok : bool; op : GLSLstd450> {
    if (name == "abs") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FAbs)
        if (cls == 0) return (ok = true, op = GLSLstd450.SAbs)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (name == "min") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FMin)
        if (cls == 0) return (ok = true, op = GLSLstd450.SMin)
        if (cls == 1) return (ok = true, op = GLSLstd450.UMin)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (name == "max") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FMax)
        if (cls == 0) return (ok = true, op = GLSLstd450.SMax)
        if (cls == 1) return (ok = true, op = GLSLstd450.UMax)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (name == "clamp") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FClamp)
        if (cls == 0) return (ok = true, op = GLSLstd450.SClamp)
        if (cls == 1) return (ok = true, op = GLSLstd450.UClamp)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (name == "sign") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FSign)
        if (cls == 0) return (ok = true, op = GLSLstd450.SSign)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (cls != 2) return (ok = false, op = GLSLstd450.Round)   // the rest are float-only
    if (name == "sqrt") return (ok = true, op = GLSLstd450.Sqrt)
    if (name == "inversesqrt") return (ok = true, op = GLSLstd450.InverseSqrt)
    if (name == "sin") return (ok = true, op = GLSLstd450.Sin)
    if (name == "cos") return (ok = true, op = GLSLstd450.Cos)
    if (name == "tan") return (ok = true, op = GLSLstd450.Tan)
    if (name == "asin") return (ok = true, op = GLSLstd450.Asin)
    if (name == "acos") return (ok = true, op = GLSLstd450.Acos)
    if (name == "atan") return (ok = true, op = GLSLstd450.Atan)
    if (name == "atan2") return (ok = true, op = GLSLstd450.Atan2)
    if (name == "pow") return (ok = true, op = GLSLstd450.Pow)
    if (name == "exp") return (ok = true, op = GLSLstd450.Exp)
    if (name == "log") return (ok = true, op = GLSLstd450.Log)
    if (name == "exp2") return (ok = true, op = GLSLstd450.Exp2)
    if (name == "log2") return (ok = true, op = GLSLstd450.Log2)
    if (name == "floor") return (ok = true, op = GLSLstd450.Floor)
    if (name == "ceil") return (ok = true, op = GLSLstd450.Ceil)
    if (name == "round") return (ok = true, op = GLSLstd450.Round)
    if (name == "trunc") return (ok = true, op = GLSLstd450.Trunc)
    if (name == "fract") return (ok = true, op = GLSLstd450.Fract)
    if (name == "radians") return (ok = true, op = GLSLstd450.Radians)
    if (name == "degrees") return (ok = true, op = GLSLstd450.Degrees)
    if (name == "mix") return (ok = true, op = GLSLstd450.FMix)
    if (name == "step") return (ok = true, op = GLSLstd450.Step)
    if (name == "smoothstep") return (ok = true, op = GLSLstd450.SmoothStep)
    if (name == "length") return (ok = true, op = GLSLstd450.Length)
    if (name == "distance") return (ok = true, op = GLSLstd450.Distance)
    if (name == "cross") return (ok = true, op = GLSLstd450.Cross)
    if (name == "normalize") return (ok = true, op = GLSLstd450.Normalize)
    if (name == "reflect") return (ok = true, op = GLSLstd450.Reflect)
    return (ok = false, op = GLSLstd450.Round)
}

// ===== compile-time constants: scalars, all-const vector constructors, const arrays =====
// Produces a deduped OpConstant / OpConstantComposite for a fully-constant expression. Used for
// array literals (which must be constant initializers for now) and their constituents. ok=false =>
// not a compile-time constant (the caller emits a clean error).
def private emit_const(var m : SpirvModule; e : ExpressionPtr; var errors : array<string>) : tuple<ok : bool; id : uint> {
    let r2v = e ?as ExprRef2Value
    if (r2v != null) return emit_const(m, r2v.subexpr, errors)
    let cu = e ?as ExprConstUInt
    if (cu != null) return (ok = true, id = const_uint(m, cu.value))
    let ci = e ?as ExprConstInt
    if (ci != null) return (ok = true, id = const_int(m, ci.value))
    let cf = e ?as ExprConstFloat
    if (cf != null) return (ok = true, id = const_float(m, cf.value))
    // unary minus on a constant: depending on fold state `-0.6` can arrive as ExprOp1("-", const)
    // rather than a folded negative literal. Fold it here so both shapes produce the same constant.
    let neg = e ?as ExprOp1
    if (neg != null && "{neg.op}" == "-") {
        let inner = neg.subexpr ?as ExprRef2Value
        let sub = inner != null ? inner.subexpr : neg.subexpr
        let ncf = sub ?as ExprConstFloat
        if (ncf != null) return (ok = true, id = const_float(m, -ncf.value))
        let nci = sub ?as ExprConstInt
        if (nci != null) return (ok = true, id = const_int(m, -nci.value))
        return (ok = false, id = 0u)
    }
    // folded vector constants (float2(0.,−.6) etc. fold to ExprConstFloatN at patch time) ->
    // OpConstantComposite of per-lane scalar constants.
    let cf2 = e ?as ExprConstFloat2
    if (cf2 != null) {
        let v = cf2.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_float(m, v.x), const_float(m, v.y)]))
    }
    let cf3 = e ?as ExprConstFloat3
    if (cf3 != null) {
        let v = cf3.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_float(m, v.x), const_float(m, v.y), const_float(m, v.z)]))
    }
    let cf4 = e ?as ExprConstFloat4
    if (cf4 != null) {
        let v = cf4.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_float(m, v.x), const_float(m, v.y), const_float(m, v.z), const_float(m, v.w)]))
    }
    let ci2 = e ?as ExprConstInt2
    if (ci2 != null) {
        let v = ci2.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_int(m, v.x), const_int(m, v.y)]))
    }
    let ci3 = e ?as ExprConstInt3
    if (ci3 != null) {
        let v = ci3.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_int(m, v.x), const_int(m, v.y), const_int(m, v.z)]))
    }
    let ci4 = e ?as ExprConstInt4
    if (ci4 != null) {
        let v = ci4.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_int(m, v.x), const_int(m, v.y), const_int(m, v.z), const_int(m, v.w)]))
    }
    let cu2 = e ?as ExprConstUInt2
    if (cu2 != null) {
        let v = cu2.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_uint(m, v.x), const_uint(m, v.y)]))
    }
    let cu3 = e ?as ExprConstUInt3
    if (cu3 != null) {
        let v = cu3.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_uint(m, v.x), const_uint(m, v.y), const_uint(m, v.z)]))
    }
    let cu4 = e ?as ExprConstUInt4
    if (cu4 != null) {
        let v = cu4.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_uint(m, v.x), const_uint(m, v.y), const_uint(m, v.z), const_uint(m, v.w)]))
    }
    // all-constant vector constructor -> OpConstantComposite
    let call = e ?as ExprCall
    if (call != null) {
        let vc = vector_ctor("{call.name}")
        if (!vc.ok) return (ok = false, id = 0u)
        var total = 0
        var parts : array<uint>
        parts |> reserve(length(call.arguments))
        for (a in call.arguments) {
            let cc = component_count(a._type.baseType)
            if (cc == 0 || scalar_class(a._type.baseType) != vc.cls) {
                errors |> push("{call.name}: constant constructor constituent of type {a._type.baseType} has the wrong component kind")
                delete parts
                return (ok = false, id = 0u)
            }
            total += cc
            let pr = emit_const(m, a, errors)
            if (!pr.ok) {
                errors |> push("{call.name}: constructor argument is not a compile-time constant")
                delete parts
                return (ok = false, id = 0u)
            }
            parts |> push(pr.id)
        }
        if (total != vc.n) {
            errors |> push("{call.name}: constant constituent lanes sum to {total}, expected {vc.n}")
            delete parts
            return (ok = false, id = 0u)
        }
        let cid = const_composite(m, emit_type(m, e._type), parts)
        delete parts
        return (ok = true, id = cid)
    }
    // const array literal -> OpConstantComposite of element constants
    let mk = e ?as ExprMakeArray
    if (mk != null) {
        // a dynamic array<T> literal is ALSO an ExprMakeArray; emit_type would panic on tArray.
        // Only fixed-array literals lower to a constant composite — reject the rest cleanly.
        if (e._type == null || e._type.baseType != Type.tFixedArray) {
            errors |> push("only fixed-array literals are supported (dynamic array<T> literals are not)")
            return (ok = false, id = 0u)
        }
        var parts : array<uint>
        parts |> reserve(length(mk.values))
        for (v in mk.values) {
            let pr = emit_const(m, v, errors)
            if (!pr.ok) {
                errors |> push("array literal element is not a compile-time constant")
                delete parts
                return (ok = false, id = 0u)
            }
            parts |> push(pr.id)
        }
        let cid = const_composite(m, emit_type(m, e._type), parts)
        delete parts
        return (ok = true, id = cid)
    }
    return (ok = false, id = 0u)
}

// ===== `*` over matrices / vectors / scalars =====
// SPIR-V splits multiplication by operand shape: OpMatrixTimesMatrix / OpMatrixTimesVector /
// OpVectorTimesScalar. Plain component-wise FMul/IMul (the scalar binop path) only works when both
// operands have the SAME shape, so the mixed cases are routed here first. Returns handled=false for
// same-shape operands (let binop_code emit FMul/IMul); handled=true with id=0 on an unsupported mix
// (error already pushed). Operands l/r are pre-emitted result-ids. Only the daslang-defined matrix
// operators are handled (matrix*matrix, matrix*vector, and float vector*scalar either order);
// daslang has no matrix*scalar or vector*matrix operator, so those SPIR-V ops are intentionally absent.
def private emit_mul(var m : SpirvModule; op2 : ExprOp2?; e : ExpressionPtr; l, r : uint; var errors : array<string>) : tuple<handled : bool; id : uint> {
    let lmat = matrix_info(op2.left._type)
    let rmat = matrix_info(op2.right._type)
    let lcc = component_count(op2.left._type.baseType)
    let rcc = component_count(op2.right._type.baseType)
    let lcls = scalar_class(op2.left._type.baseType)
    let rcls = scalar_class(op2.right._type.baseType)
    // same-shape (scalar*scalar or equal-width vector*vector): component-wise, defer to binop_code
    if (!lmat.ok && !rmat.ok && lcc == rcc) return (handled = false, id = 0u)
    let rt = emit_type(m, e._type)
    let res = alloc_id(m)
    if (lmat.ok && rmat.ok) {
        emit(m, SEC_FUNCS, SpvOp.MatrixTimesMatrix, rt, res, l, r)
    } elif (lmat.ok && rcc > 1) {
        emit(m, SEC_FUNCS, SpvOp.MatrixTimesVector, rt, res, l, r)
    } elif (lcc > 1 && rcc == 1 && lcls == 2 && rcls == 2) {
        emit(m, SEC_FUNCS, SpvOp.VectorTimesScalar, rt, res, l, r)
    } elif (rcc > 1 && lcc == 1 && lcls == 2 && rcls == 2) {
        emit(m, SEC_FUNCS, SpvOp.VectorTimesScalar, rt, res, r, l)   // SPIR-V wants the vector first
    } else {
        errors |> push("unsupported '*' between {op2.left._type.baseType} and {op2.right._type.baseType} (integer vector/scalar mixes need a splat)")
        return (handled = true, id = 0u)
    }
    return (handled = true, id = res)
}

// ===== local variable declaration (Function-storage OpVariable) =====
// SPIR-V requires every Function-storage OpVariable to be among the first instructions of the
// function's first block. So we pre-scan the whole body (one walk, all scopes — SPIR-V locals are
// function-scoped, not block-scoped) BEFORE emitting any body code, declaring an OpVariable for each
// mutable local (`var`) and loop-induction variable. Reads/writes then resolve to OpLoad/OpStore.
def private declare_local_var(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; var errors : array<string>) {
    if (key_exists(ctx.local_vars, intptr(v))) return
    let bt = v._type.baseType
    if (scalar_class(bt) < 0 && bt != Type.tBool && bt != Type.tFixedArray) {
        // emit_type would panic on an unsupported base type; reject with a clean error instead.
        errors |> push("local '{v.name}': type {bt} is not supported yet (only 32-bit int/uint/float scalars+vectors, bool, and fixed arrays)")
        return
    }
    let vt = emit_type(m, v._type)
    let pt = type_pointer(m, SpvStorageClass.Function, vt)
    let pid = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Variable, pt, pid, uint(SpvStorageClass.Function))
    ctx.local_vars |> insert(intptr(v), LocalVar(ptr_id = pid, value_type = vt))
}

def private collect_locals(var m : SpirvModule; var ctx : EmitCtx; e : ExpressionPtr; var errors : array<string>) {
    if (e == null) return
    let blk = e ?as ExprBlock
    if (blk != null) {
        for (s in blk.list) {
            collect_locals(m, ctx, s, errors)
        }
        return
    }
    let lt = e ?as ExprLet
    if (lt != null) {
        for (v in lt.variables) {
            // `var` -> memory; scalar/vector `let` -> SSA; but a fixed-array `let` needs memory too
            // (dynamic indexing requires an addressable OpVariable + OpAccessChain).
            if (!v._type.flags.constant || v._type.baseType == Type.tFixedArray) {
                declare_local_var(m, ctx, v, errors)
            }
        }
        return
    }
    let ite = e ?as ExprIfThenElse
    if (ite != null) {
        collect_locals(m, ctx, ite.if_true, errors)
        collect_locals(m, ctx, ite.if_false, errors)
        return
    }
    let wh = e ?as ExprWhile
    if (wh != null) {
        collect_locals(m, ctx, wh.body, errors)
        return
    }
    let fr = e ?as ExprFor
    if (fr != null) {
        for (fv in fr.iteratorVariables) {
            declare_local_var(m, ctx, fv, errors)
        }
        collect_locals(m, ctx, fr.body, errors)
        return
    }
}

// ===== compound assignment + increment/decrement =====
def private compound_base(op : string) : string {
    if (op == "+=") return "+"
    if (op == "-=") return "-"
    if (op == "*=") return "*"
    if (op == "/=") return "/"
    if (op == "%=") return "%"
    return ""
}

def private const_one(var m : SpirvModule; cls : int) : uint {
    if (cls == 2) return const_float(m, 1.0f)
    if (cls == 1) return const_uint(m, 1u)
    return const_int(m, 1)
}

def private const_zero(var m : SpirvModule; cls : int) : uint {
    if (cls == 2) return const_float(m, 0.0f)
    if (cls == 1) return const_uint(m, 0u)
    return const_int(m, 0)
}

// An integer-valued constant for a loop bound (range bounds are always int or uint, never float).
def private const_int_for(var m : SpirvModule; cls : int; v : int) : uint {
    if (cls == 1) return const_uint(m, uint(v))
    return const_int(m, v)
}

// ===== SpirvEmit : AstVisitor =====
// The body-codegen visitor (modeled on llvm_jit). generate_spirv builds the module scaffold (caps /
// entry-point / OpFunction / trailing return) and drives this visitor over fn.body for all codegen.
// Value threading is llvm_jit's side-map model: each node records its result keyed by intptr — e2id for
// an rvalue SSA id, e2ptr/e2pty for an lvalue pointer + pointee-type. Parents read children back out.
// ExprRef2Value is the load marker; bare lvalue nodes stay pointers and value_of() loads on demand
// (matching the hand-walker's `_type.flags.ref` branch). The patch-stage AST arrives const; each hook
// takes its node as a `var` param (`def override visitExprX(var expr : ExprX?)`), which binds a non-const
// view directly — no reinterpret const-strip, and `return expr` type-checks. We only read the AST and emit
// into the side module (never mutate it), so the body reuses the existing non-const emit helpers verbatim.
class SpirvEmit : AstVisitor {
    adapter : VisitorAdapter?
    @do_not_delete m   : SpirvModule?    // borrowed (addr of generate_spirv's module) — not owned
    @do_not_delete ctx : EmitCtx?        // borrowed (addr of generate_spirv's ctx) — not owned
    e2id    : table<uint64; uint>        // intptr(Expression) -> rvalue SSA result-id
    e2ptr   : table<uint64; uint>        // intptr(Expression) -> lvalue pointer result-id
    e2pty   : table<uint64; uint>        // intptr(Expression) -> pointee type-id (paired with e2ptr)
    ite_ids : table<uint64; IteIds>      // intptr(ExprIfThenElse) -> its {merge, then, else} labels
    loop_ids : table<uint64; LoopIds>    // intptr(ExprWhile/ExprFor) -> its 5 loop-skeleton labels
    for_src : table<uint64>              // for-source range() call ptrs to intercept in visitExprCall (6.3)
    errs    : array<string>

    // coerce a visited child to an rvalue id: a value passes through; a pointer is OpLoad'd here (the
    // bare-ref load, matching emit_value's `_type.flags.ref` branch).
    def value_of(e : ExpressionPtr) : uint {
        let k = intptr(e)
        if (key_exists(e2id, k)) return e2id?[k] ?? 0u
        if (key_exists(e2ptr, k)) {
            let pid = e2ptr?[k] ?? 0u
            let pty = e2pty?[k] ?? 0u
            if (pid == 0u || pty == 0u) {   // unresolved pointer / pointee — never OpLoad with a 0 operand
                errs |> push("internal: cannot load {e.__rtti} (unresolved pointer or pointee type)")
                return 0u
            }
            var bm & = unsafe(*m)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, pty, res, pid)
            return res
        }
        errs |> push("internal: no rvalue available for {e.__rtti}")
        return 0u
    }
    def ptr_of(e : ExpressionPtr) : tuple<id : uint; pty : uint> {
        let k = intptr(e)
        if (key_exists(e2ptr, k)) return (id = e2ptr?[k] ?? 0u, pty = e2pty?[k] ?? 0u)
        errs |> push("internal: {e.__rtti} is not an lvalue")
        return (id = 0u, pty = 0u)
    }

    // ----- constants -----
    def override visitExprConstInt(var expr : ExprConstInt?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_int(bm, expr.value)
        return expr
    }
    def override visitExprConstUInt(var expr : ExprConstUInt?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_uint(bm, expr.value)
        return expr
    }
    def override visitExprConstFloat(var expr : ExprConstFloat?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_float(bm, expr.value)
        return expr
    }

    // ----- variables: value-let -> SSA id; memory local / global -> pointer -----
    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        let k = intptr(expr)
        if (expr.varFlags.local || expr.varFlags.argument || expr.varFlags._block) {
            let vk = intptr(expr.variable)
            if (key_exists(ctx.local_vars, vk)) {
                let lv = ctx.local_vars?[vk] ?? LocalVar()
                e2ptr[k] = lv.ptr_id
                e2pty[k] = lv.value_type
            } else {
                let lid = ctx.locals?[vk] ?? 0u
                if (lid == 0u) {   // value-let read before its init was visited -> no SSA id
                    errs |> push("local '{expr.name}' used before it was assigned an SSA id")
                }
                e2id[k] = lid
            }
        } else {
            let gi = ctx.globals?[intptr(expr.variable)] ?? GlobalInfo()
            if (gi.var_id == 0u) {   // referenced global was not classified (unsupported / not collected)
                errs |> push("global '{expr.name}' is not classified")
            }
            e2ptr[k] = gi.var_id
            e2pty[k] = gi.value_type
        }
        return expr
    }

    // ----- swizzle: global single-component -> pointer (AccessChain); value -> extract/shuffle -----
    def override visitExprSwizzle(var expr : ExprSwizzle?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let k = intptr(expr)
        let gv = global_var_of(expr.value)
        if (gv != null) {
            if (length(expr.fields) != 1) {
                errs |> push("only single-component swizzle of a global is supported for now")
                return expr
            }
            let gi = ctx.globals?[intptr(gv)] ?? GlobalInfo()
            if (gi.var_id == 0u) {   // global not classified -> never AccessChain through a 0 base
                errs |> push("swizzle target global '{gv.name}' is not classified")
                return expr
            }
            let pointee = emit_type(bm, expr._type)
            let ptr_t = type_pointer(bm, gi.storage, pointee)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, gi.var_id, const_uint(bm, uint(expr.fields[0])))
            e2ptr[k] = res
            e2pty[k] = pointee
        } else {
            let base = value_of(expr.value)
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            if (length(expr.fields) == 1) {
                emit(bm, SEC_FUNCS, SpvOp.CompositeExtract, rt, res, base, uint(expr.fields[0]))
            } else {
                var ops <- [rt, res, base, base]
                for (f in expr.fields) {
                    ops |> push(uint(f))
                }
                emit_n(bm, SEC_FUNCS, SpvOp.VectorShuffle, ops)
                delete ops
            }
            e2id[k] = res
        }
        return expr
    }

    // ----- indexing: ssbo array or local fixed array -> pointer (AccessChain) -----
    def override visitExprAt(var expr : ExprAt?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let k = intptr(expr)
        let lev = expr.subexpr ?as ExprVar
        if (lev != null && (lev.varFlags.local || lev.varFlags.argument || lev.varFlags._block)) {
            let lv = ctx.local_vars?[intptr(lev.variable)] ?? LocalVar()
            if (lv.ptr_id == 0u) {
                errs |> push("indexing local '{lev.name}': only fixed-array `var`/`let` locals are indexable")
                return expr
            }
            let pointee = emit_type(bm, expr._type)
            let idx = value_of(expr.index)
            let ptr_t = type_pointer(bm, SpvStorageClass.Function, pointee)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, lv.ptr_id, idx)
            e2ptr[k] = res
            e2pty[k] = pointee
            return expr
        }
        let bv = global_var_of(expr.subexpr)
        if (bv == null) {
            errs |> push("indexing is only supported on a global ssbo array or a local fixed array for now")
            return expr
        }
        let gi = ctx.globals?[intptr(bv)] ?? GlobalInfo()
        if (!gi.is_ssbo) {
            errs |> push("only ssbo array indexing is supported for now")
            return expr
        }
        let pointee = emit_type(bm, expr._type)
        let idx = value_of(expr.index)
        let res = alloc_id(bm)
        let ptr_t = type_pointer(bm, gi.storage, pointee)
        var ops <- [ptr_t, res, gi.var_id, const_uint(bm, 0u), idx]
        emit_n(bm, SEC_FUNCS, SpvOp.AccessChain, ops)
        delete ops
        e2ptr[k] = res
        e2pty[k] = pointee
        return expr
    }

    // ----- the load marker: read an lvalue child as a value -----
    def override visitExprRef2Value(var expr : ExprRef2Value?) : ExpressionPtr {
        e2id[intptr(expr)] = value_of(expr.subexpr)
        return expr
    }

    // ----- unary: !, -, ~ (value) and statement-level ++/-- (load-op-store) -----
    def override visitExprOp1(var expr : ExprOp1?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let op = "{expr.op}"
        if (op == "++" || op == "--") {
            let cls = scalar_class(expr.subexpr._type.baseType)
            let bo = binop_code(op == "++" ? "+" : "-", cls)
            if (!bo.ok) {
                errs |> push("unsupported {expr.op} on {expr.subexpr._type.baseType}")
                return expr
            }
            let one = const_one(bm, cls)
            let p = ptr_of(expr.subexpr)
            if (p.id != 0u) {
                let cur = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Load, p.pty, cur, p.id)
                let res = alloc_id(bm)
                emit(bm, SEC_FUNCS, bo.code, p.pty, res, cur, one)
                emit(bm, SEC_FUNCS, SpvOp.Store, p.id, res)
            }
            return expr
        }
        let v = value_of(expr.subexpr)
        if (v == 0u) return expr
        if (op == "!") {
            let bt = type_bool(bm)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.LogicalNot, bt, res, v)
            e2id[intptr(expr)] = res
            return expr
        }
        let uo = unop_code(op, scalar_class(expr.subexpr._type.baseType))
        if (!uo.ok) {
            errs |> push("unsupported unary op '{expr.op}' on {expr.subexpr._type.baseType}")
            return expr
        }
        let rt = emit_type(bm, expr._type)
        let res = alloc_id(bm)
        emit(bm, SEC_FUNCS, uo.code, rt, res, v)
        e2id[intptr(expr)] = res
        return expr
    }

    // ----- binary: arithmetic / comparison / logical, plus matrix-* via emit_mul; void op2 = compound -----
    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let op = "{expr.op}"
        if (expr._type.baseType == Type.tVoid) {        // compound assignment: dst <op>= rhs
            let bo = binop_code(compound_base(op), scalar_class(expr.left._type.baseType))
            if (!bo.ok) {
                errs |> push("unsupported compound assignment '{expr.op}'")
                return expr
            }
            let rhs = value_of(expr.right)
            let p = ptr_of(expr.left)
            if (p.id != 0u && rhs != 0u) {
                let cur = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Load, p.pty, cur, p.id)
                let res = alloc_id(bm)
                emit(bm, SEC_FUNCS, bo.code, p.pty, res, cur, rhs)
                emit(bm, SEC_FUNCS, SpvOp.Store, p.id, res)
            }
            return expr
        }
        let cls = scalar_class(expr.left._type.baseType)
        let l = value_of(expr.left)
        let r = value_of(expr.right)
        if (l == 0u || r == 0u) return expr
        if (op == "*") {
            let mr = emit_mul(bm, expr, expr, l, r, errs)
            if (mr.handled) {
                if (mr.id != 0u) {
                    e2id[intptr(expr)] = mr.id
                }
                return expr
            }
        }
        let cc = cmp_code(op, cls)
        if (cc.ok) {
            let bt = type_bool(bm)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, cc.code, bt, res, l, r)
            e2id[intptr(expr)] = res
            return expr
        }
        if (op == "&&" || op == "||") {
            let bt = type_bool(bm)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, op == "&&" ? SpvOp.LogicalAnd : SpvOp.LogicalOr, bt, res, l, r)
            e2id[intptr(expr)] = res
            return expr
        }
        let bo = binop_code(op, cls)
        if (!bo.ok) {
            errs |> push("unsupported binary op '{expr.op}' on {expr.left._type.baseType}")
            return expr
        }
        let rt = emit_type(bm, expr._type)
        let res = alloc_id(bm)
        emit(bm, SEC_FUNCS, bo.code, rt, res, l, r)
        e2id[intptr(expr)] = res
        return expr
    }

    // ----- store: dst = rhs -----
    def override visitExprCopy(var expr : ExprCopy?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let p = ptr_of(expr.left)
        let val = value_of(expr.right)
        if (p.id != 0u && val != 0u) {
            emit(bm, SEC_FUNCS, SpvOp.Store, p.id, val)
        }
        return expr
    }

    // ----- let: memory local -> store init; value-let -> bind SSA id -----
    def override visitExprLetVariable(cexpr : ExprLet?; var arg : VariablePtr; lastArg : bool) : VariablePtr {
        var bm & = unsafe(*m)
        if (arg.init == null) {
            errs |> push("uninitialized local '{arg.name}' is not supported (every let/var must have an initializer)")
            return arg
        }
        let vk = intptr(arg)
        if (key_exists(ctx.local_vars, vk)) {
            let lv = ctx.local_vars?[vk] ?? LocalVar()
            let val = value_of(arg.init)
            if (val != 0u) {
                emit(bm, SEC_FUNCS, SpvOp.Store, lv.ptr_id, val)
            }
        } else {
            ctx.locals[vk] = value_of(arg.init)
        }
        return arg
    }

    // ----- return -----
    def override visitExprReturn(var expr : ExprReturn?) : ExpressionPtr {
        var bm & = unsafe(*m)
        emit(bm, SEC_FUNCS, SpvOp.Return)
        ctx.terminated = true
        return expr
    }

    // ----- member of a Block global (ubo.color) -> OpAccessChain by field index -> pointer -----
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let k = intptr(expr)
        let bv = global_var_of(expr.value)
        if (bv == null) {
            errs |> push("member access '.{expr.name}' is only supported on a @uniform/@ssbo/@push_constant block global for now")
            return expr
        }
        let gi = ctx.globals?[intptr(bv)] ?? GlobalInfo()
        if (!gi.is_block) {
            errs |> push("member access '.{expr.name}': '{bv.name}' is not a block (uniform/ssbo/push_constant) global")
            return expr
        }
        let midx = field_index_by_name(bv._type.structType, "{expr.name}")
        if (midx < 0) {
            errs |> push("member access '.{expr.name}': no such field on '{bv.name}'")
            return expr
        }
        let pointee = emit_type(bm, expr._type)
        let ptr_t = type_pointer(bm, gi.storage, pointee)
        let res = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, gi.var_id, const_uint(bm, uint(midx)))
        e2ptr[k] = res
        e2pty[k] = pointee
        return expr
    }

    // ----- calls: texture / vector constructors / dot / GLSL.std.450 math (args pre-visited) -----
    // Mirrors emit_call but reads operand ids via value_of (children already visited). for-source
    // range()/urange() calls are consumed by preVisitExprForBody and skipped here.
    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        if (key_exists(for_src, intptr(expr))) return expr
        var bm & = unsafe(*m)
        let name = "{expr.name}"
        let k = intptr(expr)
        // texture(sampler, uv): OpLoad the sampled image + OpImageSampleImplicitLod
        if (name == "texture") {
            if (length(expr.arguments) != 2) {
                errs |> push("texture expects (sampler, uv)")
                return expr
            }
            if (ctx.stage != ShaderStage.Fragment) {
                errs |> push("texture() implicit-LOD sampling is only valid in a fragment shader")
                return expr
            }
            let sg = global_var_of(expr.arguments[0])
            if (sg == null) {
                errs |> push("texture: first argument must be a sampler global")
                return expr
            }
            let gi = ctx.globals?[intptr(sg)] ?? GlobalInfo()
            if (!gi.is_sampler) {
                errs |> push("texture: '{sg.name}' is not a sampler")
                return expr
            }
            let simg = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, gi.value_type, simg, gi.var_id)
            let uv = value_of(expr.arguments[1])
            if (uv == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageSampleImplicitLod, rt, res, simg, uv)
            e2id[k] = res
            return expr
        }
        // vector constructor: OpCompositeConstruct (or splat / identity for one arg)
        let vc = vector_ctor(name)
        if (vc.ok) {
            let rt = emit_type(bm, expr._type)
            if (length(expr.arguments) == 1) {
                let a0 = expr.arguments[0]
                let cc = component_count(a0._type.baseType)
                if (scalar_class(a0._type.baseType) != vc.cls) {
                    errs |> push("{name}: component-kind conversion from {a0._type.baseType} is not supported yet")
                    return expr
                }
                let aid = value_of(a0)
                if (aid == 0u) return expr
                if (cc == vc.n) {                            // identity: float4(someFloat4)
                    e2id[k] = aid
                    return expr
                }
                if (cc == 1) {                               // splat: float4(scalar)
                    let res = alloc_id(bm)
                    var cons <- [rt, res]
                    cons |> reserve(2 + vc.n)
                    for (_l in range(vc.n)) {
                        cons |> push(aid)
                    }
                    emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
                    delete cons
                    e2id[k] = res
                    return expr
                }
                errs |> push("cannot construct {name} from a single {a0._type.baseType}")
                return expr
            }
            var total = 0
            let res = alloc_id(bm)
            var cons <- [rt, res]
            cons |> reserve(2 + length(expr.arguments))
            for (a in expr.arguments) {
                let cc = component_count(a._type.baseType)
                if (cc == 0) {
                    errs |> push("{name}: argument of type {a._type.baseType} is not a scalar/vector")
                    delete cons
                    return expr
                }
                if (scalar_class(a._type.baseType) != vc.cls) {
                    errs |> push("{name}: constituent of type {a._type.baseType} has a different component kind (conversion not supported yet)")
                    delete cons
                    return expr
                }
                total += cc
                let aid = value_of(a)
                if (aid == 0u) {
                    delete cons
                    return expr
                }
                cons |> push(aid)
            }
            if (total != vc.n) {
                errs |> push("{name}: constituent lanes sum to {total}, expected {vc.n}")
                delete cons
                return expr
            }
            emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
            delete cons
            e2id[k] = res
            return expr
        }
        // dot: core OpDot (vector . vector -> scalar)
        if (name == "dot") {
            if (length(expr.arguments) != 2) {
                errs |> push("dot expects 2 arguments")
                return expr
            }
            let a = value_of(expr.arguments[0])
            let b = value_of(expr.arguments[1])
            if (a == 0u || b == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Dot, rt, res, a, b)
            e2id[k] = res
            return expr
        }
        // GLSL.std.450 math: OpExtInst
        let cls = empty(expr.arguments) ? -1 : scalar_class(expr.arguments[0]._type.baseType)
        let ge = glsl_ext_op(name, cls)
        if (ge.ok) {
            let set = ensure_glsl_ext(bm, *ctx)
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            var ops <- [rt, res, set, uint(ge.op)]
            for (a in expr.arguments) {
                let aid = value_of(a)
                if (aid == 0u) {
                    delete ops
                    return expr
                }
                ops |> push(aid)
            }
            emit_n(bm, SEC_FUNCS, SpvOp.ExtInst, ops)
            delete ops
            e2id[k] = res
            return expr
        }
        errs |> push("unsupported call '{name}' (not a vector constructor, dot, or GLSL.std.450 math builtin)")
        return expr
    }

    // ----- fixed-array literal -> OpConstantComposite (whole-array, deduped) -----
    // emit_const recurses into the (const) elements internally, so the elements' own default visit emits
    // nothing — at patch they fold to ExprConstFloatN leaves, not sub-call nodes. Records the array's
    // constant id; a fixed-array `let` then Stores it into the Function-storage OpVariable.
    def override visitExprMakeArray(var expr : ExprMakeArray?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let r = emit_const(bm, expr, errs)
        if (r.ok) {
            e2id[intptr(expr)] = r.id
        }
        return expr
    }

    // ----- if / elif / else (OpSelectionMerge + OpBranchConditional, multi-phase) -----
    // Block ids are allocated in the IfBlock hook — AFTER the cond is visited — so the id order
    // matches the hand-walker (cond ids first, then merge/then/else), keeping the blob byte-identical.
    // `elif` chains fall out for free: an `if_false` that is itself an ExprIfThenElse just recurses.
    def override preVisitExprIfThenElse(var expr : ExprIfThenElse?) : void {}

    def override preVisitExprIfThenElseIfBlock(var expr : ExprIfThenElse?; ifBlock : ExpressionPtr) : void {
        var bm & = unsafe(*m)
        let cond = value_of(expr.cond)
        let merge = alloc_id(bm)
        let then_l = alloc_id(bm)
        let else_l = expr.if_false != null ? alloc_id(bm) : merge
        ite_ids[intptr(expr)] = IteIds(merge = merge, then_l = then_l, else_l = else_l)
        emit(bm, SEC_FUNCS, SpvOp.SelectionMerge, merge, uint(SpvSelectionControl.None))
        emit(bm, SEC_FUNCS, SpvOp.BranchConditional, cond, then_l, else_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, then_l)
        ctx.terminated = false
    }

    def override preVisitExprIfThenElseElseBlock(var expr : ExprIfThenElse?; elseBlock : ExpressionPtr) : void {
        let ids = ite_ids?[intptr(expr)] ?? IteIds()
        var bm & = unsafe(*m)
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.else_l)
        ctx.terminated = false
    }

    def override visitExprIfThenElse(var expr : ExprIfThenElse?) : ExpressionPtr {
        let ids = ite_ids?[intptr(expr)] ?? IteIds()
        var bm & = unsafe(*m)
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.merge)
        ctx.terminated = false
        return expr
    }

    // ----- while (OpLoopMerge 4-block, multi-phase) -----
    def override preVisitExprWhile(var expr : ExprWhile?) : void {
        var bm & = unsafe(*m)
        let ids = LoopIds(header = alloc_id(bm), cond_l = alloc_id(bm), body_l = alloc_id(bm),
                          cont_l = alloc_id(bm), merge = alloc_id(bm))
        loop_ids[intptr(expr)] = ids
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.header)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.header)
        ctx.terminated = false
        emit(bm, SEC_FUNCS, SpvOp.LoopMerge, ids.merge, ids.cont_l, uint(SpvLoopControl.None))
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.cond_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.cond_l)
        ctx.terminated = false
    }

    def override preVisitExprWhileBody(var expr : ExprWhile?; right : ExpressionPtr) : void {
        let ids = loop_ids?[intptr(expr)] ?? LoopIds()
        var bm & = unsafe(*m)
        let c = value_of(expr.cond)
        emit(bm, SEC_FUNCS, SpvOp.BranchConditional, c, ids.body_l, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.body_l)
        ctx.terminated = false
        ctx.loop_stack |> push(LoopTargets(merge = ids.merge, cont = ids.cont_l))
    }

    def override visitExprWhile(var expr : ExprWhile?) : ExpressionPtr {
        let ids = loop_ids?[intptr(expr)] ?? LoopIds()
        var bm & = unsafe(*m)
        if (!empty(ctx.loop_stack)) pop(ctx.loop_stack)   // guard underflow if the body hook was skipped on an earlier error
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.cont_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.cont_l)
        ctx.terminated = false
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.header)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.merge)
        ctx.terminated = false
        return expr
    }

    // ----- for (i in range(...)): synthesized 4-block loop. All setup runs in ForBody (after the
    // source is visited, so runtime range() endpoints are already in e2id); const ranges read
    // ExprConstRange.value directly. Block ids allocated here to match the hand-walker's order. -----
    def override preVisitExprFor(var expr : ExprFor?) : void {
        // record the for-source so a future visitExprCall (6.3) skips emitting the range() call; its
        // arguments are still visited normally, so runtime endpoints land in e2id.
        if (!empty(expr.sources)) {
            for_src |> insert(intptr(expr.sources[0]))
        }
    }

    def override preVisitExprForBody(var expr : ExprFor?) : void {
        var bm & = unsafe(*m)
        if (length(expr.iteratorVariables) != 1 || length(expr.sources) != 1) {
            errs |> push("for: only a single `i in range(...)` iterator is supported for now")
            return
        }
        let iv = expr.iteratorVariables[0]
        let lvr = ctx.local_vars?[intptr(iv)] ?? LocalVar()
        if (lvr.ptr_id == 0u) {
            errs |> push("for: induction variable '{iv.name}' was not declared")
            return
        }
        let cls = scalar_class(iv._type.baseType)
        if (cls != 0 && cls != 1) {
            errs |> push("for: induction variable '{iv.name}' must be int or uint (got {iv._type.baseType})")
            return
        }
        var lo = 0u
        var hi = 0u
        let cr = expr.sources[0] ?as ExprConstRange
        if (cr != null) {
            let fromto = unsafe(reinterpret<int2>(cr.value))   // range == { from : int; to : int }
            lo = const_int_for(bm, cls, fromto.x)
            hi = const_int_for(bm, cls, fromto.y)
        } else {
            let src = expr.sources[0] ?as ExprCall
            if (src == null) {
                errs |> push("for: source is {expr.sources[0].__rtti}, expected range()/urange()")
                return
            }
            if ("{src.name}" != "range" && "{src.name}" != "urange") {
                errs |> push("for: source call is '{src.name}', expected range()/urange()")
                return
            }
            let na = length(src.arguments)
            if (na != 1 && na != 2) {
                errs |> push("for: only range(n) and range(lo, hi) are supported for now (no step)")
                return
            }
            lo = na == 2 ? value_of(src.arguments[0]) : const_zero(bm, cls)
            hi = value_of(src.arguments[na - 1])
        }
        if (lo == 0u || hi == 0u) return
        emit(bm, SEC_FUNCS, SpvOp.Store, lvr.ptr_id, lo)
        let ids = LoopIds(header = alloc_id(bm), cond_l = alloc_id(bm), body_l = alloc_id(bm),
                          cont_l = alloc_id(bm), merge = alloc_id(bm))
        loop_ids[intptr(expr)] = ids
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.header)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.header)
        ctx.terminated = false
        emit(bm, SEC_FUNCS, SpvOp.LoopMerge, ids.merge, ids.cont_l, uint(SpvLoopControl.None))
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.cond_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.cond_l)
        ctx.terminated = false
        let cur = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.Load, lvr.value_type, cur, lvr.ptr_id)
        let cc = cmp_code("<", cls)
        let cres = alloc_id(bm)
        emit(bm, SEC_FUNCS, cc.code, type_bool(bm), cres, cur, hi)
        emit(bm, SEC_FUNCS, SpvOp.BranchConditional, cres, ids.body_l, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.body_l)
        ctx.terminated = false
        ctx.loop_stack |> push(LoopTargets(merge = ids.merge, cont = ids.cont_l))
    }

    def override visitExprFor(var expr : ExprFor?) : ExpressionPtr {
        if (!key_exists(loop_ids, intptr(expr))) return expr   // setup failed; errors already pushed
        let ids = loop_ids?[intptr(expr)] ?? LoopIds()
        var bm & = unsafe(*m)
        let iv = expr.iteratorVariables[0]
        let lvr = ctx.local_vars?[intptr(iv)] ?? LocalVar()
        let cls = scalar_class(iv._type.baseType)
        pop(ctx.loop_stack)
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.cont_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.cont_l)
        ctx.terminated = false
        let cur2 = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.Load, lvr.value_type, cur2, lvr.ptr_id)
        let nxt = alloc_id(bm)
        emit(bm, SEC_FUNCS, binop_code("+", cls).code, lvr.value_type, nxt, cur2, const_one(bm, cls))
        emit(bm, SEC_FUNCS, SpvOp.Store, lvr.ptr_id, nxt)
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.header)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.merge)
        ctx.terminated = false
        return expr
    }

    // ----- break / continue -> branch to the innermost loop's merge / continue block -----
    def override preVisitExprBreak(var expr : ExprBreak?) : void {
        var bm & = unsafe(*m)
        if (empty(ctx.loop_stack)) {
            errs |> push("break outside of a loop")
            return
        }
        emit(bm, SEC_FUNCS, SpvOp.Branch, ctx.loop_stack[length(ctx.loop_stack) - 1].merge)
        ctx.terminated = true
    }

    def override preVisitExprContinue(var expr : ExprContinue?) : void {
        var bm & = unsafe(*m)
        if (empty(ctx.loop_stack)) {
            errs |> push("continue outside of a loop")
            return
        }
        emit(bm, SEC_FUNCS, SpvOp.Branch, ctx.loop_stack[length(ctx.loop_stack) - 1].cont)
        ctx.terminated = true
    }

    // ===== Fail-closed rejections (Phase 6.4) =====
    // Each shader-illegal construct gets an explicit preVisit override that records a clean error, so an
    // unsupported node can never be silently walked over into an empty/wrong blob. The value_of/ptr_of
    // backstop above is the complement: it catches a walked-but-unlowered node (incl. a future AST node
    // type we have no hook for) the moment a parent reads its missing id. Mirrors dasGlsl's discipline.
    def override preVisitExprNew(expr : ExprNew?) : void {
        errs |> push("new is not supported in SPIR-V shaders")
    }
    def override preVisitExprAscend(expr : ExprAscend?) : void {
        errs |> push("new (ascend) is not supported in SPIR-V shaders")
    }
    def override preVisitExprDelete(expr : ExprDelete?) : void {
        errs |> push("delete is not supported in SPIR-V shaders")
    }
    def override preVisitExprStringBuilder(expr : ExprStringBuilder?) : void {
        errs |> push("string builder is not supported in SPIR-V shaders")
    }
    def override preVisitExprNamedCall(expr : ExprNamedCall?) : void {
        errs |> push("named call is not supported in SPIR-V shaders")
    }
    def override preVisitExprNullCoalescing(expr : ExprNullCoalescing?) : void {
        errs |> push("null coalescing (??) is not supported in SPIR-V shaders")
    }
    def override preVisitExprCast(expr : ExprCast?) : void {
        errs |> push("cast is not supported in SPIR-V shaders")
    }
    def override preVisitExprPtr2Ref(expr : ExprPtr2Ref?) : void {
        errs |> push("pointer deref is not supported in SPIR-V shaders")
    }
    def override preVisitExprRef2Ptr(expr : ExprRef2Ptr?) : void {
        errs |> push("addr is not supported in SPIR-V shaders")
    }
    def override preVisitExprAddr(expr : ExprAddr?) : void {
        errs |> push("function address (@@) is not supported in SPIR-V shaders")
    }
    def override preVisitExprConstPtr(expr : ExprConstPtr?) : void {
        errs |> push("pointer constant is not supported in SPIR-V shaders")
    }
    def override preVisitExprSafeAt(expr : ExprSafeAt?) : void {
        errs |> push("safe index (?[]) is not supported in SPIR-V shaders")
    }
    def override preVisitExprSafeField(expr : ExprSafeField?) : void {
        errs |> push("safe field (?.) is not supported in SPIR-V shaders")
    }
    def override preVisitExprIsType(expr : ExprIs?; typeDecl : TypeDeclPtr) : void {
        errs |> push("'is' type test is not supported in SPIR-V shaders")
    }
    def override preVisitExprIsVariant(expr : ExprIsVariant?) : void {
        errs |> push("variant 'is' is not supported in SPIR-V shaders")
    }
    def override preVisitExprAsVariant(expr : ExprAsVariant?) : void {
        errs |> push("variant 'as' is not supported in SPIR-V shaders")
    }
    def override preVisitExprSafeAsVariant(expr : ExprSafeAsVariant?) : void {
        errs |> push("variant '?as' is not supported in SPIR-V shaders")
    }
    def override preVisitExprMoveRight(expr : ExprMove?; right : ExpressionPtr) : void {
        errs |> push("move (<-) is not supported in SPIR-V shaders")
    }
    def override preVisitExprCloneRight(expr : ExprClone?; right : ExpressionPtr) : void {
        errs |> push("clone (:=) is not supported in SPIR-V shaders")
    }
    def override preVisitExprWith(expr : ExprWith?) : void {
        errs |> push("with is not supported in SPIR-V shaders")
    }
    def override preVisitExprTryCatch(expr : ExprTryCatch?) : void {
        errs |> push("try/recover is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeVariant(expr : ExprMakeVariant?) : void {
        errs |> push("variant construction is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeTuple(expr : ExprMakeTuple?) : void {
        errs |> push("tuple construction is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeStruct(expr : ExprMakeStruct?) : void {
        errs |> push("struct construction is not supported in SPIR-V shaders")
    }
    def override canVisitExprMakeStructBody(expr : ExprMakeStruct?) : bool {
        return false   // already rejected in preVisit; don't descend into the (unlowerable) field inits
    }
    def override preVisitExprArrayComprehension(expr : ExprArrayComprehension?) : void {
        errs |> push("array comprehension is not supported in SPIR-V shaders")
    }
    def override preVisitExprTypeInfo(expr : ExprTypeInfo?) : void {
        errs |> push("typeinfo is not supported in SPIR-V shaders")
    }
    def override preVisitExprLabel(expr : ExprLabel?) : void {
        errs |> push("label is not supported in SPIR-V shaders")
    }
    def override preVisitExprGoto(expr : ExprGoto?) : void {
        errs |> push("goto is not supported in SPIR-V shaders")
    }
    def override preVisitExprAssert(expr : ExprAssert?) : void {
        errs |> push("assert is not supported in SPIR-V shaders")
    }
    def override preVisitExprStaticAssert(expr : ExprStaticAssert?) : void {
        errs |> push("static_assert is not supported in SPIR-V shaders")
    }
    def override preVisitExprQuote(expr : ExprQuote?) : void {
        errs |> push("quote is not supported in SPIR-V shaders")
    }
    def override preVisitExprDebug(expr : ExprDebug?) : void {
        errs |> push("debug is not supported in SPIR-V shaders")
    }
    def override preVisitExprInvoke(expr : ExprInvoke?) : void {
        errs |> push("invoke is not supported in SPIR-V shaders")
    }
    def override preVisitExprErase(expr : ExprErase?) : void {
        errs |> push("erase is not supported in SPIR-V shaders")
    }
    def override preVisitExprSetInsert(expr : ExprSetInsert?) : void {
        errs |> push("set insert is not supported in SPIR-V shaders")
    }
    def override preVisitExprFind(expr : ExprFind?) : void {
        errs |> push("find is not supported in SPIR-V shaders")
    }
    def override preVisitExprKeyExists(expr : ExprKeyExists?) : void {
        errs |> push("key_exists is not supported in SPIR-V shaders")
    }
    def override preVisitExprYield(expr : ExprYield?) : void {
        errs |> push("yield is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeBlock(expr : ExprMakeBlock?) : void {
        errs |> push("closure / block is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeGenerator(expr : ExprMakeGenerator?) : void {
        errs |> push("generator is not supported in SPIR-V shaders")
    }
    def override preVisitExprMemZero(expr : ExprMemZero?) : void {
        errs |> push("memzero is not supported in SPIR-V shaders")
    }
}

// Drive the SpirvEmit visitor over a function body. The visitor owns all body codegen; the module
// scaffold (caps / entry-point / OpFunction / trailing return) stays in generate_spirv.
def private drive_emit(var m : SpirvModule; var ctx : EmitCtx; fn : FunctionPtr; var errors : array<string>) {
    var v = new SpirvEmit(m = unsafe(addr(m)), ctx = unsafe(addr(ctx)))
    make_visitor(*v) $(mv_adapter) {
        v.adapter = mv_adapter
        visit(fn.body, v.adapter)
    }
    errors |> push_from(v.errs)
    unsafe {
        delete v
    }
}

// ===== entry =====
def private execution_model(stage : ShaderStage) : SpvExecutionModel {
    if (stage == ShaderStage.Vertex) return SpvExecutionModel.Vertex
    if (stage == ShaderStage.Fragment) return SpvExecutionModel.Fragment
    return SpvExecutionModel.GLCompute
}

[macro_function]
def generate_spirv(fn : FunctionPtr; var errors : array<string>; stage : ShaderStage; local_size : int3; var reflection : SpirvReflection) : array<uint> {
    var m = make_module()
    var ctx : EmitCtx
    ctx.stage = stage

    // Fully overwrite the out-param: a caller that reuses a SpirvReflection (or retries after a
    // failed attempt) must not accumulate stale bindings / ranges from a previous call.
    reflection.stage = stage_flag(stage)
    reflection.entry_point := "main"
    reflection.local_size = stage == ShaderStage.Compute ? local_size : int3(1, 1, 1)
    reflection.bindings |> clear()
    reflection.push_constants |> clear()

    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Shader))
    emit(m, SEC_MEMMODEL, SpvOp.MemoryModel, uint(SpvAddressingModel.Logical), uint(SpvMemoryModel.GLSL450))

    collect_dependencies(fn) $(_vfun, vvar) {
        for (v in vvar) {
            classify_global(m, ctx, v, stage, reflection, errors)
        }
    }

    let t_void = type_void(m)
    let t_fn = type_function_void(m, t_void)
    let id_main = alloc_id(m)

    var ep <- [uint(execution_model(stage)), id_main]
    var nm <- string_words("main")
    ep |> push_from(nm)
    delete nm
    ep |> push_from(ctx.interface_ids)
    emit_n(m, SEC_ENTRY, SpvOp.EntryPoint, ep)
    delete ep

    // Compute pins the workgroup size; Fragment fixes the framebuffer origin; Vertex needs none.
    if (stage == ShaderStage.Compute) {
        let lx = local_size.x > 0 ? uint(local_size.x) : 1u
        let ly = local_size.y > 0 ? uint(local_size.y) : 1u
        let lz = local_size.z > 0 ? uint(local_size.z) : 1u
        var em <- [id_main, uint(SpvExecutionMode.LocalSize), lx, ly, lz]
        emit_n(m, SEC_EXECMODE, SpvOp.ExecutionMode, em)
        delete em
    } elif (stage == ShaderStage.Fragment) {
        emit(m, SEC_EXECMODE, SpvOp.ExecutionMode, id_main, uint(SpvExecutionMode.OriginUpperLeft))
    }

    emit(m, SEC_FUNCS, SpvOp.Function, t_void, id_main, 0u, t_fn)
    let id_label = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Label, id_label)
    ctx.terminated = false

    // Function-storage OpVariables must lead the entry block, before any body code.
    collect_locals(m, ctx, fn.body, errors)
    // Body codegen: the SpirvEmit AstVisitor walks fn.body, emitting into m.
    drive_emit(m, ctx, fn, errors)

    if (!ctx.terminated) {
        emit(m, SEC_FUNCS, SpvOp.Return)
    }
    emit(m, SEC_FUNCS, SpvOp.FunctionEnd)

    var words <- module_words(m)
    delete m
    delete ctx
    return <- words
}
