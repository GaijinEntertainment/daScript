options gen2
options indenting = 4

//! the text shader emitters' shared rail (`ARCHITECTURE_EMIT.md`,
//! the shader_emit_common section)
module shader_emit_common shared public

require daslib/ast
require daslib/ast_boost
require daslib/rtti
require daslib/shader_block_layout
require daslib/strings_boost
require daslib/templates_boost
require strings

//! the scalar spelling of a daslang type's lattice class
//! (`float4` -> `float`, `half2` -> `half`, `byte4` -> `char`),
//! `bool` for bool, "" when the type has no shader form
def public shader_scalar_name(t : TypeDecl?) : string {
    if (t == null) {
        return ""
    }
    let bt = t.baseType
    if (bt == Type.tBool) {
        return "bool"
    }
    if (scalar_class(bt) < 0) {
        return ""
    }
    let w = scalar_width(bt)
    if (is_float_class(bt)) {
        return w == 32 ? "float" : "half"
    } elif (is_signed_class(bt)) {
        return w == 32 ? "int" : (w == 16 ? "short" : "char")
    }
    return w == 32 ? "uint" : (w == 16 ? "ushort" : "uchar")
}

//! the CUDA/MSL spelling of a scalar or vector type (`float4`,
//! `short2`); "" when it has none
def public shader_type_name(t : TypeDecl?) : string {
    let base = shader_scalar_name(t)
    if (empty(base)) {
        return ""
    }
    let cc = component_count(t.baseType)
    return cc > 1 ? "{base}{cc}" : base
}

//! the census class of a type: `{f,i,u}{32,16,8}`, `v`-suffixed for
//! a vector of any lane count, `bool`, or `?`
def public census_class(t : TypeDecl?) : string {
    if (t == null) {
        return "?"
    }
    let bt = t.baseType
    if (bt == Type.tBool) {
        return "bool"
    }
    if (scalar_class(bt) < 0) {
        return "?"
    }
    let prefix = is_signed_class(bt) ? "i" : (is_float_class(bt) ? "f" : "u")
    let suffix = component_count(bt) > 1 ? "v" : ""
    return "{prefix}{scalar_width(bt)}{suffix}"
}

//! a `census_class` that names a vector
def public is_vec_class(cls : string) : bool => cls |> ends_with("v")

//! the census name of a binary operator; "" for one outside the
//! emitters' set
def public op2_census_name(op : string) : string {
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

//! the census name of a comparison; "" for any other operator
def public cmp_census_name(op : string) : string {
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

//! the binary operator a compound assignment applies (`+=` -> `+`);
//! "" for a non-compound operator
def public compound_base(op : string) : string {
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

//! das spelling of a scalar/vector type (`float4`, `half2`, `byte4`,
//! `int16`) - a ctor or convert call carries it as the call name; ""
//! for a type with no ctor surface
def public das_type_spelling(bt : Type) : string {
    if (scalar_class(bt) < 0) {
        return ""
    }
    let w = scalar_width(bt)
    let cc = component_count(bt)
    var base = ""
    if (is_float_class(bt)) {
        base = w == 32 ? "float" : (cc > 1 ? "half" : "float16")
    } elif (is_signed_class(bt)) {
        base = w == 32 ? "int" : (w == 16 ? (cc > 1 ? "short" : "int16")
            : (cc > 1 ? "byte" : "int8"))
    } else {
        base = w == 32 ? "uint" : (w == 16 ? (cc > 1 ? "ushort" : "uint16")
            : (cc > 1 ? "ubyte" : "uint8"))
    }
    return cc > 1 ? "{base}{cc}" : base
}

//! das interpolates uint as hex; the decimal C literal goes through
//! int64
def public uint_literal(v : uint) : string => "{int64(uint64(v))}u"

//! a mangling hash segment: a generic instance's decimal, a
//! [template] instance's hex
def private is_hash_segment(s : string) : bool {
    return true if (s |> starts_with("0x"))
    var digits = true
    peek_data(s) $(d) {
        for (c in d) {
            digits = digits && is_number(int(c))
        }
    }
    return digits
}

//! the base name of a mangled generic or [template] instance call:
//! its last segment that is neither empty nor a hash
def public call_base_name(cname : string) : string {
    if ((cname |> find("`")) < 0) {
        return cname
    }
    var parts <- split(cname, "`")
    var base = cname
    for (k in range(length(parts))) {
        let i = length(parts) - 1 - k
        continue if (empty(parts[i]) || is_hash_segment(parts[i]))
        base = parts[i]
        break
    }
    delete parts
    return base
}

//! the key one lowered user function is discovered under: generic
//! instances share a bare name, so the module and the parameter types
//! join it and every instance emits as its own function
def public user_fn_key(fn : FunctionPtr) : string {
    return build_string() $(var w) {
        w |> write(fn._module != null ? string(fn._module.name) : "")
        w |> write("::")
        w |> write(fn.name)
        for (v in fn.arguments) {
            w |> write("|")
            w |> write(v._type != null ? describe(v._type) : "?")
        }
    }
}

//! a parameter the callee may store through: a mutable
//! buffer/array/ref, or a pointer whose POINTEE is mutable (`T?`;
//! `T const?` is read-only - the pointer's own const says nothing)
def public param_is_written(pt : TypeDecl?) : bool {
    if (pt == null) {
        return false
    }
    if (pt.baseType == Type.tPointer) {
        return pt.firstType != null && !pt.firstType.flags.constant
    }
    return (pt.baseType == Type.tArray || pt.baseType == Type.tFixedArray
        || pt.flags.ref) && !pt.flags.constant
}

//! the method-field reference of an invoke (arguments[0]),
//! Ref2Value-peeled; null when the invoke is not the
//! `type<Cls>.method` shape
def public invoke_method_field(e : Expression?) : ExprField? {
    if (e == null) {
        return null
    }
    if (e is ExprRef2Value) {
        return invoke_method_field((e as ExprRef2Value).subexpr)
    }
    return e ?as ExprField
}

//! an emitter's diagnostic, collected on `ctx.errors` with its
//! source line
def public shader_error(var ctx : auto(CT); at : LineInfo; msg : string) {
    ctx.errors |> push("{msg} at line {int(at.line)}")
}

//! a name the target language reserves, `reserved` being the
//! emitter's table, takes a trailing underscore
def public shader_safe_name(reserved : table<string>; name : string) : string =>
    key_exists(reserved, name) ? "{name}_" : name

//! an 8/16-lane lattice type, rejected: shader vectors cap at 4 lanes
//! on every target
def public reject_cpu_only_width(var ctx : auto(CT); at : LineInfo; t : TypeDecl?) : bool {
    if (t != null && cpu_only_lattice_width(t.baseType)) {
        shader_error(ctx, at, "type {t.baseType} is CPU-only (8/16-lane lattice forms do not " +
            "exist in shaders - vectors cap at 4 lanes)")
        return true
    }
    return false
}

//! the kernel's args struct of int/uint/float fields - the same
//! bytes in das and the shader - recorded in `ctx.arg_structs` once;
//! its name, or "" after an error
def public scalar_args_struct(var ctx : auto(CT); st : StructurePtr; at : LineInfo;
                              reserved : table<string>;
                              annotation_name, target_name, struct_keyword : string) : string {
    if (st == null) {
        shader_error(ctx, at, "{annotation_name} struct member has no structure type")
        return ""
    }
    let sname = shader_safe_name(reserved, string(st.name))
    return sname if (key_exists(ctx.arg_structs, sname))
    var lines : array<string>
    var bad = false
    for (f in st.fields) {
        let ft = f._type
        let tn = ft == null ? "" : shader_type_name(ft)
        if (empty(tn) || !(tn == "int" || tn == "uint" || tn == "float")) {
            shader_error(ctx, at, "{annotation_name} struct `{st.name}` field `{f.name}` is " +
                "{ft == null ? "?" : describe(ft)} - only int/uint/float fields lower (a " +
                "4-byte-scalar struct is the same bytes in das and {target_name})")
            bad = true
            continue
        }
        lines |> push("    {tn} {shader_safe_name(reserved, string(f.name))};")
    }
    if (empty(lines) && !bad) {
        shader_error(ctx, at, "{annotation_name} struct `{st.name}` has no fields")
        bad = true
    }
    if (bad) {
        delete lines
        return ""
    }
    ctx.arg_structs[sname] = "{struct_keyword} {sname} \{\n{join(lines, "\n")}\n\};"
    delete lines
    return sname
}

//! `e` is the kernel instance itself, through value and cast peels
def public is_self_ref(ctx : auto(CT); e : Expression?) : bool {
    if (e == null) {
        return false
    }
    if (e is ExprRef2Value) {
        return is_self_ref(ctx, (e as ExprRef2Value).subexpr)
    }
    if (e is ExprCast) {
        return is_self_ref(ctx, (e as ExprCast).subexpr)
    }
    let ev = e ?as ExprVar
    return ev != null && ev.name == ctx.self_name
}

//! a lowered user function's emitted name: the safe base when free,
//! else `base_N` - a method spelled like its class would be a
//! constructor, so the class name counts as taken
def public unique_fn_name(ctx : auto(CT); reserved : table<string>; base_name : string) : string {
    let base = shader_safe_name(reserved, base_name)
    var taken = ctx.kcls != null && ctx.kcls.name == base
    for (u in ctx.user_fns) {
        if (u.name == base) {
            taken = true
        }
    }
    return taken ? "{base}_{length(ctx.user_fns)}" : base
}

//! the operand of a subgroup op is a float, int or uint scalar: the
//! generic CPU stubs bind any value, the emitter is the gate
def public scalar_operand_ok(var ctx : auto(CT); ec : ExprCall?; cname : string) : bool {
    let t = ec.arguments[0]._type
    let bt = t != null ? t.baseType : Type.none
    if (bt != Type.tFloat && bt != Type.tInt && bt != Type.tUInt) {
        shader_error(ctx, ec.arguments[0].at, "{cname}: the operand is a float, int or uint " +
            "scalar, not {t != null ? describe(t) : "an untyped value"}")
        return false
    }
    return true
}

//! a kernel's entry-point name: `kernel_name` when set, else the bare
//! method name (`Class`m` -> `m`) made safe
def public shader_entry_name(fn : FunctionPtr; kernel_name : string;
                             reserved : table<string>) : string {
    if (!empty(kernel_name)) {
        return kernel_name
    }
    let name = "{fn.name}"
    let tick = name |> rfind("`")
    return shader_safe_name(reserved, tick >= 0 ? (name |> slice(tick + 1)) : name)
}

//! a kernel annotation's companion global, declared in `apply` so
//! consumer code referencing it resolves during the first inference
//! pass; false with `errors` set when the module refuses the name
[macro_function]
def public add_typed_global(name : string; bt : Type; at : LineInfo;
                            var errors : das_string) : bool {
    if (!add_global_var(compiling_module(), name, new TypeDecl(at = at, baseType = bt),
            at, false)) {
        errors := "can't add global variable {name}"
        return false
    }
    return true
}

//! the mangled method name with its backticks flattened (`Class`m` ->
//! `Class_m`): spellable from source, unique per class, so a
//! template-stamped method names its companion globals per instance
def public kernel_derived_name(func : FunctionPtr) : string => "{func.name}" |> replace("`", "_")

//! the stem of a `[*_kernel]` method's companion globals: the
//! annotation's `name =` argument, else the derived name plus
//! `suffix`
def public kernel_global_name(func : FunctionPtr; args : AnnotationArgumentList;
                              suffix : string) : string {
    let named = args |> find_arg("name") ?as tString ?? ""
    return !empty(named) ? named : "{kernel_derived_name(func)}{suffix}"
}

//! the same stem, read off the method's own `annotation`
//! declaration by a lens that finds the method after the fact
[arch(at="ARCHITECTURE_EMIT.md#shader-emit-common")]
def public kernel_global_name(func : FunctionPtr; annotation : string; suffix : string) : string {
    for (ann in func.annotations) {
        if (ann.annotation.name == annotation) {
            return kernel_global_name(func, ann.arguments, suffix)
        }
    }
    return "{kernel_derived_name(func)}{suffix}"
}

//! fills a companion global declared by `add_typed_global` with a
//! constant string node, once
[macro_function]
def public set_const_string_init(name : string; value : string; at : LineInfo) {
    var glob = find_variable(compiling_module(), name)
    if (glob != null && glob.init == null) {
        glob.init = new ExprConstString(at = at, value := value,
            _type = new TypeDecl(at = at, baseType = Type.tString))
        glob._type.flags.constant = true
    }
}

//! pins same-module reachable functions against the CPU
//! auto-inliner - a cross-module mark desyncs that module's AOT
//! registration
[macro_function, arch(at="ARCHITECTURE_EMIT.md#shader-emit-common")]
def public pin_reachable_never_inline(func : FunctionPtr) {
    collect_dependencies(func) $(vfun, _vvar) {
        for (dfn in vfun) {
            if (dfn._module == compiling_module()) {
                dfn.moreFlags.neverInline = true
            }
        }
    }
}
