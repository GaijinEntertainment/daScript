options gen2

// daslang AST -> SPIR-V codegen. Manual two-function recursion (emit_value / emit_ptr) rather
// than the auto AstVisitor: SPIR-V's value/pointer (SSA + OpAccessChain) model maps cleanly to
// it, and lvalue-ness is read straight off the type's ref (&) flag. ExprRef2Value wrappers can
// still appear when generate_spirv runs at the annotation's patch() stage (pre-fold); emit_value
// peels them. generate_spirv is the [macro_function] entry, called by both the shader annotation's
// patch and the unit tests, so opcode assertions hit the real codegen path.

module spirv_emit shared public

require spirv_builder
require spirv_types
require spirv_grammar
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
}

// break -> branch to `merge`, continue -> branch to `cont`. Pushed per active loop.
struct LoopTargets {
    merge : uint
    cont  : uint
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

// ===== global variable classification + emission =====
def private classify_global(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; stage : ShaderStage; var errors : array<string>) {
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
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.StorageBuffer, value_type = st, is_ssbo = true))
        return
    }
    errors |> push("unsupported global '{vname}' (expected gl_* builtin or @ssbo)")
}

// ===== emit a global ExprVar's Variable (or null) =====
def private global_var_of(e : ExpressionPtr) : Variable const? {
    let ev = e ?as ExprVar
    if (ev == null || ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block) return null
    return ev.variable
}

// ===== lvalue: produce a pointer result-id + its pointee type-id =====
def emit_ptr(var m : SpirvModule; var ctx : EmitCtx; e : ExpressionPtr; var errors : array<string>) : tuple<id : uint; pointee : uint> {
    let at = e ?as ExprAt
    if (at != null) {
        let bv = global_var_of(at.subexpr)
        if (bv == null) {
            errors |> push("indexing is only supported on a global ssbo array for now")
            return (id = 0u, pointee = 0u)
        }
        let gi = ctx.globals?[intptr(bv)] ?? GlobalInfo()
        if (!gi.is_ssbo) {
            errors |> push("only ssbo array indexing is supported for now")
            return (id = 0u, pointee = 0u)   // no AccessChain emitted: don't return an undefined id
        }
        let pointee = emit_type(m, e._type)
        let idx = emit_value(m, ctx, at.index, errors)
        let res = alloc_id(m)
        let ptr_t = type_pointer(m, gi.storage, pointee)
        var ops <- [ptr_t, res, gi.var_id, const_uint(m, 0u), idx]
        emit_n(m, SEC_FUNCS, SpvOp.AccessChain, ops)
        delete ops
        return (id = res, pointee = pointee)
    }
    let sw = e ?as ExprSwizzle
    if (sw != null) {
        let bv = global_var_of(sw.value)
        if (bv == null || length(sw.fields) != 1) {
            errors |> push("only single-component swizzle of a global is supported for now")
            return (id = 0u, pointee = 0u)
        }
        let gi = ctx.globals?[intptr(bv)] ?? GlobalInfo()
        if (gi.var_id == 0u) {
            errors |> push("swizzle target global is not classified")
            return (id = 0u, pointee = 0u)
        }
        let pointee = emit_type(m, e._type)
        let ptr_t = type_pointer(m, gi.storage, pointee)
        let res = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, gi.var_id, const_uint(m, uint(sw.fields[0])))
        return (id = res, pointee = pointee)
    }
    let lev = e ?as ExprVar
    if (lev != null && (lev.varFlags.local || lev.varFlags.argument || lev.varFlags._block)) {
        let lv = ctx.local_vars?[intptr(lev.variable)] ?? LocalVar()
        if (lv.ptr_id != 0u) {
            return (id = lv.ptr_id, pointee = lv.value_type)
        }
        errors |> push("cannot take the address of '{lev.name}': only `var` locals are addressable (Function-storage OpVariables); `let` values and arguments are not")
        return (id = 0u, pointee = 0u)
    }
    let gv = global_var_of(e)
    if (gv != null) {
        let gi = ctx.globals?[intptr(gv)] ?? GlobalInfo()
        if (gi.var_id == 0u) {
            errors |> push("global '{gv.name}' is not classified")
            return (id = 0u, pointee = 0u)
        }
        return (id = gi.var_id, pointee = gi.value_type)
    }
    errors |> push("unsupported lvalue {e.__rtti}")
    return (id = 0u, pointee = 0u)
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

// vector constructor name (float2/3/4, int2/3/4, uint2/3/4) -> lane count, else ok=false.
def private vector_ctor(name : string) : tuple<ok : bool; n : int> {
    if (name == "float2" || name == "int2" || name == "uint2") return (ok = true, n = 2)
    if (name == "float3" || name == "int3" || name == "uint3") return (ok = true, n = 3)
    if (name == "float4" || name == "int4" || name == "uint4") return (ok = true, n = 4)
    return (ok = false, n = 0)
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

// ===== ExprCall: vector constructors, dot (core OpDot), GLSL.std.450 math (OpExtInst) =====
def private emit_call(var m : SpirvModule; var ctx : EmitCtx; call : ExprCall?; e : ExpressionPtr; var errors : array<string>) : uint {
    let name = "{call.name}"
    // ----- vector constructor: OpCompositeConstruct (or splat / identity for one arg) -----
    let vc = vector_ctor(name)
    if (vc.ok) {
        let rt = emit_type(m, e._type)
        if (length(call.arguments) == 1) {
            let cc = component_count(call.arguments[0]._type.baseType)
            let aid = emit_value(m, ctx, call.arguments[0], errors)
            if (aid == 0u) return 0u
            if (cc == vc.n) return aid                  // identity: float4(someFloat4)
            if (cc == 1) {                              // splat: float4(scalar) -> repeat N lanes
                let res = alloc_id(m)
                var cons <- [rt, res]
                cons |> reserve(2 + vc.n)
                for (_l in range(vc.n)) {
                    cons |> push(aid)
                }
                emit_n(m, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
                delete cons
                return res
            }
            errors |> push("cannot construct {name} from a single {call.arguments[0]._type.baseType}")
            return 0u
        }
        // multi-arg: each constituent contributes its lanes; total must equal n
        var total = 0
        let res = alloc_id(m)
        var cons <- [rt, res]
        cons |> reserve(2 + length(call.arguments))
        for (a in call.arguments) {
            let cc = component_count(a._type.baseType)
            if (cc == 0) {
                errors |> push("{name}: argument of type {a._type.baseType} is not a scalar/vector")
                delete cons
                return 0u
            }
            total += cc
            let aid = emit_value(m, ctx, a, errors)
            if (aid == 0u) {
                delete cons
                return 0u
            }
            cons |> push(aid)
        }
        if (total != vc.n) {
            errors |> push("{name}: constituent lanes sum to {total}, expected {vc.n}")
            delete cons
            return 0u
        }
        emit_n(m, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
        delete cons
        return res
    }
    // ----- dot: core OpDot (vector . vector -> scalar) -----
    if (name == "dot") {
        if (length(call.arguments) != 2) {
            errors |> push("dot expects 2 arguments")
            return 0u
        }
        let a = emit_value(m, ctx, call.arguments[0], errors)
        let b = emit_value(m, ctx, call.arguments[1], errors)
        if (a == 0u || b == 0u) return 0u
        let rt = emit_type(m, e._type)
        let res = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.Dot, rt, res, a, b)
        return res
    }
    // ----- GLSL.std.450 math: OpExtInst -----
    let cls = empty(call.arguments) ? -1 : scalar_class(call.arguments[0]._type.baseType)
    let ge = glsl_ext_op(name, cls)
    if (ge.ok) {
        let set = ensure_glsl_ext(m, ctx)
        let rt = emit_type(m, e._type)
        let res = alloc_id(m)
        var ops <- [rt, res, set, uint(ge.op)]
        for (a in call.arguments) {
            let aid = emit_value(m, ctx, a, errors)
            if (aid == 0u) {
                delete ops
                return 0u
            }
            ops |> push(aid)
        }
        emit_n(m, SEC_FUNCS, SpvOp.ExtInst, ops)
        delete ops
        return res
    }
    errors |> push("unsupported call '{name}' (not a vector constructor, dot, or GLSL.std.450 math builtin)")
    return 0u
}

// ===== rvalue: produce an SSA result-id =====
def emit_value(var m : SpirvModule; var ctx : EmitCtx; e : ExpressionPtr; var errors : array<string>) : uint {
    // ExprRef2Value wraps a ref lvalue to read its value; running in `patch` (pre-fold) the AST
    // still carries these (the fixup-stage AST has them folded away). Just recurse on the inner
    // expression — a value-let resolves to its SSA id, a memory ref to emit_ptr + OpLoad.
    let r2v = e ?as ExprRef2Value
    if (r2v != null) return emit_value(m, ctx, r2v.subexpr, errors)
    let cu = e ?as ExprConstUInt
    if (cu != null) return const_uint(m, cu.value)
    let ci = e ?as ExprConstInt
    if (ci != null) return const_int(m, ci.value)
    let cf = e ?as ExprConstFloat
    if (cf != null) return const_float(m, cf.value)
    let op2 = e ?as ExprOp2
    if (op2 != null) {
        let ops = "{op2.op}"
        // operand type drives the signed/unsigned/float opcode choice for every op family.
        let cls = scalar_class(op2.left._type.baseType)
        let l = emit_value(m, ctx, op2.left, errors)
        let r = emit_value(m, ctx, op2.right, errors)
        if (l == 0u || r == 0u) return 0u   // operand error already reported; don't emit with id 0
        // comparison -> bool result (operands keep their scalar class)
        let cc = cmp_code(ops, cls)
        if (cc.ok) {
            let bt = type_bool(m)
            let res = alloc_id(m)
            emit(m, SEC_FUNCS, cc.code, bt, res, l, r)
            return res
        }
        // logical and/or -> bool. daslang `&&`/`||` short-circuit, but shader condition operands are
        // side-effect free, so eager OpLogicalAnd/Or is equivalent (no extra control flow needed).
        if (ops == "&&" || ops == "||") {
            let bt = type_bool(m)
            let res = alloc_id(m)
            emit(m, SEC_FUNCS, ops == "&&" ? SpvOp.LogicalAnd : SpvOp.LogicalOr, bt, res, l, r)
            return res
        }
        // arithmetic -> same type as operands
        let bo = binop_code(ops, cls)
        if (!bo.ok) {
            errors |> push("unsupported binary op '{op2.op}' on {op2.left._type.baseType}")
            return 0u   // no instruction emitted: don't return an undefined result id
        }
        let rt = emit_type(m, e._type)
        let res = alloc_id(m)
        emit(m, SEC_FUNCS, bo.code, rt, res, l, r)
        return res
    }
    let op1 = e ?as ExprOp1
    if (op1 != null) {
        let ops = "{op1.op}"
        let v = emit_value(m, ctx, op1.subexpr, errors)
        if (v == 0u) return 0u
        if (ops == "!") {       // logical not -> bool
            let bt = type_bool(m)
            let res = alloc_id(m)
            emit(m, SEC_FUNCS, SpvOp.LogicalNot, bt, res, v)
            return res
        }
        let uo = unop_code(ops, scalar_class(op1.subexpr._type.baseType))
        if (!uo.ok) {
            errors |> push("unsupported unary op '{op1.op}' on {op1.subexpr._type.baseType}")
            return 0u
        }
        let rt = emit_type(m, e._type)
        let res = alloc_id(m)
        emit(m, SEC_FUNCS, uo.code, rt, res, v)
        return res
    }
    let call = e ?as ExprCall
    if (call != null) return emit_call(m, ctx, call, e, errors)
    let ev = e ?as ExprVar
    if (ev != null && (ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block)) {
        let lv = ctx.local_vars?[intptr(ev.variable)] ?? LocalVar()
        if (lv.ptr_id != 0u) {      // memory-backed local: read through OpLoad
            let res = alloc_id(m)
            emit(m, SEC_FUNCS, SpvOp.Load, lv.value_type, res, lv.ptr_id)
            return res
        }
        let lid = ctx.locals?[intptr(ev.variable)] ?? 0u
        if (lid == 0u) {
            errors |> push("local '{ev.name}' used before it was assigned an SSA id")
        }
        return lid
    }
    if (e._type.flags.ref) {
        let p = emit_ptr(m, ctx, e, errors)
        if (p.id == 0u) return 0u   // emit_ptr already reported the error; don't OpLoad from id 0
        let res = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.Load, p.pointee, res, p.id)
        return res
    }
    errors |> push("unsupported rvalue {e.__rtti}")
    return 0u
}

// ===== local variable declaration (Function-storage OpVariable) =====
// SPIR-V requires every Function-storage OpVariable to be among the first instructions of the
// function's first block. So we pre-scan the whole body (one walk, all scopes — SPIR-V locals are
// function-scoped, not block-scoped) BEFORE emitting any body code, declaring an OpVariable for each
// mutable local (`var`) and loop-induction variable. Reads/writes then resolve to OpLoad/OpStore.
def private declare_local_var(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; var errors : array<string>) {
    if (key_exists(ctx.local_vars, intptr(v))) return
    let bt = v._type.baseType
    if (scalar_class(bt) < 0 && bt != Type.tBool) {
        // emit_type would panic on an unsupported base type; reject with a clean error instead.
        errors |> push("local '{v.name}': type {bt} is not supported yet (only 32-bit int/uint/float scalars+vectors and bool)")
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
            if (!v._type.flags.constant) declare_local_var(m, ctx, v, errors)   // `var` -> memory; `let` -> SSA
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

// load-modify-store against an lvalue: `dst <op>= rhs` and `dst++`/`dst--`.
def private emit_load_op_store(var m : SpirvModule; var ctx : EmitCtx; lhs : ExpressionPtr; code : SpvOp; rhs : uint; var errors : array<string>) {
    if (rhs == 0u) return
    let p = emit_ptr(m, ctx, lhs, errors)
    if (p.id == 0u) return
    let cur = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Load, p.pointee, cur, p.id)
    let res = alloc_id(m)
    emit(m, SEC_FUNCS, code, p.pointee, res, cur, rhs)
    emit(m, SEC_FUNCS, SpvOp.Store, p.id, res)
}

// ===== if / else (OpSelectionMerge + OpBranchConditional) =====
def private emit_if(var m : SpirvModule; var ctx : EmitCtx; ite : ExprIfThenElse?; var errors : array<string>) {
    let cond = emit_value(m, ctx, ite.cond, errors)
    if (cond == 0u) return
    let merge = alloc_id(m)
    let then_l = alloc_id(m)
    let else_l = ite.if_false != null ? alloc_id(m) : merge
    // the header block selects the merge target, then branches on the condition
    emit(m, SEC_FUNCS, SpvOp.SelectionMerge, merge, uint(SpvSelectionControl.None))
    emit(m, SEC_FUNCS, SpvOp.BranchConditional, cond, then_l, else_l)
    ctx.terminated = true
    emit(m, SEC_FUNCS, SpvOp.Label, then_l)
    ctx.terminated = false
    emit_stmt(m, ctx, ite.if_true, errors)
    if (!ctx.terminated) {
        emit(m, SEC_FUNCS, SpvOp.Branch, merge)
        ctx.terminated = true
    }
    if (ite.if_false != null) {
        emit(m, SEC_FUNCS, SpvOp.Label, else_l)
        ctx.terminated = false
        emit_stmt(m, ctx, ite.if_false, errors)
        if (!ctx.terminated) {
            emit(m, SEC_FUNCS, SpvOp.Branch, merge)
            ctx.terminated = true
        }
    }
    emit(m, SEC_FUNCS, SpvOp.Label, merge)
    ctx.terminated = false
}

// ===== loops (OpLoopMerge, 4-block: header / cond / body / continue + merge) =====
// Canonical structured-loop shape (matches glslang): the header carries OpLoopMerge then branches to
// the condition block; the condition block does OpBranchConditional body/merge; the body branches to
// the continue block; the continue block branches back to the header. break -> merge, continue -> cont.
// The shared header/cond/continue/merge skeleton. The caller supplies the condition result-id (via
// `eval_cond`, emitted into the cond block) and the body; `emit_continue` runs in the continue block
// (range-for's `i += 1`; while passes nothing). Returns nothing — terminator state is left on a fresh
// merge block.
def private emit_while(var m : SpirvModule; var ctx : EmitCtx; wh : ExprWhile?; var errors : array<string>) {
    let header = alloc_id(m)
    let cond_l = alloc_id(m)
    let body_l = alloc_id(m)
    let cont_l = alloc_id(m)
    let merge = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Branch, header)
    emit(m, SEC_FUNCS, SpvOp.Label, header)
    ctx.terminated = false
    emit(m, SEC_FUNCS, SpvOp.LoopMerge, merge, cont_l, uint(SpvLoopControl.None))
    emit(m, SEC_FUNCS, SpvOp.Branch, cond_l)
    emit(m, SEC_FUNCS, SpvOp.Label, cond_l)
    ctx.terminated = false
    let c = emit_value(m, ctx, wh.cond, errors)
    if (c == 0u) return
    emit(m, SEC_FUNCS, SpvOp.BranchConditional, c, body_l, merge)
    emit(m, SEC_FUNCS, SpvOp.Label, body_l)
    ctx.terminated = false
    ctx.loop_stack |> push(LoopTargets(merge = merge, cont = cont_l))
    emit_stmt(m, ctx, wh.body, errors)
    pop(ctx.loop_stack)
    if (!ctx.terminated) emit(m, SEC_FUNCS, SpvOp.Branch, cont_l)
    emit(m, SEC_FUNCS, SpvOp.Label, cont_l)
    ctx.terminated = false
    emit(m, SEC_FUNCS, SpvOp.Branch, header)
    emit(m, SEC_FUNCS, SpvOp.Label, merge)
    ctx.terminated = false
}

// `for (i in range(lo, hi))` -> init i = lo; loop while i < hi; continue-block does i += 1. The
// condition (`i < hi`) and update (`i += 1`) are synthesized directly against the induction
// OpVariable — there is no daslang AST node for them.
def private emit_for(var m : SpirvModule; var ctx : EmitCtx; fr : ExprFor?; var errors : array<string>) {
    if (length(fr.iteratorVariables) != 1 || length(fr.sources) != 1) {
        errors |> push("for: only a single `i in range(...)` iterator is supported for now")
        return
    }
    let iv = fr.iteratorVariables[0]
    let lvr = ctx.local_vars?[intptr(iv)] ?? LocalVar()
    if (lvr.ptr_id == 0u) {
        errors |> push("for: induction variable '{iv.name}' was not declared")
        return
    }
    let cls = scalar_class(iv._type.baseType)
    if (cls != 0 && cls != 1) {
        // induction must be a 32-bit int/uint; otherwise the synthesized `i < hi` / `i += 1` would
        // pick a Nop opcode (cmp_code/binop_code return ok=false) and emit invalid SPIR-V silently.
        errors |> push("for: induction variable '{iv.name}' must be int or uint (got {iv._type.baseType})")
        return
    }
    // bounds (lo inclusive, hi exclusive) evaluated ONCE in the pre-header — range captures them at
    // loop entry. Constant-bound range() folds to ExprConstRange (a half-open [from,to) value);
    // runtime bounds stay an ExprCall to range()/urange().
    var lo = 0u
    var hi = 0u
    let cr = fr.sources[0] ?as ExprConstRange
    if (cr != null) {
        let fromto = unsafe(reinterpret<int2>(cr.value))   // range == { from : int; to : int }
        lo = const_int_for(m, cls, fromto.x)
        hi = const_int_for(m, cls, fromto.y)
    } else {
        let src = fr.sources[0] ?as ExprCall
        if (src == null) {
            errors |> push("for: source is {fr.sources[0].__rtti}, expected range()/urange()")
            return
        }
        if ("{src.name}" != "range" && "{src.name}" != "urange") {
            errors |> push("for: source call is '{src.name}', expected range()/urange()")
            return
        }
        let na = length(src.arguments)
        if (na != 1 && na != 2) {
            errors |> push("for: only range(n) and range(lo, hi) are supported for now (no step)")
            return
        }
        lo = na == 2 ? emit_value(m, ctx, src.arguments[0], errors) : const_zero(m, cls)
        hi = emit_value(m, ctx, src.arguments[na - 1], errors)
    }
    if (lo == 0u || hi == 0u) return
    emit(m, SEC_FUNCS, SpvOp.Store, lvr.ptr_id, lo)
    let header = alloc_id(m)
    let cond_l = alloc_id(m)
    let body_l = alloc_id(m)
    let cont_l = alloc_id(m)
    let merge = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Branch, header)
    emit(m, SEC_FUNCS, SpvOp.Label, header)
    ctx.terminated = false
    emit(m, SEC_FUNCS, SpvOp.LoopMerge, merge, cont_l, uint(SpvLoopControl.None))
    emit(m, SEC_FUNCS, SpvOp.Branch, cond_l)
    emit(m, SEC_FUNCS, SpvOp.Label, cond_l)
    ctx.terminated = false
    let cur = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Load, lvr.value_type, cur, lvr.ptr_id)
    let cc = cmp_code("<", cls)
    let cres = alloc_id(m)
    emit(m, SEC_FUNCS, cc.code, type_bool(m), cres, cur, hi)
    emit(m, SEC_FUNCS, SpvOp.BranchConditional, cres, body_l, merge)
    emit(m, SEC_FUNCS, SpvOp.Label, body_l)
    ctx.terminated = false
    ctx.loop_stack |> push(LoopTargets(merge = merge, cont = cont_l))
    emit_stmt(m, ctx, fr.body, errors)
    pop(ctx.loop_stack)
    if (!ctx.terminated) emit(m, SEC_FUNCS, SpvOp.Branch, cont_l)
    emit(m, SEC_FUNCS, SpvOp.Label, cont_l)
    ctx.terminated = false
    let cur2 = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Load, lvr.value_type, cur2, lvr.ptr_id)
    let nxt = alloc_id(m)
    emit(m, SEC_FUNCS, binop_code("+", cls).code, lvr.value_type, nxt, cur2, const_one(m, cls))
    emit(m, SEC_FUNCS, SpvOp.Store, lvr.ptr_id, nxt)
    emit(m, SEC_FUNCS, SpvOp.Branch, header)
    emit(m, SEC_FUNCS, SpvOp.Label, merge)
    ctx.terminated = false
}

// ===== statement =====
def private emit_stmt(var m : SpirvModule; var ctx : EmitCtx; s : ExpressionPtr; var errors : array<string>) {
    // nested lexical block -> inline its statements (locals are function-scoped, already pre-declared)
    let blk = s ?as ExprBlock
    if (blk != null) {
        for (st in blk.list) {
            emit_stmt(m, ctx, st, errors)
            break if (ctx.terminated)   // SPIR-V forbids instructions after a block terminator
        }
        return
    }
    let ite = s ?as ExprIfThenElse
    if (ite != null) {
        emit_if(m, ctx, ite, errors)
        return
    }
    let wh = s ?as ExprWhile
    if (wh != null) {
        emit_while(m, ctx, wh, errors)
        return
    }
    let fr = s ?as ExprFor
    if (fr != null) {
        emit_for(m, ctx, fr, errors)
        return
    }
    if (s is ExprBreak) {
        if (empty(ctx.loop_stack)) {
            errors |> push("break outside of a loop")
            return
        }
        emit(m, SEC_FUNCS, SpvOp.Branch, ctx.loop_stack[length(ctx.loop_stack) - 1].merge)
        ctx.terminated = true
        return
    }
    if (s is ExprContinue) {
        if (empty(ctx.loop_stack)) {
            errors |> push("continue outside of a loop")
            return
        }
        emit(m, SEC_FUNCS, SpvOp.Branch, ctx.loop_stack[length(ctx.loop_stack) - 1].cont)
        ctx.terminated = true
        return
    }
    let cp = s ?as ExprCopy
    if (cp != null) {
        let p = emit_ptr(m, ctx, cp.left, errors)
        let val = emit_value(m, ctx, cp.right, errors)
        if (p.id == 0u || val == 0u) return   // emit_ptr/emit_value already reported; don't OpStore with id 0
        emit(m, SEC_FUNCS, SpvOp.Store, p.id, val)
        return
    }
    // compound assignment `dst <op>= rhs` arrives as a void-typed ExprOp2
    let cop2 = s ?as ExprOp2
    if (cop2 != null && cop2._type.baseType == Type.tVoid) {
        let base = compound_base("{cop2.op}")
        let bo = binop_code(base, scalar_class(cop2.left._type.baseType))
        if (!bo.ok) {
            errors |> push("unsupported compound assignment '{cop2.op}'")
            return
        }
        emit_load_op_store(m, ctx, cop2.left, bo.code, emit_value(m, ctx, cop2.right, errors), errors)
        return
    }
    // `dst++` / `dst--` arrive as a statement-level ExprOp1
    let iop1 = s ?as ExprOp1
    if (iop1 != null && ("{iop1.op}" == "++" || "{iop1.op}" == "--")) {
        let cls = scalar_class(iop1.subexpr._type.baseType)
        let bo = binop_code("{iop1.op}" == "++" ? "+" : "-", cls)
        if (!bo.ok) {
            errors |> push("unsupported {iop1.op} on {iop1.subexpr._type.baseType}")
            return
        }
        emit_load_op_store(m, ctx, iop1.subexpr, bo.code, const_one(m, cls), errors)
        return
    }
    let lt = s ?as ExprLet
    if (lt != null) {
        for (v in lt.variables) {
            if (v.init == null) {
                errors |> push("uninitialized local '{v.name}' is not supported (every let/var must have an initializer)")
                continue
            }
            let lv = ctx.local_vars?[intptr(v)] ?? LocalVar()
            if (lv.ptr_id != 0u) {                  // mutable: OpVariable pre-declared -> store init
                let val = emit_value(m, ctx, v.init, errors)
                if (val != 0u) emit(m, SEC_FUNCS, SpvOp.Store, lv.ptr_id, val)
            } else {                                // immutable value-let -> bind SSA id
                ctx.locals |> insert(intptr(v), emit_value(m, ctx, v.init, errors))
            }
        }
        return
    }
    let rt = s ?as ExprReturn
    if (rt != null) {
        emit(m, SEC_FUNCS, SpvOp.Return)
        ctx.terminated = true
        return
    }
    errors |> push("unsupported statement {s.__rtti}")
}

// ===== function body driver =====
def private emit_body(var m : SpirvModule; var ctx : EmitCtx; body : ExpressionPtr; var errors : array<string>) {
    let blk = body ?as ExprBlock
    if (blk == null) {
        errors |> push("shader body must be a block")
        return
    }
    for (s in blk.list) {
        emit_stmt(m, ctx, s, errors)
        break if (ctx.terminated)   // SPIR-V forbids instructions after a block terminator
    }
}

// ===== entry =====
def private execution_model(stage : ShaderStage) : SpvExecutionModel {
    if (stage == ShaderStage.Vertex) return SpvExecutionModel.Vertex
    if (stage == ShaderStage.Fragment) return SpvExecutionModel.Fragment
    return SpvExecutionModel.GLCompute
}

[macro_function]
def generate_spirv(fn : FunctionPtr; var errors : array<string>; stage : ShaderStage; local_size : int3) : array<uint> {
    var m = make_module()
    var ctx : EmitCtx

    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Shader))
    emit(m, SEC_MEMMODEL, SpvOp.MemoryModel, uint(SpvAddressingModel.Logical), uint(SpvMemoryModel.GLSL450))

    collect_dependencies(fn) $(_vfun, vvar) {
        for (v in vvar) {
            classify_global(m, ctx, v, stage, errors)
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
    emit_body(m, ctx, fn.body, errors)

    if (!ctx.terminated) {
        emit(m, SEC_FUNCS, SpvOp.Return)
    }
    emit(m, SEC_FUNCS, SpvOp.FunctionEnd)

    var words <- module_words(m)
    delete m
    delete ctx
    return <- words
}
