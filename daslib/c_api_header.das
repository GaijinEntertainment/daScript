options gen2
options indenting = 4

module c_api_header shared private

require daslib/ast_boost
require daslib/rtti
require daslib/fio
require daslib/strings_boost
require strings


let C_KEYWORDS <- {"restrict", "_Bool", "_Complex", "_Imaginary", "_Alignas", "_Alignof",
    "_Atomic", "_Generic", "_Noreturn", "_Static_assert", "_Thread_local", "typeof"}


def public c_ident(name : string) : string {
    if (name |> empty()) {
        return ""
    }
    var out = build_string() $(writer) {
        name |> peek_data() $(bytes) {
            for (b in bytes) {
                let ch = int(b)
                let keep = (is_alpha(ch) || is_number(ch) || ch == '_')
                write_char(writer, keep ? ch : '_')
            }
        }
    }
    if (is_number(first_character(out))) {
        out = "_{out}"
    }
    if (is_cpp_keyword(out) || (C_KEYWORDS |> key_exists(out))) {
        out = "{out}_"
    }
    return out
}


def public lib_prefix_from_path(input : string) : string {
    let stem = input |> base_name() |> split(".")
    return c_ident(stem |> empty() ? "" : stem[0])
}


def public export_c_name_override(var fn : Function?) : string {
    var ann = fn |> find_annotation("export_c")
    if (ann == null) {
        return ""
    }
    for (arg in ann.arguments) {
        if (arg.name == "name" && arg.basicType == Type.tString) {
            return string(arg.sValue)
        }
    }
    return ""
}


def public has_export_c_annotation(var fn : Function?) : bool {
    return (fn |> find_annotation("export_c")) != null
}


def public c_export_symbol(prefix : string; var fn : Function?; name_override : string) : string {
    if (!(name_override |> empty())) {
        return "{prefix}_{name_override}"
    }
    let das_name = string(fn.origin != null ? fn.origin.name : fn.name)
    return "{prefix}_{c_ident(das_name)}"
}


struct public CType {
    ok : bool
    spelling : string
    by_pointer : bool
}

struct CVector {
    ok : bool
    name : string
    elem : string
    lanes : int
    span : bool
    bytes : int
}


def private is_unsigned_type(t : Type) : bool {
    return t == Type.tUInt || t == Type.tUInt8 || t == Type.tUInt16 || t == Type.tUInt64
}


def private c_int_name(is_signed : bool; bytes : int) : string {
    return "{is_signed ? "" : "u"}int{bytes * 8}_t"
}


def private scalar_of(t : TypeDeclPtr) : string {
    if (t.isBool) {
        return "bool"
    }
    if (t.isInteger) {
        return c_int_name(t.isSignedInteger, t.sizeOf)
    }
    if (t.isBitfield) {
        return c_int_name(false, t.sizeOf)
    }
    if (t.isFloatOrDouble) {
        return t.sizeOf == 4 ? "float" : "double"
    }
    return ""
}


def private vector_of(var t : TypeDeclPtr) : CVector {
    if (t == null || !t.isVectorType) {
        return CVector(ok = false)
    }
    let lanes = t.vectorDim
    let bytes = t.sizeOf / lanes
    let base = t.isRange ? t.rangeBaseType : t.vectorBaseType
    let is_unsigned = is_unsigned_type(base)
    var elem = c_int_name(!is_unsigned, bytes)
    var name = "{is_unsigned ? "uint" : "int"}{lanes}"
    if (base == Type.tFloat || base == Type.tDouble) {
        elem = bytes == 4 ? "float" : "double"
        name = "{elem}{lanes}"
    }
    if (t.isRange) {
        name = "{is_unsigned ? "u" : ""}range{bytes == 8 ? "64" : ""}"
    }
    return CVector(ok = true, name = name, elem = elem, lanes = lanes, span = t.isRange, bytes = t.sizeOf)
}


let VEC_LANE_NAMES <- ["x", "y", "z", "w"]
let SPAN_LANE_NAMES <- ["from", "to"]


def private vector_field_names(v : CVector) : array<string> {
    return [for (i in range(v.lanes)); v.span ? SPAN_LANE_NAMES[i] : VEC_LANE_NAMES[i]]
}


struct public CNames {
    prefix : string
    @do_not_delete this_module : Module?
}


def private c_type_name(names : CNames; var owner : Module?; name : das_string) : string {
    if (owner == null || owner == names.this_module || owner.name |> empty()) {
        return "{names.prefix}_{c_ident(string(name))}"
    }
    return "{names.prefix}_{c_ident(string(owner.name))}_{c_ident(string(name))}"
}


def private peel(var t : TypeDeclPtr) : TypeDeclPtr {
    if (t.baseType == Type.tDistinct && t.firstType != null) {
        return peel(t.firstType)
    }
    return t
}


def private is_pod_struct(var st : Structure?) : bool {
    return st != null && !st.flags.isClass && !(st.fields |> empty())
}


def private pointee_ok(var t : TypeDeclPtr) : bool {
    if (t == null) {
        return true
    }
    var pt = peel(t)
    if (pt.isStructure) {
        return is_pod_struct(pt.structType)
    }
    if (pt.isEnumT) {
        return pt.enumType != null && !pt.enumType.external
    }
    return !(scalar_of(pt) |> empty()) || vector_of(pt).ok || pt.isVoid
}


def private pointer_c_type(var t : TypeDeclPtr; names : CNames) : CType {
    var target = t.firstType
    if (target == null || target.isVoid || !pointee_ok(target)) {
        return CType(ok = true, spelling = "void *")
    }
    var pt = peel(target)
    var inner = ""
    if (pt.isStructure) {
        inner = c_type_name(names, pt.structType._module, pt.structType.name)
    } elif (pt.isEnumT) {
        inner = c_type_name(names, pt.enumType._module, pt.enumType.name)
    } else {
        let v = vector_of(pt)
        inner = v.ok ? "{names.prefix}_{v.name}" : scalar_of(pt)
    }
    let qual = pt.flags.constant ? "const " : ""
    return CType(ok = true, spelling = "{qual}{inner} *")
}


def private field_c_type(var t : TypeDeclPtr; names : CNames; var visiting : table<string>) : CType {
    var ft = peel(t)
    if (ft.baseType == Type.tFixedArray) {
        let inner = ft.firstType == null ? CType(ok = false) : field_c_type(ft.firstType, names, visiting)
        return inner.ok && !inner.by_pointer ? CType(ok = true, spelling = inner.spelling) : CType(ok = false)
    }
    if (ft.isStructure) {
        if (!struct_c_ok(ft.structType, names, visiting)) {
            return CType(ok = false)
        }
        return CType(ok = true, spelling = c_type_name(names, ft.structType._module, ft.structType.name))
    }
    return value_c_type(ft, names)
}


def private struct_c_ok(var st : Structure?; names : CNames; var visiting : table<string>) : bool {
    if (!is_pod_struct(st)) {
        return false
    }
    let key = "{st._module.name}::{st.name}"
    if (visiting |> key_exists(key)) {
        return true
    }
    visiting |> insert(key)
    var ok = true
    for (fld in st.fields) {
        if (!field_c_type(fld._type, names, visiting).ok) {
            ok = false
            break
        }
    }
    visiting |> erase(key)
    return ok
}


def private value_c_type(var t : TypeDeclPtr; names : CNames) : CType {
    if (t.isPointer) {
        return pointer_c_type(t, names)
    }
    if (t.isString) {
        return CType(ok = true, spelling = "const char *")
    }
    if (t.isEnumT) {
        if (t.enumType == null || t.enumType.external) {
            return CType(ok = false)
        }
        return CType(ok = true, spelling = c_type_name(names, t.enumType._module, t.enumType.name))
    }
    let v = vector_of(t)
    if (v.ok) {
        return CType(ok = true, spelling = "{names.prefix}_{v.name}", by_pointer = true)
    }
    let sc = scalar_of(t)
    return sc |> empty() ? CType(ok = false) : CType(ok = true, spelling = sc)
}


def public c_type_of(var t : TypeDeclPtr; names : CNames) : CType {
    if (t == null) {
        return CType(ok = false)
    }
    var pt = peel(t)
    if (pt.isVoid) {
        return CType(ok = true, spelling = "void")
    }
    if (pt.isStructure) {
        var visiting : table<string>
        if (!struct_c_ok(pt.structType, names, visiting)) {
            return CType(ok = false)
        }
        return CType(ok = true, spelling = c_type_name(names, pt.structType._module, pt.structType.name), by_pointer = true)
    }
    if (pt.baseType == Type.tFixedArray) {
        var visiting : table<string>
        let inner = pt.firstType == null ? CType(ok = false) : field_c_type(pt.firstType, names, visiting)
        return inner.ok ? CType(ok = true, spelling = inner.spelling, by_pointer = true) : CType(ok = false)
    }
    return value_c_type(pt, names)
}


struct public CParam {
    name : string
    c_type : string
    by_pointer : bool
    is_const : bool
    pointer_in_impl : bool
}


struct public CResult {
    c_type : string
    via_out : bool
    cmres : bool
}


struct public CExport {
    @do_not_delete fn : Function?
    c_name : string
    das_signature : string
    params : array<CParam>
    result : CResult
}


struct CReject {
    ok : bool
    what : string
    why : string
}


def private param_name_of(raw : string; index : int) : string {
    let n = c_ident(raw)
    if (n |> empty()) {
        return "a{index}"
    }
    return n == "ctx" || n == "out" ? "{n}_" : n
}


def private describe_params(var fn : Function?; names : CNames; var out : array<CParam>) : CReject {
    for (arg, i in fn.arguments, count()) {
        let ct = c_type_of(arg._type, names)
        if (!ct.ok || arg._type.isVoid) {
            return CReject(what = "parameter `{arg.name}`", why = arg._type.describe())
        }
        let by_ptr = ct.by_pointer || arg._type.flags.ref
        out |> emplace(CParam(name = param_name_of(string(arg.name), i),
            c_type = ct.spelling,
            by_pointer = by_ptr,
            is_const = by_ptr && arg._type.flags.constant,
            pointer_in_impl = arg._type.isRef))
    }
    return CReject(ok = true)
}


def private returns_cmres(var fn : Function?) : bool {
    return fn.flags.copyOnReturn || fn.flags.moveOnReturn
}


def public describe_c_signature(var fn : Function?; names : CNames; c_name : string) : tuple<exp : CExport; reject : CReject> {
    var params : array<CParam>
    let bad = describe_params(fn, names, params)
    if (!bad.ok) {
        return (exp = CExport(), reject = bad)
    }
    let rt = c_type_of(fn.result, names)
    if (!rt.ok || peel(fn.result).baseType == Type.tFixedArray) {
        return (exp = CExport(), reject = CReject(what = "result", why = fn.result.describe()))
    }
    let res = CResult(c_type = rt.spelling, via_out = rt.by_pointer, cmres = returns_cmres(fn))
    return (exp = CExport(fn = fn, c_name = c_name, das_signature = das_signature_of(fn),
        params <- params, result = res), reject = CReject(ok = true))
}


def private das_signature_of(var fn : Function?) : string {
    let args = [for (a in fn.arguments); "{a.name} : {a._type.describe() |> replace(" const", "")}"]
    return "def {fn.name}({args |> join("; ")}) : {fn.result.describe() |> replace(" const", "")}"
}


struct CEnum {
    @do_not_delete en : Enumeration?
    base : string
}


struct CTypeSet {
    enums : array<CEnum>
    vectors : array<CVector>
    structs : array<Structure?>
    seen : table<string>
}


def private collect_type(var t : TypeDeclPtr; names : CNames; var set : CTypeSet) {
    if (t == null) {
        return
    }
    var pt = peel(t)
    if (pt.baseType == Type.tFixedArray || pt.isPointer) {
        collect_type(pt.firstType, names, set)
        return
    }
    if (pt.isEnumT && pt.enumType != null && !pt.enumType.external) {
        let key = "e:{pt.enumType._module.name}::{pt.enumType.name}"
        if (!(set.seen |> key_exists(key))) {
            set.seen |> insert(key)
            set.enums |> push(CEnum(en = pt.enumType,
                base = c_int_name(!is_unsigned_type(pt.enumType.baseType), pt.sizeOf)))
        }
        return
    }
    let v = vector_of(pt)
    if (v.ok) {
        let key = "v:{v.name}"
        if (!(set.seen |> key_exists(key))) {
            set.seen |> insert(key)
            set.vectors |> push(v)
        }
        return
    }
    if (pt.isStructure) {
        collect_struct(pt.structType, names, set)
    }
}


def private collect_struct(var st : Structure?; names : CNames; var set : CTypeSet) {
    if (st == null) {
        return
    }
    let key = "s:{st._module.name}::{st.name}"
    if (set.seen |> key_exists(key)) {
        return
    }
    set.seen |> insert(key)
    for (fld in st.fields) {
        collect_type(fld._type, names, set)
    }
    set.structs |> push(st)
}


def public collect_c_types(var exports : array<CExport>; names : CNames) : CTypeSet {
    var set : CTypeSet
    for (e in exports) {
        for (arg in e.fn.arguments) {
            collect_type(arg._type, names, set)
        }
        collect_type(e.fn.result, names, set)
    }
    return <- set
}


def private enum_value_of(var value : ExpressionPtr) : string {
    if (value == null) {
        return "0"
    }
    if (value is ExprConstInt8) { return "{(value as ExprConstInt8).value}" }
    if (value is ExprConstUInt8) { return "{(value as ExprConstUInt8).value}u" }
    if (value is ExprConstInt16) { return "{(value as ExprConstInt16).value}" }
    if (value is ExprConstUInt16) { return "{(value as ExprConstUInt16).value}u" }
    if (value is ExprConstInt) { return "{(value as ExprConstInt).value}" }
    if (value is ExprConstUInt) { return "{(value as ExprConstUInt).value}u" }
    if (value is ExprConstInt64) { return "INT64_C({(value as ExprConstInt64).value})" }
    if (value is ExprConstUInt64) { return "UINT64_C({(value as ExprConstUInt64).value})" }
    return "0"
}


def private write_enum(var writer : StringBuilderWriter; var e : CEnum; names : CNames) {
    var en = e.en
    let cname = c_type_name(names, en._module, en.name)
    write(writer, "\n// das: enum {en.name}\ntypedef {e.base} {cname};\n")
    write(writer, "enum \{\n")
    let entries = [for (ee in en.list); "    {cname}_{c_ident(string(ee.name))} = {enum_value_of(ee.value)}"]
    write(writer, "{entries |> join(",\n")}\n\};\n")
}


def private write_vector(var writer : StringBuilderWriter; v : CVector; names : CNames) {
    let cname = "{names.prefix}_{v.name}"
    let fields = vector_field_names(v) |> join(", ")
    write(writer, "\n// das: {v.name}\ntypedef struct \{ {v.elem} {fields}; \} {cname};\n")
    write(writer, "{names.prefix |> to_upper()}_STATIC_ASSERT(sizeof({cname}) == {v.bytes}, \"{cname}: size differs from the daslang layout\");\n")
}


def private field_decl(var fld : FieldDeclaration; names : CNames) : string {
    var visiting : table<string>
    var ft = peel(fld._type)
    var dims : array<string>
    while (ft.baseType == Type.tFixedArray && ft.firstType != null) {
        dims |> push("[{ft.fixedDim}]")
        ft = peel(ft.firstType)
    }
    let ct = field_c_type(fld._type, names, visiting)
    return "    {ct.spelling} {c_ident(string(fld.name))}{dims |> join("")};"
}


def private write_struct(var writer : StringBuilderWriter; var st : Structure?; names : CNames) {
    let cname = c_type_name(names, st._module, st.name)
    let guard = names.prefix |> to_upper()
    write(writer, "\n// das: struct {st.name}\nstruct {cname} \{\n")
    for (fld in st.fields) {
        write(writer, "{field_decl(fld, names)}\n")
    }
    write(writer, "\};\n")
    write(writer, "{guard}_STATIC_ASSERT(sizeof({cname}) == {st.sizeOf}, \"{cname}: size differs from the daslang layout\");\n")
    for (fld in st.fields) {
        write(writer, "{guard}_STATIC_ASSERT(offsetof({cname}, {c_ident(string(fld.name))}) == {fld.offset}, \"{cname}.{fld.name}: offset differs from the daslang layout\");\n")
    }
}


def private param_decl(p : CParam) : string {
    if (!p.by_pointer) {
        return "{p.c_type} {p.name}"
    }
    return "{p.is_const ? "const " : ""}{p.c_type} * {p.name}"
}


def public c_declaration(e : CExport; names : CNames) : string {
    var args = ["{names.prefix}_ctx * ctx"]
    args |> reserve(length(e.params) + 2)
    for (p in e.params) {
        args |> push(param_decl(p))
    }
    if (e.result.via_out) {
        args |> push("{e.result.c_type} * out")
    }
    let ret = e.result.via_out ? "void" : e.result.c_type
    return "{ret} {e.c_name}({args |> join(", ")})"
}


def private write_prologue(var writer : StringBuilderWriter; names : CNames; input_path, output_path : string) {
    let guard = names.prefix |> to_upper()
    write(writer, "// Code generated by `daslang -lib {input_path} -output {output_path}`. DO NOT EDIT.\n")
    write(writer, "// Layouts are asserted for the daslang that generated this header; regenerate it for another target.\n")
    write(writer, "#pragma once\n\n#include <stdint.h>\n#include <stdbool.h>\n#include <stddef.h>\n\n")
    write(writer, "#if defined(__cplusplus)\n#define {guard}_STATIC_ASSERT(cond, msg) static_assert(cond, msg)\n")
    write(writer, "#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L\n")
    write(writer, "#define {guard}_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)\n")
    write(writer, "#else\n#define {guard}_STATIC_ASSERT(cond, msg)\n#endif\n\n")
    write(writer, "#ifdef __cplusplus\nextern \"C\" \{\n#endif\n")
}


def private write_fixed_api(var writer : StringBuilderWriter; names : CNames) {
    let p = names.prefix
    write(writer, "\n// One library instance and everything it owns: globals, heap, string heap. Drive one\n")
    write(writer, "// instance from one thread at a time, or make one per thread.\n")
    write(writer, "typedef struct {p}_ctx {p}_ctx;\n")
    write(writer, "\n// Creates an instance, then runs the script's global initializers and its [init] functions.\n")
    write(writer, "// Callable from any thread, and from a process that already carries a daslang runtime -\n")
    write(writer, "// another such library, or a host that registered the daslang modules itself. Returns NULL\n")
    write(writer, "// when the initializers raised; {p}_last_error(NULL) reports why.\n")
    write(writer, "{p}_ctx * {p}_create(void);\n")
    write(writer, "\n// Runs the script's [finalize] functions, then frees the instance and everything it\n")
    write(writer, "// allocated - every string and every pointer it returned included.\n")
    write(writer, "void {p}_destroy({p}_ctx * ctx);\n")
    write(writer, "\n// NULL when the last call on `ctx` completed normally; otherwise that call's exception text,\n")
    write(writer, "// owned by the instance and valid until the next call on it. Pass NULL to read a failed\n")
    write(writer, "// {p}_create. On a failed call the returned value is 0/false/NULL and `out` is left untouched.\n")
    write(writer, "const char * {p}_last_error({p}_ctx * ctx);\n")
    write(writer, "\n// Drains the daslang runtime for the whole process. Optional, and final: no instance of any\n")
    write(writer, "// daslang library may be created or called afterwards.\n")
    write(writer, "void {p}_shutdown_runtime(void);\n")
    write(writer, "\n// A `const char *` an exported function returns points into the instance's string heap, and a\n")
    write(writer, "// returned `T *` into its object heap: copy what you need past the next call on that instance,\n")
    write(writer, "// because a later call may collect. The empty daslang string comes back NULL. A pointer you\n")
    write(writer, "// pass in, `const char *` included, is borrowed for the duration of the call.\n")
}


def public build_c_header(var exports : array<CExport>; names : CNames; input_path, output_path, link_note : string) : string {
    var types <- collect_c_types(exports, names)
    return build_string() $(var writer) {
        write_prologue(writer, names, input_path, output_path)
        write_fixed_api(writer, names)
        for (en in types.enums) {
            write_enum(writer, en, names)
        }
        for (vt in types.vectors) {
            write_vector(writer, vt, names)
        }
        if (!(types.structs |> empty())) {
            write(writer, "\n")
            for (st in types.structs) {
                let cname = c_type_name(names, st._module, st.name)
                write(writer, "typedef struct {cname} {cname};\n")
            }
        }
        for (st in types.structs) {
            write_struct(writer, st, names)
        }
        write(writer, "\n")
        for (e in exports) {
            write(writer, "\n// {e.das_signature}\n{c_declaration(e, names)};\n")
        }
        if (!(link_note |> empty())) {
            write(writer, "\n// Linking:\n")
            for (line in link_note |> split("\n")) {
                write(writer, "// {line}\n")
            }
        }
        write(writer, "\n#ifdef __cplusplus\n\} // extern \"C\"\n#endif\n")
    }
}


def public emit_c_header(var exports : array<CExport>; names : CNames; input_path, output_path, link_note : string) : bool {
    let text = build_c_header(exports, names, input_path, output_path, link_note)
    let path = "{output_path}.h"
    var ok = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            f |> fwrite(text)
            ok = true
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "daslang -lib: can't write the C header {path}\n")
    }
    return ok
}


struct public CRejection {
    fn_name : string
    message : string
}


def public is_c_export_candidate(var fn : Function?; prog : Program?; export_all : bool) : bool {
    if (fn._module != prog.getThisModule || !fn.flags.exports
            || fn.flags.builtIn || fn.flags.generated || fn.moreFlags.isTemplate || fn.fromGeneric != null
            || fn.flags.init || fn.flags.shutdown || fn.flags.macroInit || fn.moreFlags.macroFunction) {
        return false
    }
    return export_all || has_export_c_annotation(fn)
}


def private reserved_c_names(names : CNames) : array<string> {
    let p = names.prefix
    return ["{p}_create", "{p}_destroy", "{p}_last_error", "{p}_shutdown_runtime", "{p}_ctx"]
}


def public collect_c_exports(prog : Program?; names : CNames; export_all : bool) : tuple<exports : array<CExport>; errors : array<CRejection>> {
    var accepted : array<CExport>
    var errors : array<CRejection>
    var claimed <- {for (r in reserved_c_names(names)); r => "the fixed library API"}
    var this_mod = prog.getThisModule
    this_mod |> for_each_function("") $(var fn) {
        if (!is_c_export_candidate(fn, prog, export_all)) {
            return
        }
        let is_explicit = has_export_c_annotation(fn)
        let c_name = c_export_symbol(names.prefix, fn, export_c_name_override(fn))
        var why = ""
        let owner = claimed?[c_name] ?? ""
        var sig <- describe_c_signature(fn, names, c_name)
        if (!(owner |> empty())) {
            why = "C symbol `{c_name}` is already taken by {owner}; C has no overloading - give one of them [export_c(name = \"...\")]"
        } elif (!sig.reject.ok) {
            why = ("{sig.reject.what} has type {sig.reject.why}, which has no C representation "
                + "(allowed: bool, int8..uint64, int, uint, float, double, string, pointers, enums, "
                + "POD structs, float2..uint4, range/urange/range64/urange64, fixed_array arguments)")
        }
        if (!(why |> empty())) {
            if (is_explicit) {
                errors |> emplace(CRejection(fn_name = string(fn.name),
                message = "{describe(fn.at)}: [export_c] {fn.name}: {why}"))
            } else {
                to_log(LOG_WARNING, "{describe(fn.at)}: daslang -lib: skipping {fn.name}: {why}\n")
            }
            return
        }
        claimed[c_name] = "{fn.name} at {describe(fn.at)}"
        accepted |> emplace(sig.exp)
    }
    return (exports <- accepted, errors <- errors)
}
