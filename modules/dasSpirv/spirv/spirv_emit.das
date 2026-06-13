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

// ===== per-global classification =====
struct GlobalInfo {
    var_id     : uint
    storage    : SpvStorageClass
    value_type : uint               // SPIR-V type-id of the whole variable's value
    is_ssbo    : bool               // wrapped in a Block-decorated struct (member 0 = the array)
}

struct EmitCtx {
    globals       : table<uint64; GlobalInfo>   // intptr(Variable) -> classification
    locals        : table<uint64; uint>         // intptr(Variable) -> SSA result-id (value lets)
    interface_ids : array<uint>                 // EntryPoint interface (Input/Output vars, SPIR-V <= 1.3)
    terminated    : bool                        // current block already has a terminator
}

def finalize(var c : EmitCtx) {
    delete c.globals
    delete c.locals
    delete c.interface_ids
}

// ===== builtin name -> SpvBuiltIn (compute subset; extended in later phases) =====
def private builtin_for_name(name : string) : tuple<ok : bool; bi : SpvBuiltIn> {
    if (name == "gl_GlobalInvocationID") return (ok = true, bi = SpvBuiltIn.GlobalInvocationId)
    if (name == "gl_LocalInvocationID") return (ok = true, bi = SpvBuiltIn.LocalInvocationId)
    if (name == "gl_WorkGroupID") return (ok = true, bi = SpvBuiltIn.WorkgroupId)
    if (name == "gl_NumWorkGroups") return (ok = true, bi = SpvBuiltIn.NumWorkgroups)
    if (name == "gl_LocalInvocationIndex") return (ok = true, bi = SpvBuiltIn.LocalInvocationIndex)
    return (ok = false, bi = SpvBuiltIn.GlobalInvocationId)
}

// ===== global variable classification + emission =====
def private classify_global(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; var errors : array<string>) {
    let vname = "{v.name}"
    let bi = builtin_for_name(vname)
    if (bi.ok) {
        let vt = emit_type(m, v._type)
        let pt = type_pointer(m, SpvStorageClass.Input, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.Input))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.BuiltIn), uint(bi.bi))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.Input, value_type = vt, is_ssbo = false))
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

// ===== rvalue: produce an SSA result-id =====
def emit_value(var m : SpirvModule; var ctx : EmitCtx; e : ExpressionPtr; var errors : array<string>) : uint {
    // ExprRef2Value wraps a ref lvalue to read its value; running in `patch` (pre-fold) the AST
    // still carries these (the fixup-stage AST has them folded away). Just recurse on the inner
    // expression — a value-let resolves to its SSA id, a memory ref to emit_ptr + OpLoad.
    let r2v = e ?as ExprRef2Value
    if (r2v != null) return emit_value(m, ctx, r2v.subexpr, errors)
    let cu = e ?as ExprConstUInt
    if (cu != null) return const_uint(m, cu.value)
    let op2 = e ?as ExprOp2
    if (op2 != null) {
        let l = emit_value(m, ctx, op2.left, errors)
        let r = emit_value(m, ctx, op2.right, errors)
        if (op2.op == "*" && e._type.isInteger) {
            let rt = emit_type(m, e._type)
            let res = alloc_id(m)
            emit(m, SEC_FUNCS, SpvOp.IMul, rt, res, l, r)
            return res
        }
        errors |> push("unsupported binary op '{op2.op}' on {e._type.baseType}")
        return 0u   // no instruction emitted: don't return an undefined result id
    }
    let ev = e ?as ExprVar
    if (ev != null && (ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block)) {
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

// ===== statement =====
def private emit_stmt(var m : SpirvModule; var ctx : EmitCtx; s : ExpressionPtr; var errors : array<string>) {
    let cp = s ?as ExprCopy
    if (cp != null) {
        let p = emit_ptr(m, ctx, cp.left, errors)
        let val = emit_value(m, ctx, cp.right, errors)
        if (p.id == 0u || val == 0u) return   // emit_ptr/emit_value already reported; don't OpStore with id 0
        emit(m, SEC_FUNCS, SpvOp.Store, p.id, val)
        return
    }
    let lt = s ?as ExprLet
    if (lt != null) {
        for (v in lt.variables) {
            if (v.init != null) {
                ctx.locals |> insert(intptr(v), emit_value(m, ctx, v.init, errors))
            } else {
                errors |> push("uninitialized let '{v.name}' not supported")
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
[macro_function]
def generate_spirv(fn : FunctionPtr; var errors : array<string>; local_size : int3) : array<uint> {
    var m = make_module()
    var ctx : EmitCtx

    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Shader))
    emit(m, SEC_MEMMODEL, SpvOp.MemoryModel, uint(SpvAddressingModel.Logical), uint(SpvMemoryModel.GLSL450))

    collect_dependencies(fn) $(_vfun, vvar) {
        for (v in vvar) {
            classify_global(m, ctx, v, errors)
        }
    }

    let t_void = type_void(m)
    let t_fn = type_function_void(m, t_void)
    let id_main = alloc_id(m)

    var ep <- [uint(SpvExecutionModel.GLCompute), id_main]
    var nm <- string_words("main")
    ep |> push_from(nm)
    delete nm
    ep |> push_from(ctx.interface_ids)
    emit_n(m, SEC_ENTRY, SpvOp.EntryPoint, ep)
    delete ep

    let lx = local_size.x > 0 ? uint(local_size.x) : 1u
    let ly = local_size.y > 0 ? uint(local_size.y) : 1u
    let lz = local_size.z > 0 ? uint(local_size.z) : 1u
    var em <- [id_main, uint(SpvExecutionMode.LocalSize), lx, ly, lz]
    emit_n(m, SEC_EXECMODE, SpvOp.ExecutionMode, em)
    delete em

    emit(m, SEC_FUNCS, SpvOp.Function, t_void, id_main, 0u, t_fn)
    let id_label = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Label, id_label)
    ctx.terminated = false

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
