options gen2
options indenting = 4

// MSL text emitter (Phase-4 subset: full scalar/vector arithmetic incl. the 16/8-bit lattice,
// comparisons, logical, control flow, threadgroup memory + barriers + simdgroup intrinsics,
// ctor/convert/saturating-narrow calls). generate_msl lowers one [metal_kernel] class method to a
// Metal compute kernel: @ssbo class members become device-pointer parameters (const when no
// statement writes them), @uniform members become constant-address-space reference parameters,
// @workgroup members become threadgroup declarations at the top of the kernel body, referenced
// builtin globals become attributed parameters, the body lowers by manual recursion. 3-lane
// buffer/uniform elements take MSL's packed_T3 spelling (das packs tightly; unified memory means
// the das array IS the buffer) — loads wrap in the plain-type ctor, stores must assign the whole
// element. Every construct records a census kind at its emit site (gate B: the union across all
// test fixtures must equal declared_msl_census, both directions — text has no disassembler, so the
// census IS the emitter's coverage proxy).

module msl_emit shared public

require msl_types
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require daslib/strings_boost
require strings

struct public MslEmitConfig {
    kernel_name : string    // "" -> the method name (class-prefix stripped)
}

struct private MslMember {
    msl_name : string
    elem : string           // layout-bearing spelling (packed_T3 for 3-lane buffer/uniform elements)
    value_elem : string     // plain value spelling — wraps packed loads back into operable form
    binding : int
    written : bool
    uniform : bool
    workgroup : bool
    packed : bool           // elem != value_elem: 3-lane element behind a packed_T3 param
    wg_dim : int            // @workgroup fixed-array count; -1 = scalar/vector
    wg_bytes : uint64       // @workgroup member's MSL byte size (whole member)
    wg_off : uint64         // @workgroup member's offset inside the dynamic shmem buffer
}

struct private MslCtx {
    errors : array<string>
    census : table<string>
    members : table<string; MslMember>          // das field name -> lowering
    builtins_used : table<string>               // referenced builtin globals (gl_*)
    renames : table<string; string>             // das identifier -> MSL-safe identifier
    ptrs : table<string; string>                // pointer local (addr(member[..])) -> its member (das names)
    helpers : table<string>                     // preamble helper functions referenced by lowered calls
    self_name : string
}

// MSL reserves the C++14 keyword set plus its own address-space/type keywords; colliding
// identifiers get a trailing underscore
let private MSL_RESERVED : table<string> <- {
    "kernel", "device", "constant", "thread", "threadgroup", "vertex", "fragment",
    "half", "short", "char", "uchar", "ushort", "auto", "template", "typename",
    "using", "namespace", "constexpr", "new", "delete", "this", "class", "struct",
    "union", "signed", "unsigned", "void", "atomic_int", "atomic_uint"
}

def private msl_safe_name(name : string) : string {
    return key_exists(MSL_RESERVED, name) ? "{name}_" : name
}

// a call to a GENERIC instance arrives named `__::module`name`hash` (the sgmat load/store
// overloads take an untyped buffer parameter, so they instantiate); the base name is the
// segment before the trailing hash — plain calls pass through
def private call_base_name(cname : string) : string {
    if ((cname |> find("`")) < 0) {
        return cname
    }
    var parts <- split(cname, "`")
    let base = length(parts) >= 2 ? parts[length(parts) - 2] : cname
    delete parts
    return base
}

def private err(var ctx : MslCtx; at : LineInfo; msg : string) {
    ctx.errors |> push("{msg} at line {int(at.line)}")
}

// the dedicated 8/16-lane diagnostic every shader emitter shares (dasGlsl wording): these lattice
// forms exist on the CPU only — shader vectors cap at 4 lanes on every target
def private reject_cpu_only_width(var ctx : MslCtx; at : LineInfo; t : TypeDecl?) : bool {
    if (t != null && cpu_only_lattice_width(t.baseType)) {
        err(ctx, at, "type {t.baseType} is CPU-only (8/16-lane lattice forms do not exist in shaders — vectors cap at 4 lanes)")
        return true
    }
    return false
}

// ===== builtin globals (name-keyed, mirroring spirv_emit's classifier) =====

def private builtin_msl_info(name : string) : tuple<ok : bool; mslType : string; attr : string> {
    if (name == "gl_GlobalInvocationID") {
        return (ok = true, mslType = "uint3", attr = "thread_position_in_grid")
    } elif (name == "gl_LocalInvocationID") {
        return (ok = true, mslType = "uint3", attr = "thread_position_in_threadgroup")
    } elif (name == "gl_WorkGroupID") {
        return (ok = true, mslType = "uint3", attr = "threadgroup_position_in_grid")
    } elif (name == "gl_NumWorkGroups") {
        return (ok = true, mslType = "uint3", attr = "threadgroups_per_grid")
    } elif (name == "gl_WorkGroupSize") {
        return (ok = true, mslType = "uint3", attr = "threads_per_threadgroup")
    } elif (name == "gl_LocalInvocationIndex") {
        return (ok = true, mslType = "uint", attr = "thread_index_in_threadgroup")
    } elif (name == "gl_SubgroupInvocationID") {
        return (ok = true, mslType = "uint", attr = "thread_index_in_simdgroup")
    } elif (name == "gl_SubgroupSize") {
        return (ok = true, mslType = "uint", attr = "threads_per_simdgroup")
    } elif (name == "gl_NumSubgroups") {
        return (ok = true, mslType = "uint", attr = "simdgroups_per_threadgroup")
    } elif (name == "gl_SubgroupID") {
        return (ok = true, mslType = "uint", attr = "simdgroup_index_in_threadgroup")
    }
    return (ok = false, mslType = "", attr = "")
}

// ===== member + builtin scans =====

def private scan_members(var ctx : MslCtx; fn : FunctionPtr) {
    for (fld in fn.classParent.fields) {
        if ((fld.name == "__rtti" || fld.name == "__finalize") ||
                (fld._type != null && (fld._type.baseType == Type.tFunction || fld._type.baseType == Type.tLambda))) {
            continue
        }
        var isSsbo = false
        var isUniform = false
        var isWorkgroup = false
        var binding = -1
        var setIdx = 0
        for (annDecl in fld.annotation) {
            if (annDecl.name == "ssbo") {
                isSsbo = true
            } elif (annDecl.name == "uniform") {
                isUniform = true
            } elif (annDecl.name == "workgroup") {
                isWorkgroup = true
            } elif (annDecl.name == "binding" && annDecl.basicType == Type.tInt) {
                binding = annDecl.iValue
            } elif (annDecl.name == "set" && annDecl.basicType == Type.tInt) {
                setIdx = annDecl.iValue
            }
        }
        if (!isSsbo && !isUniform && !isWorkgroup) {
            err(ctx, fld.at, "member `{fld.name}` is neither @ssbo, @uniform, nor @workgroup — kernels support @ssbo array<T> buffers, @uniform scalar/vector values, and @workgroup threadgroup memory")
            continue
        }
        if ((isSsbo ? 1 : 0) + (isUniform ? 1 : 0) + (isWorkgroup ? 1 : 0) > 1) {
            err(ctx, fld.at, "member `{fld.name}` mixes @ssbo/@uniform/@workgroup — pick one")
            continue
        }
        if (setIdx != 0) {
            err(ctx, fld.at, "member `{fld.name}` has @set = {setIdx} — Metal has no descriptor sets; only @set = 0 (or none) is allowed")
        }
        if (isWorkgroup) {
            if (binding >= 0) {
                err(ctx, fld.at, "@workgroup member `{fld.name}` has @binding = {binding} — threadgroup memory is not a bindable buffer")
                continue
            }
            var wgElem = ""
            var wgDim = -1
            var wgElemBytes = 0ul
            if (fld._type != null && fld._type.baseType == Type.tFixedArray && fld._type.firstType != null &&
                    fld._type.firstType.baseType != Type.tFixedArray) {
                if (reject_cpu_only_width(ctx, fld.at, fld._type.firstType)) {
                    continue
                }
                wgElem = msl_type_name(fld._type.firstType)
                wgElemBytes = msl_tg_elem_bytes(fld._type.firstType)
                wgDim = fld._type.fixedDim
            } else {
                if (reject_cpu_only_width(ctx, fld.at, fld._type)) {
                    continue
                }
                wgElem = msl_type_name(fld._type)
                wgElemBytes = msl_tg_elem_bytes(fld._type)
            }
            if (empty(wgElem) || (wgDim == 0)) {
                err(ctx, fld.at, "@workgroup member `{fld.name}` must be a scalar, vector, or single-dimension fixed array of those (e.g. uint[64]); got {fld._type == null ? "?" : describe(fld._type)}")
                continue
            }
            let wgName = string(fld.name)
            ctx.renames[wgName] = msl_safe_name(wgName)
            ctx.members[wgName] = MslMember(msl_name = msl_safe_name(wgName), elem = wgElem, value_elem = wgElem,
                binding = -1, written = false, uniform = false, workgroup = true, packed = false, wg_dim = wgDim,
                wg_bytes = wgElemBytes * uint64(wgDim > 0 ? wgDim : 1))
            continue
        }
        if (binding < 0) {
            err(ctx, fld.at, "member `{fld.name}` has no @binding — every buffer needs an explicit [[buffer(N)]] index")
            continue
        }
        var elem = ""
        var valueElem = ""
        if (isSsbo) {
            if (fld._type == null || fld._type.baseType != Type.tArray) {
                err(ctx, fld.at, "@ssbo member `{fld.name}` must be array<T>; got {fld._type == null ? "?" : describe(fld._type)}")
                continue
            }
            if (reject_cpu_only_width(ctx, fld.at, fld._type.firstType)) {
                continue
            }
            elem = msl_buffer_elem_name(fld._type.firstType)
            valueElem = msl_type_name(fld._type.firstType)
            if (empty(elem)) {
                err(ctx, fld.at, "@ssbo member `{fld.name}` element type {describe(fld._type.firstType)} has no MSL form (int/uint/float scalars, the 16/8-bit lattice, their 2/3/4 vectors)")
                continue
            }
        } else {
            if (fld._type != null && fld._type.baseType == Type.tArray) {
                err(ctx, fld.at, "@uniform member `{fld.name}` must be a scalar or vector value — array members are @ssbo buffers")
                continue
            }
            if (reject_cpu_only_width(ctx, fld.at, fld._type)) {
                continue
            }
            elem = msl_buffer_elem_name(fld._type)
            valueElem = msl_type_name(fld._type)
            if (empty(elem)) {
                err(ctx, fld.at, "@uniform member `{fld.name}` type {fld._type == null ? "?" : describe(fld._type)} has no MSL form (int/uint/float scalars, the 16/8-bit lattice, their 2/3/4 vectors)")
                continue
            }
        }
        let fldName = string(fld.name)
        for (existing, m in keys(ctx.members), values(ctx.members)) {
            if (m.binding == binding) {
                err(ctx, fld.at, "members `{existing}` and `{fldName}` share @binding = {binding} — bindings must be unique within a kernel class")
            }
        }
        ctx.renames[fldName] = msl_safe_name(fldName)
        ctx.members[fldName] = MslMember(msl_name = msl_safe_name(fldName), elem = elem, value_elem = valueElem,
            binding = binding, written = false, uniform = isUniform, workgroup = false,
            packed = elem != valueElem, wg_dim = -1)
    }
    var ssboCount = 0
    for (m in values(ctx.members)) {
        if (!m.uniform && !m.workgroup) {
            ssboCount++
        }
    }
    if (empty(ctx.errors) && ssboCount == 0) {
        err(ctx, fn.at, "kernel class `{fn.classParent.name}` has no @ssbo members — a kernel with no buffers cannot observe or produce anything")
    }
}

// Walks an lvalue chain (ExprAt / ExprSwizzle / ExprRef2Value) down to the member ExprField and
// marks that member written; a write to anything else is reported by the emit pass, not here.
def private mark_write_target(var ctx : MslCtx; e : Expression?) {
    if (e == null) {
        return
    }
    if (e is ExprRef2Value) {
        mark_write_target(ctx, (e as ExprRef2Value).subexpr)
    } elif (e is ExprAt) {
        mark_write_target(ctx, (e as ExprAt).subexpr)
    } elif (e is ExprSwizzle) {
        mark_write_target(ctx, (e as ExprSwizzle).value)
    } elif (e is ExprField) {
        let name = string((e as ExprField).name)
        if (key_exists(ctx.members, name)) {
            ctx.members[name].written = true
        }
    }
}

// Pre-scan pass over the body: collects the member write-set (drives device vs device const) and
// the referenced builtin set (drives the attributed parameters). Unhandled node classes are
// reported by the emit pass — this scan stays permissive.
def private scan_body(var ctx : MslCtx; e : Expression?) {
    if (e == null) {
        return
    }
    if (e is ExprRef2Value) {
        scan_body(ctx, (e as ExprRef2Value).subexpr)
    } elif (e is ExprBlock) {
        for (s in (e as ExprBlock).list) {
            scan_body(ctx, s)
        }
    } elif (e is ExprWith) {
        scan_body(ctx, (e as ExprWith).body)
    } elif (e is ExprLet) {
        for (v in (e as ExprLet).variables) {
            scan_body(ctx, v.init)
        }
    } elif (e is ExprCopy) {
        mark_write_target(ctx, (e as ExprCopy).left)
        scan_body(ctx, (e as ExprCopy).left)
        scan_body(ctx, (e as ExprCopy).right)
    } elif (e is ExprOp2) {
        let o2 = e as ExprOp2
        if (o2._type != null && o2._type.baseType == Type.tVoid) {   // compound assignment writes its left side
            mark_write_target(ctx, o2.left)
        }
        scan_body(ctx, o2.left)
        scan_body(ctx, o2.right)
    } elif (e is ExprOp1) {
        let o1 = e as ExprOp1
        let op = string(o1.op)
        if (op == "++" || op == "--" || op == "+++" || op == "---") {
            mark_write_target(ctx, o1.subexpr)
        }
        scan_body(ctx, o1.subexpr)
    } elif (e is ExprOp3) {
        scan_body(ctx, (e as ExprOp3).subexpr)
        scan_body(ctx, (e as ExprOp3).left)
        scan_body(ctx, (e as ExprOp3).right)
    } elif (e is ExprIfThenElse) {
        scan_body(ctx, (e as ExprIfThenElse).cond)
        scan_body(ctx, (e as ExprIfThenElse).if_true)
        scan_body(ctx, (e as ExprIfThenElse).if_false)
    } elif (e is ExprWhile) {
        scan_body(ctx, (e as ExprWhile).cond)
        scan_body(ctx, (e as ExprWhile).body)
    } elif (e is ExprFor) {
        for (src in (e as ExprFor).sources) {
            scan_body(ctx, src)
        }
        scan_body(ctx, (e as ExprFor).body)
    } elif (e is ExprCall) {
        let ecs = e as ExprCall
        // simdgroup_store writes through its buffer argument — without this the write-set scan
        // would lower the parameter `device const`
        if (call_base_name("{ecs.name}") == "simdgroup_store" && length(ecs.arguments) == 4) {
            mark_write_target(ctx, ecs.arguments[1])
        }
        for (arg in ecs.arguments) {
            scan_body(ctx, arg)
        }
    } elif (e is ExprReturn) {
        scan_body(ctx, (e as ExprReturn).subexpr)
    } elif (e is ExprAt) {
        scan_body(ctx, (e as ExprAt).subexpr)
        scan_body(ctx, (e as ExprAt).index)
    } elif (e is ExprField) {
        scan_body(ctx, (e as ExprField).value)
    } elif (e is ExprSwizzle) {
        scan_body(ctx, (e as ExprSwizzle).value)
    } elif (e is ExprVar) {
        let ev = e as ExprVar
        if (!(ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block)) {
            let bi = builtin_msl_info(string(ev.name))
            if (bi.ok) {
                ctx.builtins_used |> insert(string(ev.name))
            }
        }
    }
}

// ===== expression emission (returns text; records census + errors in ctx) =====

def private note(var ctx : MslCtx; kind : string) {
    ctx.census |> insert(kind)
}

// scalar class of a type for census keys: {f,i,u}{32,16,8} scalars (+ bool), the same with a `v`
// suffix for their 2/3/4 vectors (lane count does not change the emitted text, so the census is
// lane-agnostic). Classification rides the shared shader_block_layout rails.
def private census_class(t : TypeDecl?) : string {
    if (t == null) {
        return "?"
    }
    let bt = t.baseType
    if (bt == Type.tBool) {
        return "bool"
    }
    let cls = scalar_class(bt)
    if (cls < 0) {
        return "?"
    }
    let prefix = cls == 0 ? "i" : (cls == 1 ? "u" : "f")
    let suffix = component_count(bt) > 1 ? "v" : ""
    return "{prefix}{scalar_width(bt)}{suffix}"
}

def private is_vec_class(cls : string) : bool {
    return cls |> ends_with("v")
}

def private op2_census_name(op : string) : string {
    if (op == "+") {
        return "add"
    } elif (op == "-") {
        return "sub"
    } elif (op == "*") {
        return "mul"
    } elif (op == "/") {
        return "div"
    } elif (op == "%") {
        return "mod"
    } elif (op == "&") {
        return "band"
    } elif (op == "|") {
        return "bor"
    } elif (op == "^") {
        return "bxor"
    } elif (op == "<<") {
        return "shl"
    } elif (op == ">>") {
        return "shr"
    }
    return ""
}

def private cmp_census_name(op : string) : string {
    if (op == "==") {
        return "eq"
    } elif (op == "!=") {
        return "ne"
    } elif (op == "<") {
        return "lt"
    } elif (op == "<=") {
        return "le"
    } elif (op == ">") {
        return "gt"
    } elif (op == ">=") {
        return "ge"
    }
    return ""
}

def private compound_base(op : string) : string {
    if (op == "+=") {
        return "+"
    } elif (op == "-=") {
        return "-"
    } elif (op == "*=") {
        return "*"
    } elif (op == "/=") {
        return "/"
    } elif (op == "%=") {
        return "%"
    } elif (op == "&=") {
        return "&"
    } elif (op == "|=") {
        return "|"
    } elif (op == "^=") {
        return "^"
    } elif (op == "<<=") {
        return "<<"
    } elif (op == ">>=") {
        return ">>"
    }
    return ""
}

// das interpolates uint as hex ("{1u}" is "0x1"); go through int64 for a decimal MSL literal
def private msl_uint_literal(v : uint) : string {
    return "{int64(uint64(v))}u"
}

// das prints integral floats bare ("{2f}" is "2"), and bare-int + suffix is not a valid MSL
// literal; `suffix` is "f" for float and "h" for half (the das float16 literal's MSL spelling)
def private msl_float_literal(var ctx : MslCtx; at : LineInfo; v : float; suffix : string) : string {
    let s = "{v}"
    if (s == "inf" || s == "-inf" || s == "nan") {
        err(ctx, at, "non-finite float literal has no MSL form")
        return "0.0{suffix}"
    }
    if ((s |> find(".")) < 0 && (s |> find("e")) < 0) {
        return "{s}.0{suffix}"
    }
    return "{s}{suffix}"
}

def private emit_value(var ctx : MslCtx; e : Expression?) : string {
    if (e == null) {
        err(ctx, LineInfo(), "internal: null expression")
        return "/*null*/"
    }
    if (e is ExprRef2Value) {
        return emit_value(ctx, (e as ExprRef2Value).subexpr)
    } elif (e is ExprConstInt) {
        note(ctx, "const.int")
        return "{(e as ExprConstInt).value}"
    } elif (e is ExprConstUInt) {
        note(ctx, "const.uint")
        return msl_uint_literal((e as ExprConstUInt).value)
    } elif (e is ExprConstFloat) {
        note(ctx, "const.float")
        return msl_float_literal(ctx, e.at, (e as ExprConstFloat).value, "f")
    } elif (e is ExprConstFloat16) {
        note(ctx, "const.float16")
        return msl_float_literal(ctx, e.at, float((e as ExprConstFloat16).value), "h")
    } elif (e is ExprConstBool) {
        note(ctx, "const.bool")
        return (e as ExprConstBool).value ? "true" : "false"
    } elif (e is ExprConstInt2) {
        note(ctx, "const.i32v")
        let v = (e as ExprConstInt2).value
        return "int2({v.x}, {v.y})"
    } elif (e is ExprConstInt3) {
        note(ctx, "const.i32v")
        let v = (e as ExprConstInt3).value
        return "int3({v.x}, {v.y}, {v.z})"
    } elif (e is ExprConstInt4) {
        note(ctx, "const.i32v")
        let v = (e as ExprConstInt4).value
        return "int4({v.x}, {v.y}, {v.z}, {v.w})"
    } elif (e is ExprConstUInt2) {
        note(ctx, "const.u32v")
        let v = (e as ExprConstUInt2).value
        return "uint2({msl_uint_literal(v.x)}, {msl_uint_literal(v.y)})"
    } elif (e is ExprConstUInt3) {
        note(ctx, "const.u32v")
        let v = (e as ExprConstUInt3).value
        return "uint3({msl_uint_literal(v.x)}, {msl_uint_literal(v.y)}, {msl_uint_literal(v.z)})"
    } elif (e is ExprConstUInt4) {
        note(ctx, "const.u32v")
        let v = (e as ExprConstUInt4).value
        return "uint4({msl_uint_literal(v.x)}, {msl_uint_literal(v.y)}, {msl_uint_literal(v.z)}, {msl_uint_literal(v.w)})"
    } elif (e is ExprConstFloat2) {
        note(ctx, "const.f32v")
        let v = (e as ExprConstFloat2).value
        return "float2({msl_float_literal(ctx, e.at, v.x, "f")}, {msl_float_literal(ctx, e.at, v.y, "f")})"
    } elif (e is ExprConstFloat3) {
        note(ctx, "const.f32v")
        let v = (e as ExprConstFloat3).value
        return "float3({msl_float_literal(ctx, e.at, v.x, "f")}, {msl_float_literal(ctx, e.at, v.y, "f")}, {msl_float_literal(ctx, e.at, v.z, "f")})"
    } elif (e is ExprConstFloat4) {
        note(ctx, "const.f32v")
        let v = (e as ExprConstFloat4).value
        return "float4({msl_float_literal(ctx, e.at, v.x, "f")}, {msl_float_literal(ctx, e.at, v.y, "f")}, {msl_float_literal(ctx, e.at, v.z, "f")}, {msl_float_literal(ctx, e.at, v.w, "f")})"
    } elif (e is ExprVar) {
        let ev = e as ExprVar
        let name = string(ev.name)
        // renames also catches for-loop iterators, whose ExprVar flags are not `local`
        if (ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block || key_exists(ctx.renames, name)) {
            if (name == ctx.self_name) {
                err(ctx, ev.at, "`self` used as a value — only member access (self.field) lowers to MSL")
                return "/*self*/"
            }
            note(ctx, "expr.var")
            return ctx.renames?[name] ?? name
        }
        let bi = builtin_msl_info(name)
        if (bi.ok) {
            note(ctx, "expr.builtin.{bi.attr}")
            return name
        }
        err(ctx, ev.at, "global `{name}` is not a supported builtin — kernels reach data only through @ssbo members and builtin IDs")
        return "/*{name}*/"
    } elif (e is ExprField) {
        let ef = e as ExprField
        let fieldName = string(ef.name)
        if (base_is_self(ctx, ef.value) && key_exists(ctx.members, fieldName)) {
            let m = ctx.members[fieldName]
            if (m.packed && m.uniform) {
                // a packed 3-lane uniform reads back into its plain-type value form
                note(ctx, "expr.member.packed")
                return "{m.value_elem}({m.msl_name})"
            }
            note(ctx, "expr.member")
            return m.msl_name
        }
        err(ctx, ef.at, "field access `.{fieldName}` is not a kernel-class member access — only self.<@ssbo/@uniform member> lowers to MSL")
        return "/*.{fieldName}*/"
    } elif (e is ExprCall && call_base_name("{(e as ExprCall).name}") == "i_das_ptr_add") {
        // das pointer arithmetic (`p = unsafe(p + n)`): args = (ptr, elems, elem_size); MSL
        // pointer addition element-scales the same way
        let pc = e as ExprCall
        note(ctx, "op.ptr_add")
        return "({emit_value(ctx, pc.arguments[0])} + ({emit_value(ctx, pc.arguments[1])}))"
    } elif (e is ExprAt) {
        let ea = e as ExprAt
        let pm = member_lookup(ctx, ea.subexpr)
        let sub = emit_value(ctx, ea.subexpr)
        let idx = emit_value(ctx, ea.index)
        if (!empty(pm) && ctx.members[pm].packed) {
            // a packed 3-lane element loads back into its plain-type value form (packed types
            // have no arithmetic operators of their own)
            note(ctx, "expr.index.packed")
            return "{ctx.members[pm].value_elem}({sub}[{idx}])"
        }
        note(ctx, "expr.index")
        return "{sub}[{idx}]"
    } elif (e is ExprSwizzle) {
        let sw = e as ExprSwizzle
        let sub = emit_value(ctx, sw.value)
        let compName = fixed_array<string>("x", "y", "z", "w")
        var comps : array<string>
        for (fch in sw.fields) {
            let f = int(fch)
            if (f < 0 || f > 3) {
                err(ctx, sw.at, "swizzle component {f} out of xyzw range")
                return "/*swizzle*/"
            }
            comps |> push(compName[f])
        }
        note(ctx, "expr.swizzle")
        return "{sub}.{join(comps, "")}"
    } elif (e is ExprOp2) {
        let op2 = e as ExprOp2
        let op = string(op2.op)
        if (op2._type != null && op2._type.baseType == Type.tVoid) {
            err(ctx, op2.at, "compound assignment `{op}` used as a value")
            return "/*op {op}*/"
        }
        let cmp = cmp_census_name(op)
        if (!empty(cmp)) {
            let lcls = census_class(op2.left._type)
            let l = emit_value(ctx, op2.left)
            let r = emit_value(ctx, op2.right)
            if (is_vec_class(lcls)) {
                // das vector ==/!= yields a scalar bool (all-equal / any-differ); MSL's yields boolN
                if (op == "==") {
                    note(ctx, "op.eq.{lcls}")
                    return "all({l} == {r})"
                } elif (op == "!=") {
                    note(ctx, "op.ne.{lcls}")
                    return "any({l} != {r})"
                }
                err(ctx, op2.at, "ordered comparison `{op}` on vectors has no scalar das form")
                return "/*op {op}*/"
            }
            note(ctx, "op.{cmp}.{lcls}")
            return "({l} {op} {r})"
        }
        if (op == "&&" || op == "||") {
            let l = emit_value(ctx, op2.left)
            let r = emit_value(ctx, op2.right)
            note(ctx, op == "&&" ? "op.land" : "op.lor")
            return "({l} {op} {r})"
        }
        let opName = op2_census_name(op)
        if (empty(opName)) {
            err(ctx, op2.at, "operator `{op}` is not in the supported MSL set")
            return "/*op {op}*/"
        }
        let cls = census_class(op2._type)
        if (cls == "?") {
            err(ctx, op2.at, "operator `{op}` result type {describe(op2._type)} has no MSL form")
            return "/*op {op}*/"
        }
        let l = emit_value(ctx, op2.left)
        let r = emit_value(ctx, op2.right)
        note(ctx, "op.{opName}.{cls}")
        if (op == "%" && (cls == "f32" || cls == "f32v")) {
            return "fmod({l}, {r})"     // MSL % is integral-only
        }
        return "({l} {op} {r})"
    } elif (e is ExprOp1) {
        let op1 = e as ExprOp1
        let op = string(op1.op)
        if (op == "+") {
            return emit_value(ctx, op1.subexpr)     // unary plus is the identity
        }
        let sub = emit_value(ctx, op1.subexpr)
        if (op == "-") {
            note(ctx, "op.neg.{census_class(op1._type)}")
            return "(-{sub})"
        } elif (op == "!") {
            note(ctx, "op.lnot")
            return "(!{sub})"
        } elif (op == "~") {
            note(ctx, "op.bnot.{census_class(op1._type)}")
            return "(~{sub})"
        } elif (op == "++") {
            note(ctx, "op.preinc")
            return "(++{sub})"
        } elif (op == "--") {
            note(ctx, "op.predec")
            return "(--{sub})"
        } elif (op == "+++") {
            note(ctx, "op.postinc")
            return "({sub}++)"
        } elif (op == "---") {
            note(ctx, "op.postdec")
            return "({sub}--)"
        }
        err(ctx, op1.at, "unary operator `{op}` is not in the supported MSL set")
        return "/*op {op}*/"
    } elif (e is ExprOp3) {
        let op3 = e as ExprOp3
        if (op3.op != "?") {
            err(ctx, op3.at, "ternary operator `{op3.op}` is not supported")
            return "/*op {op3.op}*/"
        }
        let c = emit_value(ctx, op3.subexpr)
        let tv = emit_value(ctx, op3.left)
        let fv = emit_value(ctx, op3.right)
        note(ctx, "op.select")
        return "({c} ? {tv} : {fv})"
    } elif (e is ExprCall) {
        return emit_call_value(ctx, e as ExprCall)
    }
    err(ctx, e.at, "expression {e.__rtti} is not in the supported MSL subset")
    return "/*{e.__rtti}*/"
}

// das spelling of a scalar/vector type — ctor/convert calls carry it as the call name
// (float4, half2, byte4, int16, ...); "" for types with no ctor surface here
def private das_type_spelling(bt : Type) : string {
    let cls = scalar_class(bt)
    if (cls < 0) {
        return ""
    }
    let w = scalar_width(bt)
    let cc = component_count(bt)
    var base = ""
    if (cls == 2) {
        base = w == 32 ? "float" : (cc > 1 ? "half" : "float16")
    } elif (cls == 0) {
        base = w == 32 ? "int" : (w == 16 ? (cc > 1 ? "short" : "int16") : (cc > 1 ? "byte" : "int8"))
    } else {
        base = w == 32 ? "uint" : (w == 16 ? (cc > 1 ? "ushort" : "uint16") : (cc > 1 ? "ubyte" : "uint8"))
    }
    return cc > 1 ? "{base}{cc}" : base
}

// simdgroup_matrix reference structs (metal_builtins) — the das struct name IS the MSL spelling
def private sgmat_type_name(t : TypeDecl?) : string {
    if (t == null || t.baseType != Type.tStructure || t.structType == null ||
            "{t.structType._module.name}" != "metal_builtins") {
        return ""
    }
    let name = "{t.structType.name}"
    return (name == "simdgroup_float8x8" || name == "simdgroup_half8x8") ? name : ""
}

def private sgmat_census_class(sgname : string) : string {
    return sgname == "simdgroup_half8x8" ? "f16" : "f32"
}

def private sgmat_elem_name(sgname : string) : string {
    return sgname == "simdgroup_half8x8" ? "half" : "float"
}

// das zero-init has one sound MSL spelling for the opaque tile types: a zero fill (= {} does not
// construct a simdgroup_matrix)
def private sgmat_zero_fill(sgname : string) : string {
    let zero = sgname == "simdgroup_half8x8" ? "0.0h" : "0.0f"
    return "make_filled_simdgroup_matrix<{sgmat_elem_name(sgname)}, 8, 8>({zero})"
}

// scalar/vector math builtins with a 1:1 MSL spelling (das math-module names). Unary set is
// float-family (abs also signed-int); min/max take any numeric class. Fastmath is a kernel-level
// property; single ops stay correctly rounded under MTLMathModeSafe (Phase-4 measured fact).
let private MSL_MATH_UNARY : table<string> <- {
    "exp", "log", "exp2", "log2", "sqrt", "rsqrt", "sin", "cos", "tanh", "abs", "saturate", "round"
}

// daslib/math_bits' same-size bit reinterprets (scalar + 2/3/4-lane) — MSL as_type<T>(x)
let private MSL_BITCASTS : table<string> <- {
    "uint_bits_to_float", "int_bits_to_float", "float_bits_to_uint", "float_bits_to_int"
}

// shader_lingua_franca's packed 4x8-bit integer dots — MSL has no dp4a builtin, so each lowers
// to a preamble helper (single evaluation of the operands; the Metal compiler vectorizes freely)
let private MSL_DOT_HELPERS : table<string; string> <- {
    "sdot4" => "static inline int __das_sdot4(uint a, uint b) \{\n    char4 va = as_type<char4>(a), vb = as_type<char4>(b);\n    return int(va.x) * int(vb.x) + int(va.y) * int(vb.y) + int(va.z) * int(vb.z) + int(va.w) * int(vb.w);\n\}",
    "usdot4" => "static inline int __das_usdot4(uint a, uint b) \{\n    uchar4 va = as_type<uchar4>(a);\n    char4 vb = as_type<char4>(b);\n    return int(va.x) * int(vb.x) + int(va.y) * int(vb.y) + int(va.z) * int(vb.z) + int(va.w) * int(vb.w);\n\}"
}

// simdgroup intrinsics, the math whitelist, + the ctor/convert/saturating-narrow family are the
// only calls with a VALUE lowering (metal_builtins declares the simd set for float/int/uint; the
// identity stub bodies never matter here — only the call AST is read)
def private emit_call_value(var ctx : MslCtx; ec : ExprCall?) : string {
    let cname = call_base_name("{ec.name}")
    let na = length(ec.arguments)
    if (key_exists(MSL_MATH_UNARY, cname)) {
        if (na != 1) {
            err(ctx, ec.at, "{cname} takes exactly one argument")
            return "/*call {cname}*/"
        }
        if (reject_cpu_only_width(ctx, ec.at, ec.arguments[0]._type)) {
            return "/*call {cname}*/"
        }
        let acls = census_class(ec.arguments[0]._type)
        let ok = (acls |> starts_with("f")) || (cname == "abs" && (acls |> starts_with("i")))
        if (!ok) {
            err(ctx, ec.at, "{cname} lowers for float{cname == "abs" ? "/signed-int" : ""} classes only — argument is {acls}")
            return "/*call {cname}*/"
        }
        let v = emit_value(ctx, ec.arguments[0])
        note(ctx, "call.math.{cname}.{acls}")
        return "{cname}({v})"
    }
    if (key_exists(MSL_BITCASTS, cname)) {
        if (na != 1) {
            err(ctx, ec.at, "{cname} takes exactly one argument")
            return "/*call {cname}*/"
        }
        if (reject_cpu_only_width(ctx, ec.at, ec._type)) {
            return "/*call {cname}*/"
        }
        let v = emit_value(ctx, ec.arguments[0])
        note(ctx, "call.bitcast.{census_class(ec.arguments[0]._type)}.{census_class(ec._type)}")
        return "as_type<{msl_type_name(ec._type)}>({v})"
    }
    if (key_exists(MSL_DOT_HELPERS, cname)) {
        if (na != 2) {
            err(ctx, ec.at, "{cname} takes exactly two arguments")
            return "/*call {cname}*/"
        }
        let a = emit_value(ctx, ec.arguments[0])
        let b = emit_value(ctx, ec.arguments[1])
        ctx.helpers |> insert(cname)
        note(ctx, "call.{cname}")
        return "__das_{cname}({a}, {b})"
    }
    if (cname == "min" || cname == "max") {
        if (na != 2) {
            err(ctx, ec.at, "{cname} takes exactly two arguments")
            return "/*call {cname}*/"
        }
        if (reject_cpu_only_width(ctx, ec.at, ec.arguments[0]._type)) {
            return "/*call {cname}*/"
        }
        let acls = census_class(ec.arguments[0]._type)
        if (!(acls |> starts_with("f")) && !(acls |> starts_with("i")) && !(acls |> starts_with("u"))) {
            err(ctx, ec.at, "{cname} lowers for numeric classes only — argument is {acls}")
            return "/*call {cname}*/"
        }
        let a = emit_value(ctx, ec.arguments[0])
        let b = emit_value(ctx, ec.arguments[1])
        note(ctx, "call.math.{cname}.{acls}")
        return "{cname}({a}, {b})"
    }
    if (cname == "simd_sum") {
        if (na != 1) {
            err(ctx, ec.at, "simd_sum takes exactly one argument")
            return "/*call {cname}*/"
        }
        let v = emit_value(ctx, ec.arguments[0])
        note(ctx, "call.simd_sum.{census_class(ec._type)}")
        return "simd_sum({v})"
    } elif (cname == "simd_shuffle" || cname == "simd_shuffle_up" ||
            cname == "simd_shuffle_down" || cname == "simd_shuffle_xor") {
        if (na != 2) {
            err(ctx, ec.at, "{cname} takes exactly two arguments")
            return "/*call {cname}*/"
        }
        let v = emit_value(ctx, ec.arguments[0])
        let l = emit_value(ctx, ec.arguments[1])
        note(ctx, "call.{cname}.{census_class(ec._type)}")
        return "{cname}({v}, {l})"
    } elif (cname == "make_filled_simdgroup_float8x8" || cname == "make_filled_simdgroup_half8x8") {
        if (na != 1) {
            err(ctx, ec.at, "{cname} takes exactly one fill value")
            return "/*call {cname}*/"
        }
        let sgname = cname == "make_filled_simdgroup_half8x8" ? "simdgroup_half8x8" : "simdgroup_float8x8"
        let v = emit_value(ctx, ec.arguments[0])
        note(ctx, "call.sgmat_fill.{sgmat_census_class(sgname)}")
        return "make_filled_simdgroup_matrix<{sgmat_elem_name(sgname)}, 8, 8>({v})"
    }
    // ctor / convert / saturating-narrow calls: the call name IS the das spelling of the result
    // type, optionally + "_sat" for the clamping narrows. A CPU-only-width ctor (half8(...),
    // byte16(...)) gets the dedicated 8/16-lane diagnostic.
    if (ec._type != null && reject_cpu_only_width(ctx, ec.at, ec._type)) {
        return "/*call {cname}*/"
    }
    let spell = ec._type != null ? das_type_spelling(ec._type.baseType) : ""
    if (!empty(spell) && na >= 1 && (cname == spell || (cname == "half" && spell == "float16"))) {
        // lanes / splat / lane-wise convert — MSL's own conversion-ctor semantics (C-truncation
        // on integer narrowing, RNE on float->half) match das
        var args : array<string>
        args |> reserve(na)
        for (arg in ec.arguments) {
            args |> push(emit_value(ctx, arg))
        }
        let dst = census_class(ec._type)
        if (na > 1) {
            note(ctx, "ctor.{dst}")
        } else {
            let src = census_class(ec.arguments[0]._type)
            if ("{src}v" == dst) {
                note(ctx, "ctor.splat.{dst}")
            } elif (src == dst) {
                note(ctx, "ctor.{dst}")
            } else {
                note(ctx, "cvt.{src}.{dst}")
            }
        }
        return "{msl_type_name(ec._type)}({join(args, ", ")})"
    }
    if (!empty(spell) && na == 1 && cname == "{spell}_sat") {
        let cls = scalar_class(ec._type.baseType)
        let w = scalar_width(ec._type.baseType)
        if ((cls == 0 || cls == 1) && w < 32) {     // fp16 never saturates by design
            let v = emit_value(ctx, ec.arguments[0])
            var lo = "0u"
            var hi = w == 16 ? "65535u" : "255u"
            if (cls == 0) {
                lo = w == 16 ? "-32768" : "-128"
                hi = w == 16 ? "32767" : "127"
            }
            note(ctx, "cvt_sat.{census_class(ec.arguments[0]._type)}.{census_class(ec._type)}")
            return "{msl_type_name(ec._type)}(clamp({v}, {lo}, {hi}))"
        }
    }
    err(ctx, ec.at, "call `{cname}` is not a supported MSL builtin — user function calls do not lower (a kernel is a single function)")
    return "/*call {cname}*/"
}

def private base_is_self(ctx : MslCtx; e : Expression?) : bool {
    if (e == null) {
        return false
    }
    if (e is ExprRef2Value) {
        return base_is_self(ctx, (e as ExprRef2Value).subexpr)
    }
    let ev = e ?as ExprVar
    return ev != null && ev.name == ctx.self_name
}

// das field name when `e` is a self.<member> access, else ""
def private member_lookup(ctx : MslCtx; e : Expression?) : string {
    if (e == null) {
        return ""
    }
    if (e is ExprRef2Value) {
        return member_lookup(ctx, (e as ExprRef2Value).subexpr)
    }
    let ef = e ?as ExprField
    if (ef == null || !base_is_self(ctx, ef.value)) {
        return ""
    }
    let name = string(ef.name)
    return key_exists(ctx.members, name) ? name : ""
}

// true when `e` (Ref2Value-peeled) indexes a packed 3-lane buffer member
def private is_packed_elem_at(ctx : MslCtx; e : Expression?) : bool {
    if (e == null) {
        return false
    }
    if (e is ExprRef2Value) {
        return is_packed_elem_at(ctx, (e as ExprRef2Value).subexpr)
    }
    if (!(e is ExprAt)) {
        return false
    }
    let pm = member_lookup(ctx, (e as ExprAt).subexpr)
    return ctx.members?[pm]?.packed ?? false
}

// true when a store target reaches INTO a packed 3-lane element (swizzle / component / nested
// index) — those have no sound MSL lowering through a packed_T3 lvalue, so they fail closed
def private lvalue_touches_packed(ctx : MslCtx; e : Expression?) : bool {
    if (e == null) {
        return false
    }
    if (e is ExprRef2Value) {
        return lvalue_touches_packed(ctx, (e as ExprRef2Value).subexpr)
    }
    if (e is ExprSwizzle) {
        return lvalue_touches_packed(ctx, (e as ExprSwizzle).value)
    }
    if (e is ExprAt) {
        return is_packed_elem_at(ctx, e) || lvalue_touches_packed(ctx, (e as ExprAt).subexpr)
    }
    return false
}

// Store-target emission: a WHOLE packed element is assignable (packed_T3 = T3 converts
// implicitly), so it emits raw — without this the value path would wrap the load in a plain-type
// ctor and the store would silently hit a temporary. Anything reaching INTO a packed element
// fails closed; everything else is the ordinary value path.
def private emit_store_target(var ctx : MslCtx; e : Expression?) : string {
    if (e != null && e is ExprRef2Value) {
        return emit_store_target(ctx, (e as ExprRef2Value).subexpr)
    }
    if (e != null && e is ExprAt && is_packed_elem_at(ctx, e)) {
        let ea = e as ExprAt
        let sub = emit_value(ctx, ea.subexpr)
        let idx = emit_value(ctx, ea.index)
        note(ctx, "store.packed")
        return "{sub}[{idx}]"
    }
    if (lvalue_touches_packed(ctx, e)) {
        err(ctx, e.at, "component or swizzle writes into a packed 3-lane buffer element are not supported — assign the whole element")
        return "/*packed store*/"
    }
    return emit_value(ctx, e)
}

// simdgroup_load(m, src, offset, stride) / simdgroup_store(m, dst, offset, stride): the buffer
// argument must be a kernel-class member — @ssbo (device) for both, @workgroup (threadgroup)
// additionally as a load source — and lowers as `name + offset`. Element-type agreement is
// enforced upstream by the das overloads (the reference body indexes src/dst with the matrix's
// element type). A das pointer local (`var p = unsafe(addr(member[..]))`) is also accepted as
// the source/destination and lowers through the pointer.
def private emit_sgmat_load_store(var ctx : MslCtx; ec : ExprCall?; indent : string; var lines : array<string>) {
    let cname = call_base_name("{ec.name}")
    let isLoad = cname == "simdgroup_load"
    let nargs = length(ec.arguments)
    if (!(nargs == 4 || (isLoad && nargs == 5))) {
        err(ctx, ec.at, "{cname} takes (matrix, buffer, offset, stride{isLoad ? "[, transpose]" : ""})")
        return
    }
    let sgname = sgmat_type_name(ec.arguments[0]._type)
    if (empty(sgname)) {
        err(ctx, ec.at, "{cname} first argument must be a simdgroup matrix")
        return
    }
    // pointer-local source: simdgroup_load(m, p, off, stride) where p = unsafe(addr(member[..]))
    var parg = ec.arguments[1]
    if (parg != null && parg is ExprRef2Value) {
        parg = (parg as ExprRef2Value).subexpr
    }
    if (parg != null && parg is ExprVar && key_exists(ctx.ptrs, "{(parg as ExprVar).name}")) {
        let pn0 = "{(parg as ExprVar).name}"
        let pmem0 = ctx.ptrs[pn0]
        let pmm = ctx.members[pmem0]
        if (!isLoad) {
            err(ctx, ec.at, "simdgroup_store through a das pointer is not supported — store to the @ssbo member directly")
            return
        }
        let pmv = emit_value(ctx, ec.arguments[0])
        let poff = emit_value(ctx, ec.arguments[2])
        let pstride = emit_value(ctx, ec.arguments[3])
        let pcls = sgmat_census_class(sgmat_type_name(ec.arguments[0]._type))
        if (nargs == 5) {
            let ptv = emit_value(ctx, ec.arguments[4])
            note(ctx, "call.sgmat_load_ptr_t.{pcls}")
            lines |> push("{indent}{cname}({pmv}, {ctx.renames[pn0]} + ({poff}), {pstride}, ulong2(0), {ptv});")
            return
        }
        note(ctx, "call.sgmat_load_ptr.{pcls}")
        lines |> push("{indent}{cname}({pmv}, {ctx.renames[pn0]} + ({poff}), {pstride});")
        return
    }
    let mem = member_lookup(ctx, ec.arguments[1])
    if (empty(mem)) {
        err(ctx, ec.at, "{cname} {isLoad ? "source" : "destination"} must be an @ssbo{isLoad ? " or @workgroup" : ""} member, or a das pointer local over one")
        return
    }
    let m = ctx.members[mem]
    if (m.uniform) {
        err(ctx, ec.at, "{cname} cannot address a @uniform member — use an @ssbo buffer")
        return
    }
    if (m.workgroup && !isLoad) {
        err(ctx, ec.at, "simdgroup_store targets an @ssbo buffer — threadgroup stores are not supported")
        return
    }
    let mv = emit_value(ctx, ec.arguments[0])
    let cls = sgmat_census_class(sgname)
    let off = emit_value(ctx, ec.arguments[2])
    let stride = emit_value(ctx, ec.arguments[3])
    if (nargs == 5) {   // the transpose-flag load: MSL's 5-arg form (origin 0, transpose_matrix)
        note(ctx, m.workgroup ? "call.sgmat_load_tg_t.{cls}" : "call.sgmat_load_t.{cls}")
        let tv = emit_value(ctx, ec.arguments[4])
        lines |> push("{indent}{cname}({mv}, {m.msl_name} + {off}, {stride}, ulong2(0), {tv});")
        return
    }
    if (isLoad) {
        note(ctx, m.workgroup ? "call.sgmat_load_tg.{cls}" : "call.sgmat_load.{cls}")
    } else {
        note(ctx, "call.sgmat_store.{cls}")
    }
    lines |> push("{indent}{cname}({mv}, {m.msl_name} + {off}, {stride});")
}

// tg_store_half4(dst, idx, v): one aligned half4 vector store into a float16 @workgroup array —
// `*(threadgroup half4*)(dst + idx) = half4(v);` (idx % 4 == 0 by contract; 8-byte alignment)
def private emit_tg_store_half4(var ctx : MslCtx; ec : ExprCall?; indent : string; var lines : array<string>) {
    if (length(ec.arguments) != 3) {
        err(ctx, ec.at, "tg_store_half4 takes (workgroup array, index, float4)")
        return
    }
    let mem = member_lookup(ctx, ec.arguments[0])
    if (empty(mem)) {
        err(ctx, ec.at, "tg_store_half4 destination must be a @workgroup member")
        return
    }
    let m = ctx.members[mem]
    if (!m.workgroup) {
        err(ctx, ec.at, "tg_store_half4 destination must be a @workgroup member")
        return
    }
    if (m.elem != "half") {
        err(ctx, ec.at, "tg_store_half4 destination must be a float16 workgroup array (got {m.elem})")
        return
    }
    let idx = emit_value(ctx, ec.arguments[1])
    let v = emit_value(ctx, ec.arguments[2])
    note(ctx, "call.tg_store_half4")
    lines |> push("{indent}*(threadgroup half4*)({m.msl_name} + ({idx})) = half4({v});")
}

// tg_store_float4(dst, idx, v): the float twin — `*(threadgroup float4*)(dst + idx) = v;`
// (idx % 4 == 0 by contract; 16-byte alignment)
def private emit_tg_store_float4(var ctx : MslCtx; ec : ExprCall?; indent : string; var lines : array<string>) {
    if (length(ec.arguments) != 3) {
        err(ctx, ec.at, "tg_store_float4 takes (workgroup array, index, float4)")
        return
    }
    let mem = member_lookup(ctx, ec.arguments[0])
    if (empty(mem)) {
        err(ctx, ec.at, "tg_store_float4 destination must be a @workgroup member")
        return
    }
    let m = ctx.members[mem]
    if (!m.workgroup) {
        err(ctx, ec.at, "tg_store_float4 destination must be a @workgroup member")
        return
    }
    if (m.elem != "float") {
        err(ctx, ec.at, "tg_store_float4 destination must be a float workgroup array (got {m.elem})")
        return
    }
    let idx = emit_value(ctx, ec.arguments[1])
    let v = emit_value(ctx, ec.arguments[2])
    note(ctx, "call.tg_store_float4")
    lines |> push("{indent}*(threadgroup float4*)({m.msl_name} + ({idx})) = {v};")
}

// simdgroup_multiply_accumulate(d, a, b, c) / simdgroup_multiply(d, a, b) — argument-type combos
// are enforced upstream by the das overload set (all-f32, all-f16, and mac's mixed f32-acc/f16-ab)
def private emit_sgmat_multiply(var ctx : MslCtx; ec : ExprCall?; indent : string; var lines : array<string>) {
    let cname = call_base_name("{ec.name}")
    let isMac = cname == "simdgroup_multiply_accumulate"
    let want = isMac ? 4 : 3
    if (length(ec.arguments) != want) {
        err(ctx, ec.at, "{cname} takes {want} simdgroup-matrix arguments")
        return
    }
    var classes : array<string>
    var parts : array<string>
    for (arg in ec.arguments) {
        let sgname = sgmat_type_name(arg._type)
        if (empty(sgname)) {
            err(ctx, arg.at, "{cname} arguments must be simdgroup matrices")
            return
        }
        classes |> push(sgmat_census_class(sgname))
        parts |> push(emit_value(ctx, arg))
    }
    let combo = (isMac && classes[0] == "f32" && classes[1] == "f16") ? "mixed" : classes[0]
    note(ctx, isMac ? "call.sgmat_mac.{combo}" : "call.sgmat_mul.{combo}")
    lines |> push("{indent}{cname}({join(parts, ", ")});")
}

// ===== statement emission =====

def private emit_stmt(var ctx : MslCtx; e : Expression?; indent : string; var lines : array<string>) {
    if (e == null) {
        return
    }
    if (e is ExprBlock) {
        for (s in (e as ExprBlock).list) {
            emit_stmt(ctx, s, indent, lines)
        }
    } elif (e is ExprWith) {
        // the typer's with(self) scope wrapper: member access inside is already explicit
        // (self.a[i]), so the wrapper itself is transparent — and census-silent, or the gate
        // would depend on whether the optimizer already dropped it
        emit_stmt(ctx, (e as ExprWith).body, indent, lines)
    } elif (e is ExprLet) {
        for (v in (e as ExprLet).variables) {
            let sgname = sgmat_type_name(v._type)
            if (!empty(sgname)) {
                let sgVarName = string(v.name)
                ctx.renames[sgVarName] = msl_safe_name(sgVarName)
                if (v.init == null) {
                    note(ctx, "stmt.var.sgmat.zero")
                    lines |> push("{indent}{sgname} {ctx.renames[sgVarName]} = {sgmat_zero_fill(sgname)};")
                } else {
                    let init = emit_value(ctx, v.init)
                    note(ctx, "stmt.var.sgmat")
                    lines |> push("{indent}{sgname} {ctx.renames[sgVarName]} = {init};")
                }
                continue
            }
            if (v.init != null && v.init is ExprRef2Ptr && (v.init as ExprRef2Ptr).subexpr is ExprAt) {
                // `var p = unsafe(addr(member[expr]))` — a das pointer INTO a kernel member.
                // Lowers to a real MSL pointer in the member's address space; the spelling
                // exists because pointer-vs-index register-allocates differently on AGX and
                // the winner is shape-dependent — both stay writable, plain das either way
                let pat = (v.init as ExprRef2Ptr).subexpr as ExprAt
                let pmem = member_lookup(ctx, pat.subexpr)
                if (empty(pmem)) {
                    err(ctx, v.at, "addr() in a kernel must take a kernel-class member element (addr(member[expr]))")
                    continue
                }
                let pm = ctx.members[pmem]
                if (pm.uniform || pm.packed) {
                    err(ctx, v.at, "addr() over a @uniform or packed 3-lane member has no MSL pointer form")
                    continue
                }
                let pname0 = string(v.name)
                ctx.renames[pname0] = msl_safe_name(pname0)
                ctx.ptrs[pname0] = pmem
                let base = emit_value(ctx, pat.index)
                let space = pm.workgroup ? "threadgroup" : (pm.written ? "device" : "device const")
                note(ctx, pm.workgroup ? "decl.ptr.tg" : "decl.ptr.dev")
                lines |> push("{indent}{space} {pm.elem} * {ctx.renames[pname0]} = {pm.msl_name} + ({base});")
                continue
            }
            if (v._type != null && v._type.baseType == Type.tFixedArray && v._type.firstType != null &&
                    v._type.firstType.baseType != Type.tFixedArray) {
                // single-dim fixed-array local; das zero-init semantics, no initializer form
                if (v.init != null) {
                    err(ctx, v.at, "fixed-array local `{v.name}` takes no initializer — das zero-init is the MSL form")
                    continue
                }
                let et = v._type.firstType
                let n = v._type.fixedDim
                let aname = string(v.name)
                ctx.renames[aname] = msl_safe_name(aname)
                let an = ctx.renames[aname]
                let asg = sgmat_type_name(et)
                if (!empty(asg)) {
                    // the opaque tile type: `= {}` does not construct it — zero-fill in a
                    // rolled pragma'd loop (the array form exists exactly for rolled math)
                    note(ctx, "stmt.var.sgmat_array.{sgmat_census_class(asg)}")
                    lines |> push("{indent}{asg} {an}[{n}];")
                    lines |> push("{indent}#pragma clang loop unroll(full)")
                    lines |> push("{indent}for (int {an}_zi = 0; {an}_zi < {n}; ++{an}_zi) \{")
                    lines |> push("{indent}    {an}[{an}_zi] = {sgmat_zero_fill(asg)};")
                    lines |> push("{indent}\}")
                    continue
                }
                if (reject_cpu_only_width(ctx, v.at, et)) {
                    continue
                }
                let ename = msl_type_name(et)
                if (empty(ename)) {
                    err(ctx, v.at, "local array `{v.name}` element type {describe(et)} has no MSL form")
                    continue
                }
                note(ctx, "stmt.var.array")
                lines |> push("{indent}{ename} {an}[{n}] = \{\};")
                continue
            }
            if (reject_cpu_only_width(ctx, v.at, v._type)) {
                continue
            }
            let tname = msl_type_name(v._type)
            if (empty(tname)) {
                err(ctx, v.at, "local `{v.name}` type {describe(v._type)} has no MSL form")
                continue
            }
            let vname = string(v.name)
            ctx.renames[vname] = msl_safe_name(vname)
            if (v.init == null) {
                // das zero-initializes locals declared without an initializer; = {} is MSL's spelling
                note(ctx, "stmt.var.zero")
                lines |> push("{indent}{tname} {ctx.renames[vname]} = \{\};")
            } else {
                let init = emit_value(ctx, v.init)
                note(ctx, v._type.flags.constant ? "stmt.let" : "stmt.var")
                lines |> push("{indent}{tname} {ctx.renames[vname]} = {init};")
            }
        }
    } elif (e is ExprCopy) {
        let ec = e as ExprCopy
        let l = emit_store_target(ctx, ec.left)
        let r = emit_value(ctx, ec.right)
        note(ctx, "stmt.assign")
        lines |> push("{indent}{l} = {r};")
    } elif (e is ExprOp2) {
        let op2 = e as ExprOp2
        let op = string(op2.op)
        let base = compound_base(op)
        if (op2._type == null || op2._type.baseType != Type.tVoid || empty(base)) {
            err(ctx, op2.at, "operator `{op}` as a statement is not an assignment")
            return
        }
        if (is_packed_elem_at(ctx, op2.left) || lvalue_touches_packed(ctx, op2.left)) {
            err(ctx, op2.at, "compound assignment into a packed 3-lane buffer element is not supported — load into a local, compute, then assign the whole element")
            return
        }
        let l = emit_value(ctx, op2.left)
        let r = emit_value(ctx, op2.right)
        let cls = census_class(op2.left._type)
        note(ctx, "op.{op2_census_name(base)}_assign.{cls}")
        if (base == "%" && (cls == "f32" || cls == "f32v")) {
            lines |> push("{indent}{l} = fmod({l}, {r});")      // MSL % is integral-only
        } else {
            lines |> push("{indent}{l} {op} {r};")
        }
    } elif (e is ExprOp1) {
        let op1 = e as ExprOp1
        let op = string(op1.op)
        let sub = emit_value(ctx, op1.subexpr)
        if (op == "++") {
            note(ctx, "op.preinc")
            lines |> push("{indent}++{sub};")
        } elif (op == "--") {
            note(ctx, "op.predec")
            lines |> push("{indent}--{sub};")
        } elif (op == "+++") {
            note(ctx, "op.postinc")
            lines |> push("{indent}{sub}++;")
        } elif (op == "---") {
            note(ctx, "op.postdec")
            lines |> push("{indent}{sub}--;")
        } else {
            err(ctx, op1.at, "unary `{op}` is not a statement")
        }
    } elif (e is ExprIfThenElse) {
        emit_if(ctx, e as ExprIfThenElse, indent, lines, "")
    } elif (e is ExprWhile) {
        let wh = e as ExprWhile
        let c = emit_value(ctx, wh.cond)
        emit_loop_hints(ctx, wh.annotations, wh.at, indent, lines)
        note(ctx, "stmt.while")
        lines |> push("{indent}while ({c}) \{")
        emit_stmt(ctx, wh.body, indent + "    ", lines)
        lines |> push("{indent}\}")
    } elif (e is ExprFor) {
        emit_for(ctx, e as ExprFor, indent, lines)
    } elif (e is ExprCall) {
        let ec = e as ExprCall
        let cname = call_base_name("{ec.name}")
        // both lower to Metal's one threadgroup rendezvous+fence: MSL has no execution-free
        // threadgroup memory fence (pre-MSL-3.2), so memoryBarrierShared() strengthens to the full
        // barrier — the same lowering SPIRV-Cross ships under MoltenVK
        if (cname == "barrier" || cname == "memoryBarrierShared") {
            if (!empty(ec.arguments)) {
                err(ctx, ec.at, "{cname}() takes no arguments")
                return
            }
            note(ctx, cname == "barrier" ? "call.barrier" : "call.memory_barrier_shared")
            lines |> push("{indent}threadgroup_barrier(mem_flags::mem_threadgroup);")
        } elif (cname == "simdgroup_load" || cname == "simdgroup_store") {
            emit_sgmat_load_store(ctx, ec, indent, lines)
        } elif (cname == "tg_store_half4") {
            emit_tg_store_half4(ctx, ec, indent, lines)
        } elif (cname == "tg_store_float4") {
            emit_tg_store_float4(ctx, ec, indent, lines)
        } elif (cname == "simdgroup_multiply_accumulate" || cname == "simdgroup_multiply") {
            emit_sgmat_multiply(ctx, ec, indent, lines)
        } else {
            err(ctx, ec.at, "call `{cname}` as a statement has no MSL lowering — only barrier()/memoryBarrierShared() sync and the simdgroup_matrix ops are supported")
        }
    } elif (e is ExprBreak) {
        note(ctx, "stmt.break")
        lines |> push("{indent}break;")
    } elif (e is ExprContinue) {
        note(ctx, "stmt.continue")
        lines |> push("{indent}continue;")
    } elif (e is ExprReturn) {
        let er = e as ExprReturn
        if (er.subexpr != null) {
            err(ctx, er.at, "kernels return void — `return <value>` has no MSL form")
            return
        }
        note(ctx, "stmt.return")
        lines |> push("{indent}return;")
    } else {
        err(ctx, e.at, "statement {e.__rtti} is not in the supported MSL subset")
    }
}

// if/elif/else chain: an if_false that is itself an ExprIfThenElse continues the chain as
// `} else if (...) {` — `opener` carries the closing brace of the previous arm
def private emit_if(var ctx : MslCtx; ite : ExprIfThenElse?; indent : string; var lines : array<string>; opener : string) {
    let c = emit_value(ctx, ite.cond)
    note(ctx, empty(opener) ? "stmt.if" : "stmt.elif")
    lines |> push("{indent}{opener}if ({c}) \{")
    emit_stmt(ctx, ite.if_true, indent + "    ", lines)
    if (ite.if_false == null) {
        lines |> push("{indent}\}")
    } elif (ite.if_false is ExprIfThenElse) {
        emit_if(ctx, ite.if_false as ExprIfThenElse, indent, lines, "\} else ")
    } else {
        note(ctx, "stmt.else")
        lines |> push("{indent}\} else \{")
        emit_stmt(ctx, ite.if_false, indent + "    ", lines)
        lines |> push("{indent}\}")
    }
}

// for (i in range(lo, hi)) -> for (T i = lo, _end_i = hi; i < _end_i; ++i). das evaluates range
// endpoints ONCE at loop entry, so a non-constant end is hoisted into a loop-scoped local (the
// comma-declaration keeps sequential same-name loops from colliding); a body that mutates memory
// the end expression reads stays das-exact. Constant ranges inline the bounds.
// Per-loop hint annotations (`for [unroll_full] (...)`) — the SAME AnnotationArgumentList
// surface the JIT lowers to !llvm.loop.* metadata (llvm_jit.das append_loop_hint_operand);
// here each recognized hint lowers to its clang loop pragma. The pragma'd loop reaches the
// Metal frontend ROLLED and the GPU backend unrolls late, register-budget-aware — the
// hand-unrolled spelling costs an occupancy tier (mul_mm: max_threads 1024 -> 832, -13%).
def private emit_loop_hints(var ctx : MslCtx; anns : AnnotationArgumentList; at : LineInfo; indent : string; var lines : array<string>) {
    for (a in anns) {
        if (a.name == "unroll") {
            note(ctx, "stmt.for.unroll")
            lines |> push("{indent}#pragma clang loop unroll(enable)")
        } elif (a.name == "unroll_full") {
            note(ctx, "stmt.for.unroll")
            lines |> push("{indent}#pragma clang loop unroll(full)")
        } elif (a.name == "unroll_disable") {
            note(ctx, "stmt.for.unroll")
            lines |> push("{indent}#pragma clang loop unroll(disable)")
        } elif (a.name == "unroll_count" && a.basicType == Type.tInt) {
            note(ctx, "stmt.for.unroll")
            lines |> push("{indent}#pragma clang loop unroll_count({a.iValue})")
        } else {
            err(ctx, at, "loop hint `{a.name}` has no MSL lowering — supported: unroll, unroll_full, unroll_disable, unroll_count=N")
        }
    }
}

def private emit_for(var ctx : MslCtx; fr : ExprFor?; indent : string; var lines : array<string>) {
    if (length(fr.iteratorVariables) != 1 || length(fr.sources) != 1) {
        err(ctx, fr.at, "only a single `for (i in range(...))` loop lowers to MSL")
        return
    }
    let iv = fr.iteratorVariables[0]
    if (iv._type == null || (iv._type.baseType != Type.tInt && iv._type.baseType != Type.tUInt)) {
        err(ctx, iv.at, "for-loop induction variable `{iv.name}` must be int or uint (range/urange source)")
        return
    }
    let isU = iv._type.baseType == Type.tUInt
    let tname = isU ? "uint" : "int"
    var lo = ""
    var hi = ""
    var hiIsConst = false
    // infer may already have folded a literal range(...) into a const-range node — handle both shapes
    if (fr.sources[0] is ExprConstRange) {
        let fromto = unsafe(reinterpret<int2>((fr.sources[0] as ExprConstRange).value))
        lo = "{fromto.x}"
        hi = "{fromto.y}"
        hiIsConst = true
    } elif (fr.sources[0] is ExprConstURange) {
        let fromto = unsafe(reinterpret<uint2>((fr.sources[0] as ExprConstURange).value))
        lo = "{fromto.x}u"
        hi = "{fromto.y}u"
        hiIsConst = true
    } else {
        let src = fr.sources[0] ?as ExprCall
        if (src == null || (src.name != "range" && src.name != "urange")) {
            err(ctx, fr.at, "for-loop source must be range(...) or urange(...)")
            return
        }
        let na = length(src.arguments)
        if (na != 1 && na != 2) {
            err(ctx, fr.at, "range with a step is not supported")
            return
        }
        lo = na == 2 ? emit_value(ctx, src.arguments[0]) : (isU ? "0u" : "0")
        hi = emit_value(ctx, src.arguments[na - 1])
        hiIsConst = src.arguments[na - 1] is ExprConstInt || src.arguments[na - 1] is ExprConstUInt
    }
    let ivName = string(iv.name)
    ctx.renames[ivName] = msl_safe_name(ivName)
    let mn = ctx.renames[ivName]
    emit_loop_hints(ctx, fr.annotations, fr.at, indent, lines)
    note(ctx, "stmt.for")
    if (hiIsConst) {
        lines |> push("{indent}for ({tname} {mn} = {lo}; {mn} < {hi}; ++{mn}) \{")
    } else {
        lines |> push("{indent}for ({tname} {mn} = {lo}, {mn}_end = {hi}; {mn} < {mn}_end; ++{mn}) \{")
    }
    emit_stmt(ctx, fr.body, indent + "    ", lines)
    lines |> push("{indent}\}")
}

// ===== kernel signature + driver =====

[macro_function]
def public kernel_entry_name(fn : FunctionPtr; cfg : MslEmitConfig) : string {
    if (!empty(cfg.kernel_name)) {
        return cfg.kernel_name
    }
    // methods arrive named `Class`method` — the MSL entry point is the bare method name
    let name = "{fn.name}"
    let tick = name |> rfind("`")
    return msl_safe_name(tick >= 0 ? (name |> slice(tick + 1)) : name)
}

//! Lower a [metal_kernel] class method to MSL text. On any error the return is "" and `errors`
//! explains; `census` collects the construct kinds emitted (gate B).
[macro_function]
def public generate_msl(fn : FunctionPtr; var errors : array<string>; cfg : MslEmitConfig; var census : table<string>; var tgmem : uint64&) : string {
    var ctx : MslCtx
    if (fn == null) {
        errors |> push("generate_msl: null function")
        return ""
    }
    if (fn.classParent == null) {
        errors |> push("[metal_kernel] `{fn.name}` is not a class method — kernels are methods of a class whose @ssbo members declare the buffers")
        return ""
    }
    if ((fn.arguments |> length) != 1) {
        errors |> push("[metal_kernel] `{fn.name}` must take no parameters beyond self — kernels read/write only @ssbo/@uniform members")
        return ""
    }
    ctx.self_name = string(fn.arguments[0].name)
    scan_members(ctx, fn)
    scan_body(ctx, fn.body)

    // @workgroup members lower to ONE dynamic [[threadgroup(0)]] buffer + derived pointers, NOT
    // static threadgroup arrays: the AGX backend constant-folds tile pointers into a static
    // array's absolute addresses, and the large offsets re-materialize as integer adds inside
    // the math-bound loop phase (ISA-measured on mul_mm: 40 vs 24 iadds/iteration, the das
    // kernel's whole residual vs llama.cpp). The opaque dynamic base keeps small carried
    // offsets. Layout: name-sorted, each member 16B-aligned; the total ships as the `_tgmem`
    // companion and the dispatch sets setThreadgroupMemoryLength(index 0) with it.
    var tg_lines : array<string>
    var wg_names <- [for (name, m in keys(ctx.members), values(ctx.members)); name; where m.workgroup]
    wg_names |> sort()
    tgmem = 0ul
    for (name in wg_names) {
        let moff = tgmem
        ctx.members[name].wg_off = moff
        let m = ctx.members[name]
        tgmem = (moff + m.wg_bytes + 15ul) & ~15ul
        let base = moff == 0ul ? "(shmem)" : "(shmem + {int64(moff)})"
        if (m.wg_dim > 0) {
            note(ctx, "decl.shmem_ptr_array")
            tg_lines |> push("    threadgroup {m.elem} * {m.msl_name} = (threadgroup {m.elem} *){base};")
        } else {
            // scalar member: the pointer takes a derived name and every access dereferences it
            note(ctx, "decl.shmem_ptr")
            let pname = "{m.msl_name}_wg"
            tg_lines |> push("    threadgroup {m.elem} * {pname} = (threadgroup {m.elem} *){base};")
            ctx.members[name].msl_name = "(*{pname})"
        }
    }

    var body_lines : array<string>
    emit_stmt(ctx, fn.body, "    ", body_lines)

    var params : array<string>
    params |> reserve((ctx.members |> length) + (ctx.builtins_used |> length))
    var ordered <- [for (name, m in keys(ctx.members), values(ctx.members)); name; where !m.workgroup]
    ordered |> sort() $(a, b : string) : bool {
        let ba = ctx.members?[a] ?. binding ?? 0
        let bb = ctx.members?[b] ?. binding ?? 0
        return ba < bb
    }
    for (name in ordered) {
        let m = ctx.members[name]
        if (m.uniform) {
            if (m.written) {
                err(ctx, fn.at, "@uniform member `{name}` is written — uniforms are read-only; outputs are @ssbo buffers")
                continue
            }
            note(ctx, m.packed ? "param.uniform_packed" : "param.uniform")
            params |> push("constant {m.elem}& {m.msl_name} [[buffer({m.binding})]]")
        } elif (m.written) {
            note(ctx, m.packed ? "param.buffer_packed" : "param.buffer")
            params |> push("device {m.elem}* {m.msl_name} [[buffer({m.binding})]]")
        } else {
            note(ctx, m.packed ? "param.buffer_const_packed" : "param.buffer_const")
            params |> push("device const {m.elem}* {m.msl_name} [[buffer({m.binding})]]")
        }
    }
    var bnames <- [for (name in keys(ctx.builtins_used)); name]
    bnames |> sort()
    for (name in bnames) {
        let bi = builtin_msl_info(name)
        note(ctx, "param.builtin.{bi.attr}")
        params |> push("{bi.mslType} {name} [[{bi.attr}]]")
    }
    if (!empty(wg_names)) {
        note(ctx, "param.shmem")
        params |> push("threadgroup char * shmem [[threadgroup(0)]]")
    }

    if (!empty(ctx.errors)) {
        errors |> push_from(ctx.errors)
        return ""
    }

    note(ctx, "preamble")
    note(ctx, "kernel_sig")
    var helper_text = ""
    if (!empty(ctx.helpers)) {
        var hs <- [for (h in keys(ctx.helpers)); MSL_DOT_HELPERS?[h] ?? ""]
        hs |> sort()    // table iteration order is hash-based — pin the emitted order
        helper_text = join(hs, "\n") + "\n"
    }
    let chunks = [
        "#include <metal_stdlib>\nusing namespace metal;\n{helper_text}kernel void {kernel_entry_name(fn, cfg)}(",
        join(params, ",\n        "),
        ") \{\n",
        empty(tg_lines) ? "" : join(tg_lines, "\n") + "\n",
        join(body_lines, "\n"),
        "\n\}\n"
    ]
    for (kind in keys(ctx.census)) {
        census |> insert(kind)
    }
    return join(chunks, "")
}

